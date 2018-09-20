MODULE TestDefs

INTEGER, PARAMETER :: NUMTIMES1   = 5
INTEGER, PARAMETER :: NUMVALUES1  = 5
INTEGER, PARAMETER :: NUMACTIVE1  = 3
INTEGER, PARAMETER :: NUMTIMESADD = 1

CHARACTER(LEN=*), PARAMETER :: XMDF_VERSION_OUT_F = 'XMDF_Version_f.txt'
CHARACTER(LEN=*), PARAMETER :: MESH_A_FILE_F = 'mesh_a_file_f.h5'
CHARACTER(LEN=*), PARAMETER :: MESH_B_FILE_F = 'mesh_b_file_f.h5'
CHARACTER(LEN=*), PARAMETER :: MESH_A_OUT_F  = 'mesh_a_file_f.txt'
CHARACTER(LEN=*), PARAMETER :: MESH_B_OUT_F  = 'mesh_b_file_f.txt'

CHARACTER(LEN=*), PARAMETER :: GRID_CART2D_A_FILE_F = 'grid_cart2d_a_file_f.h5'
CHARACTER(LEN=*), PARAMETER :: GRID_CURV2D_A_FILE_F = 'grid_curv2d_a_file_f.h5'
CHARACTER(LEN=*), PARAMETER :: GRID_CART3D_A_FILE_F = 'grid_cart3d_a_file_f.h5'

CHARACTER(LEN=*), PARAMETER :: GRID_CART2D_A_OUT_F = 'grid_cart2d_a_out_f.txt'
CHARACTER(LEN=*), PARAMETER :: GRID_CURV2D_A_OUT_F = 'grid_curv2d_a_out_f.txt'
CHARACTER(LEN=*), PARAMETER :: GRID_CART3D_A_OUT_F = 'grid_cart3d_a_out_f.txt'

CHARACTER(LEN=*), PARAMETER :: MULTIDATASET_FILE_F = 'MultiDataSet_f.h5'
CHARACTER(LEN=*), PARAMETER :: MULTIDATASET_TEXT_F = 'MultiDataSet_f.txt'

CHARACTER(LEN=*), PARAMETER :: SCALAR_A_FILE_F   = 'ScalarA_f.h5'
CHARACTER(LEN=*), PARAMETER :: SCALAR_A_TEXT_F   = 'ScalarA_f.txt'
CHARACTER(LEN=*), PARAMETER :: SCALAR_A_PIECES_FILE_F   = 'ScalarA_Pieces_f.h5'
CHARACTER(LEN=*), PARAMETER :: SCALAR_A_EDITED_FILE_F   = 'ScalarA_edited_f.h5'
CHARACTER(LEN=*), PARAMETER :: SCALAR_A_EDITED_TEXT_F   = 'ScalarA_edited_f.txt'
CHARACTER(LEN=*), PARAMETER :: SCALAR_A_PIECES_ALT_FILE_F   = 'ScalarA_Pieces_alt_f.h5'
CHARACTER(LEN=*), PARAMETER :: SCALAR_B_FILE_F   = 'ScalarB_f.h5'
CHARACTER(LEN=*), PARAMETER :: SCALAR_B_TEXT_F   = 'ScalarB_f.txt'
CHARACTER(LEN=*), PARAMETER :: VECTOR2D_A_FILE_F = 'Vector2D_A_f.h5'
CHARACTER(LEN=*), PARAMETER :: VECTOR2D_A_TEXT_F = 'Vector2D_A_f.txt'
CHARACTER(LEN=*), PARAMETER :: VECTOR2D_A_PIECES_FILE_F = 'Vector2D_A_Pieces_f.h5'
CHARACTER(LEN=*), PARAMETER :: VECTOR2D_B_FILE_F = 'Vector2D_B_f.h5'
CHARACTER(LEN=*), PARAMETER :: VECTOR2D_B_TEXT_F = 'Vector2D_B_f.txt'

CHARACTER(LEN=*), PARAMETER :: MESH_A_FILE_C = 'mesh_a_file_c.h5'
CHARACTER(LEN=*), PARAMETER :: MESH_B_FILE_C = 'mesh_b_file_c.h5'
CHARACTER(LEN=*), PARAMETER :: MESH_A_OUT_CF  = 'mesh_a_file_cf.txt'
CHARACTER(LEN=*), PARAMETER :: MESH_B_OUT_CF  = 'mesh_b_file_cf.txt'

CHARACTER(LEN=*), PARAMETER :: GRID_CART2D_A_FILE_C = 'grid_cart2d_a_file_c.h5'
CHARACTER(LEN=*), PARAMETER :: GRID_CURV2D_A_FILE_C = 'grid_curv2d_a_file_c.h5'
CHARACTER(LEN=*), PARAMETER :: GRID_CART3D_A_FILE_C = 'grid_cart3d_a_file_c.h5'

CHARACTER(LEN=*), PARAMETER :: GRID_CART2D_A_OUT_CF = 'grid_cart2d_a_out_cf.txt'
CHARACTER(LEN=*), PARAMETER :: GRID_CURV2D_A_OUT_CF = 'grid_curv2d_a_out_cf.txt'
CHARACTER(LEN=*), PARAMETER :: GRID_CART3D_A_OUT_CF = 'grid_cart3d_a_out_cf.txt'

CHARACTER(LEN=*), PARAMETER :: SCALAR_A_FILE_C   = 'ScalarA_c.h5'
CHARACTER(LEN=*), PARAMETER :: SCALAR_A_TEXT_CF   = 'ScalarA_cf.txt'
CHARACTER(LEN=*), PARAMETER :: SCALAR_B_FILE_C   = 'ScalarB_c.h5'
CHARACTER(LEN=*), PARAMETER :: SCALAR_B_TEXT_CF   = 'ScalarB_cf.txt'
CHARACTER(LEN=*), PARAMETER :: VECTOR2D_A_FILE_C = 'Vector2D_A_c.h5'
CHARACTER(LEN=*), PARAMETER :: VECTOR2D_A_TEXT_CF = 'Vector2D_A_cf.txt'
CHARACTER(LEN=*), PARAMETER :: VECTOR2D_B_FILE_C = 'Vector2D_B_c.h5'
CHARACTER(LEN=*), PARAMETER :: VECTOR2D_B_TEXT_CF = 'Vector2D_B_cf.txt'

CHARACTER(LEN=*), PARAMETER :: CALENDAR_OUT_F = 'Calendar_f.txt'

CHARACTER(LEN=*), PARAMETER :: GEOMPATH_A_FILE_F = 'Geompath_a_file_f.h5'
CHARACTER(LEN=*), PARAMETER :: GEOMPATH_A_FILE_F_OUT = 'Geompath_a_file_f_out.txt'

CHARACTER(LEN=*), PARAMETER :: TT_MULTIDATASET_FILE_F = 'TT_MultiDataSet_f.h5'
CHARACTER(LEN=*), PARAMETER :: TT_SCALAR_A_FILE_F   = 'TT_ScalarA_f.h5'
CHARACTER(LEN=*), PARAMETER :: TT_SCALAR_A_TEXT_F   = 'TT_ScalarA_f.txt'
CHARACTER(LEN=*), PARAMETER :: TT_VECTOR2D_A_FILE_F = 'TT_Vector2D_A_f.h5'

  ! Overwrite options in the function xfSetupToWriteDatasets
