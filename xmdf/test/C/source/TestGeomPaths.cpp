#include "stdafx.h"
#include <xmdf/Xmdf.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include "TestGeomPaths.h"

/* tmReadTestPaths */
int tmReadTestPaths(LPCSTR Filename, LPCSTR OutFilename)
{
  int         nGroups, nMaxPathLength, nDims, nPaths, nTimes, i, j;
  int         PathIndices[2];
  char       *Paths = NULL, *IndividualPath = NULL;
  double     *times, *locs, NullVal = 0;
  xid         xFileId = NONE, xGroupId = NONE;
  int         nStatus;
  FILE       *fp = NULL;

  // open the XMDF file 
  nStatus = xfOpenFile(Filename, &xFileId, TRUE);
  if (nStatus < 0) {
    printf("Unable to open XMDF file tmReadTestPaths.");
    return nStatus;
  }

  // open the Output file 
  fp = fopen(OutFilename, "w");
  if (fp == NULL) {
    printf("Unable to open output file tmReadTestPaths.");
    return nStatus;
  }

  // find the geomotric path groups
  // Get the number and paths of datasets in the file.
  nStatus = xfGetGroupPathsSizeForGeomPaths(xFileId, &nGroups,
                                                    &nMaxPathLength);
  if (nStatus >= 0) {
    Paths = (char *)malloc(nMaxPathLength*nGroups*sizeof(char));
    nStatus = xfGetGroupPathsForGeomPaths(xFileId, nGroups, 
                                         nMaxPathLength, Paths);
  }
  if (nStatus < 0) {
    xfCloseFile(xFileId);
    return -1;
  }
  // Report the number and paths to individual geom paths groups in the file.
  fprintf(fp, "Number of geometric paths in file: %d\n", nGroups);
  fprintf(fp, "Paths:\n");
  for (i = 0; i < nGroups; i++) {
    IndividualPath = &Paths[i*nMaxPathLength];  
    fprintf(fp, "  %s\n", IndividualPath);
  }
  fprintf(fp, "\n");

  // Open each geometric path group
  for (i = 0; i < nGroups; i++) {
    IndividualPath = &Paths[i*nMaxPathLength];  
    
    fprintf(fp, "Reading geom paths in group: %s\n", IndividualPath);
    nStatus = xfOpenGroup(xFileId, IndividualPath, &xGroupId);
    if (nStatus >= 0) {
      // read the dimensionality of the paths
      nStatus = xfGetPathDimensionality(xGroupId, &nDims);
      if (nStatus >= 0) {
        fprintf(fp, "Group dimensionality: %d\n", nDims);
        // read the number of paths
        nStatus = xfGetNumberOfPaths(xGroupId, &nPaths);
        if (nStatus >= 0) {
          fprintf(fp, "Number of paths in group: %d\n", nPaths);
          // read the number of timesteps
          nStatus = xfGetNumberOfTimes(xGroupId, &nTimes);
          if (nStatus >= 0) {
            fprintf(fp, "Number of timesteps in group: %d\n", nTimes);
            // allocate times array
            times = (double *)malloc(nTimes*sizeof(double));
            if (times != NULL) {
              nStatus = xfGetPathTimesArray(xGroupId, nTimes, times);
              if (nStatus >= 0) {
                    // get space for the data location values
                locs = (double *)malloc(nPaths*nDims*sizeof(double));
                if (locs != NULL) {
                  // read null value 
                  nStatus = xfGetPathNullVal(xGroupId, &NullVal);
                  if (nStatus >= 0) {
                    fprintf(fp, "Null value: %lf\n", NullVal);
                    for (i=0; i<nTimes; ++i) {
                      fprintf(fp, "Timestep: %lf\n", times[i]);
                      // read the data for this timestep
                      nStatus = xfReadPathLocationsAtTime(xGroupId, i+1,
                                1, nPaths, locs);
                      if (nStatus >= 0) {
                        // write  the data for this timestep
                        fprintf(fp, "  X        Y");
                        if (nDims == 3) {
                          fprintf(fp, "       Z");
                        }
                        fprintf(fp, "\n");
                        for (j=0; j<nPaths; ++j) {
                          if (locs[j*nDims] == NullVal) {
                            fprintf(fp, "Particle not active yet\n");
                          }
                          else {
                            fprintf(fp, "%lf %lf", locs[j*nDims],
                                    locs[j*nDims+1]);
                            if (nDims == 3) {
                              fprintf(fp, " %lf", locs[j*nDims+2]);
                            }
                            fprintf(fp, "\n");
                          }
                        } // Loop through the points
                      }
                    } // Loop through timesteps
                  }
                  free(locs);
                }
                    // get space for the data location values - 1 particle 
                    // all times
                locs = (double *)malloc(nTimes*nDims*sizeof(double));
                if (locs != NULL) {
                  for (i=0; i<nPaths; ++i) {
                    // read the values for this particle for all timesteps
                    nStatus = xfReadPathLocationsForParticle(xGroupId, i+1, 
                                   1, nTimes, locs);
                    if (nStatus >= 0) {
                        // write  the data for this path
                      fprintf(fp, "Time       X        Y");
                      if (nDims == 3) {
                        fprintf(fp, "        Z");
                      }
                      fprintf(fp, "\n");
                      for (j=0; j<nTimes; ++j) {
                        if (locs[j*nDims] != NullVal) {
                          fprintf(fp, "%lf %lf %lf", times[j], locs[j*nDims],
                                  locs[j*nDims+1]);
                          if (nDims == 3) {
                            fprintf(fp, " %lf", locs[j*nDims+2]);
                          }
                          fprintf(fp, "\n");
                        }
                      } // Loop through the times
                    }
                  } // Loop through the paths
                  free(locs);
                }
                    // get space for the data location values - 2 particle 
                    // all times
                locs = (double *)malloc(nTimes*nDims*2*sizeof(double));
                if (locs != NULL) {
                  PathIndices[0] = 1;
                  PathIndices[1] = nPaths;                
                  nStatus = xfReadPathLocationsForParticles(xGroupId, 2,
                              PathIndices, 1, nTimes, locs);
                  if (nStatus >= 0) {
                      // write  the data for these 2 paths
                    if (nDims == 3) {
                      fprintf(fp, "Timestep       X1        Y1        Z1        Xn        Yn        Zn\n");
                    }
                    else {
                      fprintf(fp, "Timestep       X1        Y1        Xn        Yn\n");
                    }
                    for (j=0; j<nTimes; ++j) {
                      if (nDims == 3) {
                        fprintf(fp, "%lf %lf %lf %lf %lf %lf %lf\n", times[j],
                             locs[j*2*nDims], locs[j*2*nDims+1],
                             locs[j*2*nDims+2], locs[j*2*nDims+3],
                             locs[j*2*nDims+4], locs[j*2*nDims+5]);
                      }
                      else {
                        fprintf(fp, "%lf %lf %lf %lf %lf\n", times[j],
                             locs[j*2*nDims], locs[j*2*nDims+1],
                             locs[j*2*nDims+2], locs[j*2*nDims+3]);
                      }
                    } // Loop through the times
                  }
                  free(locs);
                }
              }
              free(times);
            }
          }
        }
      }
      xfCloseGroup(xGroupId);
    }
    if (nStatus < 0) {
      printf("Error reading geometric paths..\n");
    }
  }
    // free the paths
  if (Paths) {
    free(Paths);
    Paths = NULL;
  }
    // close the files
  fclose(fp);
  xfCloseFile(xFileId);
  return nStatus;
} /* tmReadTestPaths */

