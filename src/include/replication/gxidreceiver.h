#ifndef GXID_RECEIVER_H_
#define GXID_RECEIVER_H_

#include "utils/snapshot.h"

/* prototypes for functions in snapreceiver.c */
extern void GxidReceiverMain(void) pg_attribute_noreturn();

extern Size GxidRcvShmemSize(void);
extern void GxidRcvShmemInit(void);
extern void ShutdownGixdRcv(void);
extern bool GxidRcvStreaming(void);
extern TransactionId GixRcvGetGlobalTransactionId(bool isSubXact);
extern void GixRcvCommitTransactionId(TransactionId txid);

#endif							/* GXID_RECEIVER_H_ */