!INTEGER,PARAMETER :: XF_OVERWRITE_CLEAR_FILE          = 1
!INTEGER,PARAMETER :: XF_OVERWRITE_CLEAR_DATASET_GROUP = 2
!INTEGER,PARAMETER :: XF_OVERWRITE_NONE                = 3

END MODULE TestDefs

MODULE TestsModule

USE XMDF
USE XMDFDEFS
USE TestTimestep
USE TestDatasets
USE TestMesh
USE TestGrid
USE TestDefs
USE TestGeomPaths

CONTAINS

!**************************
!-----------------------------------------------------------------------------
! SUBROUTINE  TXI_TEST_TIMESTEPS
! PURPOSE     test to see if the code can read timestepfiles 
! NOTES
!------------------------------------------------------------------------------
SUBROUTINE TXI_TEST_TIMESTEPS(error)
INTEGER, INTENT(OUT) :: error
INTEGER    status, compression
INTEGER  MultiFileId, MultiGroupId
INTEGER         NumOpen

CHARACTER(LEN=37) SdoGuid

SdoGuid = '73289C80-6235-4fdc-9649-49E4F5AEB676'

status = 1
compression = NONE
! 'Path' should be able to be blank, but now it creates errors if it's blank
 call XF_SETUP_TO_WRITE_DATASETS(TT_MULTIDATASET_FILE_F, 'Multidatasets','', &
           SdoGuid, XF_OVERWRITE_CLEAR_FILE, MultiFileId, MultiGroupId, error)

  ! Write coordinates to multidatasets
call TT_WRITE_COORDS_TO_MULTI(MultiFileId, error)

  ! Write scalar A and Vector A to multidatasets.
call TT_WRITE_SCALAR_A_TO_MULTI(MultiGroupId, error)

call TT_WRITE_VECTOR2D_A_TO_MULTI(MultiFileId, MultiGroupId, error)

call XF_CLOSE_GROUP(MultiGroupId, status)
call XF_CLOSE_FILE(MultiFileId, status)

WRITE(*,*) 'Done writing multiple datasets...'

  ! scalar datasets
call TT_WRITE_SCALAR_A(TT_SCALAR_A_FILE_F, compression, status)
if (status < 0) then
  error = status
  return
endif
  
WRITE(*,*) 'Done writing scalar datasets...'

  ! vector datasets
call TT_WRITE_VECTOR2D_A(TT_VECTOR2D_A_FILE_F, compression, status)
if (status < 0) then
  WRITE(*,*) 'Error writing dataset vector2D_A.'
  error = status
  return
endif

WRITE(*,*) 'Done writing vector datasets...'

WRITE(*,*) 'Write edited scalar datasets...'
call TD_EDIT_SCALAR_A_VALUES(SCALAR_A_EDITED_FILE_F, compression, status);
if (status < 0) then
  error = status
  return
endif

WRITE(*,*) 'Done writing datasets...'

  ! Read the files back in
call TXI_READ_X_FORMAT_FILE(TT_SCALAR_A_FILE_F, SCALAR_A_TEXT_F, status)
if (status < 0) then
  WRITE(*,*) 'Error reading SCALAR A File (see TXI_READ_X_FORMAT_FILE)'
  error = status
  return
endif

call TXI_READ_X_FORMAT_FILE(SCALAR_A_EDITED_FILE_F, SCALAR_A_EDITED_TEXT_F, status)
if (status < 0) then
  WRITE(*,*) 'Error reading SCALAR A Edited File (see TXI_READ_X_FORMAT_FILE)'
  error = status
  return
endif

call  TXI_READ_X_FORMAT_FILE(TT_VECTOR2D_A_FILE_F, VECTOR2D_A_TEXT_F, status)
if (status < 0) then
  WRITE(*,*) 'Error reading VECTOR A Format File'
  error = status
  return
endif

call TXI_READ_X_FORMAT_FILE(TT_MULTIDATASET_FILE_F, MULTIDATASET_TEXT_F, status)
if (status < 0) then
  WRITE(*,*) 'Error reading Multidataset File (see TXI_READ_X_FORMAT_FILE)'
  error = status
  return
endif

WRITE(*,*) 'Done reading datasets...'

call XF_GET_NUM_OPEN_IDENTIFIERS(H5F_OBJ_ALL_F, NumOpen, error)

call XFI_CLOSE_OPEN_IDENTIFIERS(H5F_OBJ_ALL_F, error)

call XF_SETUP_TO_WRITE_DATASETS(TT_MULTIDATASET_FILE_F, 'Multidatasets','', &
     SdoGuid, XF_OVERWRITE_CLEAR_DATASET_GRP, MultiFileId, MultiGroupId, &
                                                                  error)

call TT_WRITE_SCALAR_A_TO_MULTI(MultiGroupId, error)

call XF_SETUP_TO_WRITE_DATASETS(TT_MULTIDATASET_FILE_F, 'Multidatasets','', &
     SdoGuid, XF_OVERWRITE_NONE, MultiFileId, MultiGroupId, error)

call TT_WRITE_VECTOR2D_A_TO_MULTI(MultiFileId, MultiGroupId, error)

  ! Test reading information at index for multiple timesteps
call TT_READ_SCALAR_A_INDEX(TT_SCALAR_A_FILE_F, 4, status)
if (status < 0) then
  error = status
  return
endif

WRITE(*,*) 'Done reading scalar data at index.'
  
call TT_READ_VECTOR2D_A_INDEX(TT_VECTOR2D_A_FILE_F, 6, status)
if (status < 0) then
  error = status
  return
endif

WRITE(*,*) 'Done reading vector data at index.'

call TT_READ_ACTIVITY_SCALAR_A_INDEX(TT_SCALAR_A_FILE_F, 6, status)
if (status < 0) then
  error = status
  return
endif

error = status
return

END SUBROUTINE


!**************************
!-----------------------------------------------------------------------------
! SUBROUTINE  TXI_TEST_DATASETS
! PURPOSE     test to see if the code can read datasetfiles 
! NOTES
!------------------------------------------------------------------------------
SUBROUTINE TXI_TEST_DATASETS(error)
INTEGER, INTENT(OUT) :: error
INTEGER    status, compression
INTEGER  MultiFileId, MultiGroupId
INTEGER         NumOpen
INTEGER, PARAMETER :: nIndices = 3
INTEGER, DIMENSION(nIndices) :: indices

CHARACTER(LEN=37) SdoGuid

SdoGuid = '73289C80-6235-4fdc-9649-49E4F5AEB676'

status = 1
compression = NONE
! 'Path' should be able to be blank, but now it creates errors if it's blank
call XF_SETUP_TO_WRITE_DATASETS(MULTIDATASET_FILE_F, 'Multidatasets','', &
           SdoGuid, XF_OVERWRITE_CLEAR_FILE, MultiFileId, MultiGroupId, error)

  ! Write coordinates to multidatasets
