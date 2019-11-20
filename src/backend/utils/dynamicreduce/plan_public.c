#include "postgres.h"

#include "access/htup_details.h"
#include "libpq/pqformat.h"
#include "miscadmin.h"
#include "utils/dynamicreduce.h"
#include "utils/dr_private.h"
#include "utils/memutils.h"

static HTAB		   *htab_plan_info = NULL;

bool DRSendPlanWorkerMessage(PlanWorkerInfo *pwi, PlanInfo *pi)
{
	shm_mq_result result;
	bool sended = false;

re_send_:
	if (pwi->sendBuffer.len > 0)
	{
		Assert(pwi->sendBuffer.cursor == 0);
		result = shm_mq_send(pwi->reduce_sender,
							 pwi->sendBuffer.len,
							 pwi->sendBuffer.data,
							 true);
		if (result == SHM_MQ_SUCCESS)
		{
			DR_PLAN_DEBUG((errmsg("send plan %d worker %d with data length %d success",
								  pi->plan_id, pwi->worker_id, pwi->sendBuffer.len)));
			pwi->sendBuffer.len = 0;
			sended = true;
		}else if (result == SHM_MQ_DETACHED)
		{
			ereport(ERROR,
					(errmsg("plan %d parallel %d MQ detached",
							pi->plan_id, pwi->worker_id)));
		}
		else if(result == SHM_MQ_WOULD_BLOCK)
		{
			return sended;
		}else
		{
			ereport(ERROR,
					(errcode(ERRCODE_INTERNAL_ERROR),
					 errmsg("unknown shm_mq_send result %d", result)));
		}
	}

	Assert(pwi->sendBuffer.len == 0);
	switch(pwi->plan_send_state)
	{
	case DR_PLAN_SEND_WORKING:
	case DR_PLAN_SEND_ENDED:
		break;
	case DR_PLAN_SEND_GENERATE_CACHE:
		if (pi->GenerateCacheMsg &&
			pi->GenerateCacheMsg(pwi, pi))
		{
			Assert(pwi->sendBuffer.len > 0);
			pwi->plan_send_state = DR_PLAN_SEND_SENDING_CACHE;
			goto re_send_;
		}
		/* do not add break, need generate EOF message */
	case DR_PLAN_SEND_SENDING_CACHE:
	case DR_PLAN_SEND_GENERATE_EOF:
		appendStringInfoChar(&pwi->sendBuffer, ADB_DR_MSG_END_OF_PLAN);
		pwi->plan_send_state = DR_PLAN_SEND_SENDING_EOF;
		goto re_send_;
	case DR_PLAN_SEND_SENDING_EOF:
		pwi->plan_send_state = DR_PLAN_SEND_ENDED;
		break;
	default:
		ereport(ERROR,
				(errcode(ERRCODE_INTERNAL_ERROR),
				 errmsg("unknown worker sending state %u", pwi->plan_send_state)));
		break;	/* never run */
	}

	return sended;
}

