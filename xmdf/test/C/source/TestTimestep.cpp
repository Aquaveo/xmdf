// This file has functions that can be used to test reading and writing of
// timesteps using HDF5.  This code is intended to be used as tests for the
// XMDF library as well as sample code for distribution.

// Untested XMDF functions used for timesteps:
// xfGetDatasetMinsDouble
// xfGetDatasetMaxsDouble
// xfReadScalarValuesTimestepInt
// xfReadVectorValuesAtIndexDouble

// All XMDF functions used for timesteps:
// xfGetDatasetTimes
// xfGetDatasetMins
// xfGetDatasetMaxs
// xfGetDatasetMinsFloat
// xfGetDatasetMaxsFloat
// xfGetDatasetMinsDouble
// xfGetDatasetMaxsDouble
// xfReadActivityTimestep
// xfReadActivityValuesAtIndex
// xfReadScalarValuesTimestep
// xfReadScalarValuesTimestepFloat
// xfReadScalarValuesTimestepFloatPortion
// xfReadScalarValuesTimestepDouble
// xfReadScalarValuesTimestepDoublePortion
// xfReadScalarValuesTimestepInt
// xfReadScalarValuesAtIndex
// xfReadScalarValuesAtIndexFloat
// xfReadScalarValuesAtIndexDouble
// xfReadScalarValuesAtIndices
// xfReadScalarValuesAtIndicesFloat
// xfReadVectorValuesTimestep
// xfReadVectorValuesTimestepFloat
// xfReadVectorValuesTimestepFloatPortion
// xfReadVectorValuesTimestepDouble
// xfReadVectorValuesTimestepDoublePortion


#include "stdafx.h"
#include <xmdf/Xmdf.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>

#define    DATASETS_LOCATION "Datasets"
#define    SCALAR_A_LOCATION "Scalars/ScalarA"
#define    SCALAR_B_LOCATION "Scalars/ScalarB"
#define    VECTOR2D_A_LOCATION "Vectors/Vector2D_A"
#define    VECTOR2D_B_LOCATION "Vectors/Vector2D_B"

void ttiDatasetArray (double a_dMin, double a_dMax, int a_nCycle, 
                     int a_SeedMultiplier, int a_nValues, float *a_Array);
double ttiRandomNumberInRange (double a_dMin, double a_dMax, int a_nSeed);
int ttiReadScalar (xid a_xScalarId, FILE *a_fp);
int ttiReadVector (xid a_xVectorId, FILE *a_fp);
int ttiTestNumTimes( xid a_DatasetId, int a_Itimestep );

