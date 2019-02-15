#include "CanIf.h"
#include "CanTp.h"
#include "stdio.h"

typedef enum { // Protocol Control Information
	PCI_SINGLE_FRAME = 0,
	PCI_FIRST_FRAME = 1,
	PCI_CONSECUTIVE_FRAME = 2,
	PCI_FLOW_CONTROL_FRAME = 3
} CanTp_PCI_FrameType;

typedef enum { // Flow Control Transfer Info Frame Type
	CONTINUETOSEND = 0,
	WAIT = 1,
	OVERFLOW_ABORT = 2,
} CanTp_PCI_FlowControlInfoType;

Std_ReturnType CanIfReturnValue = E_OK;
unsigned char CanIfConsecutiveFramesNumber = 0;

void Set_CanIf_Return_Value( Std_ReturnType RT )
{
    CanIfReturnValue = RT;
}

Std_ReturnType CanIf_Transmit( PduIdType TxPduId, const PduInfoType* PduInfoPtr )
{
    if ( NULL == PduInfoPtr )
    {
        return CanIfReturnValue;
    }
    printf( "CanIf_Transmit:\r\n" );
    printf( "\tTxPduId: %d\r\n", TxPduId );
    printf( "\tPduInfoPtr:\r\n" );
    printf( "\t\tPduInfoPtr->SduLenght: %d\r\n", PduInfoPtr->SduLenght );
    
    unsigned char * data = PduInfoPtr->SduDataPtr;
    unsigned char nibble = data[0] >> 4;

	CanTp_PCI_FrameType frameType = ( CanTp_PCI_FrameType ) nibble;

    if ( PCI_FLOW_CONTROL_FRAME == frameType )
    {
        CanTp_PCI_FlowControlInfoType flowType = ( CanTp_PCI_FlowControlInfoType ) ( data[0] & 0x0F );
        if ( CONTINUETOSEND == flowType )
        {
            CanIfConsecutiveFramesNumber = data[1];
        }
    }

    return CanIfReturnValue;
}

void Send_CanTp_RxIndication( PduIdType RxPduId, const PduInfoType* PduInfoPtr )
{
    if ( NULL == PduInfoPtr )
    {
        return;
    }
    printf( "Send_CanTp_RxIndication:\r\n" );
    printf( "\tRxPduId: %d\r\n", RxPduId );
    printf( "\tPduInfoPtr:\r\n" );
    printf( "\t\tPduInfoPtr->SduLenght: %d\r\n", PduInfoPtr->SduLenght );

    CanTp_RxIndication(RxPduId, PduInfoPtr);
}
