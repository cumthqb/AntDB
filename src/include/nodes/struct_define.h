
/*
 * NOTES
 *  ******************************
 *  *** DO NOT EDIT THIS FILE! ***
 *  ******************************
 *
 *  It has been GENERATED by src/backend/nodes/gen_nodes.pl
 */
#ifndef BEGIN_STRUCT
#	define BEGIN_STRUCT(t)
#endif
#ifndef END_STRUCT
#	define END_STRUCT(t)
#endif

#ifndef NODE_BASE2
#	define NODE_BASE2(t,m) NODE_BASE(t)
#endif
#ifndef NODE_SAME
#	define NODE_SAME(t1,t2) \
		BEGIN_NODE(t1)		\
			NODE_BASE(t2)	\
		END_NODE(t1)
#endif
#ifndef NODE_ARG_
#	define NODE_ARG_ node
#endif

#ifndef NODE_BASE
#	define NODE_BASE(b)
#endif
#ifndef NODE_NODE
#	define NODE_NODE(t,m)
#endif
#ifndef NODE_NODE_MEB
#	define NODE_NODE_MEB(t,m)
#endif
#ifndef NODE_NODE_ARRAY
#	define NODE_NODE_ARRAY(t,m,l)
#endif
#ifndef NODE_BITMAPSET
#	define NODE_BITMAPSET(t,m)
#endif
#ifndef NODE_BITMAPSET_ARRAY
#	define NODE_BITMAPSET_ARRAY(t,m,l)
#endif
#ifndef NODE_RELIDS
#	define NODE_RELIDS(t,m) NODE_BITMAPSET(Bitmapset,m)
#endif
#ifndef NODE_RELIDS_ARRAY
#	define NODE_RELIDS_ARRAY(t,m,l) NODE_BITMAPSET_ARRAY(Bitmapset,m,l)
#endif
#ifndef NODE_LOCATION
#	define NODE_LOCATION(t,m) NODE_SCALAR(t,m)
#endif
#ifndef NODE_SCALAR
#	define NODE_SCALAR(t,m)
#endif
#ifndef NODE_OID
#	define NODE_OID(t,m) NODE_SCALAR(Oid, m)
#endif
#ifndef NODE_SCALAR_POINT
#	define NODE_SCALAR_POINT(t,m,l)
#endif
#ifndef NODE_SCALAR_ARRAY
#	define NODE_SCALAR_ARRAY NODE_SCALAR_POINT
#endif
#ifndef NODE_OTHER_POINT
#	define NODE_OTHER_POINT(t,m)
#endif
#ifndef NODE_STRING
#	define NODE_STRING(m)
#endif
#ifndef NODE_StringInfo
#	define NODE_StringInfo(m)
#endif
#ifndef NODE_STRUCT
#	define NODE_STRUCT(t,m)
#endif
#ifndef NODE_STRUCT_ARRAY
#	define NODE_STRUCT_ARRAY(t,m,l)
#endif
#ifndef NODE_STRUCT_LIST
#	define NODE_STRUCT_LIST(t,m)
#endif
#ifndef NODE_STRUCT_MEB
#	define NODE_STRUCT_MEB(t,m)
#endif
#ifndef NODE_ENUM
#	define NODE_ENUM(t,m)
#endif
#ifndef NODE_DATUM
#	define NODE_DATUM(t, m, o, n)
#endif

#ifndef NO_STRUCT_PartitionPruneStep
BEGIN_STRUCT(PartitionPruneStep)
	NODE_ENUM(NodeTag,type)
	NODE_SCALAR(int,step_id)
END_STRUCT(PartitionPruneStep)
#endif /* NO_STRUCT_PartitionPruneStep */

#ifndef NO_STRUCT_QualCost
BEGIN_STRUCT(QualCost)
	NODE_SCALAR(Cost,startup)
	NODE_SCALAR(Cost,per_tuple)
END_STRUCT(QualCost)
#endif /* NO_STRUCT_QualCost */

#ifndef NO_STRUCT_AggClauseCosts
BEGIN_STRUCT(AggClauseCosts)
	NODE_SCALAR(int,numAggs)
	NODE_SCALAR(int,numOrderedAggs)
	NODE_SCALAR(bool,hasNonPartial)
	NODE_SCALAR(bool,hasNonSerial)
	NODE_STRUCT_MEB(QualCost,transCost)
	NODE_SCALAR(Cost,finalCost)
	NODE_SCALAR(Size,transitionSpace)