/* tmWriteTestPaths */
int tmWriteTestPaths(LPCSTR Filename, int Compression)
{
  int         nPaths = 0;
  double      pLocs[6];
  float       pSpeeds[2], NullVal = (float)-99999.9;
  xid         xFileId = NONE, xPathGroupId = NONE, xSpeedId = NONE,
              xPropGroupId = NONE;
  int         status;

  /* create the file */
  status = xfCreateFile(Filename, &xFileId, XTRUE);
  if (status < 0) {
    return FALSE;
  }

  /* create the group to store the particle paths */
  status = xfCreateGeometricPathGroup(xFileId, "particles",
               "abcdefglasdfjoaieur", Compression,
               &xPathGroupId, -99999.9);
  if (status < 0) {
    return FALSE;
  }

  /* create the data set to store the speed */
  status = xfCreateScalarDatasetExtendable(xPathGroupId, "Vmag", "m/s",
             TS_SECONDS, NullVal, Compression, &xSpeedId);
  if (status < 0) {
    return -1;
  }

  if (xfCreatePropertyGroup(xSpeedId, &xPropGroupId) < 0) {
    xfCloseGroup(xSpeedId);
    return -1;
  }
  xfWritePropertyFloat(xPropGroupId, PROP_NULL_VALUE, 1, &NullVal, -1);
  xfCloseGroup(xPropGroupId);

  // Setup the arrays for the path group at timestep 0 
    // particle location at the first timestep
  nPaths = 1;
  pLocs[0] = 1.0; pLocs[1] = 2.0; pLocs[2] = 3.0;
  
    // store the particles for the first timestep
  status = xfWriteParticleTimestep(xPathGroupId, 3, 0.0, nPaths, pLocs);
  if (status < 0) {
    return FALSE;
  }
    // set up and store the speed at timestep 0
  pSpeeds[0] = (float)(1.1);
  status = xfWriteScalarTimestep(xSpeedId, 0.0, 1, pSpeeds); 
  if (status < 0) {
    return FALSE;
  }

  // Setup the arrays for the path group at timestep 1 
    // particle location at the first timestep
  pLocs[0] = 4.0; pLocs[1] = 5.0; pLocs[2] = 6.0;
  
    // store the particles for the first timestep
  status = xfWriteParticleTimestep(xPathGroupId, 3, 1.0, nPaths, pLocs);
  if (status < 0) {
    return FALSE;
  }
  
    // set up and store the speed at timestep 1
  pSpeeds[0] = (float)(1.2);
  status = xfWriteScalarTimestep(xSpeedId, 1.0, 1, pSpeeds); 
  if (status < 0) {
    return FALSE;
  }

  // Setup the arrays for the path group at timestep 2-add a particle
    // particle location at the first timestep
  nPaths = 2;
  pLocs[0] = 7.0;  pLocs[1] = 8.0;  pLocs[2] = 9.0;
  pLocs[3] = -1.0; pLocs[4] = -2.0; pLocs[5] = -3.0;
  
    // store the particles for the timestep 2
  status = xfWriteParticleTimestep(xPathGroupId, 3, 2.0, nPaths, pLocs);
  if (status < 0) {
    return FALSE;
  }

    // extend the data set for speed
  status = xfExtendScalarDataset(xSpeedId, 2);
  if (status < 0) {
    return FALSE;
  }

    // set up and store the speed at timestep 2
  pSpeeds[0] = (float)1.3;
  pSpeeds[1] = (float)2.1;
  status = xfWriteScalarTimestep(xSpeedId, 2.0, 2, pSpeeds); 
  if (status < 0) {
    return FALSE;
  }

  // Setup the arrays for the path group at timestep 3-inactive particle(static)
    // particle location at the first timestep
  pLocs[0] = 7.0;  pLocs[1] = 8.0;  pLocs[2] = 9.0;
  pLocs[3] = -4.0; pLocs[4] = -5.0; pLocs[5] = -6.0;
  
    // store the particles for timestep 3
  status = xfWriteParticleTimestep(xPathGroupId, 3, 3.0, nPaths, pLocs);
  if (status < 0) {
    return FALSE;
  }

    // set up and store the speed at timestep 3
  pSpeeds[0] = NullVal;
  pSpeeds[1] = (float)2.2;
  status = xfWriteScalarTimestep(xSpeedId, 3.0, 2, pSpeeds); 
  if (status < 0) {
    return FALSE;
  }

  // close the resources
  xfCloseGroup(xSpeedId);
  xfCloseGroup(xPathGroupId);
  xfCloseFile(xFileId);

  return TRUE;
}