call TD_WRITE_COORDS_TO_MULTI(MultiFileId, error)

  ! Write scalar A and Vector A to multidatasets.
call TD_WRITE_SCALAR_A_TO_MULTI(MultiGroupId, error)

call TD_WRITE_VECTOR2D_A_TO_MULTI(MultiFileId, MultiGroupId, error)

call XF_CLOSE_GROUP(MultiGroupId, status)
call XF_CLOSE_FILE(MultiFileId, status)

WRITE(*,*) 'Done writing multiple datasets...'

  ! scalar datasets
call TD_WRITE_SCALAR_A(SCALAR_A_FILE_F, compression, status)
if (status < 0) then
  error = status
  return
endif

call TD_WRITE_SCALAR_A_PIECES(SCALAR_A_PIECES_FILE_F, compression, status)
if (status < 0) then
  error = status
  return
endif

call TD_WRITE_SCALAR_A_PIECES_ALT_MIN_MAX(SCALAR_A_PIECES_ALT_FILE_F, &
                                          compression, status)
if (status < 0) then
  error = status
  return
endif
  
WRITE(*,*) 'Done writing scalar datasets...'

  ! vector datasets
call TD_WRITE_VECTOR2D_A(VECTOR2D_A_FILE_F, compression, status)
if (status < 0) then
  WRITE(*,*) 'Error writing dataset vector2D_A.'
  error = status
  return
endif

call TD_WRITE_VECTOR2D_A_PIECES(VECTOR2D_A_PIECES_FILE_F, compression, status)
if (status < 0) then
  WRITE(*,*) 'Error writing dataset vector2D_A.'
  error = status
  return
endif

WRITE(*,*) 'Done writing vector datasets...'

WRITE(*,*) 'Done writing datasets...'

  ! Read the files back in
call TXI_READ_X_FORMAT_FILE(SCALAR_A_FILE_F, SCALAR_A_TEXT_F, status)
if (status < 0) then
  WRITE(*,*) 'Error reading SCALAR A File (see TXI_READ_X_FORMAT_FILE)'
  error = status
  return
endif

call  TXI_READ_X_FORMAT_FILE(VECTOR2D_A_FILE_F, VECTOR2D_A_TEXT_F, status)
if (status < 0) then
  WRITE(*,*) 'Error reading VECTOR A Format File'
  error = status
  return
endif

call TXI_READ_X_FORMAT_FILE(MULTIDATASET_FILE_F, MULTIDATASET_TEXT_F, status)
if (status < 0) then
  WRITE(*,*) 'Error reading Multidataset File (see TXI_READ_X_FORMAT_FILE)'
  error = status
  return
endif

WRITE(*,*) 'Done reading datasets...'

call XF_GET_NUM_OPEN_IDENTIFIERS(H5F_OBJ_ALL_F, NumOpen, error)

call XFI_CLOSE_OPEN_IDENTIFIERS(H5F_OBJ_ALL_F, error)

call XF_SETUP_TO_WRITE_DATASETS(MULTIDATASET_FILE_F, 'Multidatasets','', &
     SdoGuid, XF_OVERWRITE_CLEAR_DATASET_GRP, MultiFileId, MultiGroupId, &
                                                                  error)

call TD_WRITE_SCALAR_A_TO_MULTI(MultiGroupId, error)

call XF_SETUP_TO_WRITE_DATASETS(MULTIDATASET_FILE_F, 'Multidatasets','', &
     SdoGuid, XF_OVERWRITE_NONE, MultiFileId, MultiGroupId, error)

call TD_WRITE_VECTOR2D_A_TO_MULTI(MultiFileId, MultiGroupId, error)

  ! Test reading information at index for multiple timesteps
call TD_READ_SCALAR_A_INDEX(SCALAR_A_FILE_F, 4, status)
if (status < 0) then
  error = status
  return
endif

WRITE(*,*) 'Done reading scalar data at index.'
  
call TD_READ_VECTOR2D_A_INDEX(VECTOR2D_A_FILE_F, 6, status)
if (status < 0) then
  error = status
  return
endif

WRITE(*,*) 'Done reading vector data at index.'

call TD_READ_ACTIVITY_SCALAR_A_INDEX(SCALAR_A_FILE_F, 6, status)
if (status < 0) then
  error = status
  return
endif

    ! Test reading information at multiple indices
  indices(1) = 2;
  indices(2) = 3;
  indices(3) = 5;
  CALL TD_READ_SCALAR_A_INDICES(SCALAR_A_FILE_F, nIndices, indices, error)

error = status
return

END SUBROUTINE

!------------------------------------------------------------------------------
!  FUNCTION  TXI_TEST_OVERWRITE_DSETS
!  PURPOSE   Check to see if already-written datasets can be overwritten
!  NOTES
!------------------------------------------------------------------------------
SUBROUTINE TXI_TEST_OVERWRITE_DSETS(error)
INTEGER, INTENT(OUT) :: error
INTEGER    status, compression

  status = 1
  compression = NONE

    ! scalar datasets
  call TD_WRITE_SCALAR_B(SCALAR_B_FILE_F, compression, .FALSE., status)
  if (status < 0) then
    error = status
    return
  endif
    !overwrite scalar datasets
  call TD_WRITE_SCALAR_B(SCALAR_B_FILE_F, compression, .TRUE., status)
  if (status < 0) then
    error = status
    return
  endif
  
    ! vector datasets
  call TD_WRITE_VECTOR2D_B(VECTOR2D_B_FILE_F, compression, .FALSE., status)
  if (status < 0) then
    WRITE(*,*) 'Error writing dataset vector2D_B.'
    error = status
    return
  endif
    ! overwrite vector datasets
  call TD_WRITE_VECTOR2D_B(VECTOR2D_B_FILE_F, compression, .TRUE., status)
  if (status < 0) then
    WRITE(*,*) 'Error writing dataset vector2D_B.'
    error = status
    return
  endif

    ! Read the files back in
  call TXI_READ_X_FORMAT_FILE(SCALAR_B_FILE_F, SCALAR_B_TEXT_F, status)
  if (status < 0) then
    WRITE(*,*) 'Error reading SCALAR B File'
    error = status
    return
  endif

  call  TXI_READ_X_FORMAT_FILE(VECTOR2D_B_FILE_F, VECTOR2D_B_TEXT_F, status)
  if (status < 0) then
    WRITE(*,*) 'Error reading VECTOR B Format File'
    error = status
    return
  endif

  error = status
  return

END SUBROUTINE

!------------------------------------------------------------------------------
! FUNCTION TXI_TEST_GRIDS
! PURPOSE  Test to see if we can read and write grids
! NOTES
!------------------------------------------------------------------------------
SUBROUTINE TXI_TEST_GRIDS (error)
INTEGER, INTENT(OUT) :: error
INTEGER    compression

compression = NONE

WRITE(*,*) ''
WRITE(*,*) ''
WRITE(*,*) 'Writing grid data.'
WRITE(*,*) ''
 
