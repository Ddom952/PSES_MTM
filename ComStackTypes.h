#ifndef COMSTACKTYPES_H_
#define COMSTACKTYPES_H_

typedef unsigned char PduIdType;
typedef unsigned char PduLengthType;

typedef struct {
    unsigned char* SduDataPtr;
    unsigned char* MetaDataPtr;
    PduLengthType SduLenght;
} PduInfoType;

typedef enum {TP_STMIN, TP_BS, TP_BC} TPParameterType;
enum BufReq_ReturnType {BUFREQ_OK, BUFREQ_E_NOT_OK, BUFREQ_E_BUSY, BUFREQ_E_OVFL};
typedef enum {TP_DATACONF, TP_DATARETRY, TP_CONFPENDING} TpDataStateType;

struct {
    TpDataStateType TpDataState;
    PduLengthType TxTpDataCnt;
} RetryInfoType;

#endif // COMSTACKTYPES_H_