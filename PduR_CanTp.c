#include "PduR_CanTp.h"
#include "stdio.h"
#include "string.h"

BufReq_ReturnType PduR_ReturnValue = BUFREQ_OK;
unsigned char * PduR_buffer = NULL;
int PduR_buffer_size;

void PduR_CanTp_Set_BufReq_Return_Value( BufReq_ReturnType RT )
{
    PduR_ReturnValue = RT;
}

void PduR_CanTpRxIndication( PduIdType RxPduId, Std_ReturnType result )
{
    printf( "PduR_CanTpRxIndication:\r\n" );
    printf( "\tRxPduId: %d\r\n", RxPduId );
    printf( "\result: %d\r\n", result );
}

void PduR_CanTpTxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
    printf( "PduR_CanTpTxConfirmation:\r\n" );
    printf( "\tTxPduId: %d\r\n", TxPduId );
    printf( "\t Std_ReturnType Result: %d\r\n", result );
}

BufReq_ReturnType PduR_CanTpCopyRxData( PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr )
{
    if ( NULL == info )
    {
        return PduR_ReturnValue;
    }
    printf( "PduR_CanTpCopyRxData:\r\n" );
    printf( "\tPduR_buffer_size: %d\r\n", PduR_buffer_size );
    printf( "\tRxPduId: %d\r\n", id );
    printf( "\tPduInfoPtr:\r\n" );
    printf( "\t\tPduInfoPtr->SduLenght: %d\r\n", info->SduLenght );
    int i = 0;
    for (i = 0; i < info->SduLenght; i++)
    {
        *(PduR_buffer +i) = *(info->SduDataPtr + i);
    }
    printf( "\rreturn:%d\r\n", PduR_ReturnValue );
    // write remaining buffer size to bufferSizePtr SWS_PduR_00512
    * bufferSizePtr = PduR_buffer_size - info->SduLenght;
    return PduR_ReturnValue;
}

BufReq_ReturnType PduR_CanTpStartOfReception( PduIdType id, const PduInfoType* info, PduLengthType TpSduLength, PduLengthType* bufferSizePtr )
{
    printf( "PduR_CanTpStartOfReception:\r\n" );
    printf( "\rreturn:%d\r\n", PduR_ReturnValue );
    return PduR_ReturnValue;
}

BufReq_ReturnType PduR_CanTpCopyTxData( PduIdType id, const PduInfoType* info, const RetryInfoType* retry, PduLengthType* availableDataPtr )
{
    printf( "PduR_CanTpCopyTxData:\r\n" );
    printf( "\treturn:%d\r\n", PduR_ReturnValue );
    return PduR_ReturnValue;
}