call TG_WRITE_TEST_GRID_CART_2D(GRID_CART2D_A_FILE_F, error)
if (error < 0) then
  WRITE(*,*) 'Error writing grid Cartesian 2D A'
endif
WRITE(*,*) 'Finished writing grid Cartesian 2D A'

call TG_WRITE_TEST_GRID_CURV_2D(GRID_CURV2D_A_FILE_F, compression, error)
if (error < 0) then
  WRITE(*,*) 'Error writing grid Curvilinear 2D A'
endif
WRITE(*,*) 'Finished writing grid Curvilinear 2D A'

call TG_WRITE_TEST_GRID_CART_3D(GRID_CART3D_A_FILE_F, compression, error)
if (error < 0) then
  WRITE(*,*) 'Error writing grid Cartesian 3D A'
endif
WRITE(*,*) 'Finished writing grid Cartesian 3D A'
  ! read the files back in
call TXI_READ_X_FORMAT_FILE(GRID_CART2D_A_FILE_F, GRID_CART2D_A_OUT_F, error)
if (error < 0) then
  WRITE(*,*) 'Error reading grid Cartesian 2D A'
endif
WRITE(*,*) 'Finished reading grid Cartesian 2D A'

call TXI_READ_X_FORMAT_FILE(GRID_CURV2D_A_FILE_F, GRID_CURV2D_A_OUT_F, error)
if (error < 0) then
  WRITE(*,*) 'Error reading grid Curvilinear 2D A'
endif
WRITE(*,*) 'Finished reading grid Curvilinear 2D A'

call TXI_READ_X_FORMAT_FILE(GRID_CART3D_A_FILE_F, GRID_CART3D_A_OUT_F, error)
if (error < 0) then
  WRITE(*,*) 'Error reading grid Cartesian 3D A'
endif
WRITE(*,*) 'Finished reading grid Cartesian 3D A'
 
END SUBROUTINE

!**************************
! ---------------------------------------------------------------------------
! FUNCTION  TXI_TEST_MESHS
! PURPOSE   test to see if we can read and write meshes
! NOTES     
! ---------------------------------------------------------------------------
SUBROUTINE TXI_TEST_MESHS (error)
INTEGER, INTENT(OUT) :: error
INTEGER    status
INTEGER    compression

status = 1
compression = NONE

call TM_WRITE_TEST_MESH_A(MESH_A_FILE_F, compression, status)
if (status < 0) then
  WRITE(*,*) 'Error writing TestMeshA'
  error = status
  return
endif

call TM_WRITE_TEST_MESH_B(MESH_B_FILE_F, compression, status)
if (status < 0) then
  WRITE(*,*) 'Error writing TestMeshB'
  error = status
  return
endif

WRITE(*,*) 'Finished writing meshes.'

  ! read the files back in
call TXI_READ_X_FORMAT_FILE(MESH_A_FILE_F, MESH_A_OUT_F, status)
if (status < 0) then
  WRITE(*,*) 'Error reading TestMeshA'
  error = status
  return
endif

  ! read the files back in
call TXI_READ_X_FORMAT_FILE(MESH_B_FILE_F, MESH_B_OUT_F, status)
if (status < 0) then
  WRITE(*,*) 'Error reading TestMeshB'
  error = status
  return
endif

WRITE(*,*) 'Finished reading meshes.'

error = status
return

END SUBROUTINE

!**************************

!---------------------------------------------------------------------------
! FUNCTION  txiTestC
! PURPOSE   test to see if fortran code can read file written with C.
! NOTES     
!---------------------------------------------------------------------------
SUBROUTINE TXI_TEST_C (error)
INTEGER, INTENT(OUT) :: error
INTEGER            nStatus
INTEGER     xFileId

error = 1

  !Check to see if files written with C exist
  ! Turn off error handling
!call H5Eset_auto_f(0, error)
  ! Try opening a file written with C to see if one exists.
call XF_OPEN_FILE(SCALAR_A_FILE_C, .TRUE., xFileId, nStatus)
  ! If the file written with C doesn't exist, return.
if (nStatus < 0) then
  call XF_CLOSE_FILE(xFileId, error)
    ! Restore previous error handler
  !call H5Eset_Auto_f(1, error)
  error = -1
  return
  ! If the file written with C does exist, assume all C files exist.
else
  call XF_CLOSE_FILE(xFileId, error)
    ! Restore previous error handler
  !call H5Eset_Auto_f(1, error)
endif

  ! Read the files back in
call TXI_READ_X_FORMAT_FILE(SCALAR_A_FILE_C, SCALAR_A_TEXT_CF, error)
if (error < 0) then
  return
endif
call TXI_READ_X_FORMAT_FILE(SCALAR_B_FILE_C, SCALAR_B_TEXT_CF, error)
if (error < 0) then
  return
endif

call TXI_READ_X_FORMAT_FILE(VECTOR2D_A_FILE_C, VECTOR2D_A_TEXT_CF, error)
if (error < 0) then
  return
endif
call TXI_READ_X_FORMAT_FILE(VECTOR2D_B_FILE_C, VECTOR2D_B_TEXT_CF, error)
if (error < 0) then
  return
endif
  
WRITE(*,*) 'Done reading C datasets...'

call TXI_READ_X_FORMAT_FILE(GRID_CART2D_A_FILE_C, GRID_CART2D_A_OUT_CF, error)
if (error < 0) then
  WRITE(*,*) 'Error reading C grid Cartesian 2D A'
endif
WRITE(*,*) 'Finished reading C grid Cartesian 2D A'

call TXI_READ_X_FORMAT_FILE(GRID_CURV2D_A_FILE_C, GRID_CURV2D_A_OUT_CF, error)
if (error < 0) then
  WRITE(*,*) 'Error reading C grid Curvilinear 2D A'
endif
WRITE(*,*) 'Finished reading C grid Curvilinear 2D A'

call TXI_READ_X_FORMAT_FILE(GRID_CART3D_A_FILE_C, GRID_CART3D_A_OUT_CF, error)
if (error < 0) then
  WRITE(*,*) 'Error reading C grid Cartesian 3D A'
endif
WRITE(*,*) 'Finished reading C grid Cartesian 3D A'

  ! read the files back in
call TXI_READ_X_FORMAT_FILE(MESH_A_FILE_C, MESH_A_OUT_CF, error)
if (error < 0) then
  WRITE(*,*) 'Error reading C TestMeshA'
  return
endif

  ! read the files back in
call TXI_READ_X_FORMAT_FILE(MESH_B_FILE_C, MESH_B_OUT_CF, error)
if (error < 0) then
  WRITE(*,*) 'Error reading C TestMeshB'
  return
endif

WRITE(*,*) 'Finished reading C meshes.'
  
return

END SUBROUTINE

