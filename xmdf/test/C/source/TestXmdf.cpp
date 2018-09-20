// TestXFormat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <hdf5.h>
#include <xmdf/Xmdf.h>
#include "xmdf/xmdf_private.h"
#include <stdio.h>

#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include "TestTimestep.h"
#include "TestDatasets.h"
#include "TestMesh.h"
#include "TestGrid.h"
#include "TestGeomPaths.h"
#include "TestXmdf.h"

#define NUMTIMES1 5
#define NUMVALUES1 5
#define NUMACTIVE1 3

#define NUMTIMESADD 1

int txiTestCalendar();
int txiTestCoordSystem(xid xGroupId, FILE *a_OutFile);
int txiTestDatasets();
int txiTestFortran();
int txiTestGeometricPaths();
int txiTestGrids();
int txiTestMeshs();
int txiTestOverwriteDsets();
int txiTestTimesteps();
int txiTestVersion();
int txiReadXFormatFile(LPCSTR a_XmdfFile, LPCSTR a_OutFile);
int txiReadXFormatFileT (LPCSTR a_XmdfFile, LPCSTR a_OutFile);


//-----------------------------------------------------------------------------
// FUNCTION  main
// PURPOSE   runs the test cases for XMDF
// NOTES
//-----------------------------------------------------------------------------
int main()
{
  int status = 0;
  ::txiReadXFormatFile("C:\\SMS\\Testcases\\bugs\\adcp_observed\\sms_adcp_observed - pete_comb_vel_vector (2).h5",
                       "C:\\SMS\\Testcases\\bugs\\adcp_observed\\sms_adcp_observed out.txt");

#if 1
  // test the timestep routines
  status = txiTestTimesteps();
  printf("=== Finished testing timesteps ===\n");
  if (status < 0) return status;

  // test the dataset routines
  status = txiTestDatasets();
  printf("=== Finished testing datasets ===\n");
  if (status < 0) return status;

  // test overwriting already-existing datasets
  status = txiTestOverwriteDsets();
  if (status < 0) return status;

  // Test Mesh stuff
  status = txiTestMeshs();
  if (status < 0) return status;

  // Test grid stuff
  status = txiTestGrids();
  if (status < 0) return status;

  // Test writing particle paths


  status = txiTestGeometricPaths();
  if (status < 0) return status;

  // Test reading files that were written by fortan.
  status = txiTestFortran();
  if (status < 0) return status;

  // Test Calendar Conversion Stuff
  status = txiTestCalendar();
  if (status < 0) return status;

  // Test reading the version written to the files
  status = txiTestVersion();
  if (status < 0) return status;
#endif


  printf("Press ENTER to Exit...");
  char pause[1024];

  //scanf("%c", pause);
   
  return 0;
}

//-----------------------------------------------------------------------------
// FUNCTION  txiTestTimesteps
// PURPOSE
// NOTES
//-----------------------------------------------------------------------------
int txiTestTimesteps()
{
  const char *SCALAR_A_FILE_C = "TT_ScalarA_c.h5";
  const char *SCALAR_A_TEXT_C = "TT_ScalarA_c.txt";
  const char *VECTOR2D_A_FILE_C = "TT_Vector2D_A_c.h5";
  const char *VECTOR2D_A_TEXT_C = "TT_Vector2D_A_c.txt";

  const char *MULTIDATASET_FILE_C = "TT_MultiDataSet_c.h5";
  const char *MULTIDATASET_TEXT_C = "TT_MultiDataSet_c.txt";

  int status = 1;
  int compression = NONE;
  xid  MultiFileId=NONE, MultiGroupId=NONE;
  int         NumOpen=0;

  char SdoGuid[37];

  strcpy(SdoGuid, "73289C80-6235-4fdc-9649-49E4F5AEB676");

  xfSetupToWriteDatasets(MULTIDATASET_FILE_C, "Multidatasets","", 
           SdoGuid, XF_OVERWRITE_CLEAR_FILE, &MultiFileId, &MultiGroupId);

  // Write coordinates to multidatasets
  ttWriteCoordsToMulti(MultiFileId);

 // Write scalar A and Vector A to multidatasets.
  ttWriteScalarAToMulti(MultiGroupId);

  ttWriteVector2DAToMulti(MultiGroupId);

  xfCloseGroup(MultiGroupId);
  xfCloseFile(MultiFileId);
  printf("Done writing multiple datasets...\n");


  // scalar datasets
  status = ttWriteScalarA(SCALAR_A_FILE_C, compression);
  if (status < 0) {
    return status;
  }
  
  printf("Done writing scalar datasets...\n");

  // vector datasets
  status = ttWriteVector2D_A(VECTOR2D_A_FILE_C, compression);
  if (status < 0) {
    printf("Error writing dataset vector2D_A.");
    return status;
  }

  printf("Done writing vector datasets...\n");

  printf("Done writing datasets...\n");

  // Read the files back in
  status = txiReadXFormatFileT(SCALAR_A_FILE_C, SCALAR_A_TEXT_C);
  if (status < 0) {
    return status;
  }

  status = txiReadXFormatFileT(VECTOR2D_A_FILE_C, VECTOR2D_A_TEXT_C);
  if (status < 0) {
    return status;
  }

  status = txiReadXFormatFileT(MULTIDATASET_FILE_C, MULTIDATASET_TEXT_C);
  if (status < 0) {
    return status;
  }

  printf("Done reading datasets...\n");

  xfGetNumOpenIdentifiers(H5F_OBJ_ALL, &NumOpen);

  xfpCloseOpenIdentifiers(H5F_OBJ_ALL);

  xfSetupToWriteDatasets(MULTIDATASET_FILE_C, "Multidatasets","", 
     SdoGuid, XF_OVERWRITE_CLEAR_DATASET_GROUP, &MultiFileId, &MultiGroupId);

  ttWriteScalarAToMulti(MultiGroupId);

  xfSetupToWriteDatasets(MULTIDATASET_FILE_C, "Multidatasets","", 
     SdoGuid, XF_OVERWRITE_NONE, &MultiFileId, &MultiGroupId);

  ttWriteVector2DAToMulti(MultiGroupId);


  // Test reading information at index for multiple timesteps
  status = ttReadScalarAIndex(SCALAR_A_FILE_C, 2);
  if (status < 0) {
    return status;
  }
  printf("Done reading scalar data at index.\n");
  
  status = ttReadVector2DAIndex(VECTOR2D_A_FILE_C, 4);
  if (status < 0) {
    return status;
  }
  printf("Done reading vector data at index.\n");

  status = ttReadActivityScalarAIndex(SCALAR_A_FILE_C, 5);
  if (status < 0) {
    return status;
  }

  return status;
} // txiTestTimesteps

