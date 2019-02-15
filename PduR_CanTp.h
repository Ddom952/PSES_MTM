#ifndef PDURCANTP_H_
#define PDURCANTP_H_

#include "ComStackTypes.h"

void PduR_CanTp_Set_BufReq_Return_Value( BufReq_ReturnType RT );

void PduR_CanTpRxIndication( PduIdType RxPduId, Std_ReturnType result );
void PduR_CanTpTxConfirmation(PduIdType TxPduId, Std_ReturnType result);
BufReq_ReturnType PduR_CanTpCopyRxData( PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr );
BufReq_ReturnType PduR_CanTpStartOfReception( PduIdType id, const PduInfoType* info, PduLengthType TpSduLength, PduLengthType* bufferSizePtr );
BufReq_ReturnType PduR_CanTpCopyTxData( PduIdType id, const PduInfoType* info, const RetryInfoType* retry, PduLengthType* availableDataPtr );

#endif // PDURCANTP_H_