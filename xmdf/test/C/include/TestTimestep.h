#ifndef TESTTIMESTEPdotH
#define TESTTIMESTEPdotH

int ttReadDatasets (xid a_xGroupId, FILE *a_fp);
int ttReadActivityScalarAIndex(LPCSTR a_Filename, int a_Index);
int ttReadScalarAIndex(LPCSTR a_Filename, int a_Index);
int ttReadVector2DAIndex(LPCSTR a_Filename, int a_Index);
int ttWriteScalarA(LPCSTR a_Filename, int a_Compression);
int ttWriteScalarB(LPCSTR a_Filename, int a_Compression, int a_Overwrite);
int ttWriteCoordsToMulti (xid a_xFileId);
int ttWriteScalarAToMulti(xid a_GroupId);
int ttWriteVector2D_A (LPCSTR a_Filename, int a_Compression);
int ttWriteVector2D_B (LPCSTR a_Filename, int a_Compression, int a_Overwrite);
int ttWriteVector2DAToMulti (xid a_GroupId);

#endif
