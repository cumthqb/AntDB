#ifndef REDUCEINFO_H
#define REDUCEINFO_H

#define REDUCE_TYPE_HASH		'H'
#define REDUCE_TYPE_CUSTOM		'C'
#define REDUCE_TYPE_MODULO		'M'
#define REDUCE_TYPE_REPLICATED	'R'
#define REDUCE_TYPE_ROUND		'L'
#define REDUCE_TYPE_COORDINATOR	'O'

#define REDUCE_MARK_STORAGE		0x0001
#define REDUCE_MARK_EXCLUDE		0x0002
#define REDUCE_MARK_PARAMS		0x0004
#define REDUCE_MARK_EXPR		0x0008
#define REDUCE_MARK_RELIDS		0x0010
#define REDUCE_MARK_TYPE		0x0020
#define REDUCE_MARK_NO_EXCLUDE		  \
			(REDUCE_MARK_STORAGE	| \
			 REDUCE_MARK_PARAMS		| \
			 REDUCE_MARK_EXPR		| \
			 REDUCE_MARK_RELIDS		| \
			 REDUCE_MARK_TYPE)
#define REDUCE_MARK_ALL	(REDUCE_MARK_NO_EXCLUDE|REDUCE_MARK_EXCLUDE)

typedef struct ReduceInfo
{
	List	   *storage_nodes;			/* when not reduce by value, it's sorted */
	List	   *exclude_exec;
	List	   *params;
	Expr	   *expr;					/* for custom only */
	Relids		relids;					/* params include */
	char		type;					/* REDUCE_TYPE_XXX */
}ReduceInfo;

typedef struct SemiAntiJoinContext
{
	RelOptInfo *outer_rel;
	RelOptInfo *inner_rel;
	Path *outer_path;
	Path *inner_path;
	List *outer_reduce_list;
	List *inner_reduce_list;
	List *restrict_list;
} SemiAntiJoinContext;

extern ReduceInfo *MakeHashReduceInfo(const List *storage, const List *exclude, const Expr *param);
extern ReduceInfo *MakeCustomReduceInfoByRel(const List *storage, const List *exclude,
						const List *attnums, Oid funcid, Oid reloid, Index rel_index);
extern ReduceInfo *MakeCustomReduceInfo(const List *storage, const List *exclude, List *params, Oid funcid, Oid reloid);
extern ReduceInfo *MakeModuloReduceInfo(const List *storage, const List *exclude, const Expr *param);
extern ReduceInfo *MakeReplicateReduceInfo(const List *storage);
extern ReduceInfo *MakeRoundReduceInfo(const List *storage);
extern ReduceInfo *MakeCoordinatorReduceInfo(void);
extern ReduceInfo *MakeReduceInfoAs(const ReduceInfo *reduce, List *params);
extern List *SortOidList(List *list);

#define IsReduceInfoByValue(r) ((r)->type == REDUCE_TYPE_HASH || \
								(r)->type == REDUCE_TYPE_CUSTOM || \
								(r)->type == REDUCE_TYPE_MODULO)
extern bool IsReduceInfoListByValue(List *list);
#define IsReduceInfoReplicated(r)	((r)->type == REDUCE_TYPE_REPLICATED)
extern bool IsReduceInfoListReplicated(List *list);
#define IsReduceInfoRound(r)		((r)->type == REDUCE_TYPE_ROUND)
extern bool IsReduceInfoListRound(List *list);
#define IsReduceInfoCoordinator(r)	((r)->type == REDUCE_TYPE_COORDINATOR)
extern bool IsReduceInfoListCoordinator(List *list);

#define IsReduceInfoInOneNode(r) (list_length(r->storage_nodes) - list_length(r->exclude_exec) == 1)
extern bool IsReduceInfoListInOneNode(List *list);

extern bool IsReduceInfoStorageSubset(const ReduceInfo *rinfo, List *oidlist);
extern bool IsReduceInfoExecuteSubset(const ReduceInfo *rinfo, List *oidlist);
extern bool IsReduceInfoListExecuteSubset(List *reduce_info_list, List *oidlist);
extern List *ReduceInfoListGetExecuteOidList(const List *list);

/* copy reduce info */
#define CopyReduceInfo(r) CopyReduceInfoExtend(r, REDUCE_MARK_ALL)
#define CopyReduceInfoList(l) CopyReduceInfoListExtend(l, mark)
#define CopyReduceInfoListExtend(l, mark) ReduceInfoListConcatExtend(NIL, l, mark)
#define ReduceInfoListConcat(dest, src) ReduceInfoListConcatExtend(dest, src, REDUCE_MARK_ALL)
extern ReduceInfo *CopyReduceInfoExtend(const ReduceInfo *reduce, int mark);
extern List *ReduceInfoListConcatExtend(List *dest, List *src, int mark);

/* compare reduce info */
extern bool CompReduceInfo(const ReduceInfo *left, const ReduceInfo *right, int mark);
#define IsReduceInfoSame(l,r) CompReduceInfo(l, r, REDUCE_MARK_STORAGE|REDUCE_MARK_TYPE|REDUCE_MARK_EXPR)
#define IsReduceInfoEqual(l,r) CompReduceInfo(l, r, REDUCE_MARK_ALL)

extern int ReduceInfoIncludeExpr(ReduceInfo *reduce, Expr *expr);
extern bool ReduceInfoListIncludeExpr(List *reduceList, Expr *expr);

extern List* ReduceInfoFindTarget(ReduceInfo* reduce, PathTarget *target);
extern List* MakeVarList(List *attnos, Index relid, PathTarget *target);
extern bool IsGroupingReduceExpr(PathTarget *target, ReduceInfo *info);
extern bool IsReduceInfoListCanInnerJoin(List *outer_reduce_list,
									List *inner_reduce_list,
									List *restrictlist);
extern bool IsReduceInfoCanInnerJoin(ReduceInfo *outer_rinfo,
									 ReduceInfo *inner_rinfo,
									 List *restrictlist);
extern bool IsReduceInfoListCanLeftOrRightJoin(List *outer_reduce_list,
											   List *inner_reduce_list,
											   List *restrictlist);
extern bool CanMakeSemiAntiClusterJoinPath(PlannerInfo *root, SemiAntiJoinContext *context);
extern List *FindJoinEqualExprs(ReduceInfo *rinfo, List *restrictlist, RelOptInfo *inner_rel);

extern bool CanOnceGroupingClusterPath(PathTarget *target, Path *path);
extern bool CanOnceDistinctReduceInfoList(List *distinct, List *reduce_list);
extern bool CanOnceDistinctReduceInfo(List *distinct, ReduceInfo *reduce_info);

extern Var *makeVarByRel(AttrNumber attno, Oid rel_oid, Index rel_index);
extern Expr *CreateExprUsingReduceInfo(ReduceInfo *reduce);

#endif /* REDUCEINFO_H */