!**************************
! --------------------------------------------------------------------------
! FUNCTION txiReadXFormatFile
! PURPOSE  Read a file using XMDF and write information about the data
!          contained in the file to a output file
! --------------------------------------------------------------------------
SUBROUTINE TXI_READ_X_FORMAT_FILE (a_XmdfFile, a_OutFile, error)
CHARACTER(LEN=*), INTENT(IN) :: a_XmdfFile
CHARACTER(LEN=*), INTENT(IN) :: a_OutFile
INTEGER, INTENT(OUT)  :: error
CHARACTER(LEN=BIG_STRING_SIZE) :: IndividualPath
CHARACTER,ALLOCATABLE :: Paths(:)
INTEGER     xFileId, xGroupId
INTEGER            nMeshGroups, nMaxPathLength, nGridGroups
INTEGER            FileUnit, StartLoc, nStatus, i, j
REAL               Version

xFileId  = NONE
xGroupId = NONE

  ! Open the XMDF file
call XF_OPEN_FILE(a_XmdfFile, .TRUE., xFileId, nStatus)
if (nStatus < 0) then
  call XF_CLOSE_FILE(xFileId, error)
  error = -1
  return
endif

  ! open the status file
FileUnit = 53
OPEN(UNIT=FileUnit, FILE=a_OutFile, STATUS='REPLACE', ACTION='WRITE', &
     IOSTAT = error)
if (FileUnit == 0) then
  call XF_CLOSE_FILE(xFileId, error)
  error = -1
  return
endif

WRITE(FileUnit,*) 'File ', a_XmdfFile, ' opened.'

  ! write the version number to the file
call XF_GET_LIBRARY_VERSION_FILE(xFileId, Version, error)
WRITE(FileUnit,*) 'XMDF Version: ', Version
WRITE(FileUnit,*) ''

  ! Read Coordinate System Informatioin to the .txt file if contained in
  ! file, if not skip
call TXI_TEST_COORD_SYSTEM(xFileId, FileUnit, nStatus)
WRITE(FileUnit,*) ''

  ! read all datasets not beneath a mesh, grid, or cross-sections
call TD_READ_DATASETS(xFileId,FileUnit, nStatus)
if (nStatus < 0) then
  call XF_CLOSE_FILE(xFileId, error)
  error = -1
  return
endif

  ! Get the number and paths of datasets in the file.
call XF_GRP_PTHS_SZ_FOR_MESHES(xFileId, nMeshGroups, &
                                       nMaxPathLength, error)
if (error >= 0 .AND. nMeshGroups > 0) then
  allocate (Paths(nMaxPathLength*nMeshGroups))
  call XF_GET_GROUP_PATHS_FOR_MESHES(xFileId, nMeshGroups, nMaxPathLength, &
                                     Paths, error)
endif

if (error < 0) then
  call XF_CLOSE_FILE(xFileId, error)
  error = -1
  return
endif

  ! Report the number and paths to individual meshes in the file.
WRITE(FileUnit,*) 'Number of meshes in file: ', nMeshGroups
WRITE(FileUnit,*) 'Paths:'
do i=1, nMeshGroups
  do j=1, nMaxPathLength-1
    IndividualPath(j:j) = Paths((i-1)*nMaxPathLength+j)
  enddo
  WRITE(FileUnit,*) IndividualPath(1:nMaxPathLength-1)
enddo

WRITE(FileUnit,*) ''

  ! Open each mesh group
!if (nMeshGroups > 0) allocate(IndividualPath(nMaxPathLength + 1))

do i=1, nMeshGroups
  ! copy the portion of the array where a single path is stored
  StartLoc = (i-1)*nMaxPathLength + 1
  IndividualPath = ''
  do j = 1, nMaxPathLength - 1
    IndividualPath(j:j) = Paths(StartLoc+j-1)
  enddo
     
  WRITE(FileUnit,*) 'Reading mesh in group: ', &
                     IndividualPath(1:nMaxPathLength-1)
  call XF_OPEN_GROUP(xFileId, IndividualPath(1:LEN_TRIM(IndividualPath)), &
            xGroupId, nStatus)
  if (nStatus >= 0) then
    call TM_READ_MESH(xGroupId, FileUnit, nStatus)
  endif
  if (nStatus < 0) then
    WRITE(*,*) 'Error reading mesh..'
  endif
enddo

if (allocated(Paths)) deallocate(Paths)
!if (allocated(IndividualPath)) deallocate(IndividualPath)

  ! Grid stuff
call XF_GRP_PTHS_SZ_FOR_GRIDS(xFileId, nGridGroups, &
                                       nMaxPathLength, nStatus)
if (nStatus >= 0 .AND. nGridGroups > 0) then
  allocate (Paths(nMaxPathLength*nGridGroups))
  call XF_GET_GROUP_PATHS_FOR_GRIDS(xFileId, nGridGroups, &
                                    nMaxPathLength, Paths, nStatus)
endif
if (nStatus < 0) then
  call XF_CLOSE_FILE(xFileId, error)
  error = -1
  return
endif

  ! Report the number and paths to individual meshes in the file.
WRITE(FileUnit,*) 'Number of grids in file: ', nGridGroups
WRITE(FileUnit,*) 'Paths:'
do i=1, nGridGroups
  do j=1, nMaxPathLength-1
    IndividualPath(j:j) = Paths((i-1)*nMaxPathLength+j)
  enddo
  WRITE(FileUnit,*) IndividualPath(1:LEN_TRIM(IndividualPath))
enddo

WRITE(FileUnit,*) ''

!if (nGridGroups > 0) allocate(IndividualPath(nMaxPathLength + 1))

  ! Open each grid group
do i=1, nGridGroups
  do j = 1, nMaxPathLength - 1
    IndividualPath(j:j) = Paths((i-1)*nMaxPathLength+j)
  enddo
  WRITE(FileUnit,*) 'Reading grid in group: ', &
                     IndividualPath(1:LEN_TRIM(IndividualPath))
  call XF_OPEN_GROUP(xFileId, IndividualPath(1:LEN_TRIM(IndividualPath)), &
                     xGroupId, nStatus)
  if (nStatus >= 0) then
    call TG_READ_GRID(xGroupId, FileUnit, nStatus)
  endif
  if (nStatus < 0) then
     WRITE(FileUnit,*) 'Error reading grid..'
  endif
enddo

if (allocated(Paths)) deallocate(Paths)
!if (allocated(IndividualPath)) deallocate(IndividualPath)
  
  ! TODO do grid, and cross-section stuff.
   
  ! close the files
call XF_CLOSE_FILE(xFileId, error)
CLOSE(FileUnit)
  
return

END SUBROUTINE

