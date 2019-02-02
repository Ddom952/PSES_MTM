typedef unsigned char PduIdType;
typedef unsigned char PduLengthType;

struct PduInfoType{
    unsigned char* SduDataPtr;
    unsigned char* MetaDataPtr;
    PduLengthType SduLenght;
};

enum TPParameterType {TP_STMIN, TP_BS, TP_BC};
enum BufReq_ReturnType {BUFREQ_OK, BUFREQ_E_NOT_OK, BUFREQ_E_BUSY, BUFREQ_E_OVFL};
enum TpDataStateType {TP_DATACONF, TP_DATARETRY, TP_CONFPENDING};

struct RetryInfoType{
    TpDataStateType TpDataState;
    PduLengthType TxTpDataCnt;
};