END_STRUCT(AggClauseCosts)
#endif /* NO_STRUCT_AggClauseCosts */

#ifndef NO_STRUCT_PartitionSchemeData
BEGIN_STRUCT(PartitionSchemeData)
	NODE_SCALAR(char,strategy)
	NODE_SCALAR(int16,partnatts)
	NODE_SCALAR_POINT(Oid,partopfamily,NODE_ARG_->------)
	NODE_SCALAR_POINT(Oid,partopcintype,NODE_ARG_->------)
	NODE_SCALAR_POINT(Oid,partcollation,NODE_ARG_->------)
	NODE_SCALAR_POINT(int16,parttyplen,NODE_ARG_->------)
	NODE_SCALAR_POINT(bool,parttypbyval,NODE_ARG_->------)
FmgrInfo *partsupfunc
END_STRUCT(PartitionSchemeData)
#endif /* NO_STRUCT_PartitionSchemeData */

#ifndef NO_STRUCT_MergeScanSelCache
BEGIN_STRUCT(MergeScanSelCache)
	NODE_SCALAR(Oid,opfamily)
	NODE_SCALAR(Oid,collation)
	NODE_SCALAR(int,strategy)
	NODE_SCALAR(bool,nulls_first)
	NODE_SCALAR(Selectivity,leftstartsel)
	NODE_SCALAR(Selectivity,leftendsel)
	NODE_SCALAR(Selectivity,rightstartsel)
	NODE_SCALAR(Selectivity,rightendsel)
END_STRUCT(MergeScanSelCache)
#endif /* NO_STRUCT_MergeScanSelCache */

#ifndef NO_STRUCT_SemiAntiJoinFactors
BEGIN_STRUCT(SemiAntiJoinFactors)
	NODE_SCALAR(Selectivity,outer_match_frac)
	NODE_SCALAR(Selectivity,match_count)
END_STRUCT(SemiAntiJoinFactors)
#endif /* NO_STRUCT_SemiAntiJoinFactors */

#ifndef NO_STRUCT_JoinPathExtraData
BEGIN_STRUCT(JoinPathExtraData)
	NODE_NODE(List,restrictlist)
	NODE_NODE(List,mergeclause_list)
	NODE_SCALAR(bool,inner_unique)
	NODE_NODE(SpecialJoinInfo,sjinfo)
	NODE_STRUCT_MEB(SemiAntiJoinFactors,semifactors)
	NODE_RELIDS(Relids,param_source_rels)
END_STRUCT(JoinPathExtraData)
#endif /* NO_STRUCT_JoinPathExtraData */

#ifndef NO_STRUCT_JoinCostWorkspace
BEGIN_STRUCT(JoinCostWorkspace)
	NODE_SCALAR(Cost,startup_cost)
	NODE_SCALAR(Cost,total_cost)
	NODE_SCALAR(Cost,run_cost)
	NODE_SCALAR(Cost,inner_run_cost)
	NODE_SCALAR(Cost,inner_rescan_run_cost)
	NODE_SCALAR(double,outer_rows)
	NODE_SCALAR(double,inner_rows)
	NODE_SCALAR(double,outer_skip_rows)
	NODE_SCALAR(double,inner_skip_rows)
	NODE_SCALAR(int,numbuckets)
	NODE_SCALAR(int,numbatches)
	NODE_SCALAR(double,inner_rows_total)
#ifdef ADB
	NODE_SCALAR(bool,is_cluster)
#endif
END_STRUCT(JoinCostWorkspace)
#endif /* NO_STRUCT_JoinCostWorkspace */

#ifndef NO_STRUCT_ExprContext_CB
BEGIN_STRUCT(ExprContext_CB)
	NODE_STRUCT(ExprContext_CB,next)
	NODE_OTHER_POINT(ExprContextCallbackFunction, function)
	NODE_SCALAR(Datum, arg)
END_STRUCT(ExprContext_CB)
#endif /* NO_STRUCT_ExprContext_CB */

#ifndef NO_STRUCT_ExecRowMark
BEGIN_STRUCT(ExecRowMark)
	NODE_OTHER_POINT(Relation, relation)
	NODE_SCALAR(Oid,relid)
	NODE_SCALAR(Index,rti)
	NODE_SCALAR(Index,prti)
	NODE_SCALAR(Index,rowmarkId)
	NODE_ENUM(RowMarkType,markType)
	NODE_ENUM(LockClauseStrength,strength)
	NODE_ENUM(LockWaitPolicy,waitPolicy)
	NODE_SCALAR(bool,ermActive)
	NODE_OTHER_POINT(ItemPointerData, curCtid)
	NODE_OTHER_POINT(void,ermExtra)
