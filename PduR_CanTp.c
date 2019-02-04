#include "PduR_CanTp.h"
#include "stdio.h"

static BufReq_ReturnType return_value = BUFREQ_OK;

void PduR_CanTp_Set_BufReq_Return_Value( BufReq_ReturnType RT )
{
    return_value = RT;
}

void PduR_CanTpRxIndication( PduIdType RxPduId, const PduInfoType* PduInfoPtr )
{
    printf( "PduR_CanTpRxIndication:\r\n" );
    printf( "\tRxPduId: %d\r\n", RxPduId );
    printf( "\tPduInfoPtr:\r\n" );
    printf( "\t\tPduInfoPtr->SduLenght: %d\r\n", PduInfoPtr->SduLenght );
}

void PduR_CanTpTxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
    printf( "PduR_CanTpTxConfirmation:\r\n" );
    printf( "\tTxPduId: %d\r\n", TxPduId );
    printf( "\t Std_ReturnType Result: %d\r\n", result );
}

BufReq_ReturnType PduR_CanTpCopyRxData( PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr )
{
    printf( "PduR_CanTpCopyRxData:\r\n" );
    printf( "\rreturn:%d\r\n", return_value );
    return return_value;
}

BufReq_ReturnType PduR_CanTpStartOfReception( PduIdType id, const PduInfoType* info, PduLengthType TpSduLength, PduLengthType* bufferSizePtr )
{
    printf( "PduR_CanTpStartOfReception:\r\n" );
    printf( "\rreturn:%d\r\n", return_value );
    return return_value;
}

BufReq_ReturnType PduR_CanTpCopyTxData( PduIdType id, const PduInfoType* info, const RetryInfoType* retry, PduLengthType* availableDataPtr )
{
    printf( "PduR_CanTpCopyTxData:\r\n" );
    printf( "\treturn:%d\r\n", return_value );
    return return_value;
}