//-----------------------------------------------------------------------------
// #defines used in txiTestDatasets
//-----------------------------------------------------------------------------
#define XMDF_VERSION_OUT_C "XMDF_Version_c.txt"

#define SCALAR_A_FILE_C "ScalarA_c.h5"
#define SCALAR_A_TEXT_C "ScalarA_c.txt"
#define SCALAR_A_PIECES_FILE_C "ScalarA_Pieces_c.h5"
#define SCALAR_A_PIECES_TEXT_C "ScalarA_Pieces_c.txt"
#define SCALAR_A_EDITED_FILE_C "ScalarA_edited_c.h5"
#define SCALAR_A_EDITED_TEXT_C "ScalarA_edited_c.txt"
#define VECTOR2D_A_FILE_C "Vector2D_A_c.h5"
#define SCALAR_A_PIECES_ALT_FILE_C "ScalarA_Pieces_alt_c.h5"
#define SCALAR_A_PIECES_ALT_TEXT_C "ScalarA_Pieces_alt_c.txt"
#define SCALAR_A_EDITED_ALT_FILE_C "ScalarA_edited_alt_c.h5"
#define SCALAR_A_EDITED_ALT_TEXT_C "ScalarA_edited_alt_c.txt"
#define VECTOR2D_A_TEXT_C "Vector2D_A_c.txt"
#define VECTOR2D_A_FILE_PIECES_C "Vector2D_A_Pieces_c.h5"

#define SCALAR_A_FILE_F "ScalarA_f.h5"
#define SCALAR_A_TEXT_FC "ScalarA_fc.txt"
#define VECTOR2D_A_FILE_F "Vector2D_A_f.h5"
#define VECTOR2D_A_TEXT_FC "Vector2D_A_fc.txt"

#define MULTIDATASET_FILE_C "MultiDataSet_c.h5"
#define MULTIDATASET_TEXT_C "MultiDataSet_c.txt"

//-----------------------------------------------------------------------------
// FUNCTION  txiTestDatasets
// PURPOSE
// NOTES
//-----------------------------------------------------------------------------
int txiTestDatasets()
{
  int status = 1;
  int compression = NONE;
  xid  MultiFileId=NONE, MultiGroupId=NONE;
  int         NumOpen=0;

  char SdoGuid[37];

  strcpy(SdoGuid, "73289C80-6235-4fdc-9649-49E4F5AEB676");

  xfSetupToWriteDatasets(MULTIDATASET_FILE_C, "Multidatasets","", 
           SdoGuid, XF_OVERWRITE_CLEAR_FILE, &MultiFileId, &MultiGroupId);

  // Write coordinates to multidatasets
  tdWriteCoordsToMulti(MultiFileId);

 // Write scalar A and Vector A to multidatasets.
  tdWriteScalarAToMulti(MultiGroupId);

  tdWriteVector2DAToMulti(MultiGroupId);

  xfCloseGroup(MultiGroupId);
  xfCloseFile(MultiFileId);
  printf("Done writing multiple datasets...\n");


  // scalar datasets
  status = tdWriteScalarA(SCALAR_A_FILE_C, compression);
  if (status < 0) {
    return status;
  }

  status = tdWriteScalarAPieces(SCALAR_A_PIECES_FILE_C, compression);
  if (status < 0) {
    return status;
  }

  status = tdWriteScalarAPiecesAltMinMax(SCALAR_A_PIECES_ALT_FILE_C,
                                         compression);
  if (status < 0) {
    return status;
  }
  
  printf("Done writing scalar datasets...\n");

  // vector datasets
  status = tdWriteVector2D_A(VECTOR2D_A_FILE_C, compression);
  printf("Done writing dataset vector2D_A. Status %d\n", status);
  if (status < 0) {
    return status;
  }

  status = tdWriteVector2D_A_Pieces(VECTOR2D_A_FILE_PIECES_C, compression);
  printf("Done writing dataset vector2D_A_Pieces. Status %d\n", status);
  if (status < 0) {
   return status;
  }

  printf("Done writing vector datasets...\n");

  status = tdEditScalarAValues(SCALAR_A_EDITED_FILE_C, compression);
  printf("Done writing edited scalar datasets. Status %d\n", status);
  if (status < 0) {
    return status;
  }

  printf("Done writing datasets...\n");

  // Read the files back in
  status = txiReadXFormatFile(SCALAR_A_FILE_C, SCALAR_A_TEXT_C);
  if (status < 0) {
    return status;
  }

  status = txiReadXFormatFile(VECTOR2D_A_FILE_C, VECTOR2D_A_TEXT_C);
  if (status < 0) {
    return status;
  }

  status = txiReadXFormatFile(MULTIDATASET_FILE_C, MULTIDATASET_TEXT_C);
  if (status < 0) {
    return status;
  }

  status = txiReadXFormatFile(SCALAR_A_EDITED_FILE_C, SCALAR_A_EDITED_TEXT_C);
  if (status < 0) {
    return status;
  }

  printf("Done reading datasets...\n");

  xfGetNumOpenIdentifiers(H5F_OBJ_ALL, &NumOpen);

  xfpCloseOpenIdentifiers(H5F_OBJ_ALL);

  xfSetupToWriteDatasets(MULTIDATASET_FILE_C, "Multidatasets","", 
     SdoGuid, XF_OVERWRITE_CLEAR_DATASET_GROUP, &MultiFileId, &MultiGroupId);

  tdWriteScalarAToMulti(MultiGroupId);

  xfSetupToWriteDatasets(MULTIDATASET_FILE_C, "Multidatasets","", 
     SdoGuid, XF_OVERWRITE_NONE, &MultiFileId, &MultiGroupId);

  tdWriteVector2DAToMulti(MultiGroupId);


  // Test reading information at index for multiple timesteps
  status = tdReadScalarAIndex(SCALAR_A_FILE_C, 2);
  printf("Done reading scalar data at index. Status %d\n", status);
  if (status < 0) {
    return status;
  }
  
  status = tdReadVector2DAIndex(VECTOR2D_A_FILE_C, 4);
  printf("Done reading vector data at index. Status %d\n", status);
  if (status < 0) {
    return status;
  }

  status = tdReadActivityScalarAIndex(SCALAR_A_FILE_C, 5);
  printf("Done reading scalar activity at index. Status %d\n", status);
  if (status < 0) {
    return status;
  }

    // Test reading information at multiple indices
  const int  nIndices = 3;
  int indices[nIndices];
  indices[0] = 2;
  indices[1] = 3;
  indices[2] = 5;
  status = tdReadScalarAIndices(SCALAR_A_FILE_C, nIndices, indices);
  printf("Done reading scalar data at indices. Status %d\n", status);
  if (status < 0) {
    return status;
  }

  status = tdReadVectorAIndices(VECTOR2D_A_FILE_C, nIndices, indices);
  printf("Done reading vector data at indices. Status %d\n", status);

  return status;
} // txiTestDatasets