END_STRUCT(ExecRowMark)
#endif /* NO_STRUCT_ExecRowMark */

#ifndef NO_STRUCT_ExecAuxRowMark
BEGIN_STRUCT(ExecAuxRowMark)
	NODE_STRUCT(ExecRowMark,rowmark)
	NODE_SCALAR(AttrNumber,ctidAttNo)
	NODE_SCALAR(AttrNumber,toidAttNo)
	NODE_SCALAR(AttrNumber,wholeAttNo)
END_STRUCT(ExecAuxRowMark)
#endif /* NO_STRUCT_ExecAuxRowMark */

#ifndef NO_STRUCT_TupleHashEntryData
BEGIN_STRUCT(TupleHashEntryData)
	NODE_OTHER_POINT(MinimalTuple, firstTuple)
	NODE_OTHER_POINT(void,additional)
	NODE_SCALAR(uint32,status)
	NODE_SCALAR(uint32,hash)
END_STRUCT(TupleHashEntryData)
#endif /* NO_STRUCT_TupleHashEntryData */

#ifndef NO_STRUCT_TupleHashTableData
BEGIN_STRUCT(TupleHashTableData)
	NODE_OTHER_POINT(HTAB, hashtab)
	NODE_SCALAR(int,numCols)
	NODE_SCALAR_POINT(AttrNumber,keyColIdx,NODE_ARG_->numCols)
	NODE_OTHER_POINT(FmgrInfo, tab_hash_funcs)
	NODE_NODE(ExprState,tab_eq_func)
	NODE_NODE_MEB(MemoryContext,tablecxt)
	NODE_NODE_MEB(MemoryContext,tempcxt)
	NODE_SCALAR(Size,entrysize)
	NODE_NODE(TupleTableSlot,tableslot)
	NODE_NODE(TupleTableSlot,inputslot)
	NODE_OTHER_POINT(FmgrInfo, in_hash_funcs)
	NODE_NODE(ExprState,cur_eq_func)
	NODE_SCALAR(uint32,hash_iv)
	NODE_NODE(ExprContext,exprcontext)
END_STRUCT(TupleHashTableData)
#endif /* NO_STRUCT_TupleHashTableData */

#ifndef NO_STRUCT_EPQState
BEGIN_STRUCT(EPQState)
	NODE_NODE(EState,estate)
	NODE_NODE(PlanState,planstate)
	NODE_NODE(TupleTableSlot,origslot)
	NODE_NODE(Plan,plan)
	NODE_NODE(List,arowMarks)
	NODE_SCALAR(int,epqParam)
END_STRUCT(EPQState)
#endif /* NO_STRUCT_EPQState */

#ifndef NO_STRUCT_SharedSortInfo
BEGIN_STRUCT(SharedSortInfo)
	NODE_SCALAR(int,num_workers)
TuplesortInstrumentation sinstrument[FLEXIBLE_ARRAY_MEMBER]
END_STRUCT(SharedSortInfo)
#endif /* NO_STRUCT_SharedSortInfo */

#ifndef NO_STRUCT_GenericIndexOpts
BEGIN_STRUCT(GenericIndexOpts)
	NODE_SCALAR(int32,vl_len_)
	NODE_SCALAR(bool,recheck_on_update)
END_STRUCT(GenericIndexOpts)
#endif /* NO_STRUCT_GenericIndexOpts */

#ifndef NO_STRUCT_AutoVacOpts
BEGIN_STRUCT(AutoVacOpts)
	NODE_SCALAR(bool,enabled)
	NODE_SCALAR(int,vacuum_threshold)
	NODE_SCALAR(int,analyze_threshold)
	NODE_SCALAR(int,vacuum_cost_delay)
	NODE_SCALAR(int,vacuum_cost_limit)
	NODE_SCALAR(int,freeze_min_age)
	NODE_SCALAR(int,freeze_max_age)
	NODE_SCALAR(int,freeze_table_age)
	NODE_SCALAR(int,multixact_freeze_min_age)
	NODE_SCALAR(int,multixact_freeze_max_age)
	NODE_SCALAR(int,multixact_freeze_table_age)
	NODE_SCALAR(int,log_min_duration)
	NODE_SCALAR(float8,vacuum_scale_factor)
	NODE_SCALAR(float8,analyze_scale_factor)
