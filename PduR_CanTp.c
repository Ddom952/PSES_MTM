#include "PduR_CanTp.h"

static BufReq_ReturnType return_value = BUFREQ_OK;

void Set_PduRouter_BufReq_Return_Value( BufReq_ReturnType RT )
{
    return_value = RT;
}

void PduR_CanTpRxIndication( PduIdType RxPduId, const PduInfoType* PduInfoPtr )
{

}

void PduR_CanTpTxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{

}

BufReq_ReturnType PduR_CanTpCopyRxData( PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr )
{
    return return_value;
}

BufReq_ReturnType PduR_CanTpStartOfReception( PduIdType id, const PduInfoType* info, PduLengthType TpSduLength, PduLengthType* bufferSizePtr )
{
    return return_value;
}

BufReq_ReturnType PduR_CanTpCopyTxData( PduIdType id, const PduInfoType* info, const RetryInfoType* retry, PduLengthType* availableDataPtr )
{
    return return_value;
}