bool DRRecvPlanWorkerMessage(PlanWorkerInfo *pwi, PlanInfo *pi)
{
	unsigned char  *addr,*saved_addr;
	MinimalTuple	mtup;
	HeapTupleData	tup;
	MemoryContext	oldcontext;
	Size			size;
	shm_mq_result	result;
	int				msg_type;
	uint32			msg_head;

	if (pwi->end_of_plan_recv ||
		pwi->last_data != NULL)
		return false;

	result = shm_mq_receive(pwi->worker_sender, &size, (void**)&addr, true);
	if (result == SHM_MQ_WOULD_BLOCK)
	{
		return false;
	}else if(result == SHM_MQ_DETACHED)
	{
		pwi->end_of_plan_recv = true;
		ereport(ERROR,
				(errmsg("plan %d parallel %d MQ detached",
						pi->plan_id, pwi->worker_id)));
	}
	Assert(result == SHM_MQ_SUCCESS);
	if (size < sizeof(msg_head))
		goto invalid_plan_message_;

	msg_head = *(uint32*)addr;
	msg_type = (msg_head >> 24) & 0xff;
	DR_PLAN_DEBUG((errmsg("plan %d got message %d from MQ size %zu head %08x",
						  pi->plan_id, msg_type, size, msg_head)));
	if (msg_type == ADB_DR_MSG_TUPLE)
	{
		saved_addr = addr;

		pwi->dest_cursor = 0;
		pwi->dest_count = (msg_head & 0xffffff);
		addr += sizeof(msg_head);

		pwi->dest_oids = (Oid*)addr;
		addr += sizeof(Oid)*pwi->dest_count;
		if ((addr - saved_addr) >= size)
			goto invalid_plan_message_;
		pwi->last_size = size - (addr - saved_addr);
		pwi->last_data = addr;
		pwi->last_msg_type = ADB_DR_MSG_TUPLE;

		if (pi->type_convert)
		{
			MemoryContextReset(pi->convert_context);
			oldcontext = MemoryContextSwitchTo(pi->convert_context);

			DRStoreTypeConvertTuple(pwi->slot_plan_src, pwi->last_data, pwi->last_size, &tup);
			do_type_convert_slot_out(pi->type_convert, pwi->slot_plan_src, pwi->slot_plan_dest, false);
			mtup = ExecFetchSlotMinimalTuple(pwi->slot_plan_dest);
			ExecClearTuple(pwi->slot_plan_src);

			pwi->last_size = mtup->t_len - MINIMAL_TUPLE_DATA_OFFSET;
			pwi->last_data = (char*)mtup + MINIMAL_TUPLE_DATA_OFFSET;

			MemoryContextSwitchTo(oldcontext);
		}

		return true;
	}else if(msg_type == ADB_DR_MSG_END_OF_PLAN)
	{
		DR_PLAN_DEBUG_EOF((errmsg("plan %d worker %d got end of plan message from backend",
								  pi->plan_id, pwi->worker_id)));
		pwi->last_msg_type = ADB_DR_MSG_END_OF_PLAN;
		return true;
	}

invalid_plan_message_:
	pwi->end_of_plan_recv = true;
	ereport(ERROR,
			(errmsg("Invalid MQ message format plan %d parallel %d", pi->plan_id, pwi->worker_id)));
	return false;	/* keep compiler quiet */
}

void DRSendWorkerMsgToNode(PlanWorkerInfo *pwi, PlanInfo *pi, DRNodeEventData *ned)
{
	uint32			i,count;
	if (pwi->last_msg_type == ADB_DR_MSG_INVALID)
		return;

	for (i=pwi->dest_cursor,count=pwi->dest_count; i<count; ++i)
	{
		if (ned == NULL ||
			pwi->dest_oids[i] != ned->nodeoid)
			ned = DRSearchNodeEventData(pwi->dest_oids[i], HASH_FIND, NULL);

		if (ned == NULL ||
			PutMessageToNode(ned,
							 pwi->last_msg_type,
							 pwi->last_data,
							 pwi->last_size,
							 pi->plan_id) == false)
		{
			pwi->dest_cursor = i;
			pi->waiting_node = pwi->waiting_node = pwi->dest_oids[i];
			return;
		}
	}
	pwi->last_msg_type = ADB_DR_MSG_INVALID;
	pwi->last_data = NULL;
	pi->waiting_node = pwi->waiting_node = InvalidOid;
}

TupleTableSlot* DRStoreTypeConvertTuple(TupleTableSlot *slot, const char *data, uint32 len, HeapTuple head)
{
	MinimalTuple mtup;
	if (((Size)data - MINIMAL_TUPLE_DATA_OFFSET) % MAXIMUM_ALIGNOF == 0)
	{
		head->t_len = len - (MINIMAL_TUPLE_OFFSET + MINIMAL_TUPLE_DATA_OFFSET);
		head->t_data = (HeapTupleHeader)((char*)data - (MINIMAL_TUPLE_OFFSET + MINIMAL_TUPLE_DATA_OFFSET));
		ExecStoreTuple(head, slot, InvalidBuffer, false);
	}else
	{
		mtup = palloc(len + MINIMAL_TUPLE_DATA_OFFSET);
		mtup->t_len = len + MINIMAL_TUPLE_DATA_OFFSET;
		memcpy((char*)mtup + MINIMAL_TUPLE_DATA_OFFSET, data, len);
		ExecStoreMinimalTuple(mtup, slot, false);
	}

	return slot;
}

