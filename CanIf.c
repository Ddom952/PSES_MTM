#include "CanIf.h"
#include "CanTp.h"
#include "stdio.h"

static Std_ReturnType return_value = E_OK;

void Set_CanIf_Return_Value( Std_ReturnType RT )
{
    return_value = RT;
}

Std_ReturnType CanIf_Transmit( PduIdType TxPduId, const PduInfoType* PduInfoPtr )
{
    return return_value;
}

void Send_CanTp_RxIndication( PduIdType RxPduId, const PduInfoType* PduInfoPtr )
{
    printf( "Send_CanTp_RxIndication:\r\n" );
    printf( "\tRxPduId: %d\r\n", RxPduId );
    printf( "\tPduInfoPtr:\r\n" );
    printf( "\t\tPduInfoPtr->SduLenght: %d\r\n", PduInfoPtr->SduLenght );

    CanTp_RxIndication(RxPduId, PduInfoPtr);
}

/*
void Send_CanTp_RxIndication( PduIdType RxPduId, const PduInfoType* PduInfoPtr )
{
    CanTp_RxIndication(RxPduId, PduInfoPtr);
}
*/