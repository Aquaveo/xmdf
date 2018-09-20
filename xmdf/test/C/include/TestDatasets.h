#ifndef TESTDATASETSdotH
#define TESTDATASETSdotH

int tdEditScalarAValues(LPCSTR a_Filename, int a_Compression);
int tdReadDatasets (xid a_xGroupId, FILE *a_fp);
int tdReadActivityScalarAIndex(LPCSTR a_Filename, int a_Index);
int tdReadScalarAIndex(LPCSTR a_Filename, int a_Index);
int tdReadScalarAIndices(LPCSTR a_Filename, int a_nIndices, int *a_indices);
int tdReadVectorAIndices (LPCSTR a_Filename, int a_nIndices, int *a_indices);
int tdReadVector2DAIndex(LPCSTR a_Filename, int a_Index);
int tdWriteScalarA(LPCSTR a_Filename, int a_Compression);
int tdWriteScalarAPieces (LPCSTR a_Filename, int a_Compression);
int tdWriteScalarAPiecesAltMinMax (LPCSTR a_Filename, int a_Compression);
int tdWriteScalarB(LPCSTR a_Filename, int a_Compression, int a_Overwrite);
int tdWriteCoordsToMulti (xid a_xFileId);
int tdWriteScalarAToMulti(xid a_GroupId);
int tdWriteVector2D_A (LPCSTR a_Filename, int a_Compression);
int tdWriteVector2D_A_Pieces (LPCSTR a_Filename, int a_Compression);
int tdWriteVector2D_B (LPCSTR a_Filename, int a_Compression, int a_Overwrite);
int tdWriteVector2DAToMulti (xid a_GroupId);

#endif
