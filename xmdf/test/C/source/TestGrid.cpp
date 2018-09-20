#include "stdafx.h"
#include <xmdf/Xmdf.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>

#define GRID_CART2D_GROUP_NAME "Grid Cart2D Group"
#define GRID_CURV2D_GROUP_NAME "Grid Curv2D Group"
#define GRID_CART3D_GROUP_NAME "Grid Cart3D Group"
// ---------------------------------------------------------------------------
// FUNCTION  tgReadGrid
// PURPOSE   Read a grid and write data to a text file
// NOTES     
// ---------------------------------------------------------------------------
int tgReadGrid (xid a_Id, FILE *a_Outfile)
{
  int      nGridType = 0, nExtrudeType = 0, nDims = 0, nCellsI = 0, nCellsJ = 0;
  int      nCellsK = 0, nLayers = 0, nOrientation = 0, nValsI = 0, nValsJ = 0;
  int      nValsK = 0, nExtrudeVals = 0, nCompOrigin = 1, nUDir = 1;
  char     strGridType[256], strExtrudeType[256];
  xmbool    bDefined = XFALSE;
  double   dOrigin[3], dBearing = 0.0, dDip = 0.0, dRoll =0.0;
  double  *dExtrudeVals = NULL, *dCoordI = NULL, *dCoordJ = NULL;
  double  *dCoordK = NULL;
  int      i = 0, error = 1;

  // Grid type
  error = xfGetGridType(a_Id, &nGridType);
  if (error < 0) {
    return error;
  }
  switch (nGridType) {
    case GRID_TYPE_CARTESIAN:
      strcpy(strGridType, "Cartesian");
      break;
    case GRID_TYPE_CURVILINEAR:
      strcpy(strGridType, "Curvilinear");
      break;
    case GRID_TYPE_CARTESIAN_EXTRUDED:
      strcpy(strGridType, "Cartesian extruded");
      break;
    case GRID_TYPE_CURVILINEAR_EXTRUDED:
      strcpy(strGridType, "Curvilinear extruded");
      break;
    default:
      printf("Invalid grid type.");
      return -1;
      break;
  }
  fprintf(a_Outfile, "The grid type is: %s\n", strGridType);

  // Number of dimensions
  error = xfGetNumberOfDimensions(a_Id, &nDims);
  if (error < 0) {
    return error;
  }
  if (nDims == 2) {
    fprintf(a_Outfile, "The grid is two-dimensional\n");
  }
  else if (nDims == 3) {
    fprintf(a_Outfile, "The grid is three-dimensional\n");
  }
  else {
    printf("The grid dimensions are invalid.");
    return -1;
  }

  // Extrusion type if applicable
  if (nGridType == GRID_TYPE_CARTESIAN_EXTRUDED ||
      nGridType == GRID_TYPE_CURVILINEAR_EXTRUDED) {
    error = xfGetExtrusionType(a_Id, &nExtrudeType);
    if (error < 0) {
      return error;
    }
    switch(nExtrudeType) {
      case EXTRUDE_SIGMA:
        strcpy(strExtrudeType, "Sigma stretch");
        break;
      case EXTRUDE_CARTESIAN:
        strcpy(strExtrudeType, "Cartesian");
        break;
      case EXTRUDE_CURV_AT_CORNERS:
        strcpy(strExtrudeType, "Curvilinear at Corners");
        break;
      case EXTRUDE_CURV_AT_CELLS:
        strcpy(strExtrudeType, "Curvilinear at Cells");
        break;
    }
    fprintf(a_Outfile, "The grid is extruding using: %s\n", strExtrudeType);
  }

  // Origin
  error = xfOriginDefined(a_Id, &bDefined);
  if (error < 0) {
    return error;
  }
  if (bDefined) {
    error = xfGetOrigin(a_Id, &dOrigin[0], &dOrigin[1], &dOrigin[2]);
    if (error < 0) {
      return error;
    }
    fprintf(a_Outfile, "The grid origin is %lf %lf %lf\n", dOrigin[0], 
                       dOrigin[1], dOrigin[2]);
  }
  
  // Orientation
  error = xfGetOrientation(a_Id, &nOrientation);
  if (error < 0) {
    return error;
  }
  if (nOrientation == ORIENTATION_RIGHT_HAND) {
    fprintf(a_Outfile, "The grid has a right hand orientation\n");
  }
  else if (nOrientation == ORIENTATION_LEFT_HAND) {
    fprintf(a_Outfile, "The grid has a left hand orientation\n");
  }
  else {
    printf("Invalid grid orientation.");
    return -1;
  }

  // Bearing
  error = xfBearingDefined(a_Id, &bDefined);
  if (error < 0) {
    return error;
  }
  if (bDefined) {
    error = xfGetBearing(a_Id, &dBearing);
    if (error < 0) {
      return error;
    }
    fprintf(a_Outfile, "The grid bearing is %lf\n", dBearing);
  }

  // Dip
  error = xfDipDefined(a_Id, &bDefined);
  if (error < 0) {
    return error;
  }
  if (bDefined) {
    error = xfGetDip(a_Id, &dDip);
    if (error < 0) {
      return error;
    }
    fprintf(a_Outfile, "The grid Dip is %lf\n", dDip);
  }

  if (nDims == 3) {
    // Roll
    error = xfRollDefined(a_Id, &bDefined);
    if (error < 0) {
      return error;
    }
    if (bDefined) {
      error = xfGetRoll(a_Id, &dRoll);
      if (error < 0) {
        return error;
      }
      fprintf(a_Outfile, "The grid Roll is %lf\n", dRoll);
    }
  }
  // Computational origin
  error = xfComputationalOriginDefined(a_Id, &bDefined);
  if (error < 0) {
    return error;
  }
  if (bDefined) {
    error = xfGetComputationalOrigin(a_Id, &nCompOrigin);
    if (error < 0) {
      return error;
    }
    fprintf(a_Outfile, "The grid Computational Origin is %d\n", nCompOrigin);
  }
  else {
    fprintf(a_Outfile, "The grid Computational Origin is not defined\n");
  }


  // U Direction
  error = xfGetUDirectionDefined(a_Id, &bDefined);
  if (error < 0) {
    return error;
  }
  if (bDefined) {
    error = xfGetUDirection(a_Id, &nUDir);
    if (error < 0) {
      return error;
    }
    fprintf(a_Outfile, "The grid U Direction is %d\n", nUDir);
  }
  else {
    fprintf(a_Outfile, "The grid U Direction is not defined\n");
  }


  // number of cells in each direction
  error = xfGetNumberCellsInI(a_Id, &nCellsI);
  if (error >= 0) {
    error = xfGetNumberCellsInJ(a_Id, &nCellsJ);
    if (error >= 0 && nDims == 3) { 
      error = xfGetNumberCellsInK(a_Id, &nCellsK);
    }
  }
  if (error < 0) {
    return error;
  }
  fprintf(a_Outfile, "Number of cells in I %d\n", nCellsI);
  fprintf(a_Outfile, "Number of cells in J %d\n", nCellsJ);
  if (nDims == 3) {
    fprintf(a_Outfile, "Number of cells in K %d\n", nCellsK);
  }

  // Grid coordinates
  switch (nGridType) {
    case GRID_TYPE_CARTESIAN:
    case GRID_TYPE_CARTESIAN_EXTRUDED:
      nValsI = nCellsI;
      nValsJ = nCellsJ;
      if (nDims == 3) {
        nValsK = nCellsK;
      }
      break;
    case GRID_TYPE_CURVILINEAR:
    case GRID_TYPE_CURVILINEAR_EXTRUDED:
      if (nDims == 3) {
        // three dimensions
        nValsI = nValsJ = nValsK = (nCellsI + 1) * (nCellsJ + 1) *(nCellsK + 1);
      }
      else {
        // two dimensions
        nValsI = nValsJ = (nCellsI + 1) * (nCellsJ + 1);
      }
      break;
    default:
      printf("Invalid grid type.");
      return -1;
      break;
  }

  dCoordI = new double[nValsI];
  dCoordJ = new double[nValsJ];
  if (nDims == 3) {
    dCoordK = new double[nValsK];
  }

  error = xfGetGridCoordsI(a_Id, nValsI, dCoordI);
  if (error >= 0) {
    error = xfGetGridCoordsJ(a_Id, nValsJ, dCoordJ);
    if (error >= 0 && nDims == 3) {
      error = xfGetGridCoordsK(a_Id, nValsK, dCoordK);
    }
  }
  if (error < 0) {
    printf("Error reading coordinates.\n");
    return -1;
  }

  fprintf(a_Outfile, "The Coordinates in direction I:\n");
  for (i = 0; i < nValsI; i++) {
    if ((i + 1) % 5 == 0) {
      fprintf(a_Outfile, "\n");
    }
    fprintf(a_Outfile, "%lf ", dCoordI[i]);
  }
  fprintf(a_Outfile, "\n");

  fprintf(a_Outfile, "The Coordinates in direction J:\n");
  for (i = 0; i < nValsJ; i++) {
    if ((i + 1) % 5 == 0) {
      fprintf(a_Outfile, "\n");
    }
    fprintf(a_Outfile, "%lf ", dCoordJ[i]);
  }
  fprintf(a_Outfile, "\n");

  if (nDims == 3) {
    fprintf(a_Outfile, "The Coordinates in direction K:\n");
    for (i = 0; i < nValsK; i++) {
      if ((i + 1) % 5 == 0) {
        fprintf(a_Outfile, "\n");
      }
      fprintf(a_Outfile, "%lf ", dCoordK[i]);
    }
  }
  fprintf(a_Outfile, "\n");

  if (dCoordI) {
    delete dCoordI;
  }
  if (dCoordJ) {
    delete dCoordJ;
  }
  if (dCoordK) {
    delete dCoordK;
  }

  // Extrude data
  if (nGridType == GRID_TYPE_CARTESIAN_EXTRUDED ||
      nGridType == GRID_TYPE_CURVILINEAR_EXTRUDED) {
    error = xfGetExtrudeNumLayers(a_Id, &nLayers);
    if (error < 0) {
      return error;
    }

    switch(nExtrudeType) {
      case EXTRUDE_SIGMA:
        nExtrudeVals = nLayers;
        break;

      case EXTRUDE_CURV_AT_CORNERS:
        nExtrudeVals = (nCellsI + 1) * (nCellsJ + 1) * nLayers;
        break;
      
      case EXTRUDE_CURV_AT_CELLS:
        nExtrudeVals = nCellsI * nCellsJ * nLayers;
        break;
    }
    dExtrudeVals = new double[nExtrudeVals];

    error = xfGetExtrudeValues(a_Id, nExtrudeVals, dExtrudeVals);
    if (error < 0) {
      return error;
    }

    printf("The extrude values are:\n");
    for (i = 0; i < nExtrudeVals; i++) {
      if ((i + 1) % 5 == 0) {
        fprintf(a_Outfile, "\n");
      }
      fprintf(a_Outfile, "%lf ", dExtrudeVals[i]);
    }

    if (dExtrudeVals) {
      delete dExtrudeVals;
    }
  }

  return error;
} // tgReadGrid
//////////////////////////////////////////////////////////////////////////////
// FUNCTION   tgWriteTestGridCart2D
// PURPOSE    Write a file that contains data for a 2D Cartesian Grid
// NOTES      A picture of the grid is in the file (TestGridCart2D.gif)
//            returns TRUE on success and FALSE on failure
//////////////////////////////////////////////////////////////////////////////
int tgWriteTestGridCart2D(LPCSTR Filename, int Compression)
{
  int         nDimensions = 2;
  int         nCellsI = 5, nCellsJ = 5;
  int         nGridType = GRID_TYPE_CARTESIAN;
  int         nCompOrigin = 4, nUDir = -2;
  double      dOriginX = 10.0, dOriginY = 10.0, dOriginZ = 0.0;
  int         nOrientation = ORIENTATION_RIGHT_HAND;
  double      dBearing = 45.0;
  double      PlanesI[5], PlanesJ[5];
  int         i, j, iSpcZone;
  xid         xFileId = NONE, xGridId = NONE, xCoordId = NONE;
  int         status;

  // Fill in the grid plane data with a constant size of 30
  for (i = 1; i <= nCellsI; i++) {
    PlanesI[i - 1] = (double)i*30.0;
  }
  for (j = 1; j <= nCellsJ; j++) {
    PlanesJ[j - 1] = (double)j*30.0;
  }
  
  // create the file
  status = xfCreateFile(Filename, &xFileId, XTRUE);
  if (status < 0) {
    return FALSE;
  }

  // create the group to store the grid
  status = xfCreateGroupForGrid(xFileId, GRID_CART2D_GROUP_NAME, &xGridId);
  if (status < 0) {
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Write the grid information to the file
  if (xfSetGridType(xGridId, nGridType) < 0 ||
      xfSetNumberOfDimensions(xGridId, nDimensions) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // set origin and orientation
  if (xfSetOrigin(xGridId, dOriginX, dOriginY, dOriginZ) < 0 ||
      xfSetOrientation(xGridId, nOrientation) < 0 ) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }
 
  // Set bearing
  if (xfSetBearing(xGridId, dBearing) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Set computational origin
  if (xfSetComputationalOrigin(xGridId, nCompOrigin) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Set u direction
  if (xfSetUDirection(xGridId, nUDir) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Write the grid geometry to the file
  // Set the number of cells in each direction
  if (xfSetNumberCellsInI(xGridId, nCellsI) < 0 ||
      xfSetNumberCellsInJ(xGridId, nCellsJ) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Set the grid plane locations
  if (xfSetGridCoordsI(xGridId, nCellsI, PlanesI) < 0 ||
      xfSetGridCoordsJ(xGridId, nCellsJ, PlanesJ) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Write Coordinate file - for GridCart2D, we will set the coordinate system
  //   to be State Plane NAD27.
  status = xfCreateCoordinateGroup(xFileId, &xCoordId);
  if (status <= 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
	return -1;
  }

  iSpcZone = 3601; // Oregon North

  xfSetHorizDatum(xCoordId, HORIZ_DATUM_STATE_PLANE_NAD27);
  xfSetHorizUnits(xCoordId, COORD_UNITS_US_FEET);

  xfSetVertDatum(xCoordId, VERT_DATUM_LOCAL);
  xfSetVertUnits(xCoordId, COORD_UNITS_US_FEET);

    // write additional information
  xfSetSPCZone(xCoordId, iSpcZone);

  xfCloseGroup(xCoordId);
  xCoordId = 0;

  // release memory
  xfCloseGroup(xGridId);
  xfCloseFile(xFileId);
  return TRUE;
} // tgWriteTestGridCart2D
//////////////////////////////////////////////////////////////////////////////
// FUNCTION   tgWriteTestGridCurv2D
// PURPOSE    Write a file that contains data for a 2D Curvilinear Grid
// NOTES      A picture of the grid is TestGridCurv2D.gif
//            returns TRUE on success and FALSE on failure
//////////////////////////////////////////////////////////////////////////////
int tgWriteTestGridCurv2D(LPCSTR Filename, int Compression)
{
  int         nDimensions = 2;
  int         nCompOrigin = 1, nUDir = 1;
  int         nCellsI = 2, nCellsJ = 3;
  int         nCells = nCellsI*nCellsJ;
  int         nCorners = (nCellsI + 1)*(nCellsJ + 1);
  int         nGridType = GRID_TYPE_CURVILINEAR, i;
  double      xVals[16], yVals[16]; // 16 is the number of corners
  double      dCppLat, dCppLon;
  xid         xFileId = NONE, xGridId = NONE, xPropId = NONE,
              xDatasetsId = NONE, xScalarId = NONE, xCoordId = NONE;
  double      dNullValue = -999.0;
  int         nOrientation = ORIENTATION_RIGHT_HAND;
  float       fDsetCellVals[6]; // for cell-centered dataset
  float       fDsetCornerVals[12]; // for corner-centered dataset
  xmbool       bDsetCellActive[6];
  xmbool       bDsetCornerActive[12];
  int         status;

  // There is no cell in the top right corner so we have a NullValue for
  // the top right corner

  // xValues row by row
  xVals[0] = 0.0; xVals[1] = 7.5; xVals[2] = 15.0;
  xVals[3] = 2.5; xVals[4] = 10.0; xVals[5] = 17.5;
  xVals[6] = 3.5; xVals[7] = 11.0; xVals[8] = 18.5;
  xVals[9] = 0.0; xVals[10] = 7.5; xVals[11] = dNullValue;

  // yValues row by row
  yVals[0] = 0.0; yVals[1] = 0.0; yVals[2] = 0.0;
  yVals[3] = 10.0; yVals[4] = 10.0; yVals[5] = 10.0;
  yVals[6] = 20.0; yVals[7] = 20.0; yVals[8] = 20.0;
  yVals[9] = 30.0; yVals[10] = 30.0; yVals[11] = dNullValue;

  // cell centered velocity dataset values
  fDsetCellVals[0] = (float)2.1; fDsetCellVals[1] = (float)2.0;
  fDsetCellVals[2] = (float)1.9; fDsetCellVals[3] = (float)2.3;
  fDsetCellVals[4] = (float)2.5; fDsetCellVals[5] = (float)dNullValue;
  // all are active except the last value
  for (i = 0; i < nCells; i++) {
    bDsetCellActive[i] = XTRUE;
  }
  bDsetCellActive[nCells - 1] = XFALSE;

  // corner centered elevation dataset values
  fDsetCornerVals[0] = (float)1.0; fDsetCornerVals[1] = (float)0.8; 
  fDsetCornerVals[2] = (float)1.2;
  fDsetCornerVals[3] = (float)1.4; fDsetCornerVals[4] = (float)1.8;
  fDsetCornerVals[5] = (float)2.2;
  fDsetCornerVals[6] = (float)1.8; fDsetCornerVals[7] = (float)1.4;
  fDsetCornerVals[8] = (float)2.0;
  fDsetCornerVals[9] = (float)1.0; fDsetCornerVals[10] = (float)1.8;
  fDsetCornerVals[11] = (float)2.2;
  // all are active except the last value
  for (i = 0; i < nCorners; i++) {
    bDsetCornerActive[i] = XTRUE;
  }
  bDsetCornerActive[nCorners - 1] = XFALSE;

  // create the file
  status = xfCreateFile(Filename, &xFileId, XTRUE);
  if (status < 0) {
    return FALSE;
  }

  // create the group to store the grid
  status = xfCreateGroupForGrid(xFileId, GRID_CURV2D_GROUP_NAME, &xGridId);
  if (status < 0) {
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Write the grid information to the file
  if (xfSetGridType(xGridId, nGridType) < 0 ||
      xfSetNumberOfDimensions(xGridId, nDimensions) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // set orientation
  if (xfSetOrientation(xGridId, nOrientation) < 0 ) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Set computational origin
  if (xfSetComputationalOrigin(xGridId, nCompOrigin) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Set u direction
  if (xfSetUDirection(xGridId, nUDir) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Write the grid geometry to the file
  // Set the number of cells in each direction
  if (xfSetNumberCellsInI(xGridId, nCellsI) < 0 ||
      xfSetNumberCellsInJ(xGridId, nCellsJ) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Set a NullValue.  This is used to identify locations in the grid that are
  // not being used.  In our case no geometry is defined for the top right
  // corner.
  if (xfCreateGridPropertyGroup(xGridId, &xPropId) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  if (xfWritePropertyDouble(xPropId, PROP_NULL_VALUE, 1, &dNullValue, NONE) < 0){
    xfCloseGroup(xPropId);
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }
  xfCloseGroup(xPropId);

  // Set the grid plane locations
  if (xfSetGridCoordsI(xGridId, nCorners, xVals) < 0 ||
      xfSetGridCoordsJ(xGridId, nCorners, yVals) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Create the datasets group
  if (xfCreateGenericGroup(xGridId, "Datasets", &xDatasetsId) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Create the cell-centered dataset
  if (xfCreateScalarDataset(xDatasetsId, "Velocity Mag",  "ft/s", TS_MINUTES,
                            Compression, &xScalarId) < 0) {
    xfCloseGroup(xDatasetsId);
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // specify that the dataset is cell-centered
  if (xfScalarDataLocation(xScalarId, GRID_LOC_CENTER) < 0) {
    xfCloseGroup(xScalarId);
    xfCloseGroup(xDatasetsId);
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Write the data
  if (xfWriteScalarTimestep(xScalarId, 0.0, nCells, fDsetCellVals) < 0 ||
      xfWriteActivityTimestep(xScalarId, nCells, bDsetCellActive) < 0) {
    xfCloseGroup(xScalarId);
    xfCloseGroup(xDatasetsId);
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // close the cell-centered dataset
  xfCloseGroup(xScalarId);

  // Create the corner-centered dataset
  if (xfCreateScalarDataset(xDatasetsId, "elevation",  "ft", TS_MINUTES,
                            Compression, &xScalarId) < 0) {
    xfCloseGroup(xDatasetsId);
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // specify that the dataset is corner-centered
  if (xfScalarDataLocation(xScalarId, GRID_LOC_CORNER) < 0) {
    xfCloseGroup(xScalarId);
    xfCloseGroup(xDatasetsId);
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Write the data
  if (xfWriteScalarTimestep(xScalarId, 0.0, nCorners, fDsetCornerVals) < 0 ||
      xfWriteActivityTimestep(xScalarId, nCorners, bDsetCornerActive) < 0){
    xfCloseGroup(xScalarId);
    xfCloseGroup(xDatasetsId);
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Write Coordinate file - for GridCurv2D, we will set the coordinate system
  //   to be CPP, with CPP Latitude and CPP Longitude settings written 
  //   to the file.
  status = xfCreateCoordinateGroup(xFileId, &xCoordId);
  if (status <= 0) {
    xfCloseGroup(xScalarId);
    xfCloseGroup(xDatasetsId);
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
	  return -1;
  }

  dCppLat = 56.0;   // Made-up value
  dCppLon = 23.0;   // Made-up value

  xfSetHorizDatum(xCoordId, HORIZ_DATUM_CPP);
  xfSetHorizUnits(xCoordId, COORD_UNITS_METERS);

  xfSetVertDatum(xCoordId, VERT_DATUM_LOCAL);
  xfSetVertUnits(xCoordId, COORD_UNITS_METERS);

    // write additional information
  xfSetCPPLat(xCoordId, dCppLat);
  xfSetCPPLon(xCoordId, dCppLon);

  xfCloseGroup(xCoordId);
  xCoordId = 0;

  // release memory
  xfCloseGroup(xScalarId);
  xfCloseGroup(xDatasetsId);
  xfCloseGroup(xGridId);
  xfCloseFile(xFileId);
  return TRUE;
} // tgWriteTestGridCurv2D
//////////////////////////////////////////////////////////////////////////////
// FUNCTION   tgWriteTestGridCart3D
// PURPOSE    Write a file that contains data for a 2D Cartesian Grid
// NOTES      A picture of the grid is in the file (TestGridCart2D.gif)
//            returns TRUE on success and FALSE on failure
//////////////////////////////////////////////////////////////////////////////
int tgWriteTestGridCart3D(LPCSTR Filename, int Compression)
{
  int         nDimensions = 3;
  int         nCompOrigin = 8, nUDir = -2;
  int         nCellsI = 5, nCellsJ = 5, nCellsK = 3;
  int         nGridType = GRID_TYPE_CARTESIAN;
  double      dOriginX = 10.0, dOriginY = 10.0, dOriginZ = 0.0;
  int         nOrientation = ORIENTATION_RIGHT_HAND;
  double      dBearing = 45.0, dDip = 0.0, dRoll = 0.0;
  double      PlanesI[5], PlanesJ[5], PlanesK[3];
  int         i, j, status, iSpcZone;
  xid         xFileId = NONE, xGridId = NONE;
  xid         xPropId = NONE, xCoordId = NONE;
  int         nCells = nCellsI * nCellsJ * nCellsK;
  int         Active[75];

  // Fill in the grid plane data with a constant size of 30
  for (i = 1; i <= nCellsI; i++) {
    PlanesI[i - 1] = (double)i*30.0;
  }
  for (j = 1; j <= nCellsJ; j++) {
    PlanesJ[j - 1] = (double)j*30.0;
  }
  for (j = 1; j <= nCellsK; j++) {
    PlanesK[j - 1] = (double)j*30.0;
  }

  // fill in the activity array
  // default array to active
  for (i = 0; i < nCells; i++) {
    Active[i] = (int)XTRUE;
  }

  // two cells are inactive (identified by array index)
  // i = 0, j = 0, k = 0  and i = 4, j = 4, k = 0
  Active[0] = (int)XFALSE;
  Active[4*nCellsJ*nCellsK+4*nCellsK] = (int)XFALSE;

  // create the file
  if (xfCreateFile(Filename, &xFileId, XTRUE) < 0) {
    return FALSE;
  }

  // create the group to store the grid
  if (xfCreateGroupForGrid(xFileId, GRID_CART3D_GROUP_NAME, &xGridId) < 0) {
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Write the grid information to the file
  if (xfSetGridType(xGridId, nGridType) < 0 ||
      xfSetNumberOfDimensions(xGridId, nDimensions) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // set origin and orientation
  if (xfSetOrigin(xGridId, dOriginX, dOriginY, dOriginZ) < 0 ||
      xfSetOrientation(xGridId, nOrientation) < 0 ) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }
 
  // Set bearing, dip and roll
  if (xfSetBearing(xGridId, dBearing) < 0 ||
      xfSetDip(xGridId, dDip) < 0 ||
      xfSetRoll(xGridId, dRoll) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Set computational origin
  if (xfSetComputationalOrigin(xGridId, nCompOrigin) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Set u direction
  if (xfSetUDirection(xGridId, nUDir) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Write the grid geometry to the file
  // Set the number of cells in each direction
  if (xfSetNumberCellsInI(xGridId, nCellsI) < 0 ||
      xfSetNumberCellsInJ(xGridId, nCellsJ) < 0 ||
      xfSetNumberCellsInK(xGridId, nCellsK) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Set the grid plane locations
  if (xfSetGridCoordsI(xGridId, nCellsI, PlanesI) < 0 ||
      xfSetGridCoordsJ(xGridId, nCellsJ, PlanesJ) < 0 ||
      xfSetGridCoordsK(xGridId, nCellsK, PlanesK) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  // Write the activity array
  if (xfCreateGridCellPropertyGroup(xGridId, &xPropId) < 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }

  if (xfWritePropertyInt(xPropId, PROP_ACTIVITY, nCells, Active, 
                         Compression) < 0) {
    xfCloseGroup(xPropId);
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
    return FALSE;
  }
  xfCloseGroup(xPropId);

  // Write Coordinate file - for GridCart3D, we will set the coordinate system
  //   to be State Plane NAD27.
  status = xfCreateCoordinateGroup(xFileId, &xCoordId);
  if (status <= 0) {
    xfCloseGroup(xGridId);
    xfCloseFile(xFileId);
	  return -1;
  }

  iSpcZone = 3601; // Oregon North

  xfSetHorizDatum(xCoordId, HORIZ_DATUM_STATE_PLANE_NAD27);
  xfSetHorizUnits(xCoordId, COORD_UNITS_US_FEET);

  xfSetVertDatum(xCoordId, VERT_DATUM_LOCAL);
  xfSetVertUnits(xCoordId, COORD_UNITS_US_FEET);

    // write additional information
  xfSetSPCZone(xCoordId, iSpcZone);

  xfCloseGroup(xCoordId);
  xCoordId = 0;

  // release memory
  xfCloseGroup(xGridId);
  xfCloseFile(xFileId);
  return TRUE;
} // tgWriteTestGridCart3D