void DRSerializePlanInfo(int plan_id, dsm_segment *seg, void *addr, Size size, TupleDesc desc, List *work_nodes, StringInfo buf)
{
	Size		offset;
	ListCell   *lc;
	dsm_handle	handle;
	uint32		length;

	Assert(plan_id >= 0);
	if ((length=list_length(work_nodes)) == 0)
	{
		ereport(ERROR,
				(errcode(ERRCODE_INTERNAL_ERROR),
				 errmsg("invalid work nodes fro dynamic reduce plan %d", plan_id)));
	}
	Assert(IsA(work_nodes, OidList));

	handle = dsm_segment_handle(seg);
	offset = (char*)addr - (char*)dsm_segment_address(seg);
	Assert((char*)addr >= (char*)dsm_segment_address(seg));
	Assert(offset + size <= dsm_segment_map_length(seg));

	pq_sendbytes(buf, (char*)&plan_id, sizeof(plan_id));
	pq_sendbytes(buf, (char*)&handle, sizeof(handle));
	pq_sendbytes(buf, (char*)&offset, sizeof(offset));
	SerializeTupleDesc(buf, desc);
	pq_sendbytes(buf, (char*)&length, sizeof(length));
	foreach(lc, work_nodes)
		pq_sendbytes(buf, (char*)&lfirst_oid(lc), sizeof(Oid));
}

PlanInfo* DRRestorePlanInfo(StringInfo buf, void **shm, Size size, void(*clear)(PlanInfo*))
{
	Size			offset;
	dsm_handle		handle;
	struct tupleDesc * volatile
					desc = NULL;
	PlanInfo * volatile
					pi = NULL;
	int				plan_id;
	uint32			node_count;
	Oid				oid;
	bool			found;

	PG_TRY();
	{
		pq_copymsgbytes(buf, (char*)&plan_id, sizeof(plan_id));
		pq_copymsgbytes(buf, (char*)&handle, sizeof(handle));
		pq_copymsgbytes(buf, (char*)&offset, sizeof(offset));
		desc = RestoreTupleDesc(buf);

		if (plan_id < 0)
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_OBJECT_DEFINITION),
					 errmsg("invalid plan ID %d", plan_id)));

		pi = DRPlanSearch(plan_id, HASH_ENTER, &found);
		if (found)
		{
			pi = NULL;
			ereport(ERROR,
					(errcode(ERRCODE_OBJECT_IN_USE),
					 errmsg("plan ID %d is in use", plan_id)));
		}
		MemSet(pi, 0, sizeof(*pi));
		pi->plan_id = plan_id;
		pi->OnDestroy = clear;
		pi->base_desc = desc;
		desc = NULL;

		if ((pi->seg = dsm_find_mapping(handle)) == NULL)
			pi->seg = dsm_attach(handle);
		if (offset + size > dsm_segment_map_length(pi->seg))
		{
			ereport(ERROR,
					(errcode(ERRCODE_PROTOCOL_VIOLATION),
					 errmsg("invalid shared memory offset or size of DSM")));
		}
		*shm = ((char*)dsm_segment_address(pi->seg)) + offset;

		initOidBuffer(&pi->end_of_plan_nodes);
		pq_copymsgbytes(buf, (char*)&node_count, sizeof(node_count));
		if (node_count == 0 ||
			node_count > dr_latch_data->work_oid_buf.len+1)
		{
			ereport(ERROR,
					(errcode(ERRCODE_PROTOCOL_VIOLATION),
					 errmsg("invalid work node length %u", node_count)));
		}
		initOidBufferEx(&pi->working_nodes, node_count, CurrentMemoryContext);
		found = false;
		while (node_count > 0)
		{
			--node_count;
			pq_copymsgbytes(buf, (char*)&oid, sizeof(Oid));
			if (oid == PGXCNodeOid)
			{
				found = true;
				continue;
			}

			if (oidBufferMember(&pi->working_nodes, oid, NULL))
			{
				ereport(ERROR,
						(errcode(ERRCODE_PROTOCOL_VIOLATION),
						 errmsg("replicate node %u for plan %d", oid, plan_id)));
			}
			if (oidBufferMember(&dr_latch_data->work_oid_buf, oid, NULL) == false)
			{
				ereport(ERROR,
						(errcode(ERRCODE_PROTOCOL_VIOLATION),
						 errmsg("node %u not in dynamic reduce work", oid)));
			}

			appendOidBufferOid(&pi->working_nodes, oid);
		}
		if (found == false)
		{
			ereport(ERROR,
					(errcode(ERRCODE_PROTOCOL_VIOLATION),
					 errmsg("our node %u not found in plan %d for dynamic reduce",
							PGXCNodeOid, plan_id)));
		}
	}PG_CATCH();
	{
		if (pi && pi->type_convert)
			desc = NULL;
		if (desc)
			FreeTupleDesc(desc);
		if (pi)
			(*pi->OnDestroy)(pi);
		PG_RE_THROW();
	}PG_END_TRY();

	return pi;
}

