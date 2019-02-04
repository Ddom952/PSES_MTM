#ifndef CANIF_H_
#define CANIF_H_

#include "ComStackTypes.h"

void Set_CanIf_Return_Value( Std_ReturnType RT );
Std_ReturnType CanIf_Transmit( PduIdType TxPduId, const PduInfoType* PduInfoPtr );
void Send_CanTp_RxIndication( PduIdType RxPduId, const PduInfoType* PduInfoPtr );

#endif // CANIF_H_