#define GEOMPATH_A_FILE_C "geompath_a_file_c.h5"
#define GEOMPATH_A_FILE_COUT "geompath_a_file_c_out.txt"

int txiTestGeometricPaths()
{
  int status = 1;
  int compression = NONE;
    /* test writing a geometric path file */
  printf("\n\nWriting geometric path data.\n\n");
 
  status = tmWriteTestPaths(GEOMPATH_A_FILE_C, compression);
  if (status <= 0) {
    printf("Error writing geometric path data A\n");
  }
  printf("Finished writing geometric path data A\n");

    /* test reading a geometric path file */
  status = tmReadTestPaths(GEOMPATH_A_FILE_C, GEOMPATH_A_FILE_COUT);

  return status;
}

//-----------------------------------------------------------------------------
// #defines used in txiTestOverwriteDsets
//-----------------------------------------------------------------------------

#define SCALAR_B_FILE_C "ScalarB_c.h5"
#define SCALAR_B_TEXT_C "ScalarB_c.txt"
#define VECTOR2D_B_FILE_C "Vector2D_B_c.h5"
#define VECTOR2D_B_TEXT_C "Vector2D_B_c.txt"

#define SCALAR_B_FILE_F "ScalarB_f.h5"
#define SCALAR_B_TEXT_FC "ScalarB_fc.txt"
#define VECTOR2D_B_FILE_F "Vector2D_B_f.h5"
#define VECTOR2D_B_TEXT_FC "Vector2D_B_fc.txt"

//-----------------------------------------------------------------------------
// FUNCTION  txiTestOverwriteDsets
// PURPOSE   Check to see if already-written datasets can be overwritten
// NOTES
//-----------------------------------------------------------------------------
int txiTestOverwriteDsets()
{
  int status = 1;
  int compression = NONE;

    // scalar datasets
  status = tdWriteScalarB(SCALAR_B_FILE_C, compression, 0);
  if (status < 0) {
    return status;
  }
    // overwrite scalar datasets
  status = tdWriteScalarB(SCALAR_B_FILE_C, compression, 1);
  if (status < 0) {
    return status;
  }
  
    // vector datasets
  status = tdWriteVector2D_B(VECTOR2D_B_FILE_C, compression, 0);
  if (status < 0) {
    printf("Error writing dataset vector2D_B.");
    return status;
  }
    // overwrite vector datasets
  status = tdWriteVector2D_B(VECTOR2D_B_FILE_C, compression, 1);
  if (status < 0) {
    printf("Error writing dataset vector2D_B.");
    return status;
  }

  // Read the files back in
  status = txiReadXFormatFile(SCALAR_B_FILE_C, SCALAR_B_TEXT_C);
  if (status < 0) {
    return status;
  }

  status = txiReadXFormatFile(VECTOR2D_B_FILE_C, VECTOR2D_B_TEXT_C);
  if (status < 0) {
    return status;
  }

  return status;
} // txiTestDatasets

//-----------------------------------------------------------------------------
// #defines used in txiTestGrids
//-----------------------------------------------------------------------------

#define GRID_CART2D_A_FILE_C "grid_cart2d_a_file_c.h5"
#define GRID_CURV2D_A_FILE_C "grid_curv2d_a_file_c.h5"
#define GRID_CART3D_A_FILE_C "grid_cart3d_a_file_c.h5"

#define GRID_CART2D_A_OUT_C "grid_cart2d_a_out_c.txt"
#define GRID_CURV2D_A_OUT_C "grid_curv2d_a_out_c.txt"
#define GRID_CART3D_A_OUT_C "grid_cart3d_a_out_c.txt"

#define GRID_CART2D_A_FILE_F "grid_cart2d_a_file_f.h5"
#define GRID_CURV2D_A_FILE_F "grid_curv2d_a_file_f.h5"
#define GRID_CART3D_A_FILE_F "grid_cart3d_a_file_f.h5"

#define GRID_CART2D_A_OUT_FC "grid_cart2d_a_out_fc.txt"
#define GRID_CURV2D_A_OUT_FC "grid_curv2d_a_out_fc.txt"
#define GRID_CART3D_A_OUT_FC "grid_cart3d_a_out_fc.txt"