void DRSetupPlanWorkInfo(PlanInfo *pi, PlanWorkerInfo *pwi, DynamicReduceMQ mq, int worker_id)
{
	pwi->worker_id = worker_id;
	pwi->waiting_node = InvalidOid;
	pwi->plan_send_state = DR_PLAN_SEND_WORKING;

	shm_mq_set_receiver((shm_mq*)mq->worker_sender_mq, MyProc);
	shm_mq_set_sender((shm_mq*)mq->reduce_sender_mq, MyProc);
	pwi->worker_sender = shm_mq_attach((shm_mq*)mq->worker_sender_mq, pi->seg, NULL);
	pwi->reduce_sender = shm_mq_attach((shm_mq*)mq->reduce_sender_mq, pi->seg, NULL);
	initStringInfo(&pwi->sendBuffer);
}

/* active waiting plan */
void ActiveWaitingPlan(DRNodeEventData *ned)
{
	PlanInfo		   *pi;
	HASH_SEQ_STATUS		seq_status;
	bool				hint = false;

	DRPlanSeqInit(&seq_status);
	while ((pi = hash_seq_search(&seq_status)) != NULL)
	{
		if (pi->waiting_node == ned->nodeoid)
		{
			DR_PLAN_DEBUG((errmsg("activing plan %d by node %u", pi->plan_id, ned->nodeoid)));
			(*pi->OnNodeIdle)(pi, NULL, ned);
			hint = true;
		}
	}

	if (hint)
		SetLatch(MyLatch);
}

void DRSetupPlanWorkTypeConvert(PlanInfo *pi, PlanWorkerInfo *pwi)
{
	MemoryContext oldcontext = MemoryContextSwitchTo(TopMemoryContext);

	pi->type_convert = create_type_convert(pi->base_desc, true, true);
	if (pi->type_convert)
	{
		pi->convert_context = AllocSetContextCreate(TopMemoryContext,
													"plan tuple convert",
													ALLOCSET_DEFAULT_SIZES);
		pwi->slot_plan_src = MakeSingleTupleTableSlot(pi->type_convert->base_desc);
		pwi->slot_plan_dest = MakeSingleTupleTableSlot(pi->type_convert->out_desc);
		pwi->slot_node_src = MakeSingleTupleTableSlot(pi->type_convert->out_desc);
		pwi->slot_node_dest = MakeSingleTupleTableSlot(pi->type_convert->base_desc);
	}

	MemoryContextSwitchTo(oldcontext);
}

void DRInitPlanSearch(void)
{
	HASHCTL ctl;
	if (htab_plan_info == NULL)
	{
		MemSet(&ctl, 0, sizeof(ctl));
		ctl.keysize = sizeof(int);
		ctl.entrysize = sizeof(PlanInfo);
		ctl.hash = uint32_hash;
		ctl.hcxt = TopMemoryContext;
		htab_plan_info = hash_create("Dynamic reduce plan info",
									 DR_HTAB_DEFAULT_SIZE,
									 &ctl,
									 HASH_ELEM|HASH_CONTEXT|HASH_FUNCTION);
	}
}

PlanInfo* DRPlanSearch(int planid, HASHACTION action, bool *found)
{
	return hash_search(htab_plan_info, &planid, action, found);
}