!-----------------------------------------------------------------------------
! SUBROUTINE TXI_TestCalendar
! PURPOSE    Check the Calculations of Julian date from calendar date or  
!            vice-versa.
! NOTES      era is #defined (use #defines): ERA_IS_BCE (BC), ERA_IS_CE (AD)
!-----------------------------------------------------------------------------
SUBROUTINE TXI_TEST_CALENDAR (error)
  INTEGER, INTENT(OUT) :: error
  INTEGER  era1, yr1, mo1, day1, hr1, min1, sec1
  INTEGER  era2, yr2, mo2, day2, hr2, min2, sec2
  INTEGER  era3, yr3, mo3, day3, hr3, min3, sec3, FileUnit
  INTEGER  era4, yr4, mo4, day4, hr4, min4, sec4, calendarworks
  DOUBLE PRECISION  julian1, julian2, julian3, julian4

  calendarworks = 0
  FileUnit = 53
  OPEN(UNIT=FileUnit, FILE=CALENDAR_OUT_F, STATUS='REPLACE', ACTION='WRITE', &
     IOSTAT = error)

  WRITE(FileUnit,*) 'Calendar conversion:'

  era1 = ERA_IS_BCE
  yr1  = 0
  mo1  = 0
  day1 = 0
  hr1  = 0
  min1 = 0
  sec1 = 0
  julian1 = 2655.5
  call XF_JULIAN_TO_CALENDAR(era1, yr1, mo1, day1, hr1, min1, sec1, julian1, error)

  era2 = ERA_IS_BCE
  yr2  = 4706
  mo2  = 4
  day2 = 10
  hr2  = 0
  min2 = 0
  sec2 = 0
  julian2 = 0.0
  call XF_CALENDAR_TO_JULIAN(era2, yr2, mo2, day2, hr2, min2, sec2, julian2, error)

  era3 = ERA_IS_CE
  yr3  = 2004
  mo3  = 6
  day3 = 3
  hr3  = 2
  min3 = 8
  sec3 = 32
  julian3 = 0.0
  call XF_CALENDAR_TO_JULIAN(era3, yr3, mo3, day3, hr3, min3, sec3, julian3, error)

  era4 = ERA_IS_BCE
  yr4  = 0
  mo4  = 0
  day4 = 0
  hr4  = 0
  min4 = 0
  sec4 = 0
  julian4 = 2453159.5892592594_double
  call XF_JULIAN_TO_CALENDAR(era4, yr4, mo4, day4, hr4, min4, sec4, julian4, error)

WRITE(FileUnit,*) ''
WRITE(FileUnit,*) 'Dates #1 & #2  were calculated with the same date:'
WRITE(FileUnit,*) ''
WRITE(FileUnit,*) era1, '/', yr1, '/', mo1, '/', day1
WRITE(FileUnit,*) '', hr1, ':', min1, ':',sec1, '--- julian =',julian1
WRITE(FileUnit,*) ''
WRITE(FileUnit,*) era2, '/', yr2, '/', mo2, '/', day2
WRITE(FileUnit,*) '', hr2, ':', min2, ':',sec2, '--- julian =',julian2
WRITE(FileUnit,*) ''
WRITE(FileUnit,*) 'Dates #3 & #4  were calculated with the same date:'
WRITE(FileUnit,*) ''
WRITE(FileUnit,*) era3, '/', yr3, '/', mo3, '/', day3
WRITE(FileUnit,*) '', hr3, ':', min3, ':',sec3, '--- julian =',julian3
WRITE(FileUnit,*) ''
WRITE(FileUnit,*) era4, '/', yr4, '/', mo4, '/', day4
WRITE(FileUnit,*) '', hr4, ':', min4, ':',sec4, '--- julian =',julian4

  if (era1==era2 .AND. era3==era4) then
    if (yr1==yr2 .AND. yr3==yr4) then
	  if (mo1==mo2 .AND. mo3==mo4) then
	    if (day1==day2 .AND. day3==day4) then
		  if (hr1==hr2 .AND. hr3==hr4) then
		    if (min1==min2 .AND. min3==min4) then
              if (julian1==julian2 .AND. (julian3-.0000001)<julian4 .AND. (julian3+.0000001)>julian4) then
          WRITE(*,*) ''
          WRITE(*,*) 'Calendar conversion works correctly.'
        calendarworks = 1
		      endif
            endif
          endif
        endif
      endif
    endif
  endif
  if (calendarworks .NE. 1) then
    WRITE(*,*) 'Calendar Stuff DOES NOT Work Correctly'
  error = -1
  else
    error = 1
  endif
  return

END SUBROUTINE
!------------------------------------------------------------------------------
! FUNCTION TXI_TEST_MULTI_DATASETS
! PURPOSE  To test the newly translated functions
! NOTES
!------------------------------------------------------------------------------
SUBROUTINE TXI_TEST_MULTI_DATASETS

!call XF_SETUP_TO_WRITE_DATASETS(a_Filename, a_MultiDatasetsGroupPath, &
!                        a_PathInMultiDatasetsGroup, a_SpatialDataObjectGuid, &
 !                       a_OverwriteOptions, a_FileId, a_GroupId, error)
!XF_OPEN_MULTI_DATASETS_GROUP
!XF_DELETE_GROUP

END SUBROUTINE

!------------------------------------------------------------------------------
! SUBROUTINE TXI_WRITE_XMDF_VERSION
! PURPOSE    Write the XMDF version number to the screen
! NOTES
!------------------------------------------------------------------------------
SUBROUTINE TXI_TEST_VERSION ()
INTEGER  error
REAL     Version

  WRITE(*,*) ''
  call XF_GET_LIBRARY_VERSION(Version, error)
  WRITE(*,*) 'The current version of XMDF is: ', Version
  WRITE(*,*) ''

  return

END SUBROUTINE

