#ifndef PDUROUTERCAN_H_
#define PDUROUTERCAN_H_

#include "ComStackTypes.h"

void Set_PduRouter_BufReq_Return_Value( BufReq_ReturnType RT );

void PduR_CanTpRxIndication( PduIdType RxPduId, const PduInfoType* PduInfoPtr );
void PduR_CanTpTxConfirmation(PduIdType TxPduId, Std_ReturnType result);
BufReq_ReturnType PduR_CanTpCopyRxData( PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr );
BufReq_ReturnType PduR_CanTpStartOfReception( PduIdType id, const PduInfoType* info, PduLengthType TpSduLength, PduLengthType* bufferSizePtr );
BufReq_ReturnType PduR_CanTpCopyTxData( PduIdType id, const PduInfoType* info, const RetryInfoType* retry, PduLengthType* availableDataPtr );

#endif // PDUROUTERCAN_H_