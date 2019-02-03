#include "CanIf.h"

static Std_ReturnType return_value = E_OK;

void Set_CanIf_Return_Value( Std_ReturnType RT )
{
    return_value = RT;
}

Std_ReturnType CanIf_Transmit( PduIdType TxPduId, const PduInfoType* PduInfoPtr )
{
    return return_value;
}