!------------------------------------------------------------------------------
! SUBROUTINE TXI_TEST_COORD_SYSTEM
! PURPOSE    Reads a file's Coordinate Group and prints coordinate data out
!            to each text file.
! NOTES
!------------------------------------------------------------------------------
SUBROUTINE TXI_TEST_COORD_SYSTEM (xFileId, a_OutFile, error)
INTEGER, INTENT(IN) :: xFileId
INTEGER, INTENT(IN)        :: a_OutFile
INTEGER, INTENT(OUT)       :: error
INTEGER   iHorizDatum, iHorizUnits, iVertDatum, iVertUnits
INTEGER   iLat, iLon, iUtmZone, iSpcZone, iHpgnArea, iEllipse
INTEGER   bHorizDatum, nStatus
REAL(DOUBLE)        dCppLat, dCppLon, dMajorR, dMinorR
INTEGER      xCoordId
CHARACTER(LEN=BIG_STRING_SIZE)    strHorizUnits, strVertDatum
CHARACTER(LEN=BIG_STRING_SIZE)    strVertUnits

  ! Coordinate stuff
  ! Read Coordinate Info
    ! Open the Coordinate group
  !call H5Eset_auto_f(0, error)
  call XF_OPEN_COORDINATE_GROUP(xFileId, xCoordId, nStatus)
  if (nStatus < 0) then
    WRITE(a_OutFile,*) ''
    WRITE(a_OutFile,*) 'Coordinate Group not found'
    WRITE(a_OutFile,*) ''
    error = nStatus
    return
  endif
  !call H5Eset_auto_f(1, error)

  WRITE(a_OutFile,*) ''
  WRITE(a_OutFile,*) 'Coordinate System:'

  call XF_GET_HORIZ_DATUM(xCoordId, iHorizDatum, bHorizDatum)
  call XF_GET_HORIZ_UNITS(xCoordId, iHorizUnits, error)
  call XF_GET_VERT_DATUM(xCoordId, iVertDatum, error)
  call XF_GET_VERT_UNITS(xCoordId, iVertUnits, error)

    ! set horizontal units
  if (iHorizUnits == 0) then
    strHorizUnits = 'Horizontal units = US Survey Feet (=0)'
  else if (iHorizUnits == 1) then
    strHorizUnits = 'Horizontal units = International Feet (=1)'
  else if (iHorizUnits == 2) then
    strHorizUnits = 'Horizontal units = Meters (=2)'
  else
    strHorizUnits = 'ERROR in reading Horizontal units'
  endif

    ! set vertical datum
  if (iVertDatum == 0) then
    strVertDatum = 'Vertical datum = Local (=0)'
  else if (iVertDatum == 1) then
    strVertDatum = 'Vertical datum = NGVD 29 (=1)'
  else if (iVertDatum == 2) then
    strVertDatum = 'Vertical datum = NGVD 88 (=2)'
  else
    strVertDatum = 'ERROR in reading the Vertical datum'
  endif

    ! set vertocal units
  if (iVertUnits == 0) then
    strVertUnits = 'Vertical units = US Survey Feet (=0)'
  else if (iVertUnits == 1) then
    strVertUnits = 'Vertical units = International Feet (=1)'
  else if (iVertUnits == 2) then
    strVertUnits = 'Vertical units = Meters (=2)'
  else
    strVertUnits = 'ERROR in reading the Vertical units'
  endif

  if (bHorizDatum >= 0) then
    SELECT CASE (iHorizDatum)
      CASE (HORIZ_DATUM_GEOGRAPHIC)
        call XF_GET_ELLIPSE(xCoordId, iEllipse, error)
        call XF_GET_LAT(xCoordId, iLat, error)
        call XF_GET_LON(xCoordId, iLon, error)
          ! Write Horizontal and Vertical Info
        WRITE(a_OutFile,*) 'Horizontal datum = Geographic'
        WRITE(a_OutFile,*) 'Horizontal units = ', strHorizUnits(1:LEN_TRIM(strHorizUnits))
        WRITE(a_OutFile,*) 'Vertical datum = ', strVertDatum(1:LEN_TRIM(strVertDatum))
        WRITE(a_OutFile,*) 'Vertical units = ', strVertUnits(1:LEN_TRIM(strVertUnits))
          ! Write Latitude data
        if (iLat == 0) then
          WRITE(a_OutFile,*) '  Latitude = North (=0)'
        else if (iLat == 1) then
          WRITE(a_OutFile,*) '  Latitude = South (=1)'
        else
          WRITE(a_OutFile,*) '  LATITUDE INFO INCORRECT'
        endif
          ! Write Longitude data
        if (iLon == 0) then
          WRITE(a_OutFile,*) '  Longitude = East (=0)'
        else if (iLon == 1) then
          WRITE(a_OutFile,*) '  Longitude = West (=1)'
        else
          WRITE(a_OutFile,*) '  LONGITUDE INFO INCORRECT'
        endif
          ! Ellipse Information
          ! User-defined Ellipse (==32)
        if (iEllipse == 32) then
          WRITE(a_OutFile,*) 'Ellipse = User-defined:'
          call XF_GET_MAJOR_R(xCoordId, dMajorR, error)
          call XF_GET_MINOR_R(xCoordId, dMinorR, error)
          WRITE(a_OutFile,*) '  MajorR = ', dMajorR
          WRITE(a_OutFile,*) '  MinorR = ', dMinorR
        else
          WRITE(a_OutFile,*) 'Ellipse = ', iEllipse
        endif
        WRITE(a_OutFile,*) ''
      CASE (HORIZ_DATUM_UTM)
        call XF_GET_UTM_ZONE(xCoordId, iUtmZone, error)
          ! output info to text file
        if (iHorizDatum == HORIZ_DATUM_UTM) then
          WRITE(a_OutFile,*) 'Horizontal datum = UTM'
        else if (iHorizDatum == HORIZ_DATUM_UTM_NAD27) then
          WRITE(a_OutFile,*) 'Horizontal datum = UTM NAD27 (US)'
        else
          WRITE(a_OutFile,*) 'Horizontal datum = UTM NAD83 (US)'
        endif
        WRITE(a_OutFile,*) 'Horizontal units = ', &
                        strHorizUnits(1:LEN_TRIM(strHorizUnits))
        WRITE(a_OutFile,*) 'Vertical datum = ', &
                        strVertDatum(1:LEN_TRIM(strVertDatum))
        WRITE(a_OutFile,*) 'Vertical units = ', &
                        strVertUnits(1:LEN_TRIM(strVertUnits))
        WRITE(a_OutFile,*) 'UTM Zone = ', iUtmZone
        WRITE(a_OutFile,*) ''

      CASE (HORIZ_DATUM_UTM_NAD27, HORIZ_DATUM_UTM_NAD83)
        call XF_GET_UTM_ZONE(xCoordId, iUtmZone, error)
          ! output info to text file
        if (iHorizDatum == HORIZ_DATUM_UTM) then
          WRITE(a_OutFile,*) 'Horizontal datum = UTM'
        else if (iHorizDatum == HORIZ_DATUM_UTM_NAD27) then
          WRITE(a_OutFile,*) 'Horizontal datum = UTM NAD27 (US)'
        else
          WRITE(a_OutFile,*) 'Horizontal datum = UTM NAD83 (US)'
        endif
        WRITE(a_OutFile,*) 'Horizontal units = ', &
                        strHorizUnits(1:LEN_TRIM(strHorizUnits))
        WRITE(a_OutFile,*) 'Vertical datum = ', &
                        strVertDatum(1:LEN_TRIM(strVertDatum))
        WRITE(a_OutFile,*) 'Vertical units = ', &
                        strVertUnits(1:LEN_TRIM(strVertUnits))
        WRITE(a_OutFile,*) 'UTM Zone = ', iUtmZone
        WRITE(a_OutFile,*) ''
      CASE (HORIZ_DATUM_STATE_PLANE_NAD27, HORIZ_DATUM_STATE_PLANE_NAD83)
        call XF_GET_SPC_ZONE(xCoordId, iSpcZone, error)
          ! output info to text file
        if (iHorizDatum == HORIZ_DATUM_STATE_PLANE_NAD27) then
          WRITE(a_OutFile,*) 'Horizontal datum = State Plane NAD27 (US)'
        else 
          WRITE(a_OutFile,*) 'Horizontal datum = State Plane NAD83 (US)'
        endif
        WRITE(a_OutFile,*) 'Horizontal units = ', &
                        strHorizUnits(1:LEN_TRIM(strHorizUnits))
        WRITE(a_OutFile,*) 'Vertical datum = ', &
                        strVertDatum(1:LEN_TRIM(strVertDatum))
        WRITE(a_OutFile,*) 'Vertical units = ', &
                        strVertUnits(1:LEN_TRIM(strVertUnits))
        WRITE(a_OutFile,*) 'SPC Zone = ', iSpcZone
        WRITE(a_OutFile,*) ''
      CASE (HORIZ_DATUM_UTM_HPGN, HORIZ_DATUM_STATE_PLANE_HPGN, &
                                  HORIZ_DATUM_GEOGRAPHIC_HPGN)
        call XF_GET_HPGN_AREA(xCoordId, iHpgnArea, error)
        if (iHorizDatum == HORIZ_DATUM_UTM_HPGN) then
          WRITE(a_OutFile,*) 'Horizontal datum = UTM HPGN (US)'
        else if (iHorizDatum == HORIZ_DATUM_STATE_PLANE_HPGN) then
          WRITE(a_OutFile,*) 'Horizontal datum = State Plane HPGN (US)'
        else
          WRITE(a_OutFile,*) 'Horizontal datum = Geographic HPGN (US)'
        endif
        WRITE(a_OutFile,*) 'Horizontal units = ', &
                        strHorizUnits(1:LEN_TRIM(strHorizUnits))
        WRITE(a_OutFile,*) 'Vertical datum = ', &
                        strVertDatum(1:LEN_TRIM(strVertDatum))
        WRITE(a_OutFile,*) 'Vertical units = ', &
                        strVertUnits(1:LEN_TRIM(strVertUnits))
        WRITE(a_OutFile,*) 'HPGN Area = ', iHpgnArea
        WRITE(a_OutFile,*) ''
      CASE (HORIZ_DATUM_CPP)
        call XF_GET_CPP_LAT(xCoordId, dCppLat, error)
        call XF_GET_CPP_LON(xCoordId, dCppLon, error)
        WRITE(a_OutFile,*) 'Horizontal datum = CPP (Carte Parallelo-Grammatique Projection)'
        WRITE(a_OutFile,*) 'Horizontal units = ', &
                        strHorizUnits(1:LEN_TRIM(strHorizUnits))
        WRITE(a_OutFile,*) 'Vertical datum = ', &
                        strVertDatum(1:LEN_TRIM(strVertDatum))
        WRITE(a_OutFile,*) 'Vertical units = ', &
                        strVertUnits(1:LEN_TRIM(strVertUnits))
        WRITE(a_OutFile,*) 'CPP Latitude = ', dCppLat
        WRITE(a_OutFile,*) 'CPP Longitude = ', dCppLon
        WRITE(a_OutFile,*) ''
    CASE (HORIZ_DATUM_LOCAL, HORIZ_DATUM_GEOGRAPHIC_NAD27, &
          HORIZ_DATUM_GEOGRAPHIC_NAD83)
          ! do other systems
        if (iHorizDatum == HORIZ_DATUM_LOCAL) then
          WRITE(a_OutFile,*) 'Horizontal datum = Local'
        else if (iHorizDatum == HORIZ_DATUM_GEOGRAPHIC_NAD27) then
          WRITE(a_OutFile,*) 'Horizontal datum = Geographic NAD27 (US)'
        else
          WRITE(a_OutFile,*) 'Horizontal datum = Geographic NAD83 (US)'
    endif
        WRITE(a_OutFile,*) 'Horizontal units = ', &
                        strHorizUnits(1:LEN_TRIM(strHorizUnits))
        WRITE(a_OutFile,*) 'Vertical datum = ', &
                        strVertDatum(1:LEN_TRIM(strVertDatum))
        WRITE(a_OutFile,*) 'Vertical units = ', &
                        strVertUnits(1:LEN_TRIM(strVertUnits))
        WRITE(a_OutFile,*) ''
      CASE DEFAULT
          WRITE(a_OutFile,*) 'ERROR: The coordinate information is not found in the .h5 file'
          error = -1
      return
    END SELECT
  else
    WRITE(a_OutFile,*) 'Coordinate information in HDF5 file is incomplete.'
    WRITE(a_OutFile,*) ''
  endif

  call XF_CLOSE_GROUP(xCoordId, error)
  xCoordId = 0

  return