//-----------------------------------------------------------------------------
// FUNCTION  txiTestGrids
// PURPOSE
// NOTES
//-----------------------------------------------------------------------------
int txiTestGrids()
{
  int status = 1;
  int compression = NONE;

  printf("\n\nWriting grid data.\n\n");
 
  status = tgWriteTestGridCart2D(GRID_CART2D_A_FILE_C, compression);
  if (status < 0) {
    printf("Error writing grid Cartesian 2D A\n");
  }
  printf("Finished writing grid Cartesian 2D A\n");

  status = tgWriteTestGridCurv2D(GRID_CURV2D_A_FILE_C, compression);
  if (status < 0) {
    printf("Error writing grid Curvilinear 2D A\n");
  }
  printf("Finished writing grid Curvilinear 2D A\n");

  status = tgWriteTestGridCart3D(GRID_CART3D_A_FILE_C, compression);
  if (status < 0) {
    printf("Error writing grid Cartesian 3D A\n");
  }
  printf("Finished writing grid Cartesian 3D A\n"); 
  

  // read the files back in
  status = txiReadXFormatFile(GRID_CART2D_A_FILE_C, GRID_CART2D_A_OUT_C);
  if (status < 0) {
    printf("Error reading grid Cartesian 2D A\n");
  }
  printf("Finished reading grid Cartesian 2D A\n");

  status = txiReadXFormatFile(GRID_CURV2D_A_FILE_C, GRID_CURV2D_A_OUT_C);
  if (status < 0) {
    printf("Error reading grid Curvilinear 2D A\n");
  }
  printf("Finished reading grid Curvilinear 2D A\n");

  status = txiReadXFormatFile(GRID_CART3D_A_FILE_C, GRID_CART3D_A_OUT_C);
  if (status < 0) {
    printf("Error reading grid Cartesian 3D A\n");
  }
  printf("Finished reading grid Cartesian 3D A\n"); 
 

  return status;
} // txiTestGrids

//-----------------------------------------------------------------------------
// #defines used in txiTestMeshs
//-----------------------------------------------------------------------------

#define MESH_A_FILE_C  "mesh_a_file_c.h5"
#define MESH_B_FILE_C  "mesh_b_file_c.h5"
#define MESH_A_OUT_C   "mesh_a_file_c.txt"
#define MESH_B_OUT_C   "mesh_b_file_c.txt"

#define MESH_A_FILE_F  "mesh_a_file_f.h5"
#define MESH_B_FILE_F  "mesh_b_file_f.h5"
#define MESH_A_OUT_FC  "mesh_a_file_fc.txt"
#define MESH_B_OUT_FC  "mesh_b_file_fc.txt"