void DRPlanSeqInit(HASH_SEQ_STATUS *seq)
{
	Assert(htab_plan_info);
	hash_seq_init(seq, htab_plan_info);
}

void DRClearPlanWorkInfo(PlanInfo *pi, PlanWorkerInfo *pwi)
{
	if (pwi == NULL)
		return;
	if (pwi->sendBuffer.data)
		pfree(pwi->sendBuffer.data);
	if (pwi->reduce_sender)
		shm_mq_detach(pwi->reduce_sender);
	if (pwi->worker_sender)
		shm_mq_detach(pwi->worker_sender);
	if (pwi->slot_node_dest)
		ExecDropSingleTupleTableSlot(pwi->slot_node_dest);
	if (pwi->slot_node_src)
		ExecDropSingleTupleTableSlot(pwi->slot_node_src);
	if (pwi->slot_plan_dest)
		ExecDropSingleTupleTableSlot(pwi->slot_plan_dest);
	if (pwi->slot_plan_src)
		ExecDropSingleTupleTableSlot(pwi->slot_plan_src);
}

void DRClearPlanInfo(PlanInfo *pi)
{
	if (pi == NULL)
		return;

	if (pi->type_convert)
	{
		free_type_convert(pi->type_convert);
		pi->type_convert = NULL;
	}
	if (pi->convert_context)
	{
		MemoryContextDelete(pi->convert_context);
		pi->convert_context = NULL;
	}
	if (pi->base_desc)
		FreeTupleDesc(pi->base_desc);
	if (pi->end_of_plan_nodes.oids)
	{
		pfree(pi->end_of_plan_nodes.oids);
		pi->end_of_plan_nodes.oids = NULL;
	}
	if (pi->working_nodes.oids)
	{
		pfree(pi->working_nodes.oids);
		pi->working_nodes.oids = NULL;
	}
	if (pi->seg)
	{
		HASH_SEQ_STATUS seq;
		PlanInfo *tmp;
		bool found = false;

		hash_seq_init(&seq, htab_plan_info);
		while ((tmp=hash_seq_search(&seq)) != NULL)
		{
			if (tmp != pi &&
				tmp->seg == pi->seg)
			{
				found = true;
				hash_seq_term(&seq);
				break;
			}
		}

		if (found == false)
			dsm_detach(pi->seg);
		pi->seg = NULL;
	}
}

void OnDefaultPlanPreWait(PlanInfo *pi)
{
	PlanWorkerInfo *pwi = pi->pwi;
	if (pwi->end_of_plan_recv &&
		pwi->last_msg_type == ADB_DR_MSG_INVALID &&
		pwi->plan_send_state == DR_PLAN_SEND_ENDED)
		(*pi->OnDestroy)(pi);
}

void OnDefaultPlanLatch(PlanInfo *pi)
{
	PlanWorkerInfo *pwi;
	uint32			msg_type;

	pwi = pi->pwi;
	if (DRSendPlanWorkerMessage(pwi, pi))
		DRActiveNode(pi->plan_id);

	while (pwi->waiting_node == InvalidOid &&
		   pwi->end_of_plan_recv == false &&
		   pwi->last_msg_type == ADB_DR_MSG_INVALID)
	{
		if (DRRecvPlanWorkerMessage(pwi, pi) == false)
			break;
		msg_type = pwi->last_msg_type;

		if (msg_type == ADB_DR_MSG_END_OF_PLAN)
		{
			pwi->end_of_plan_recv = true;
			DRGetEndOfPlanMessage(pi, pwi);
		}else
		{
			Assert(msg_type == ADB_DR_MSG_TUPLE);
		}

		/* send message to remote */
		DRSendWorkerMsgToNode(pwi, pi, NULL);
	}
}

void OnDefaultPlanIdleNode(PlanInfo *pi, WaitEvent *w, DRNodeEventData *ned)
{
	PlanWorkerInfo *pwi = pi->pwi;;
	if (pwi->last_msg_type == ADB_DR_MSG_INVALID)
		return;
	if (pwi->dest_oids[pwi->dest_cursor] != ned->nodeoid)
		return;

	DRSendWorkerMsgToNode(pwi, pi, ned);
}
