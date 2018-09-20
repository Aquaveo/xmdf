#ifndef TESTGRIDdotH
#define TESTGRIDdotH

int tgReadGrid(xid a_Id, FILE *a_Outfile);

int tgWriteTestGridCart2D(LPCSTR Filename, int Compression);
int tgWriteTestGridCurv2D(LPCSTR Filename, int Compression);
int tgWriteTestGridCart3D(LPCSTR Filename, int Compression);

#endif