// ---------------------------------------------------------------------------
// FUNCTION  txiTestMeshs
// PURPOSE   
// NOTES     
// ---------------------------------------------------------------------------
int txiTestMeshs ()
{
  int status = 1;
  int compression = NONE;

  status = tmWriteTestMeshA(MESH_A_FILE_C, compression);
  if (status != TRUE) {
    printf("Error writing TestMeshA\n");
    return status;
  }

  status = tmWriteTestMeshB(MESH_B_FILE_C, compression);
  if (status != TRUE) {
    printf("Error writing TestMeshB\n");
    return status;
  }

  printf("Finished writing meshes.\n");

  // read the files back in
  status = txiReadXFormatFile(MESH_A_FILE_C, MESH_A_OUT_C);
  if (status != TRUE) {
    printf("Error reading TestMeshA\n");
    return status;
  }

  // read the files back in
  status = txiReadXFormatFile(MESH_B_FILE_C, MESH_B_OUT_C);
  if (status != TRUE) {
    printf("Error reading TestMeshB\n");
    return status;
  }

  printf("Finished reading meshes.\n");

  return status;
} // txiTestMeshs
// ---------------------------------------------------------------------------
// FUNCTION  txiTestFortran
// PURPOSE   test to see if C code can read file written with fortran.
// NOTES     
// ---------------------------------------------------------------------------
int txiTestFortran ()
{
  xid    xFileId = 0;
  herr_t (*old_func)(void*);
  int     status = 1;
  int     nStatus = 1;
  void    *old_client_data;

    // Check to see if files written with C exist
  H5Eget_auto1(&old_func, &old_client_data);
    // Turn off error handling
  H5Eset_auto1(NULL, NULL);

    // Try opening a file written with C to see if one exists.
  nStatus = xfOpenFile(SCALAR_A_FILE_F, &xFileId, XTRUE);
    // If the file written with FORTRAN doesn't exist, return.
  if (nStatus < 0) {
    xfCloseFile(xFileId);
      // Restore previous error handler
    H5Eset_auto1(old_func, old_client_data);
    return FALSE;
      // If the file written with C does exist, assume all C files exist.
  }
  else {
    xfCloseFile(xFileId);
      // Restore previous error handler
    H5Eset_auto1(old_func, old_client_data);
  }

  // Read the files back in
  status = txiReadXFormatFile(SCALAR_A_FILE_F, SCALAR_A_TEXT_FC);
  if (status < 0) {
    return status;
  }
  status = txiReadXFormatFile(SCALAR_B_FILE_F, SCALAR_B_TEXT_FC);
  if (status < 0) {
    return status;
  }

  status = txiReadXFormatFile(VECTOR2D_A_FILE_F, VECTOR2D_A_TEXT_FC);
  if (status < 0) {
    return status;
  }
  status = txiReadXFormatFile(VECTOR2D_B_FILE_F, VECTOR2D_B_TEXT_FC);
  if (status < 0) {
    return status;
  }

  printf("Done reading fortran datasets...\n");

  status = txiReadXFormatFile(GRID_CART2D_A_FILE_F, GRID_CART2D_A_OUT_FC);
  if (status < 0) {
    printf("Error reading fortran grid Cartesian 2D A\n");
  }
  printf("Finished reading fortran grid Cartesian 2D A\n");

  status = txiReadXFormatFile(GRID_CURV2D_A_FILE_F, GRID_CURV2D_A_OUT_FC);
  if (status < 0) {
    printf("Error reading fortran grid Curvilinear 2D A\n");
  }
  printf("Finished reading fortran grid Curvilinear 2D A\n");

  status = txiReadXFormatFile(GRID_CART3D_A_FILE_F, GRID_CART3D_A_OUT_FC);
  if (status < 0) {
    printf("Error reading grid fortran Cartesian 3D A\n");
  }
  printf("Finished reading fortran grid Cartesian 3D A\n");


  // read the files back in
  status = txiReadXFormatFile(MESH_A_FILE_F, MESH_A_OUT_FC);
  if (status != TRUE) {
    printf("Error reading fortran TestMeshA\n");
    return status;
  }

  // read the files back in
  status = txiReadXFormatFile(MESH_B_FILE_F, MESH_B_OUT_FC);
  if (status != TRUE) {
    printf("Error reading fortran TestMeshB\n");
    return status;
  }

  printf("Finished reading fortran meshes.\n");
  
  return status;
} // txiTestFortran
// --------------------------------------------------------------------------
// FUNCTION txiReadXFormatFile
// PURPOSE  Read a file using XMDF and write information about the data
//          contained in the file to a output file
// --------------------------------------------------------------------------
int txiReadXFormatFile (LPCSTR a_XmdfFile, LPCSTR a_OutFile)
{
  int    nMeshGroups, nMaxPathLength, nGridGroups;
  char  *Paths = NULL, *IndividualPath = NULL;
  int    nStatus, i;
  float  Version;
  xid    xFileId = NONE, xGroupId = NONE;
  FILE  *fp = NULL;
//  int    NameSize;
//  char  *Name = NULL;
//  H5I_type_t IdentifierType;

  // Open the XMDF file
  nStatus = xfOpenFile(a_XmdfFile, &xFileId, XTRUE);
  if (nStatus < 0) {
    return -1;
  }

  // open the status file
  fp = fopen(a_OutFile, "w");
  if (fp == NULL) {
    xfCloseFile(xFileId);
    return -1;
  }

  fprintf(fp, "File %s opened.\n", a_XmdfFile);

  // write the version number to the file
  xfGetLibraryVersionFile(xFileId, &Version);
  fprintf(fp, "XMDF Version: %f\n", Version);

  // Write Coordinate System Informatioin to the .txt file
  nStatus = txiTestCoordSystem(xFileId, fp);
  fprintf(fp, "\n");
  if (nStatus < 0) {
    xfCloseFile(xFileId);
    return -1;
  }

  // read all datasets not beneath a mesh, grid, or cross-sections
  nStatus = tdReadDatasets(xFileId, fp);
  if (nStatus < 0) {
    xfCloseFile(xFileId);
    return -1;
  } 

  // Get the number and paths of datasets in the file.
  nStatus = xfGetGroupPathsSizeForMeshes(xFileId, &nMeshGroups,
                                                    &nMaxPathLength);
  if (nStatus >= 0) {
    Paths = (char *)malloc(nMaxPathLength*nMeshGroups*sizeof(char));
    nStatus = xfGetGroupPathsForMeshes(xFileId, nMeshGroups, 
                                         nMaxPathLength, Paths);
  }
  if (nStatus < 0) {
    xfCloseFile(xFileId);
    return -1;
  }

  // Report the number and paths to individual meshes in the file.
  fprintf(fp, "Number of meshes in file: %d\n", nMeshGroups);
  fprintf(fp, "Paths:\n");
  for (i = 0; i < nMeshGroups; i++) {
    IndividualPath = &Paths[i*nMaxPathLength];  
    fprintf(fp, "  %s\n", IndividualPath);
  }
  fprintf(fp, "\n");

  // Open each mesh group
  for (i = 0; i < nMeshGroups; i++) {
    IndividualPath = &Paths[i*nMaxPathLength];  
    
    fprintf(fp, "Reading mesh in group: %s\n", IndividualPath);
    nStatus = xfOpenGroup(xFileId, IndividualPath, &xGroupId);
    if (nStatus >= 0) {
      // RDJ to delete
//      IdentifierType = H5Iget_type(xGroupId);
//      NameSize = H5Iget_name(xGroupId, NULL, NULL);
//      Name = new char[NameSize + 1];
//      NameSize = H5Iget_name(xGroupId, Name, NULL);
//      if (Name) {
//        delete [] Name;
//      }

      nStatus = tmReadMesh(xGroupId, fp);
    }
    if (nStatus < 0) {
      printf("Error reading mesh..\n");
    }
  }

  if (Paths) {
    free(Paths);
    Paths = NULL;
  }

  // Grid stuff
  nStatus = xfGetGroupPathsSizeForGrids(xFileId, &nGridGroups,
                                                    &nMaxPathLength);
  if (nStatus >= 0) {
    Paths = (char *)malloc(nMaxPathLength*nGridGroups*sizeof(char));
    nStatus = xfGetGroupPathsForGrids(xFileId, nGridGroups, 
                                         nMaxPathLength, Paths);
  }
  if (nStatus < 0) {
    xfCloseFile(xFileId);
    return -1;
  }

  // Report the number and paths to individual meshes in the file.
  fprintf(fp, "Number of grids in file: %d\n", nGridGroups);
  fprintf(fp, "Paths:\n");
  for (i = 0; i < nGridGroups; i++) {
    IndividualPath = &Paths[i*nMaxPathLength];  
    fprintf(fp, "  %s\n", IndividualPath);
  }
  fprintf(fp, "\n");

  // Open each grid group
  for (i = 0; i < nGridGroups; i++) {
    IndividualPath = &Paths[i*nMaxPathLength];  
    
    fprintf(fp, "Reading grid in group: %s\n", IndividualPath);
    nStatus = xfOpenGroup(xFileId, IndividualPath, &xGroupId);
    if (nStatus >= 0) {
      nStatus = tgReadGrid(xGroupId, fp);
    }
    if (nStatus < 0) {
      printf("Error reading grid..\n");
    }
  }

  if (Paths) {
    free(Paths);
    Paths = NULL;
  }
  
  // TODO do grid, and cross-section stuff.
   

  // close the files
  xfCloseFile(xFileId);
  fclose(fp);
  
  return TRUE;
} // txiReadXFormatFile