// --------------------------------------------------------------------------
// FUNCTION ttReadDatasets
// PURPOSE  Read a dataset group from an XMDF file and output information to
//          to a text file
// NOTES    
// --------------------------------------------------------------------------
int ttReadDatasets (xid a_xGroupId, FILE *a_fp)
{
  int   nPaths=0, nMaxPathLength=0, nMultiDatasets=0;
  char *Paths = NULL, *IndividualPath = NULL;
  int   nStatus, i, j;
  xid   xScalarId = NONE, xVectorId = NONE, xMultiId = NONE;

  // Look for scalar datasets
  nStatus = xfGetScalarDatasetsInfo(a_xGroupId, &nPaths, &nMaxPathLength);
  if (nStatus >= 0) {
    Paths = (char *)malloc(nPaths*nMaxPathLength*sizeof(char));
    xfGetScalarDatasetPaths(a_xGroupId, nPaths, nMaxPathLength, Paths);
  }
  if (nStatus < 0) {
    return -1;
  }
  
  // Output number and paths to scalar datasets
  fprintf(a_fp, "Number of Scalars %d\n", nPaths);
  for (i = 0; i < nPaths; i++) {
    IndividualPath = &Paths[i*nMaxPathLength];
    fprintf(a_fp, "Reading scalar: %s\n", IndividualPath);
   
    nStatus = xfOpenGroup(a_xGroupId, IndividualPath, &xScalarId);
    if (nStatus < 0) {
      return -1; 
    }

    nStatus = ttiReadScalar(xScalarId, a_fp);
    xfCloseGroup(xScalarId);
    if (nStatus < 0) {
      printf("%d: ERROR reading scalar dataset.\n", __LINE__);
      return -1;
    }
  }

  if (Paths) {
    free(Paths);
    Paths = NULL;
  }

  // Look for vector datasets
  nStatus = xfGetVectorDatasetsInfo(a_xGroupId, &nPaths, &nMaxPathLength);
  if (nStatus >= 0 && nPaths > 0) {
    Paths = (char *)malloc(nPaths*nMaxPathLength*sizeof(char));
    xfGetVectorDatasetPaths(a_xGroupId, nPaths, nMaxPathLength, Paths);
  }
  if (nStatus < 0) {
    return -1;
  }


  // Output number and paths to scalar datasets
  fprintf(a_fp, "Number of Vectors %d\n", nPaths);
  for (i = 0; i < nPaths; i++) {
    IndividualPath = &Paths[i*nMaxPathLength];
    fprintf(a_fp, "Reading Vector: %s\n", IndividualPath);
   
    nStatus = xfOpenGroup(a_xGroupId, IndividualPath, &xVectorId);
    if (nStatus < 0) {
      return -1; 
    }

    nStatus = ttiReadVector(xVectorId, a_fp);
    xfCloseGroup(xVectorId);
    if (nStatus < 0) {
      printf("%d: ERROR reading vector dataset.\n", __LINE__);
      return -1;
    }
  }

  // find multidataset folders
  nStatus = xfGetGroupPathsSizeForMultiDatasets(a_xGroupId, &nMultiDatasets,
                                                      &nMaxPathLength);
  if (nStatus >= 0 && nMultiDatasets > 0) {
    Paths = (char *)malloc(nMultiDatasets*nMaxPathLength*sizeof(char));
    nStatus = xfGetAllGroupPathsForMultiDatasets(a_xGroupId, nMultiDatasets, 
                                                 nMaxPathLength, Paths);
    if (nStatus < 0) {
      return -1;
    }

  // Output number and paths to multidatasets
    fprintf(a_fp, "Number of Multidatasets: %d\n", nMultiDatasets);
    for (i=0; i<nMultiDatasets; i++) {
      IndividualPath = "";
      for (j=0; j<nMaxPathLength-1; j++) {
        IndividualPath = &Paths[i*nMaxPathLength];
      }
      fprintf(a_fp, "Reading multidataset: %s\n", IndividualPath);
      nStatus = xfOpenGroup(a_xGroupId, IndividualPath, &xMultiId);
      if (nStatus < 0) {
  	    return -1;
      }

      nStatus = ttReadDatasets(xMultiId, a_fp);
      nStatus = xfCloseGroup(xMultiId);
      if (nStatus < 0) {
        printf("%d: ERROR reading multidatasets.\n", __LINE__);
  	    return -1;
      }
    }
  }

  if (Paths) {
    free(Paths);
    Paths = NULL;
  }

  return 1;
} // ttReadDatasets
// --------------------------------------------------------------------------
// FUNCTION ttReadActivityScalarAIndex
// PURPOSE  Read all timestep values for a particular index
// NOTES    
// --------------------------------------------------------------------------
int ttReadActivityScalarAIndex (LPCSTR a_Filename, int a_Index)
{
  int     status;
  xid     xFileId = NONE, xDsetsId = NONE, xScalarAId = NONE;
  int     nTimesteps;
  xmbool  *bActive;

  // open the file
  status = xfOpenFile(a_Filename, &xFileId, XTRUE);
  if (status < 0) {
    return FALSE;
  }

  // open the dataset group
  status = xfOpenGroup(xFileId, DATASETS_LOCATION, &xDsetsId);
  if (status >= 0) {
    status = xfOpenGroup(xDsetsId, SCALAR_A_LOCATION, &xScalarAId);
  }
  if (status < 0) {
    return status;
  }

  // Find out the number of timesteps in the file
  status = xfGetDatasetNumTimes(xScalarAId, &nTimesteps);
  if (status < 0) {
    return status;
  }
  if (nTimesteps < 1) {
    return -1;
  }

  // Read the values for the index
  bActive = new xmbool[nTimesteps];
  status = xfReadActivityValuesAtIndex(xScalarAId, a_Index, 1, nTimesteps,
                                       bActive);

  // output the data
  printf("\nReading activity for scalar A slice at index: %d\n", a_Index);
  for (int i = 0; i < nTimesteps; i++) {
    printf("%d ", bActive[i]);
  }
  printf("\n");

  delete [] bActive;

  return status;
} // ttReadActivityScalarAtIndex
// --------------------------------------------------------------------------
// FUNCTION ttReadScalarAIndex
// PURPOSE  Read all timestep values for a particular index
// NOTES    
// --------------------------------------------------------------------------
int ttReadScalarAIndex (LPCSTR a_Filename, int a_Index)
{
  int     status;
  xid     xFileId = NONE, xDsetsId = NONE, xScalarAId = NONE;
  int     nTimesteps;
  float  *fValues;

  // open the file
  status = xfOpenFile(a_Filename, &xFileId, XTRUE);
  if (status < 0) {
    return FALSE;
  }

  // open the dataset group
  status = xfOpenGroup(xFileId, DATASETS_LOCATION, &xDsetsId);
  if (status >= 0) {
    status = xfOpenGroup(xDsetsId, SCALAR_A_LOCATION, &xScalarAId);
  }
  if (status < 0) {
    return status;
  }

  // Find out the number of timesteps in the file
  status = xfGetDatasetNumTimes(xScalarAId, &nTimesteps);
  if (status < 0) {
    return status;
  }
  if (nTimesteps < 1) {
    return -1;
  }

  // Read the values for the index
  fValues = new float[nTimesteps];
  status = xfReadScalarValuesAtIndex(xScalarAId, a_Index, 1, nTimesteps,
                                     fValues);

  // output the data
  printf("\nReading scalar A slice at index: %d\n", a_Index);
  for (int i = 0; i < nTimesteps; i++) {
    printf("%f ", fValues[i]);
  }
  printf("\n");

  delete [] fValues;

  return status;
} // ttReadScalarAtIndex
// --------------------------------------------------------------------------
// FUNCTION ttWriteScalarA
// PURPOSE  Write scalar Dataset to an HDF5 File
// NOTES    This tests dynamic data sets, and activity
//          This dataset is dynamic concentrations (mg/L) with output times
//          in minutes.
//          Dataset is for a mesh and so nActive is the number of elements
//          which is not the same as the nValues which would be number of nodes
//          reads/writes a reference time in julian days
// --------------------------------------------------------------------------
int ttWriteScalarA (LPCSTR a_Filename, int a_Compression)
{
  xid      xFileId, xDsetsId, xScalarAId, xCoordId = NONE;
  int      nValues = 10, nTimes = 3, nActive = 8;
  double   dTime = 0.0;
  int      iTimestep, iActive;
  float    fValues[10]; // nValues
  xmbool    bActivity[10]; // activity
  int      status, iHpgnZone;
  double   dJulianReftime;
  int      nErrors = 0, i = 0;
  char     **Errors = NULL;

  // 5th item in data set is always inactive, others active
  for (iActive = 0; iActive < nActive; iActive++) {
    bActivity[iActive] = XTRUE;
  }
  bActivity[5] = XFALSE;

  // create the file
  status = xfCreateFile(a_Filename, &xFileId, XTRUE);
  if (status < 0) {
    return FALSE;
  }

  // create the group where we will put all the datasets 
  status = xfCreateGenericGroup(xFileId, DATASETS_LOCATION, &xDsetsId);
  if (status < 0) {
    xfCloseFile(xFileId);
    return FALSE;
  }

  // uncomment next line to test error handling function
  // xfCloseGroup(xDsetsId);

  // Create the scalar A dataset group
  status = xfCreateScalarDataset(xDsetsId, SCALAR_A_LOCATION,
                                 "mg/L", TS_HOURS, a_Compression,
                                 &xScalarAId);
  if (status < 0) {
    // print the error stack
    xfGetNumErrorMessages(&nErrors);
    if (nErrors > 0) {
      Errors = new char*[nErrors];
      for (i = 0; i < nErrors; i++) {
        Errors[i] = new char[XF_MAX_ERROR_MSG_SIZE];
      }
      status = xfGetErrorMessages(nErrors, Errors);
      if (status > 0) {
        for (i = 0; i < nErrors; i++) {
          printf("%s\n", Errors[i]);
        }
      }
      for (i = 0; i < nErrors; i++) {
        delete Errors[i];
      }
      delete Errors;
    }

    xfCloseGroup(xDsetsId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Add in a reftime.  This is a julian day for:
  // noon July 1, 2003
  dJulianReftime = 2452822.0;
  status = xfDatasetReftime(xScalarAId, dJulianReftime);
  if (status < 0) {
    xfCloseGroup(xScalarAId);
    xfCloseGroup(xDsetsId);
    xfCloseFile(xFileId);
  }

  // Loop through timesteps adding them to the file
  for (iTimestep = 0; iTimestep < nTimes; iTimestep++) {
    // We will have an 0.5 hour timestep
    dTime = (iTimestep + 1) * 0.5;

    fValues[0] = (float)dTime;
    for (i = 1; i < nValues; i++) {
      fValues[i] = float(fValues[i-1]*2.5);
    }

    // write the dataset array values
    status = xfWriteScalarTimestep(xScalarAId, dTime, nValues, fValues);
    if (status >= 0) {
      // write activity array
      xfWriteActivityTimestep(xScalarAId, nActive, bActivity);
      if (status < 0) {
        printf( "%d: ERROR writing activity timestep: %d\n",
               __LINE__, status );
      }
    }
    if (status < 0) {
      printf( "%d: ERROR writing scalar timestep.\n", __LINE__ );
      xfCloseGroup(xScalarAId);
      xfCloseGroup(xDsetsId);
      xfCloseFile(xFileId);
    }

    status = ttiTestNumTimes( xScalarAId, iTimestep );
  }

  // Write Coordinate file - for ScalarA, we will set the coordinate system
  //   to be Geographic HPGN, with HPGN settings written to the file.
  status = xfCreateCoordinateGroup(xFileId, &xCoordId);
  if (status <= 0) {
    xfCloseGroup(xScalarAId);
    xfCloseGroup(xDsetsId);
    xfCloseFile(xFileId);
	return -1;
  }

    // set HPGN Zone for test
  iHpgnZone = 29;      // Utah

    // Write Coordinate Information to file
  xfSetHorizDatum(xCoordId, HORIZ_DATUM_GEOGRAPHIC_HPGN);
  xfSetHorizUnits(xCoordId, COORD_UNITS_METERS);
  xfSetVertDatum(xCoordId, VERT_DATUM_LOCAL);
  xfSetVertUnits(xCoordId, COORD_UNITS_METERS);

    // write additional information
  xfSetHPGNArea(xCoordId, iHpgnZone);

  xfCloseGroup(xCoordId);
  xCoordId = 0;

  // close the dataset
  xfCloseGroup(xScalarAId);
  xfCloseGroup(xDsetsId);
  xfCloseFile(xFileId);

  return FALSE;
} // ttWriteScalarA

// --------------------------------------------------------------------------
// FUNCTION ttWriteScalarB
// PURPOSE  Write scalar Dataset to an HDF5 File
// NOTES    This tests dynamic data sets, and activity
//          This dataset is dynamic concentrations (mg/L) with output times
//          in minutes.
//          Dataset is for a mesh and so nActive is the number of elements
//          which is not the same as the nValues which would be number of nodes
//          reads/writes a reference time in julian days
// --------------------------------------------------------------------------
int ttWriteScalarB (LPCSTR a_Filename, int a_Compression, int a_Overwrite)
{
  xid      xFileId, xDsetsId, xScalarBId, xCoordId = NONE;
  int      nValues = 10, nTimes = 3, nActive = 8;
  double   dTime = 0.0, dJulianReftime;
  int      iTimestep, iActive;
  float    fValues[10]; // nValues
  xmbool    bActivity[10]; // activity
  int      status, nErrors = 0, i = 0;
  char     **Errors = NULL;

  // 5th item in data set is always inactive, others active
  for (iActive = 0; iActive < nActive; iActive++) {
    bActivity[iActive] = XTRUE;
  }
  bActivity[5] = XFALSE;

  if (a_Overwrite) {
      // open the already-existing file
    status = xfOpenFile(a_Filename, &xFileId, XFALSE);
    if (status < 0) {
      return -1;
    }
      // open the group where we have all the datasets 
    status = xfOpenGroup(xFileId, DATASETS_LOCATION, &xDsetsId);
    if (status < 0) {
      xfCloseFile(xFileId);
      return -1;
    }
  }
  else {
      // create the file
    status = xfCreateFile(a_Filename, &xFileId, XTRUE);
    if (status < 0) {
      return -1;
    }
      // create the group where we will put all the datasets 
    status = xfCreateGenericGroup(xFileId, DATASETS_LOCATION, &xDsetsId);
    if (status < 0) {
      xfCloseFile(xFileId);
      return -1;
    }
  }

  // uncomment next line to test error handling function
  // xfCloseGroup(xDsetsId);

  // Create/Overwrite the scalar B dataset group
  status = xfCreateScalarDataset(xDsetsId, SCALAR_B_LOCATION,
                                 "mg/L", TS_HOURS, a_Compression,
                                 &xScalarBId);
  if (status < 0) {
    // print the error stack
    xfGetNumErrorMessages(&nErrors);
    if (nErrors > 0) {
      Errors = new char*[nErrors];
      for (i = 0; i < nErrors; i++) {
        Errors[i] = new char[XF_MAX_ERROR_MSG_SIZE];
      }
      status = xfGetErrorMessages(nErrors, Errors);
      if (status > 0) {
        for (i = 0; i < nErrors; i++) {
          printf("%s\n", Errors[i]);
        }
      }
      for (i = 0; i < nErrors; i++) {
        delete Errors[i];
      }
      delete Errors;
    }

    xfCloseGroup(xDsetsId);
    xfCloseFile(xFileId);
    return -1;
  }

  // Add in a reftime.  This is a julian day for:
  // noon July 1, 2003
  dJulianReftime = 2452822.0;
  status = xfDatasetReftime(xScalarBId, dJulianReftime);
  if (status < 0) {
    xfCloseGroup(xScalarBId);
    xfCloseGroup(xDsetsId);
    xfCloseFile(xFileId);
  }

  if (!a_Overwrite) {
    // Loop through timesteps adding them to the file
    for (iTimestep = 0; iTimestep < nTimes; iTimestep++) {
      // We will have an 0.5 hour timestep
      dTime = (iTimestep + 1) * 0.5;

      fValues[0] = (float)dTime;
      for (i = 1; i < nValues; i++) {
        fValues[i] = float(fValues[i-1]*2.5);
      }

      // write the dataset array values
      status = xfWriteScalarTimestep(xScalarBId, dTime, nValues, fValues);
      if (status >= 0) {
        // write activity array
        xfWriteActivityTimestep(xScalarBId, nActive, bActivity);
      }
      if (status < 0) {
        xfCloseGroup(xScalarBId);
        xfCloseGroup(xDsetsId);
        xfCloseFile(xFileId);
      }
    }
  }
  else {
    // Loop through timesteps adding them to the file
    for (iTimestep = 0; iTimestep < nTimes; iTimestep++) {
      // We will have an 1.5 hour timestep
      dTime = (iTimestep + 1) * 1.5;

      fValues[0] = (float)dTime;
      for (i = 1; i < nValues; i++) {
        fValues[i] = float(fValues[i-1]*1.5);
      }

      // write the dataset array values
      status = xfWriteScalarTimestep(xScalarBId, dTime, nValues, fValues);
      if (status >= 0) {
        // write activity array
        xfWriteActivityTimestep(xScalarBId, nActive, bActivity);
      }
      if (status < 0) {
        xfCloseGroup(xScalarBId);
        xfCloseGroup(xDsetsId);
        xfCloseFile(xFileId);
      }
    }
  }

  if (!a_Overwrite) {
    // Write Coordinate file
    status = xfCreateCoordinateGroup(xFileId, &xCoordId);
    if (status <= 0) {
      xfCloseGroup(xScalarBId);
      xfCloseGroup(xDsetsId);
      xfCloseFile(xFileId);
      return -1;
    }

    // For ScalarB, we will set the coordinate system
    // to be UTM, with UTM Zone settings written to the file.
      // Write Coord Info to file
    xfSetHorizDatum(xCoordId, HORIZ_DATUM_UTM);
    xfSetHorizUnits(xCoordId, COORD_UNITS_METERS);

    xfSetVertDatum(xCoordId, VERT_DATUM_LOCAL);
    xfSetVertUnits(xCoordId, COORD_UNITS_METERS);

      // write additional information - we'll use the max value for this test
    xfSetUTMZone(xCoordId, UTM_ZONE_MAX);

    xfCloseGroup(xCoordId);
    xCoordId = 0;
  }

  // close the dataset
  xfCloseGroup(xScalarBId);
  xfCloseGroup(xDsetsId);
  xfCloseFile(xFileId);

  return 1;
} // ttWriteScalarB
// --------------------------------------------------------------------------
// FUNCTION ttWriteCoordsToMulti
// PURPOSE  Write coordinate system to a multidataset file
// NOTES
// --------------------------------------------------------------------------
int ttWriteCoordsToMulti (xid a_xFileId)
{
  xid   xCoordId = NONE;
  int   status;

  // Write Coordinate file - for Multidatasets, we will set the coordinate system
  //   to be UTM, with UTM Zone settings written to the file.
  status = xfCreateCoordinateGroup(a_xFileId, &xCoordId);
  if (status <= 0) {
    return -1;
  }

   // Write Coord Info to file
  xfSetHorizDatum(xCoordId, HORIZ_DATUM_UTM);
  xfSetHorizUnits(xCoordId, COORD_UNITS_METERS);

  xfSetVertDatum(xCoordId, VERT_DATUM_LOCAL);
  xfSetVertUnits(xCoordId, COORD_UNITS_METERS);

    // write additional information - we'll use the max value for this test
  xfSetUTMZone(xCoordId, UTM_ZONE_MAX);

  xfCloseGroup(xCoordId);
  xCoordId = 0;

  return XTRUE;
} // ttWriteCoordsToMulti
// --------------------------------------------------------------------------
// FUNCTION ttWriteScalarAToMulti
// PURPOSE  Write scalar Dataset to a multidataset
// NOTES    This tests dynamic data sets, and activity
//          This dataset is dynamic concentrations (mg/L) with output times
//          in minutes.
//          Dataset is for a mesh and so nActive is the number of elements
//          which is not the same as the nValues which would be number of nodes
//          reads/writes a reference time in julian days
// --------------------------------------------------------------------------
int ttWriteScalarAToMulti (xid a_GroupId)
{
  xid      xScalarAId;
  int      nValues = 10, nTimes = 3, nActive = 8;
  double   dTime = 0.0;
  int      iTimestep, iActive;
  float    fValues[10]; // nValues
  xmbool    bActivity[10]; // activity
  int      status;
  double   dJulianReftime;
  int      i = 0;

  // 5th item in data set is always inactive, others active
  for (iActive = 0; iActive < nActive; iActive++) {
    bActivity[iActive] = XTRUE;
  }
  bActivity[5] = XFALSE;

  // Create the scalar A dataset group
  status = xfCreateScalarDataset(a_GroupId, SCALAR_A_LOCATION,
                                 "mg/L", TS_HOURS, NONE,
                                 &xScalarAId);
  
  // Add in a reftime.  This is a julian day for:
  // noon July 1, 2003
  dJulianReftime = 2452822.0;
  status = xfDatasetReftime(xScalarAId, dJulianReftime);
  if (status < 0) {
    xfCloseGroup(xScalarAId);
  }

  // Loop through timesteps adding them to the file
  for (iTimestep = 0; iTimestep < nTimes; iTimestep++) {
    // We will have an 0.5 hour timestep
    dTime = (iTimestep + 1) * 0.5;

    fValues[0] = (float)dTime;
    for (i = 1; i < nValues; i++) {
      fValues[i] = float(fValues[i-1]*2.5);
    }

    // write the dataset array values
    status = xfWriteScalarTimestep(xScalarAId, dTime, nValues, fValues);
    if (status >= 0) {
      // write activity array
      xfWriteActivityTimestep(xScalarAId, nActive, bActivity);
    }
    if (status < 0) {
      xfCloseGroup(xScalarAId);
    }

    status = ttiTestNumTimes( xScalarAId, iTimestep );
  }

  // close the dataset
  xfCloseGroup(xScalarAId);

  return FALSE;
} // ttWriteScalarAToMulti
// --------------------------------------------------------------------------
// FUNCTION ttReadVector2DAIndex
// PURPOSE  Read all timestep values for a particular index
// NOTES    
// --------------------------------------------------------------------------
int ttReadVector2DAIndex (LPCSTR a_Filename, int a_Index)
{
  int     status;
  xid     xFileId = NONE, xDsetsId = NONE, xVector2DA = NONE;
  int     nTimesteps;
  float  *fValues;

  // open the file
  status = xfOpenFile(a_Filename, &xFileId, XTRUE);
  if (status < 0) {
    return FALSE;
  }

  // open the dataset group
  status = xfOpenGroup(xFileId, DATASETS_LOCATION, &xDsetsId);
  if (status >= 0) {
    status = xfOpenGroup(xDsetsId, VECTOR2D_A_LOCATION, &xVector2DA);
  }
  if (status < 0) {
    return status;
  }

  // Find out the number of timesteps in the file
  status = xfGetDatasetNumTimes(xVector2DA, &nTimesteps);
  if (status < 0) {
    return status;
  }
  if (nTimesteps < 1) {
    return -1;
  }

  // Read the values for the index
  fValues = new float[nTimesteps*2];
  status = xfReadVectorValuesAtIndex(xVector2DA, a_Index, 1, nTimesteps, 2,
                                     fValues);

  // output the data
  printf("\nReading vector 2D A slice at index: %d\n", a_Index);
  for (int i = 0; i < nTimesteps; i++) {
    printf("%f %f \n", fValues[i*2], fValues[i*2 + 1]);
  }
  printf("\n");

  delete [] fValues;

  return status;
} // ttReadVector2DAIndex
// --------------------------------------------------------------------------
// FUNCTION ttWriteVector2D_A
// PURPOSE  Write 2D vector dataset to an HDF5 File
// NOTES    This tests dynamic data sets, and activity
//          Add reftime when it is completed
//          This dataset is dynamic water velocities with output times
//          in minutes.
//          Dataset is for a mesh and so nActive is the number of elements
//          which is not the same as the nValues which would be number of nodes
// --------------------------------------------------------------------------
int ttWriteVector2D_A (LPCSTR a_Filename, int a_Compression)
{
  xid      xFileId, xDsetsId, xVectorA, xCoordId = NONE;
  int      nTimes = 6, nValues = 100, nComponents = 2, nActive = 75;
  double   dTime = 0.0;
  int      iTimestep, iActive;
  float    fValues[100*2]; // nValues*nComponents
  xmbool    bActivity[75]; // activity
//  double   dMin = 0.5, dMax = 3.5;
//  int      nSeedMultiplier = 138592;
  int      i, j, status;
  int      iHpgnZone;

  // 5th item in data set is always inactive, others active
  for (iActive = 0; iActive < nActive; iActive++) {
    if (iActive % 3 == 0) {
      bActivity[iActive] = XFALSE;
    }
    else {
      bActivity[iActive] = XTRUE;
    }
  }

  // create the file
  status = xfCreateFile(a_Filename, &xFileId, XTRUE);
  if (status < 0) {
    return FALSE;
  }

  // create the group to store all datasets 
  status = xfCreateGenericGroup(xFileId, DATASETS_LOCATION, &xDsetsId);
  if (status < 0) {
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Create the scalar A dataset group
  status = xfCreateVectorDataset(xDsetsId, VECTOR2D_A_LOCATION, "ft/s",
               TS_SECONDS, a_Compression, &xVectorA);
  if (status < 0) {
    xfCloseGroup(xDsetsId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Loop through timesteps adding them to the file
  for (iTimestep = 0; iTimestep < nTimes; iTimestep++) {
    // We will have an 0.5 hour timestep
    dTime = (iTimestep + 1) * 0.5;

    for (i = 0; i < nValues; i++) {
      for (j = 0; j < nComponents; j++) {
        fValues[i*nComponents + j] = (float)(((i)*nComponents + (j+1))*dTime);
      }
    }
//    // generate values array using random numbers between dMin and dMax
//    ttiDatasetArray(dMin, dMax, iTimestep + 1, nSeedMultiplier,
//                    nValues*nComponents, fValues);

    // write the dataset array values
    status = xfWriteVectorTimestep(xVectorA, dTime, nValues, nComponents,
                                   fValues);
    if (status >= 0) {
      // write activity array
      xfWriteActivityTimestep(xVectorA, nActive, bActivity);
    }
    if (status < 0) {
      xfCloseGroup(xVectorA);
      xfCloseGroup(xDsetsId);
      xfCloseFile(xFileId);
    }

    status = ttiTestNumTimes( xVectorA, iTimestep );
  }

  // Write Coordinate file - for Vector2D_A, we will set the coordinate system
  //   to be Geographic HPGN, with HPGN settings written to the file.
  status = xfCreateCoordinateGroup(xFileId, &xCoordId);
  if (status <= 0) {
    xfCloseGroup(xVectorA);
    xfCloseGroup(xDsetsId);
    xfCloseFile(xFileId);
	return -1;
  }

    // set HPGN info for test
  iHpgnZone = 29;      // Utah

  xfSetHorizDatum(xCoordId, HORIZ_DATUM_GEOGRAPHIC_HPGN);
  xfSetHorizUnits(xCoordId, COORD_UNITS_METERS);
  xfSetVertDatum(xCoordId, VERT_DATUM_LOCAL);
  xfSetVertUnits(xCoordId, COORD_UNITS_METERS);

    // write additional information
  xfSetHPGNArea(xCoordId, iHpgnZone);

  xfCloseGroup(xCoordId);
  xCoordId = 0;

  // close the dataset
  xfCloseGroup(xVectorA);
  xfCloseGroup(xDsetsId);
  xfCloseFile(xFileId);

  return FALSE;
} // ttWriteVector2D_A
// --------------------------------------------------------------------------
// FUNCTION ttWriteVector2D_B
// PURPOSE  Write 2D vector dataset to an HDF5 File
// NOTES    This tests dynamic data sets, and activity
//          Add reftime when it is completed
//          This dataset is dynamic water velocities with output times
//          in minutes.
//          Dataset is for a mesh and so nActive is the number of elements
//          which is not the same as the nValues which would be number of nodes
// --------------------------------------------------------------------------
int ttWriteVector2D_B (LPCSTR a_Filename, int a_Compression, int a_Overwrite)
{
  xid      xFileId, xDsetsId, xVectorB, xCoordId = NONE;
  int      nTimes = 6, nValues = 100, nComponents = 2, nActive = 75;
  double   dTime = 0.0;
  int      iTimestep, iActive;
  float    fValues[100*2]; // nValues*nComponents
  xmbool    bActivity[75]; // activity
//  double   dMin = 0.5, dMax = 3.5;
//  int      nSeedMultiplier = 138592;
  int      i, j, status;

  // 5th item in data set is always inactive, others active
  for (iActive = 0; iActive < nActive; iActive++) {
    if (iActive % 3 == 0) {
      bActivity[iActive] = XFALSE;
    }
    else {
      bActivity[iActive] = XTRUE;
    }
  }

  if (a_Overwrite) {
      //open the already-existing file
    status = xfOpenFile(a_Filename, &xFileId, XFALSE);
    if (status < 0) {
      return -1;
    }
      // open the group where we have all the datasets
    status = xfOpenGroup(xFileId, DATASETS_LOCATION, &xDsetsId);
    if (status < 0) {
      xfCloseFile(xFileId);
      return -1;
    }
  }
  else {
      // create the file
    status = xfCreateFile(a_Filename, &xFileId, XTRUE);
    if (status < 0) {
      return FALSE;
    }
      // create the group to store all datasets 
    status = xfCreateGenericGroup(xFileId, DATASETS_LOCATION, &xDsetsId);
    if (status < 0) {
      xfCloseFile(xFileId);
      return FALSE;
    }
  }

  // Create the Vector B dataset group
  status = xfCreateVectorDataset(xDsetsId, VECTOR2D_B_LOCATION, "ft/s",
               TS_SECONDS, a_Compression, &xVectorB);
  if (status < 0) {
    xfCloseGroup(xDsetsId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  if (!a_Overwrite) {
    // Loop through timesteps adding them to the file
    for (iTimestep = 0; iTimestep < nTimes; iTimestep++) {
      // We will have an 0.5 hour timestep
      dTime = (iTimestep + 1) * 0.5;

      for (i = 0; i < nValues; i++) {
        for (j = 0; j < nComponents; j++) {
          fValues[i*nComponents + j] = (float)(((i)*nComponents + (j+1))*dTime);
        }
      }

      // write the dataset array values
      status = xfWriteVectorTimestep(xVectorB, dTime, nValues, nComponents,
                                     fValues);
      if (status >= 0) {
        // write activity array
        xfWriteActivityTimestep(xVectorB, nActive, bActivity);
      }
      if (status < 0) {
        xfCloseGroup(xVectorB);
        xfCloseGroup(xDsetsId);
        xfCloseFile(xFileId);
      }
    }
  }
  else {
    // Loop through timesteps adding them to the file
    for (iTimestep = 0; iTimestep < nTimes; iTimestep++) {
      // We will have an 1.5 hour timestep
      dTime = (iTimestep + 1) * 1.5;

      for (i = 0; i < nValues; i++) {
        for (j = 0; j < nComponents; j++) {
          fValues[i*nComponents + j] = (float)(((i)*nComponents + (j+1))*dTime);
        }
      }

      // write the dataset array values
      status = xfWriteVectorTimestep(xVectorB, dTime, nValues, nComponents,
                                     fValues);
      if (status >= 0) {
        // write activity array
        xfWriteActivityTimestep(xVectorB, nActive, bActivity);
      }
      if (status < 0) {
        xfCloseGroup(xVectorB);
        xfCloseGroup(xDsetsId);
        xfCloseFile(xFileId);
      }
    }
  }

  // Write Coordinate file - for ScalarB, we will set the coordinate system
  //   to be UTM, with UTM Zone settings written to the file.
  status = xfCreateCoordinateGroup(xFileId, &xCoordId);
  if (status <= 0) {
    xfCloseGroup(xVectorB);
    xfCloseGroup(xDsetsId);
    xfCloseFile(xFileId);
	return -1;
  }

    // write the coordinate data to the file
  xfSetHorizDatum(xCoordId, HORIZ_DATUM_UTM);
  xfSetHorizUnits(xCoordId, COORD_UNITS_METERS);
  xfSetVertDatum(xCoordId, VERT_DATUM_LOCAL);
  xfSetVertUnits(xCoordId, COORD_UNITS_METERS);

    // write additional information - we'll use the max UTM zone for the test
  xfSetUTMZone(xCoordId, UTM_ZONE_MAX);

  xfCloseGroup(xCoordId);
  xCoordId = 0;

  // close the dataset
  xfCloseGroup(xVectorB);
  xfCloseGroup(xDsetsId);
  xfCloseFile(xFileId);

  return 1;
} // ttWriteVector2D_B
// --------------------------------------------------------------------------
// FUNCTION ttWriteVector2DAToMulti
// PURPOSE  Write 2D vector dataset to an HDF5 File
// NOTES    This tests dynamic data sets, and activity
//          Add reftime when it is completed
//          This dataset is dynamic water velocities with output times
//          in minutes.
//          Dataset is for a mesh and so nActive is the number of elements
//          which is not the same as the nValues which would be number of nodes
// --------------------------------------------------------------------------
int ttWriteVector2DAToMulti (xid a_GroupId)
{
  xid      xVectorA;
  int      nTimes = 6, nValues = 100, nComponents = 2, nActive = 75;
  double   dTime = 0.0;
  int      iTimestep, iActive;
  float    fValues[100*2]; // nValues*nComponents
  xmbool    bActivity[75]; // activity
//  double   dMin = 0.5, dMax = 3.5;
//  int      nSeedMultiplier = 138592;
  int      i, j, status;

  // 5th item in data set is always inactive, others active
  for (iActive = 0; iActive < nActive; iActive++) {
    if (iActive % 3 == 0) {
      bActivity[iActive] = XFALSE;
    }
    else {
      bActivity[iActive] = XTRUE;
    }
  }

  // Create the scalar A dataset group
  status = xfCreateVectorDataset(a_GroupId, VECTOR2D_A_LOCATION, "ft/s",
               TS_SECONDS, NONE, &xVectorA);
  if (status < 0) {
    return FALSE;
  }

  // Loop through timesteps adding them to the file
  for (iTimestep = 0; iTimestep < nTimes; iTimestep++) {
    // We will have an 0.5 hour timestep
    dTime = (iTimestep + 1) * 0.5;

    for (i = 0; i < nValues; i++) {
      for (j = 0; j < nComponents; j++) {
        fValues[i*nComponents + j] = (float)(((i)*nComponents + (j+1))*dTime);
      }
    }
//    // generate values array using random numbers between dMin and dMax
//    ttiDatasetArray(dMin, dMax, iTimestep + 1, nSeedMultiplier,
//                    nValues*nComponents, fValues);

    // write the dataset array values
    status = xfWriteVectorTimestep(xVectorA, dTime, nValues, nComponents,
                                   fValues);
    if (status >= 0) {
      // write activity array
      xfWriteActivityTimestep(xVectorA, nActive, bActivity);
    }
    if (status < 0) {
      xfCloseGroup(xVectorA);
    }

    status = ttiTestNumTimes( xVectorA, iTimestep );
  }

  // close the dataset
  xfCloseGroup(xVectorA);

  return FALSE;
} // ttWriteVector2DAToMulti
// --------------------------------------------------------------------------
// FUNCTION ttiRandomNumberInRange
// PURPOSE  generate Psuedo Random numbers.  We use the same seeds every
//          time so we end up with consistent values for testing purposes.
// --------------------------------------------------------------------------
double ttiRandomNumberInRange (double a_dMin, double a_dMax, int a_nSeed)
{
  int nRandom;
  double dValue;

  srand(a_nSeed);
  nRandom = rand();

  dValue = a_dMin + ((double)(nRandom)*(a_dMax - a_dMin))/RAND_MAX;

  return dValue;
} // ttiRandomNumberInRange
// --------------------------------------------------------------------------
// FUNCTION ttDatasetArray
// PURPOSE  Get dataset data to use in a dataset
// NOTES    Generates random numbers between a range to fill in the array
// --------------------------------------------------------------------------
void ttiDatasetArray (double a_dMin, double a_dMax, int a_nCycle, 
                     int a_SeedMultiplier, int a_nValues, float *a_Array)
{
  int       i, nSeedBase;

  for (i = 0; i < a_nValues; i++) {
    nSeedBase = a_nCycle*a_nValues + i;
    a_Array[i] = (float)ttiRandomNumberInRange(a_dMin, a_dMax, 
                                              nSeedBase*a_SeedMultiplier);
  }
} // ttDatasetArray
// --------------------------------------------------------------------------
// FUNCTION ttiReadScalar
// PURPOSE  Read a scalar from an XMDF file and output information to
//          to a text file
// NOTES    
// --------------------------------------------------------------------------
int ttiReadScalar (xid a_xScalarId, FILE *a_fp)
{
  int    nTimes = NONE, nValues = NONE, nActive = NONE;
  int    nStatus = TRUE, iTime, iVal, iActive;
  char   TimeUnits[100], Units[100];
  double *Times = NULL;
  float  *Values = NULL, *Mins = NULL, *Maxs = NULL;
  xmbool  *Active = NULL;
  xmbool  bUseReftime;
  double Reftime;

  // read the time units
  nStatus = xfGetDatasetTimeUnits(a_xScalarId, TimeUnits);
  if (nStatus < 0) {
    return nStatus;
  }
  fprintf(a_fp, "Time units: %s\n", TimeUnits);

  // see if we are using a reftime
  nStatus = xfUseDatasetReftime(a_xScalarId, &bUseReftime);
  if (nStatus < 0) {
    return nStatus;
  }
  if (bUseReftime) {
    nStatus = xfReadDatasetReftime(a_xScalarId, &Reftime);
    if (nStatus < 0) {
      return nStatus;
    }
    fprintf(a_fp, "Reftime: %f\n", Reftime);
  }

  // read the units
  nStatus = xfGetDatasetUnits(a_xScalarId, Units);
  if (nStatus < 0) { 
    return nStatus;
  }
  fprintf(a_fp, "units: %s\n", Units);

  // read in the number of values and number of active values
  nStatus = xfGetDatasetNumVals(a_xScalarId, &nValues);
  if (nStatus >= 0) {
    nStatus = xfGetDatasetNumActive(a_xScalarId, &nActive);
  }
  if (nStatus < 0) {
    return nStatus;
  }

  if (nValues <= 0) {
    printf("No data to read in.");
    return -1;
  }

  // read in the number of times
  nStatus = xfGetDatasetNumTimes(a_xScalarId, &nTimes);
  if (nStatus < 0) {
    return nStatus;
  }

  // Read in the individual time values
  Times = (double *)malloc(nTimes*sizeof(double));
  if (Times == NULL) {
    printf("Out of memory");
    return -1;
  }
  nStatus = xfGetDatasetTimes(a_xScalarId, nTimes, Times);
  if (nStatus < 0) {
    return nStatus;
  }

  // Read in the minimum and maximum values
  Mins = (float *)malloc(nTimes*sizeof(float));
  Maxs = (float *)malloc(nTimes*sizeof(float));
  if (Mins == NULL || Maxs == NULL) {
    free(Times);
    printf("Out of memory");
    return -1;
  }

  nStatus = xfGetDatasetMins(a_xScalarId, nTimes, Mins);
  if (nStatus >= 0) {
    nStatus = xfGetDatasetMaxs(a_xScalarId, nTimes, Maxs);
  }
  if (nStatus < 0) {
    free(Times);
    free(Mins);
    free(Maxs);
    return nStatus;
  }

  Values = (float *)malloc(nValues*sizeof(float));
  if (nActive > 0) {
    Active = (xmbool *)malloc(nActive*sizeof(xmbool));
  }

  fprintf(a_fp, "Number Timesteps: %d\n", nTimes);
  fprintf(a_fp, "Number Values: %d\n", nValues);
  fprintf(a_fp, "Number Active: %d\n", nActive);

  // loop through the timesteps, read the values and active values and write
  // them to the text file
  for (iTime = 0; iTime < nTimes; iTime++) {
      // indices should be 1 based
    nStatus = xfReadScalarValuesTimestep(a_xScalarId, iTime + 1,
                                         nValues, Values);
    if (nStatus >= 0 && nActive > 0) {
        // indices should be 1 based
      nStatus = xfReadActivityTimestep(a_xScalarId, iTime + 1, nActive, Active);
    }

    // Write the time, min, max, values and active values to the text output
    // file.
    fprintf(a_fp, "\nTimestep at  %6.3lf\nMin: %6.3lf\nMax: %6.3lf\n", 
                  Times[iTime], Mins[iTime], Maxs[iTime]);

    fprintf(a_fp, "Values:\n");
    // print 5 values per line
    for (iVal = 0; iVal < nValues; iVal++) {
      fprintf(a_fp, "%6.3f ", Values[iVal]);
      if ((iVal + 1) % 5 == 0) {
        fprintf(a_fp, "\n");
      }
    }
    fprintf(a_fp, "\n");

    fprintf(a_fp, "Activity:\n");
    // print 5 values per line
    for (iActive = 0; iActive < nActive; iActive++) {
      fprintf(a_fp, "%4d ", (int)Active[iActive]);
      if ((iActive + 1) % 5 == 0) {
        fprintf(a_fp, "\n");
      }
    }
    fprintf(a_fp, "\n\n");
  }

  if (Times) {
    free(Times);
    Times = NULL;
  }
  
  if (Mins) {
    free(Mins);
    Mins = NULL;
  }

  if (Maxs) {
    free(Maxs);
    Maxs = NULL;
  }

  if (Values) {
    free(Values);
    Values = NULL;
  }

  if (Active) {
    free(Active);
    Active = NULL;
  }

  return TRUE;
} // ttiReadScalar
// --------------------------------------------------------------------------
// FUNCTION ttiReadVector
// PURPOSE  Read a vector from an XMDF file and output information to
//          to a text file
// NOTES    
// --------------------------------------------------------------------------
int ttiReadVector (xid a_xVectorId, FILE *a_fp)
{
  int    nTimes = NONE, nValues = NONE, nComponents = NONE, nActive = NONE;
  int    nStatus = TRUE, iTime, iVal, iActive;
  char   TimeUnits[100];
  double *Times = NULL;
  float  *Values = NULL, *Mins = NULL, *Maxs = NULL;
  xmbool  *Active = NULL;
  xmbool  bUseReftime;
  double Reftime;

  // read the time units
  nStatus = xfGetDatasetTimeUnits(a_xVectorId, TimeUnits);
  if (nStatus < 0) {
    return nStatus;
  }
  fprintf(a_fp, "Time units: %s\n", TimeUnits);

  // see if we are using a reftime
  nStatus = xfUseDatasetReftime(a_xVectorId, &bUseReftime);
  if (nStatus < 0) {
    return nStatus;
  }
  if (bUseReftime) {
    nStatus = xfReadDatasetReftime(a_xVectorId, &Reftime);
    if (nStatus < 0) {
      return nStatus;
    }
    fprintf(a_fp, "Reftime: %f", Reftime);
  }

  // read in the number of values and number of active values
  nStatus = xfGetDatasetNumVals(a_xVectorId, &nValues);
  if (nStatus >= 0) {
    nStatus = xfGetDatasetVecNumComponents(a_xVectorId, &nComponents);
    if (nStatus >= 0) {
      nStatus = xfGetDatasetNumActive(a_xVectorId, &nActive);
    }
  }
  if (nStatus < 0) {
    return nStatus;
  }

  if (nValues <= 0) {
    printf("No data to read in.");
    return -1;
  }

  // read in the number of times
  nStatus = xfGetDatasetNumTimes(a_xVectorId, &nTimes);
  if (nStatus < 0) {
    return nStatus;
  }

  // Read in the individual time values
  Times = (double *)malloc(nTimes*sizeof(double));
  if (Times == NULL) {
    printf("Out of memory");
    return -1;
  }
  nStatus = xfGetDatasetTimes(a_xVectorId, nTimes, Times);
  if (nStatus < 0) {
    return nStatus;
  }

  // Read in the minimum and maximum values
  Mins = (float *)malloc(nTimes*sizeof(float));
  Maxs = (float *)malloc(nTimes*sizeof(float));
  if (Mins == NULL || Maxs == NULL) {
    free(Times);
    printf("Out of memory");
    return -1;
  }

  nStatus = xfGetDatasetMins(a_xVectorId, nTimes, Mins);
  if (nStatus >= 0) {
    nStatus = xfGetDatasetMaxs(a_xVectorId, nTimes, Maxs);
  }
  if (nStatus < 0) {
    free(Times);
    free(Mins);
    free(Maxs);
    return nStatus;
  }

  Values = (float *)malloc(nValues*nComponents*sizeof(float));
  if (nActive > 0) {
    Active = (xmbool *)malloc(nActive*sizeof(xmbool));
  }

  fprintf(a_fp, "Number Timesteps: %d\n", nTimes);
  fprintf(a_fp, "Number Values: %d\n", nValues);
  fprintf(a_fp, "Number Components: %d\n", nComponents);
  fprintf(a_fp, "Number Active: %d\n", nActive);

  // loop through the timesteps, read the values and active values and write
  // them to the text file
  for (iTime = 0; iTime < nTimes; iTime++) {
    nStatus = xfReadVectorValuesTimestep(a_xVectorId, iTime + 1,
                                         nValues, nComponents, Values);
    if (nStatus >= 0 && nActive > 0) {
      nStatus = xfReadActivityTimestep(a_xVectorId, iTime + 1, nActive, Active);
    }

    // Write the time, min, max, values and active values to the text output
    // file.
    fprintf(a_fp, "\nTimestep at  %6.3lf\nMin: %6.3lf\nMax: %6.3lf\n", 
                  Times[iTime], Mins[iTime], Maxs[iTime]);

    fprintf(a_fp, "Values:\n");
    // print a set of vector values per line
    for (iVal = 0; iVal < nValues; iVal++) {
      fprintf(a_fp, "%6.3f %6.3f\n", Values[iVal*nComponents], 
                                     Values[(iVal*nComponents) + 1]);
    }
    fprintf(a_fp, "\n");

    fprintf(a_fp, "Activity:\n");
    // print 5 values per line
    for (iActive = 0; iActive < nActive; iActive++) {
      fprintf(a_fp, "%4d ", (int)Active[iActive]);
      if ((iActive + 1) % 5 == 0) {
        fprintf(a_fp, "\n");
      }
    }
    fprintf(a_fp, "\n\n");
  }

  if (Times) {
    free(Times);
    Times = NULL;
  }
  
  if (Mins) {
    free(Mins);
    Mins = NULL;
  }

  if (Maxs) {
    free(Maxs);
    Maxs = NULL;
  }

  if (Values) {
    free(Values);
    Values = NULL;
  }

  if (Active) {
    free(Active);
    Active = NULL;
  }

  return TRUE;
} // ttiReadVector

// --------------------------------------------------------------------------
// FUNCTION ttiTestNumTimes
// PURPOSE  Change the NumTimes to truncate timesteps
// NOTES    
// --------------------------------------------------------------------------
int ttiTestNumTimes( xid a_DatasetId, int a_Itimestep )
{
  int status = 1;

  // truncate just written timestep and test error conditions
  if (1 == a_Itimestep ||
      3 == a_Itimestep ||
      5 == a_Itimestep)
    {
      int NumTimes;

      // Test setting NumTimes after end of dataset
      status = xfSetDatasetNumTimes( a_DatasetId, a_Itimestep + 2 );
      if (status >= 0)
	printf( "%d: ERROR: xfSetDatasetNumTimes must return ERROR.\n",
		__LINE__ );

      if (1 == a_Itimestep) a_Itimestep = 1;
      if (3 == a_Itimestep) a_Itimestep = 2;
      if (5 == a_Itimestep) a_Itimestep = 3;

      // Write actual NumTimes
      status = xfSetDatasetNumTimes( a_DatasetId, a_Itimestep );
      if (status < 0)
	printf( "%d: ERROR: xfSetDatasetNumTimes must NOT return error.\n",
		__LINE__ );

      // Test setting NumTimes after end step.
      status = xfSetDatasetNumTimes( a_DatasetId, a_Itimestep + 1 );
      if (status >= 0)
	printf( "%d: ERROR: xfSetDatasetNumTimes must return ERROR.\n",
		__LINE__ );

      // Test reading NumTimes
      status = xfGetDatasetNumTimes( a_DatasetId, &NumTimes );
      if (status < 0)
	printf( "%d: ERROR: xfGetDatasetNumTimes must NOT return error.\n",
		__LINE__ );
      if (NumTimes != a_Itimestep)
	printf( "%d: ERROR: xfGetDatasetNumTimes must return CORRECT NumTimes.\n",
		__LINE__ );
    }

  return status;
}