END_STRUCT(AutoVacOpts)
#endif /* NO_STRUCT_AutoVacOpts */

#ifndef NO_STRUCT_StdRdOptions
BEGIN_STRUCT(StdRdOptions)
	NODE_SCALAR(int32,vl_len_)
	NODE_SCALAR(int,fillfactor)
	NODE_SCALAR(float8,vacuum_cleanup_index_scale_factor)
	NODE_SCALAR(int,toast_tuple_target)
	NODE_STRUCT_MEB(AutoVacOpts,autovacuum)
	NODE_SCALAR(bool,user_catalog_table)
	NODE_SCALAR(int,parallel_workers)
END_STRUCT(StdRdOptions)
#endif /* NO_STRUCT_StdRdOptions */

#ifndef NO_STRUCT_ViewOptions
BEGIN_STRUCT(ViewOptions)
	NODE_SCALAR(int32,vl_len_)
	NODE_SCALAR(bool,security_barrier)
	NODE_SCALAR(int,check_option_offset)
END_STRUCT(ViewOptions)
#endif /* NO_STRUCT_ViewOptions */

#ifndef NO_STRUCT_ParamExternData
BEGIN_STRUCT(ParamExternData)
	NODE_DATUM(Datum,value,NODE_ARG_->ptype, NODE_ARG_->isnull)
	NODE_SCALAR(bool,isnull)
	NODE_SCALAR(uint16,pflags)
	NODE_SCALAR(Oid,ptype)
END_STRUCT(ParamExternData)
#endif /* NO_STRUCT_ParamExternData */

#ifndef NO_STRUCT_ParamListInfoData
BEGIN_STRUCT(ParamListInfoData)
	NODE_OTHER_POINT(ParamFetchHook,paramFetch)
	NODE_OTHER_POINT(void,paramFetchArg)
	NODE_OTHER_POINT(ParamCompileHook,paramCompile)
	NODE_OTHER_POINT(void,paramCompileArg)
	NODE_OTHER_POINT(ParserSetupHook,parserSetup)
	NODE_OTHER_POINT(void,parserSetupArg)
	NODE_SCALAR(int,numParams)
	NODE_STRUCT_ARRAY(ParamExternData,params, NODE_ARG_->numParams)
END_STRUCT(ParamListInfoData)
#endif /* NO_STRUCT_ParamListInfoData */

#if defined(ADB)
#ifndef NO_STRUCT_ReduceInfo
BEGIN_STRUCT(ReduceInfo)
	NODE_NODE(List,storage_nodes)
	NODE_NODE(List,exclude_exec)
	NODE_NODE(List,params)
	NODE_NODE(Expr,expr)
	NODE_RELIDS(Relids,relids)
	NODE_SCALAR(char,type)
END_STRUCT(ReduceInfo)
#endif /* NO_STRUCT_ReduceInfo */
#endif

#ifndef NO_STRUCT_HashInstrumentation
BEGIN_STRUCT(HashInstrumentation)
	NODE_SCALAR(int,nbuckets)
	NODE_SCALAR(int,nbuckets_original)
	NODE_SCALAR(int,nbatch)
	NODE_SCALAR(int,nbatch_original)
size_t space_peak
END_STRUCT(HashInstrumentation)
#endif /* NO_STRUCT_HashInstrumentation */

#ifndef NO_STRUCT_SharedHashInfo
BEGIN_STRUCT(SharedHashInfo)
	NODE_SCALAR(int,num_workers)
HashInstrumentation hinstrument[FLEXIBLE_ARRAY_MEMBER]
END_STRUCT(SharedHashInfo)
#endif /* NO_STRUCT_SharedHashInfo */

#ifndef NO_STRUCT_ExtensibleNodeMethods
BEGIN_STRUCT(ExtensibleNodeMethods)
	NODE_STRING(extnodename)
	NODE_SCALAR(Size,node_size)
	NODE_OTHER_POINT(void,nodeCopy)
	NODE_OTHER_POINT(void,nodeEqual)
	NODE_OTHER_POINT(void,nodeOut)
	NODE_OTHER_POINT(void,nodeRead)
END_STRUCT(ExtensibleNodeMethods)
#endif /* NO_STRUCT_ExtensibleNodeMethods */