// --------------------------------------------------------------------------
// FUNCTION txiReadXFormatFileT
// PURPOSE  Read a file using XMDF and write information about the data
//          contained in the file to a output file
// --------------------------------------------------------------------------
int txiReadXFormatFileT (LPCSTR a_XmdfFile, LPCSTR a_OutFile)
{
  int    nMeshGroups, nMaxPathLength, nGridGroups;
  char  *Paths = NULL, *IndividualPath = NULL;
  int    nStatus, i;
  float  Version;
  xid    xFileId = NONE, xGroupId = NONE;
  FILE  *fp = NULL;

  // Open the XMDF file
  nStatus = xfOpenFile(a_XmdfFile, &xFileId, XTRUE);
  if (nStatus < 0) {
    return -1;
  }

  // open the status file
  fp = fopen(a_OutFile, "w");
  if (fp == NULL) {
    xfCloseFile(xFileId);
    return -1;
  }

  fprintf(fp, "File %s opened.\n", a_XmdfFile);

  // write the version number to the file
  xfGetLibraryVersionFile(xFileId, &Version);
  fprintf(fp, "XMDF Version: %f\n", Version);

  // Write Coordinate System Informatioin to the .txt file
  nStatus = txiTestCoordSystem(xFileId, fp);
  fprintf(fp, "\n");
  if (nStatus < 0) {
    xfCloseFile(xFileId);
    return -1;
  }

  // read all datasets not beneath a mesh, grid, or cross-sections
  nStatus = ttReadDatasets(xFileId, fp);
  if (nStatus < 0) {
    xfCloseFile(xFileId);
    return -1;
  } 

  // Get the number and paths of datasets in the file.
  nStatus = xfGetGroupPathsSizeForMeshes(xFileId, &nMeshGroups,
                                                    &nMaxPathLength);
  if (nStatus >= 0) {
    Paths = (char *)malloc(nMaxPathLength*nMeshGroups*sizeof(char));
    nStatus = xfGetGroupPathsForMeshes(xFileId, nMeshGroups, 
                                         nMaxPathLength, Paths);
  }
  if (nStatus < 0) {
    xfCloseFile(xFileId);
    return -1;
  }

  // Report the number and paths to individual meshes in the file.
  fprintf(fp, "Number of meshes in file: %d\n", nMeshGroups);
  fprintf(fp, "Paths:\n");
  for (i = 0; i < nMeshGroups; i++) {
    IndividualPath = &Paths[i*nMaxPathLength];  
    fprintf(fp, "  %s\n", IndividualPath);
  }
  fprintf(fp, "\n");

  // Open each mesh group
  for (i = 0; i < nMeshGroups; i++) {
    IndividualPath = &Paths[i*nMaxPathLength];  
    
    fprintf(fp, "Reading mesh in group: %s\n", IndividualPath);
    nStatus = xfOpenGroup(xFileId, IndividualPath, &xGroupId);
    if (nStatus >= 0) {
      nStatus = tmReadMesh(xGroupId, fp);
    }
    if (nStatus < 0) {
      printf("Error reading mesh..\n");
    }
  }

  if (Paths) {
    free(Paths);
    Paths = NULL;
  }

  // Grid stuff
  nStatus = xfGetGroupPathsSizeForGrids(xFileId, &nGridGroups,
                                                    &nMaxPathLength);
  if (nStatus >= 0) {
    Paths = (char *)malloc(nMaxPathLength*nGridGroups*sizeof(char));
    nStatus = xfGetGroupPathsForGrids(xFileId, nGridGroups, 
                                         nMaxPathLength, Paths);
  }
  if (nStatus < 0) {
    xfCloseFile(xFileId);
    return -1;
  }

  // Report the number and paths to individual meshes in the file.
  fprintf(fp, "Number of grids in file: %d\n", nGridGroups);
  fprintf(fp, "Paths:\n");
  for (i = 0; i < nGridGroups; i++) {
    IndividualPath = &Paths[i*nMaxPathLength];  
    fprintf(fp, "  %s\n", IndividualPath);
  }
  fprintf(fp, "\n");

  // Open each grid group
  for (i = 0; i < nGridGroups; i++) {
    IndividualPath = &Paths[i*nMaxPathLength];  
    
    fprintf(fp, "Reading grid in group: %s\n", IndividualPath);
    nStatus = xfOpenGroup(xFileId, IndividualPath, &xGroupId);
    if (nStatus >= 0) {
      nStatus = tgReadGrid(xGroupId, fp);
    }
    if (nStatus < 0) {
      printf("Error reading grid..\n");
    }
  }

  if (Paths) {
    free(Paths);
    Paths = NULL;
  }
  
  // TODO do grid, and cross-section stuff.
   

  // close the files
  xfCloseFile(xFileId);
  fclose(fp);
  
  return TRUE;
} // txiReadXFormatFileT

//-----------------------------------------------------------------------------
// FUNCTION TXI_WRITE_XMDF_VERSION
// PURPOSE  Write the XMDF version number to the screen
// NOTES
//-----------------------------------------------------------------------------
int txiTestVersion ()
{
  float    Version;

  printf("\n");

  xfGetLibraryVersion(&Version);
  printf("The current version of XMDF is: %f\n\n", Version);

  return 1;
}  // txiTestVersion

//-----------------------------------------------------------------------------
// #defines used in txiTestCalendar
//-----------------------------------------------------------------------------

#define CALENDAR_OUT_C "Calendar_c.txt"

