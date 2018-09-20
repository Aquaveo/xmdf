#include "stdafx.h"
#include <xmdf/Xmdf.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>

#define MESH_A_GROUP_NAME "MeshA Group"
#define MESH_B_GROUP_NAME "MeshB Group"
// ---------------------------------------------------------------------------
// FUNCTION  tmReadMesh
// PURPOSE   
// NOTES     
// ---------------------------------------------------------------------------
int tmReadMesh (xid xGroupId, FILE *a_OutFile)
{
  int    nElems = 0, nNodes = 0, nNodesPerElem = 0, nElemType, nNodeId;
  xmbool  bElementsOneType;
  int    status;
  int   *ElemTypes = NULL;
  int    i, j, StrType, UIntType, IntType, DblType, FloatType;
  int   *NodesInElem = NULL;
  double *XNodeLocs, *YNodeLocs, *ZNodeLocs;
  xid     xPropGrpId;

  // Get the number of elements, nodes, and Maximum number of nodes per element
  status = xfGetNumberOfElements(xGroupId, &nElems);
  if (status >= 0) {
    status = xfGetNumberOfNodes(xGroupId, &nNodes);
    if (status >= 0) {
      status = xfGetMaxNodesInElem(xGroupId, &nNodesPerElem);
    }
  }
  if (status < 0) {
    return -1;
  }

  // Do Element information first
  fprintf(a_OutFile, "Number of Elements: %d\n", nElems);

  // Element types
  status = xfAreAllElemsSameType(xGroupId, &bElementsOneType);
  if (status < 0) {
    return -1;
  }

  if (bElementsOneType == (xmbool)XTRUE) {
    status = xfReadElemTypesSingleValue(xGroupId, &nElemType);
    fprintf(a_OutFile, "All elements are type %d\n", nElemType);
  }
  else {
    ElemTypes = new int[nElems];
    if (ElemTypes == NULL) {
      printf("Memory Error");
      return -1;
    }
    status = xfReadElemTypes(xGroupId, nElems, ElemTypes);
    if (status < 0) {
      return -1;
    }
    fprintf(a_OutFile, "Element Types:\n");
    for (i = 0; i < nElems; i++) {
      fprintf(a_OutFile, "Elem %d, Type %d\n", i+1, ElemTypes[i]);
    }
    delete ElemTypes;
    ElemTypes = NULL;
  }

  // Nodes in each element
  NodesInElem  = new int[nElems*nNodesPerElem];
  xfReadElemNodeIds(xGroupId, nElems, nNodesPerElem, NodesInElem);
  for (i = 0; i < nElems; i++) {
    fprintf(a_OutFile, "Elem: %4d - ", i+1);
    for (j = 0; j < nNodesPerElem; j++) {
      nNodeId = NodesInElem[i*nNodesPerElem + j];
      if (nNodeId > 0) { // -1 is for unused array locations
        fprintf(a_OutFile, "%d ", nNodeId);
      }
    }
    fprintf(a_OutFile, "\n");
  }
  delete NodesInElem;
  NodesInElem = NULL;

  // NodeLocations
  XNodeLocs = new double[nNodes];
  YNodeLocs = new double[nNodes];
  ZNodeLocs = new double[nNodes];
  if (XNodeLocs == NULL || YNodeLocs == NULL || ZNodeLocs == NULL) {
    if (XNodeLocs != NULL) {
      delete XNodeLocs;
      XNodeLocs = NULL;
    }
    if (YNodeLocs != NULL) {
      delete YNodeLocs;
      YNodeLocs = NULL;
    }
    if (ZNodeLocs != NULL) {
      delete ZNodeLocs;
      ZNodeLocs = NULL;
    }
    printf("Memory Error!");
    return -1;
  }

  status = xfReadXNodeLocations(xGroupId, nNodes, XNodeLocs);
  if (status >= 0) {
    status = xfReadYNodeLocations(xGroupId, nNodes, YNodeLocs);
    if (status >= 0) {
      status = xfReadZNodeLocations(xGroupId, nNodes, ZNodeLocs);
    }
    else {
      return -1;
    }
  }
  else {
    return -1;
  }

  fprintf(a_OutFile, "Node Locations:\n");
  for (i = 0; i < nNodes; i++) {
    fprintf(a_OutFile, "Node: %d Location: %lf %lf %lf\n", i + 1, XNodeLocs[i],
                        YNodeLocs[i], ZNodeLocs[i]);
  }

  // Open the property group
  status = xfOpenGroup(xGroupId, "PROPERTIES", &xPropGrpId);
  if (status < 0) {
    fprintf(a_OutFile, "\n");
    fprintf(a_OutFile, "Properties Group not found\n");
    fprintf(a_OutFile, "\n");
    return -1; 
  }

  // Get the Property Types
  status = xfGetPropertyType(xPropGrpId, "String", &StrType);
  status = xfGetPropertyType(xPropGrpId, "UInt", &UIntType);
  status = xfGetPropertyType(xPropGrpId, "Int", &IntType);
  status = xfGetPropertyType(xPropGrpId, "Double", &DblType);
  status = xfGetPropertyType(xPropGrpId, "Float", &FloatType);

    // Property Types:
  fprintf(a_OutFile, "\n");
  if (StrType == XF_TYPE_STRING) {
    fprintf(a_OutFile, "String Property Type Read Correctly\n");
  }
  else {
    fprintf(a_OutFile, "Error in Getting String Property Type\n");
  }
  if (UIntType == XF_TYPE_UINT) {
    fprintf(a_OutFile, "Unsigned Integer Property Type Read Correctly\n");
  }
  else {
    fprintf(a_OutFile, "Error in Getting Unsigned Integer Property Type\n");
  }
  if (IntType == XF_TYPE_INT) {
    fprintf(a_OutFile, "Integer Property Type Read Correctly\n");
  }
  else {
    fprintf(a_OutFile, "Error in Getting Integer Property Type\n");
  }
  if (DblType == XF_TYPE_DOUBLE) {
    fprintf(a_OutFile, "Double Property Type Read Correctly\n");
  }
  else {
    fprintf(a_OutFile, "Error in Getting Double Property Type\n");
  }
  if (FloatType == XF_TYPE_FLOAT) {
    fprintf(a_OutFile, "Float Property Type Read Correctly\n");
  }
  else {
    fprintf(a_OutFile, "Error in Getting Float Property Type\n");
  }
  fprintf(a_OutFile, "\n");

  if (XNodeLocs) {
    delete XNodeLocs;
  }
  if (YNodeLocs) {
    delete YNodeLocs;
  }
  if (ZNodeLocs) {
    delete ZNodeLocs;
  }

  return TRUE;
} // tmReadMesh
//////////////////////////////////////////////////////////////////////////////
// FUNCTION   tmWriteTestMeshA
// PURPOSE    Write a file that contains data for an all tri mesh
// NOTES      A picture of the mesh is in the file (TestMeshA.gif)
//            returns TRUE on success and FALSE on failure
//////////////////////////////////////////////////////////////////////////////
int tmWriteTestMeshA(LPCSTR Filename, int Compression)
{
  int         nElements = 3, nNodes = 5;
  xid         xFileId = NONE, xMeshId = NONE;
  xid         xPropGrpId = NONE, xCoordId = NONE;
  double      dNodeLocsX[5], dNodeLocsY[5], dNodeLocsZ[5];
  int         iElementNodes[3][3]; // numelements, max elems per node
  int         status, propint, iEllipse;
  char          *propstring;
  unsigned int  propuint;
  double        propdouble, dMajorR, dMinorR;
  float         propfloat;

  // Setup the arrays for the mesh data
    // nodes
  dNodeLocsX[0] = 0.0; dNodeLocsY[0] = 5.0; dNodeLocsZ[0] = 0.0;
  dNodeLocsX[1] = 5.0; dNodeLocsY[1] = 5.0; dNodeLocsZ[1] = 0.0;
  dNodeLocsX[2] = 0.0; dNodeLocsY[2] = 0.0; dNodeLocsZ[2] = 0.0;
  dNodeLocsX[3] = 5.0; dNodeLocsY[3] = 0.0; dNodeLocsZ[3] = 0.0;
  dNodeLocsX[4] = 7.5; dNodeLocsY[4] = 2.5; dNodeLocsZ[4] = 0.0;
  
    // nodes for each element
    // must be counter-clockwize
  iElementNodes[0][0] = 1; iElementNodes[0][1] = 3; iElementNodes[0][2] = 2;
  iElementNodes[1][0] = 2; iElementNodes[1][1] = 3; iElementNodes[1][2] = 4;
  iElementNodes[2][0] = 5; iElementNodes[2][1] = 2; iElementNodes[2][2] = 4;

  // create the file
  status = xfCreateFile(Filename, &xFileId, XTRUE);
  if (status < 0) {
    return FALSE;
  }

  // create the group to store the mesh
  status = xfCreateGroupForMesh(xFileId, MESH_A_GROUP_NAME, &xMeshId);
  if (status < 0) {
    return FALSE;
  }

  // Element types - all are linear triangles
  status = xfSetAllElemsSameType(xMeshId, EL_TYPE_TRI_LINEAR);
  if (status < 0) {
    return FALSE;
  }

  // node information
  status = xfSetNumberOfNodes(xMeshId, nNodes);
  if (status < 0) {
    return FALSE;
  }

  status = xfWriteXNodeLocations(xMeshId, nNodes, dNodeLocsX, Compression);
  if (status < 0) {
    return FALSE;
  }
  status = xfWriteYNodeLocations(xMeshId, nNodes, dNodeLocsY);
  if (status < 0) {
    return FALSE;
  }
  status = xfWriteZNodeLocations(xMeshId, nNodes, dNodeLocsZ);
  if (status < 0) {
    return FALSE;
  }

  // element information
  status = xfSetNumberOfElements(xMeshId, nElements);
  if (status < 0) {
    return FALSE;
  }
 
  // Write the node array ids for the nodes in each element
  status = xfWriteElemNodeIds(xMeshId, nElements, 3, &iElementNodes[0][0],
                              Compression);
  if (status < 0) {
    return FALSE;
  }

  // Write the property file
  status = xfCreateMeshPropertyGroup(xMeshId, &xPropGrpId);
  if (status < 0) {
    xfCloseGroup(xFileId);
    return FALSE;
  }
  propstring = "Property String";
  propuint = 5;
  propint = -5;
  propdouble = 5.6789012345;
  propfloat = (float)5.6789;

  status = xfWritePropertyString(xPropGrpId, "String", 1, 
                                 strlen(propstring), propstring);
  status = xfWritePropertyUnsignedInt(xPropGrpId, "UInt", 1, &propuint, NONE);
  status = xfWritePropertyInt(xPropGrpId, "Int", 1, &propint, NONE);
  status = xfWritePropertyDouble(xPropGrpId, "Double", 1, &propdouble, NONE);
  status = xfWritePropertyFloat(xPropGrpId, "Float", 1, &propfloat, NONE);

  // Write Coordinate file - for MeshA, we will set the coordinate system to be
  //   Geogrpahic, with Latitude, Longitude, and user-defined ellipsoid settings
  //   written to the file.
  status = xfCreateCoordinateGroup(xFileId, &xCoordId);
  if (status <= 0) {
    xfCloseGroup(xPropGrpId);
    xfCloseGroup(xMeshId);
    xfCloseFile(xFileId);
    return -1;
  }

    // set coordinate values
  iEllipse = 32;   // User defined
  dMajorR = 45.0;    // Made up
  dMinorR = 32.0;    // Made up

  xfSetHorizDatum(xCoordId, HORIZ_DATUM_GEOGRAPHIC);
  xfSetHorizUnits(xCoordId, COORD_UNITS_US_FEET);

  xfSetVertDatum(xCoordId, VERT_DATUM_LOCAL);
  xfSetVertUnits(xCoordId, COORD_UNITS_US_FEET);

    // write additional information
  xfSetEllipse(xCoordId, iEllipse);
  xfSetLat(xCoordId, LATITUDE_NORTH);
  xfSetLon(xCoordId, LONGITUDE_EAST);
  xfSetMajorR(xCoordId, dMajorR);
  xfSetMinorR(xCoordId, dMinorR);

  xfCloseGroup(xCoordId);
  xCoordId = 0;

  // close the resources
  xfCloseGroup(xPropGrpId);
  xfCloseGroup(xMeshId);
  xfCloseFile(xFileId);

  return TRUE;
} // tmWriteTestMeshA
//////////////////////////////////////////////////////////////////////////////
// FUNCTION   tmWriteTestMeshB
// PURPOSE    Write a file that contains data for an mixed quad/tri linear mesh
// NOTES      A picture of the mesh is in the file (TestMeshB.gif)
//            returns TRUE on success and FALSE on failure
//////////////////////////////////////////////////////////////////////////////
int tmWriteTestMeshB(LPCSTR Filename, int Compression)
{
  int         nElements = 2, nNodes = 5, nMaxNodePerElem = 4;
  xid         xFileId = NONE, xMeshId = NONE;
  xid         xPropGrpId = NONE, xCoordId = NONE;
  double      dNodeLocsX[5], dNodeLocsY[5], dNodeLocsZ[5];
  int         iElementNodes[2][4]; // numelements, max nodes per elem
  int         iElementTypes[2];
  int         status, propint, iEllipse;
  char          *propstring;
  unsigned int  propuint;
  double        propdouble;
  float         propfloat;

  // Setup the arrays for the mesh data
    // nodes
  dNodeLocsX[0] = 0.0; dNodeLocsY[0] = 5.0; dNodeLocsZ[0] = 0.0;
  dNodeLocsX[1] = 5.0; dNodeLocsY[1] = 5.0; dNodeLocsZ[1] = 0.0;
  dNodeLocsX[2] = 0.0; dNodeLocsY[2] = 0.0; dNodeLocsZ[2] = 0.0;
  dNodeLocsX[3] = 5.0; dNodeLocsY[3] = 0.0; dNodeLocsZ[3] = 0.0;
  dNodeLocsX[4] = 7.5; dNodeLocsY[4] = 2.5; dNodeLocsZ[4] = 0.0;
  
    // nodes for each element
    // must be counter-clockwize
  iElementNodes[0][0] = 1; iElementNodes[0][1] = 3; iElementNodes[0][2] = 4;
  iElementNodes[0][3] = 2;
  iElementNodes[1][0] = 2; iElementNodes[1][1] = 4; iElementNodes[1][2] = 5;
  iElementNodes[1][3] = NONE;

  iElementTypes[0] = EL_TYPE_QUADRILATERAL_LINEAR;
  iElementTypes[1] = EL_TYPE_TRI_LINEAR;

  // create the file
  status = xfCreateFile(Filename, &xFileId, XTRUE);
  if (status < 0) {
    return FALSE;
  }

  // create the group to store the mesh
  status = xfCreateGroupForMesh(xFileId, MESH_B_GROUP_NAME, &xMeshId);
  if (status < 0) {
    return FALSE;
  }

  // node information
  status = xfSetNumberOfNodes(xMeshId, nNodes);
  if (status < 0) {
    return FALSE;
  }

  status = xfWriteXNodeLocations(xMeshId, nNodes, dNodeLocsX, Compression);
  if (status < 0) {
    return FALSE;
  }
  status = xfWriteYNodeLocations(xMeshId, nNodes, dNodeLocsY);
  if (status < 0) {
    return FALSE;
  }
  status = xfWriteZNodeLocations(xMeshId, nNodes, dNodeLocsZ);
  if (status < 0) {
    return FALSE;
  }

  // element information
  status = xfSetNumberOfElements(xMeshId, nElements);
  if (status < 0) {
    return FALSE;
  }

  // Element types
  status = xfWriteElemTypes(xMeshId, nElements, iElementTypes, Compression);
  if (status < 0) {
    return FALSE;
  }
 
  // Write the node array ids for the nodes in each element
  status = xfWriteElemNodeIds(xMeshId, nElements, nMaxNodePerElem,
                              &iElementNodes[0][0], Compression);
  if (status < 0) {
    return FALSE;
  }

  // Write the property file
  status = xfCreateMeshPropertyGroup(xMeshId, &xPropGrpId);
  if (status < 0) {
    xfCloseGroup(xFileId);
    return FALSE;
  }
  propstring = "String Property";
  propuint = 2;
  propint = -2;
  propdouble = 2.3456789012;
  propfloat = (float)2.3456;

  status = xfWritePropertyString(xPropGrpId, "String", 1, 
                                 strlen(propstring), propstring);
  status = xfWritePropertyUnsignedInt(xPropGrpId, "UInt", 1, &propuint, NONE);
  status = xfWritePropertyInt(xPropGrpId, "Int", 1, &propint, NONE);
  status = xfWritePropertyDouble(xPropGrpId, "Double", 1, &propdouble, NONE);
  status = xfWritePropertyFloat(xPropGrpId, "Float", 1, &propfloat, NONE);

  // Write Coordinate file - for MeshB, we will set the coordinate system to be
  //   Geogrpahic, with Latitude, Longitude, and standard ellipsoid settings
  //   written to the file.
  status = xfCreateCoordinateGroup(xFileId, &xCoordId);
  if (status <= 0) {
    xfCloseGroup(xPropGrpId);
	  xfCloseGroup(xMeshId);
	  xfCloseFile(xFileId);
	  return -1;
  }

    // set coordinate values
  iEllipse = 21;   // International 1924

  xfSetHorizDatum(xCoordId, HORIZ_DATUM_GEOGRAPHIC);
  xfSetHorizUnits(xCoordId, COORD_UNITS_METERS);

  xfSetVertDatum(xCoordId, VERT_DATUM_NGVD_88);
  xfSetVertUnits(xCoordId, COORD_UNITS_METERS);

    // write additional information
  xfSetEllipse(xCoordId, iEllipse);
  xfSetLat(xCoordId, LATITUDE_SOUTH);
  xfSetLon(xCoordId, LONGITUDE_WEST);

  xfCloseGroup(xCoordId);
  xCoordId = 0;

  // close the resources
  xfCloseGroup(xPropGrpId);
  xfCloseGroup(xMeshId);
  xfCloseFile(xFileId);

  return TRUE;
} // tmWriteTestMeshB