#ifndef NO_STRUCT_CustomPathMethods
BEGIN_STRUCT(CustomPathMethods)
	NODE_STRING(CustomName)
	NODE_OTHER_POINT(void,PlanCustomPath)
	NODE_OTHER_POINT(void,ReparameterizeCustomPathByChild)
END_STRUCT(CustomPathMethods)
#endif /* NO_STRUCT_CustomPathMethods */

#ifndef NO_STRUCT_CustomScanMethods
BEGIN_STRUCT(CustomScanMethods)
	NODE_STRING(CustomName)
	NODE_OTHER_POINT(void,CreateCustomScanState)
END_STRUCT(CustomScanMethods)
#endif /* NO_STRUCT_CustomScanMethods */

#ifndef NO_STRUCT_CustomExecMethods
BEGIN_STRUCT(CustomExecMethods)
	NODE_STRING(CustomName)
	NODE_OTHER_POINT(void,BeginCustomScan)
	NODE_OTHER_POINT(void,ExecCustomScan)
	NODE_OTHER_POINT(void,EndCustomScan)
	NODE_OTHER_POINT(void,ReScanCustomScan)
	NODE_OTHER_POINT(void,MarkPosCustomScan)
	NODE_OTHER_POINT(void,RestrPosCustomScan)
	NODE_OTHER_POINT(void,EstimateDSMCustomScan)
	NODE_OTHER_POINT(void,InitializeDSMCustomScan)
	NODE_OTHER_POINT(void,ReInitializeDSMCustomScan)
	NODE_OTHER_POINT(void,InitializeWorkerCustomScan)
	NODE_OTHER_POINT(void,ShutdownCustomScan)
	NODE_OTHER_POINT(void,ExplainCustomScan)
END_STRUCT(CustomExecMethods)
#endif /* NO_STRUCT_CustomExecMethods */

#ifndef NO_STRUCT_MGRAddHba
BEGIN_STRUCT(MGRAddHba)
	NODE_ENUM(NodeTag,type)
	NODE_STRING(name)
	NODE_NODE(List,options)
END_STRUCT(MGRAddHba)
#endif /* NO_STRUCT_MGRAddHba */

#ifndef NO_STRUCT_MGRAlterParm
BEGIN_STRUCT(MGRAlterParm)
	NODE_ENUM(NodeTag,type)
	NODE_SCALAR(bool,if_not_exists)
	NODE_STRING(parmkey)
	NODE_STRING(parmnode)
	NODE_NODE(List,options)
END_STRUCT(MGRAlterParm)
#endif /* NO_STRUCT_MGRAlterParm */

#ifndef NO_STRUCT_MGRMonitorAgent
BEGIN_STRUCT(MGRMonitorAgent)
	NODE_ENUM(NodeTag,type)
	NODE_NODE(List,hosts)
END_STRUCT(MGRMonitorAgent)
#endif /* NO_STRUCT_MGRMonitorAgent */

#ifndef NO_STRUCT_MGRStopAgent
BEGIN_STRUCT(MGRStopAgent)
	NODE_ENUM(NodeTag,type)
	NODE_NODE(List,hosts)
END_STRUCT(MGRStopAgent)
#endif /* NO_STRUCT_MGRStopAgent */

#if defined(ADB)
#ifndef NO_STRUCT_RelationLocInfo
BEGIN_STRUCT(RelationLocInfo)
	NODE_SCALAR(Oid,relid)
	NODE_SCALAR(char,locatorType)
	NODE_SCALAR(AttrNumber,partAttrNum)
	NODE_NODE(List,nodeids)
	NODE_SCALAR(Oid,funcid)
	NODE_NODE(List,funcAttrNums)
END_STRUCT(RelationLocInfo)
#endif /* NO_STRUCT_RelationLocInfo */
#endif

#ifndef NO_STRUCT_LockRelId
BEGIN_STRUCT(LockRelId)
	NODE_SCALAR(Oid,relId)
	NODE_SCALAR(Oid,dbId)
END_STRUCT(LockRelId)
#endif /* NO_STRUCT_LockRelId */

#ifndef NO_STRUCT_LockInfoData
BEGIN_STRUCT(LockInfoData)
	NODE_STRUCT_MEB(LockRelId,lockRelId)
END_STRUCT(LockInfoData)
#endif /* NO_STRUCT_LockInfoData */
