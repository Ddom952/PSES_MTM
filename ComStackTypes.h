#ifndef COMSTACKTYPES_H_
#define COMSTACKTYPES_H_

#include "StandardTypes.h"

typedef unsigned char PduIdType;
typedef unsigned char PduLengthType;

typedef struct {
    unsigned char* SduDataPtr;
    unsigned char* MetaDataPtr;
    PduLengthType SduLenght;
} PduInfoType;

typedef enum {TP_STMIN, TP_BS, TP_BC} TPParameterType;
typedef enum {BUFREQ_OK, BUFREQ_E_NOT_OK, BUFREQ_E_BUSY, BUFREQ_E_OVFL} BufReq_ReturnType;
typedef enum {TP_DATACONF, TP_DATARETRY, TP_CONFPENDING} TpDataStateType;

typedef struct {
    TpDataStateType TpDataState;
    PduLengthType TxTpDataCnt;
} RetryInfoType;

#endif // COMSTACKTYPES_H_