END SUBROUTINE

SUBROUTINE TXI_TEST_GEOMETRIC_PATHS(error)
  INTEGER, INTENT(OUT) :: error
  INTEGER                 compression

  compression = -1

    ! test writing a geometric path file */
  WRITE(*,*)''
  WRITE(*,*)'Writing geometric path data'
  WRITE(*,*)''
 
  call TM_WRITE_TEST_PATHS(GEOMPATH_A_FILE_F, compression, error);
  if (error < 0) then
    WRITE(*,*) 'Error writing geometric path data A'
    return
  endif
  WRITE(*,*) 'Finished writing geometric path data A'

    ! test reading a geometric path file */
  call TM_READ_TEST_PATHS(GEOMPATH_A_FILE_F, GEOMPATH_A_FILE_F_OUT, error)

  return

END SUBROUTINE TXI_TEST_GEOMETRIC_PATHS

!****************************

END MODULE TestsModule

PROGRAM TESTS

USE TestDatasets
USE Xmdf
USE TestMesh
USE TestDefs
USE TestsModule

INTEGER    error
! Assign pp to force crash on error

call XF_INITIALIZE(error)

  ! test the dataset routines
call TXI_TEST_TIMESTEPS(error)
if (error < 0) then
  WRITE(*,*) 'Error in writing timesteps!'
  PAUSE 'Press ENTER to exit...'
endif

  ! test the dataset routines
call TXI_TEST_DATASETS(error)
if (error < 0) then
  WRITE(*,*) 'Error in writing datasets!'
  PAUSE 'Press ENTER to exit...'
endif

  ! test overwriting datasets
call TXI_TEST_OVERWRITE_DSETS(error)
if (error < 0) then
  WRITE(*,*) 'Error in overwriting datasets!'
  PAUSE 'Press ENTER to exit...'
else
  WRITE(*,*) 'Finished writing datasets...'
endif

  ! test mesh stuff
call TXI_TEST_MESHS(error)
if (error < 0) then
  WRITE(*,*) 'Error in TXI_TEST_MESHS!'
  PAUSE 'Press ENTER to exit...'
endif

  ! test grid stuff
call TXI_TEST_GRIDS(error)
if (error < 0) then
  WRITE(*,*) 'Error in TXI_TEST_GRIDS!'
  PAUSE 'Press ENTER to exit...'
endif

  ! test c in fortran
call TXI_TEST_C(error)
! Ignore error because fortran will be run once again with file there

  ! test calendar stuff
call TXI_TEST_CALENDAR(error)
if (error < 0) then
  WRITE(*,*) 'Error in TXI_TEST_CALENDAR!'
  PAUSE 'Press ENTER to exit...'
endif

  ! test version
call TXI_TEST_VERSION

   ! test geometric paths
call TXI_TEST_GEOMETRIC_PATHS(error)
if (error < 0) then
  WRITE(*,*) 'Error in TXI_TEST_GEOMETRIC_PATHS!'
  PAUSE 'Press ENTER to exit...'
endif

call XF_CLOSE (error)
if (error < 0) then
  WRITE(*,*) 'Error in XF_CLOSE!'
  PAUSE 'Press ENTER to exit...'
endif

! PAUSE 'Press ENTER to exit...'

END PROGRAM