#ifndef STD_RETURNTYPEDEFINED 
#define STD_RETURNTYPEDEFINED 
#define E_OK 0x00u 

typedef unsigned char Std_ReturnType; // tutaj bylo StatusType...
#endif 

#define E_NOT_OK 0x01u

struct Std_VersionInfoType{
    unsigned short int vendorID;
    unsigned short int moduleID;
    unsigned char sw_major_version;
    unsigned char sw_minor_version;
    unsigned char sw_patch_version;
};