//----------------------------------------------------------------------------
// SUBROUTINE txiTestCalendar
// PURPOSE    Check the Calculations of Julian date from calendar date or  
//            vice-versa.
// NOTES      Use #defines: ERA_IS_BCE (BC), and ERA_IS_CE (AD).
//----------------------------------------------------------------------------
int txiTestCalendar ()
{
  FILE    *fp = NULL;
  xmbool   era1, era2, era3, era4;
  int     yr1, mo1, day1, hr1, min1, sec1;
  int     yr2, mo2, day2, hr2, min2, sec2;
  int     yr3, mo3, day3, hr3, min3, sec3;
  int     yr4, mo4, day4, hr4, min4, sec4, calendarworks;
  double  julian1, julian2, julian3, julian4;

  calendarworks = 0;

  // open the status file
  fp = fopen(CALENDAR_OUT_C, "w");
  if (fp == NULL) {
    return FALSE;
  }


  fprintf(fp, "Calendar conversion:\n\n");

  era1 = ERA_IS_BCE;
  yr1 = mo1 = day1 = hr1 = min1 = sec1 = 0;
  julian1 = 2655.5;
  xfJulianToCalendar(&era1, &yr1, &mo1, &day1, &hr1, &min1, &sec1, julian1);

  yr2  = 4706;
  mo2  = 4;
  day2 = 10;
  era2 = ERA_IS_BCE;
  hr2  = min2 = sec2 = 0;
  julian2 = 0.0;
  xfCalendarToJulian(era2, yr2, mo2, day2, hr2, min2, sec2, &julian2);

  era3 = ERA_IS_CE;
  yr3  = 2004;
  mo3  = 6;
  day3 = 3;
  hr3  = 2;
  min3 = 8;
  sec3 = 32;
  julian3 = 0.0;
  xfCalendarToJulian(era3, yr3, mo3, day3, hr3, min3, sec3, &julian3);

  era4 = ERA_IS_BCE;
  yr4 = mo4 = day4 = hr4 = min4 = sec4 = 0;
  julian4 = 2453159.5892592594;
  xfJulianToCalendar(&era4, &yr4, &mo4, &day4, &hr4, &min4, &sec4, julian4);

fprintf(fp, "Dates #1 & #2  were calculated with the same date:\n\n");
fprintf(fp, "%d / %d / %d / %d / %d / %d / %d --- Julian = %f\n\n",
        era1, yr1, mo1, day1, hr1, min1, sec1, julian1);
fprintf(fp, "%d / %d / %d / %d / %d / %d / %d --- Julian = %f\n\n\n",
        era2, yr2, mo2, day2, hr2, min2, sec2, julian2);
fprintf(fp, "Dates #3 & #4  were calculated with the same date:\n\n");
fprintf(fp, "%d / %d / %d / %d / %d / %d / %d --- Julian = %f\n\n",
        era3, yr3, mo3, day3, hr3, min3, sec3, julian3);
fprintf(fp, "%d / %d / %d / %d / %d / %d / %d --- Julian = %f\n\n",
        era4, yr4, mo4, day4, hr4, min4, sec4, julian4);



  if (era1==era2 && era3==era4) {
    if (yr1==yr2 && yr3==yr4) {
      if (mo1==mo2 && mo3==mo4) {
        if (day1==day2 && day3==day4) {
          if (hr1==hr2 && hr3==hr4) {
            if (min1==min2 && min3==min4) {
              if (EQ_EPS(julian1, julian2, DBL_EPS) && 
                  EQ_EPS(julian3, julian4, DBL_EPS)) {
			          printf("\n");
			          printf("Calendar conversion works correctly.\n");
				        calendarworks = 1;
    
              }
            }
          }
        }
      }
    }
  }

  if (calendarworks != 1) {
    printf("\n");
    printf("Calendar Conversion sdfse DOES NOT Work Correctly.\n");
	  return -1;
  }
  else {
    return 1;
  }

}
//----------------------------------------------------------------------------
// SUBROUTINE txiTestCoordSystem
// PURPOSE    Reads a file's Coordinate Group and prints coordinate data out
//            to each text file.
// NOTES
//----------------------------------------------------------------------------
int txiTestCoordSystem (xid xFileId, FILE *a_OutFile)
{
  int     iHorizDatum, iHorizUnits, iVertDatum, iVertUnits;
  int     iLat, iLon, iUtmZone, iSpcZone, iHpgnArea, iEllipse;
  int     bHorizDatum, nStatus;
  double  dCppLat, dCppLon, dMajorR, dMinorR;
  xid     xCoordId = NONE;
  char    strHorizUnits[256], strVertDatum[256], strVertUnits[256];

  // Coordinate stuff
  // Read Coordinate Info
    // Open the Coordinate group
  nStatus = xfOpenCoordinateGroup(xFileId, &xCoordId);
  if (nStatus <= 0) {
    fprintf(a_OutFile, "\n");
    fprintf(a_OutFile, "Coordinate Group not found\n");
    fprintf(a_OutFile, "\n");
    return -1;
  }

  fprintf(a_OutFile, "\n");
  fprintf(a_OutFile, "Coordinate System:\n");

  bHorizDatum = xfGetHorizDatum(xCoordId, &iHorizDatum);
  xfGetHorizUnits(xCoordId, &iHorizUnits);
  xfGetVertDatum(xCoordId, &iVertDatum);
  xfGetVertUnits(xCoordId, &iVertUnits);
    // set horizontal units
  if (iHorizUnits == 0) {
    strcpy(strHorizUnits, "Horizontal units = US Survey Feet (=0)");
  }
  else if (iHorizUnits == 1) {
    strcpy(strHorizUnits, "Horizontal units = International Feet (=1)");
  }
  else if (iHorizUnits == 2) {
    strcpy(strHorizUnits, "Horizontal units = Meters (=2)");
  }
  else {
    strcpy(strHorizUnits, "ERROR in reading Horizontal units");
  }
    // set vertical datum
  if (iVertDatum == 0) {
    strcpy(strVertDatum, "Vertical datum = Local (=0)");
  }
  else if (iVertDatum == 1) {
    strcpy(strVertDatum, "Vertical datum = NGVD 29 (=1)");
  }
  else if (iVertDatum == 2) {
    strcpy(strVertDatum, "Vertical datum = NGVD 88 (=2)");
  }
  else {
    strcpy(strVertDatum, "ERROR in reading the Vertical datum\n");
  }
    // set vertocal units
  if (iVertUnits == 0) {
    strcpy(strVertUnits, "Vertical units = US Survey Feet (=0)");
  }
  else if (iVertUnits == 1) {
    strcpy(strVertUnits, "Vertical units = International Feet (=1)");
  }
  else if (iVertUnits == 2) {
    strcpy(strVertUnits, "Vertical units = Meters (=2)");
  }
  else {
    strcpy(strVertUnits, "ERROR in reading the Vertical units");
  }

  if (bHorizDatum >= 0) {
    switch (iHorizDatum) {
      case HORIZ_DATUM_GEOGRAPHIC:
        xfGetEllipse(xCoordId, &iEllipse);
        xfGetLat(xCoordId, &iLat);
        xfGetLon(xCoordId, &iLon);
          // Write Horizontal and Vertical Info
        fprintf(a_OutFile, "Horizontal datum = Geographic\n");
        fprintf(a_OutFile, "Horizontal units = %s\n", strHorizUnits);
        fprintf(a_OutFile, "Vertical datum = %s\n", strVertDatum);
        fprintf(a_OutFile, "Vertical units = %s\n", strVertUnits);
          // Write Latitude data
        if (iLat == 0) {
          fprintf(a_OutFile, "  Latitude = North (=%d)\n", iLat);
        }
        else if (iLat == 1) {
          fprintf(a_OutFile, "  Latitude = South (=%d)\n", iLat);
        }
        else {
          fprintf(a_OutFile, "  LATITUDE INFO INCORRECT\n");
        }
          // Longitude
        if (iLon == 0) {
          fprintf(a_OutFile, "  Longitude = East (=%d)\n", iLon);
        }
        else if (iLon == 1) {
          fprintf(a_OutFile, "  Longitude = West (=%d)\n", iLon);
        }
        else {
          fprintf(a_OutFile, "  LONGITUDE INFO INCORRECT\n");
        }
          // Ellipse Information
          // User-defined Ellipse (==32)
        if (iEllipse == 32) {
          fprintf(a_OutFile, "Ellipse = User-defined:\n");
          xfGetMajorR(xCoordId, &dMajorR);
          xfGetMinorR(xCoordId, &dMinorR);
          fprintf(a_OutFile, "  MajorR = %lf\n", dMajorR);
          fprintf(a_OutFile, "  MinorR = %lf\n\n", dMinorR);
        }
        else {
          fprintf(a_OutFile, "Ellipse = %d\n\n", iEllipse);
        }
        break;
      case HORIZ_DATUM_UTM:
      case HORIZ_DATUM_UTM_NAD27:
      case HORIZ_DATUM_UTM_NAD83:
        xfGetUTMZone(xCoordId, &iUtmZone);
          // output info to text file
        if (iHorizDatum == HORIZ_DATUM_UTM) {
          fprintf(a_OutFile, "Horizontal datum = UTM\n");
        }
        else if (iHorizDatum == HORIZ_DATUM_UTM_NAD27) {
          fprintf(a_OutFile, "Horizontal datum = UTM NAD27 (US)\n");
        }
        else {
          fprintf(a_OutFile, "Horizontal datum = UTM NAD83 (US)\n");
        }
        fprintf(a_OutFile, "Horizontal units = %s\n", strHorizUnits);
        fprintf(a_OutFile, "Vertical datum = %s\n", strVertDatum);
        fprintf(a_OutFile, "Vertical units = %s\n", strVertUnits);
        fprintf(a_OutFile, "UTM Zone = %d\n\n", iUtmZone);
        break;
      case HORIZ_DATUM_STATE_PLANE_NAD27:
      case HORIZ_DATUM_STATE_PLANE_NAD83:
        xfGetSPCZone(xCoordId, &iSpcZone);
          // output info to text file
        if (iHorizDatum == HORIZ_DATUM_STATE_PLANE_NAD27) {
          fprintf(a_OutFile, "Horizontal datum = State Plane NAD27 (US)\n");
        }
        else {
          fprintf(a_OutFile, "Horizontal datum = State Plane NAD83 (US)\n");
        }
        fprintf(a_OutFile, "Horizontal units = %s\n", strHorizUnits);
        fprintf(a_OutFile, "Vertical datum = %s\n", strVertDatum);
        fprintf(a_OutFile, "Vertical units = %s\n", strVertUnits);
        fprintf(a_OutFile, "SPC Zone = %d\n\n", iSpcZone);
        break;
      case HORIZ_DATUM_UTM_HPGN:
      case HORIZ_DATUM_STATE_PLANE_HPGN:
      case HORIZ_DATUM_GEOGRAPHIC_HPGN:
        xfGetHPGNArea(xCoordId, &iHpgnArea);
        if (iHorizDatum == HORIZ_DATUM_UTM_HPGN) {
          fprintf(a_OutFile, "Horizontal datum = UTM HPGN (US)\n");
        }
        else if (iHorizDatum == HORIZ_DATUM_STATE_PLANE_HPGN) {
          fprintf(a_OutFile, "Horizontal datum = State Plane HPGN (US)\n");
        }
        else {
          fprintf(a_OutFile, "Horizontal datum = Geographic HPGN (US)\n");
        }
        fprintf(a_OutFile, "Horizontal units = %s\n", strHorizUnits);
        fprintf(a_OutFile, "Vertical datum = %s\n", strVertDatum);
        fprintf(a_OutFile, "Vertical units = %s\n", strVertUnits);
        fprintf(a_OutFile, "HPGN Area = %d\n\n", iHpgnArea);
        break;
      case HORIZ_DATUM_CPP:
        xfGetCPPLat(xCoordId, &dCppLat);
        xfGetCPPLon(xCoordId, &dCppLon);
        fprintf(a_OutFile, "Horizontal datum = CPP (Carte Parallelo-Grammatique Projection)\n");
        fprintf(a_OutFile, "Horizontal units = %s\n", strHorizUnits);
        fprintf(a_OutFile, "Vertical datum = %s\n", strVertDatum);
        fprintf(a_OutFile, "Vertical units = %s\n", strVertUnits);
        fprintf(a_OutFile, "CPP Latitude = %lf\n", dCppLat);
        fprintf(a_OutFile, "CPP Longitude = %lf\n\n", dCppLon);
        break;
      default:
        // do other systems
        if (iHorizDatum == HORIZ_DATUM_LOCAL) {
          fprintf(a_OutFile, "Horizontal datum = Local\n");
        }
        else if (iHorizDatum == HORIZ_DATUM_GEOGRAPHIC_NAD27) {
          fprintf(a_OutFile, "Horizontal datum = Geographic NAD27 (US)\n");
        }
        else if (iHorizDatum == HORIZ_DATUM_GEOGRAPHIC_NAD83) {
          fprintf(a_OutFile, "Horizontal datum = Geographic NAD83 (US)\n");
        }
        else {
          fprintf(a_OutFile, "ERROR: The Horizontal Datum in the .h5 file is not recognizable\n");
          return -1;
        }
        fprintf(a_OutFile, "Horizontal units = %s\n", strHorizUnits);
        fprintf(a_OutFile, "Vertical datum = %s\n", strVertDatum);
        fprintf(a_OutFile, "Vertical units = %s\n\n", strVertUnits);
        break;
    }
  }
  else {
    fprintf(a_OutFile, "Coordinate information in HDF5 file is incomplete.");
    fprintf(a_OutFile, "\n");
  }

  xfCloseGroup(xCoordId);
  xCoordId = 0;

  return TRUE;
} // txiTestCoordSystem
