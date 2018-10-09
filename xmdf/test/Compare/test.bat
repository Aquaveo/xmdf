REM project is in the Compare folder

REM start /wait  cmd /C clean.bat

rem echo off

REM ### Clean Directories

mkdir "test_out"
cd "test_out"

dir /w *.htm
dir /w *.h5
dir /w *.txt
echo ================================================================
echo = THE ABOVE FILES WILL BE DELETED BEFORE TESTING
echo ================================================================
REM exit 1
del /q *.htm
del /q *.h5
del /q *.txt

REM start cmd /C compare.bat testfdx64.exe TestCdx64.exe

rem echo off

REM ### Run tests

REM dir

REM %1
REM echo === LOOK FOR ANY ERRORS ABOVE.
copy ..\test_files\test_out_f1\*.* .
if errorlevel 1 exit 1
REM exit 1
REM %2
..\..\..\..\dev\build_cpp\TestC\bin\TestC.exe
echo === LOOK FOR ANY ERRORS ABOVE.
if errorlevel 1 exit 1
REM exit 1
REM %1
REM echo === CHECK FOR ANY ERRORS ABOVE.
copy ..\test_files\test_out_f2\*.* .
if errorlevel 1 exit 1
REM exit 1
REM %2
..\..\..\..\dev\build_cpp\TestC\bin\TestC.exe
echo === LOOK FOR ANY ERRORS ABOVE.
if errorlevel 1 exit 1
REM exit 1

cd "..\test_files"

REM ### Compare output files.
echo ====================================================================
echo = NEXT, OUTPUT FILES WILL BE COMPARED AGAINST THE REFERENCE FILES
echo = COMPARISONS WILL exit 1 TO DISPLAY ANY DIFFERENCES.
echo ====================================================================
copy ..\
REM exit 1
echo on
fc Calendar_c.txt "..\test_out\Calendar_c.txt"
if errorlevel 1 exit 1
fc Calendar_f.txt "..\test_out\Calendar_f.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r geompath_a_file_c.h5 "..\test_out\geompath_a_file_c.h5" 
if errorlevel 1 exit 1
fc geompath_a_file_c_out.txt "..\test_out\geompath_a_file_c_out.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r Geompath_a_file_f.h5 "..\test_out\Geompath_a_file_f.h5" 
if errorlevel 1 exit 1
fc Geompath_a_file_f_out.txt "..\test_out\Geompath_a_file_f_out.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r grid_cart2d_a_file_c.h5 "..\test_out\grid_cart2d_a_file_c.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r grid_cart2d_a_file_f.h5 "..\test_out\grid_cart2d_a_file_f.h5" 
if errorlevel 1 exit 1
fc grid_cart2d_a_out_c.txt "..\test_out\grid_cart2d_a_out_c.txt"
if errorlevel 1 exit 1
fc grid_cart2d_a_out_cf.txt "..\test_out\grid_cart2d_a_out_cf.txt"
if errorlevel 1 exit 1
fc grid_cart2d_a_out_f.txt "..\test_out\grid_cart2d_a_out_f.txt"
if errorlevel 1 exit 1
fc grid_cart2d_a_out_fc.txt "..\test_out\grid_cart2d_a_out_fc.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r grid_cart3d_a_file_c.h5 "..\test_out\grid_cart3d_a_file_c.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r grid_cart3d_a_file_f.h5 "..\test_out\grid_cart3d_a_file_f.h5" 
if errorlevel 1 exit 1
fc grid_cart3d_a_out_c.txt "..\test_out\grid_cart3d_a_out_c.txt"
if errorlevel 1 exit 1
fc grid_cart3d_a_out_cf.txt "..\test_out\grid_cart3d_a_out_cf.txt"
if errorlevel 1 exit 1
fc grid_cart3d_a_out_f.txt "..\test_out\grid_cart3d_a_out_f.txt"
if errorlevel 1 exit 1
fc grid_cart3d_a_out_fc.txt "..\test_out\grid_cart3d_a_out_fc.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r grid_curv2d_a_file_c.h5 "..\test_out\grid_curv2d_a_file_c.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r grid_curv2d_a_file_f.h5 "..\test_out\grid_curv2d_a_file_f.h5" 
if errorlevel 1 exit 1
fc grid_curv2d_a_out_c.txt "..\test_out\grid_curv2d_a_out_c.txt"
if errorlevel 1 exit 1
fc grid_curv2d_a_out_cf.txt "..\test_out\grid_curv2d_a_out_cf.txt"
if errorlevel 1 exit 1
fc grid_curv2d_a_out_f.txt "..\test_out\grid_curv2d_a_out_f.txt"
if errorlevel 1 exit 1
fc grid_curv2d_a_out_fc.txt "..\test_out\grid_curv2d_a_out_fc.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r mesh_a_file_c.h5 "..\test_out\mesh_a_file_c.h5" 
if errorlevel 1 exit 1
fc mesh_a_file_c.txt "..\test_out\mesh_a_file_c.txt"
if errorlevel 1 exit 1
fc mesh_a_file_cf.txt "..\test_out\mesh_a_file_cf.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r mesh_a_file_f.h5 "..\test_out\mesh_a_file_f.h5" 
if errorlevel 1 exit 1
fc mesh_a_file_f.txt "..\test_out\mesh_a_file_f.txt"
if errorlevel 1 exit 1
fc mesh_a_file_fc.txt "..\test_out\mesh_a_file_fc.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r mesh_b_file_c.h5 "..\test_out\mesh_b_file_c.h5" 
if errorlevel 1 exit 1
fc mesh_b_file_c.txt "..\test_out\mesh_b_file_c.txt"
if errorlevel 1 exit 1
fc mesh_b_file_cf.txt "..\test_out\mesh_b_file_cf.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r mesh_b_file_f.h5 "..\test_out\mesh_b_file_f.h5" 
if errorlevel 1 exit 1
fc mesh_b_file_f.txt "..\test_out\mesh_b_file_f.txt"
if errorlevel 1 exit 1
fc mesh_b_file_fc.txt "..\test_out\mesh_b_file_fc.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r MultiDataSet_c.h5 "..\test_out\MultiDataSet_c.h5" 
if errorlevel 1 exit 1
fc MultiDataSet_c.txt "..\test_out\MultiDataSet_c.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r MultiDataSet_f.h5 "..\test_out\MultiDataSet_f.h5" 
if errorlevel 1 exit 1
fc MultiDataSet_f.txt "..\test_out\MultiDataSet_f.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r ScalarA_c.h5 "..\test_out\ScalarA_c.h5" 
if errorlevel 1 exit 1
fc ScalarA_c.txt "..\test_out\ScalarA_c.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r ScalarA_edited_c.h5 "..\test_out\ScalarA_edited_c.h5" 
if errorlevel 1 exit 1
fc ScalarA_edited_c.txt "..\test_out\ScalarA_edited_c.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r ScalarA_edited_f.h5 "..\test_out\ScalarA_edited_f.h5" 
if errorlevel 1 exit 1
fc ScalarA_edited_f.txt "..\test_out\ScalarA_edited_f.txt"
if errorlevel 1 exit 1
fc ScalarA_cf.txt "..\test_out\ScalarA_cf.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r ScalarA_f.h5 "..\test_out\ScalarA_f.h5" 
if errorlevel 1 exit 1
fc ScalarA_f.txt "..\test_out\ScalarA_f.txt"
if errorlevel 1 exit 1
fc ScalarA_fc.txt "..\test_out\ScalarA_fc.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r ScalarB_c.h5 "..\test_out\ScalarB_c.h5" 
if errorlevel 1 exit 1
fc ScalarB_c.txt "..\test_out\ScalarB_c.txt"
if errorlevel 1 exit 1
fc ScalarB_cf.txt "..\test_out\ScalarB_cf.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r ScalarB_f.h5 "..\test_out\ScalarB_f.h5" 
if errorlevel 1 exit 1
fc ScalarB_f.txt "..\test_out\ScalarB_f.txt"
if errorlevel 1 exit 1
fc ScalarB_fc.txt "..\test_out\ScalarB_fc.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r TT_MultiDataSet_c.h5 "..\test_out\TT_MultiDataSet_c.h5" 
if errorlevel 1 exit 1
fc TT_MultiDataSet_c.txt "..\test_out\TT_MultiDataSet_c.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r TT_ScalarA_c.h5 "..\test_out\TT_ScalarA_c.h5" 
if errorlevel 1 exit 1
fc TT_ScalarA_c.txt "..\test_out\TT_ScalarA_c.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r TT_Vector2D_A_c.h5 "..\test_out\TT_Vector2D_A_c.h5" 
if errorlevel 1 exit 1
fc TT_Vector2D_A_c.txt "..\test_out\TT_Vector2D_A_c.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r Vector2D_A_c.h5 "..\test_out\Vector2D_A_c.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r Vector2D_A_c.h5 "..\test_out\Vector2D_A_Pieces_c.h5" 
if errorlevel 1 exit 1
fc Vector2D_A_c.txt "..\test_out\Vector2D_A_c.txt"
if errorlevel 1 exit 1
fc Vector2D_A_cf.txt "..\test_out\Vector2D_A_cf.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r Vector2D_A_f.h5 "..\test_out\Vector2D_A_f.h5" 
if errorlevel 1 exit 1
fc Vector2D_A_f.txt "..\test_out\Vector2D_A_f.txt"
if errorlevel 1 exit 1
fc Vector2D_A_fc.txt "..\test_out\Vector2D_A_fc.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r Vector2D_A_f.h5 "..\test_out\Vector2D_A_Pieces_f.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r Vector2D_B_c.h5 "..\test_out\Vector2D_B_c.h5" 
if errorlevel 1 exit 1
fc Vector2D_B_c.txt "..\test_out\Vector2D_B_c.txt"
if errorlevel 1 exit 1
fc Vector2D_B_cf.txt "..\test_out\Vector2D_B_cf.txt"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r Vector2D_B_f.h5 "..\test_out\Vector2D_B_f.h5" 
if errorlevel 1 exit 1
fc Vector2D_B_f.txt "..\test_out\Vector2D_B_f.txt"
if errorlevel 1 exit 1
fc Vector2D_B_fc.txt "..\test_out\Vector2D_B_fc.txt"
if errorlevel 1 exit 1

REM Check selected Fortran == C output files

REM ..\test_files\h5diff.exe -r Geompath_a_file_f.h5 "..\test_out\Geompath_a_file_c.h5" 
REM if errorlevel 1 exit 1
..\test_files\h5diff.exe -r grid_cart2d_a_file_f.h5 "..\test_out\grid_cart2d_a_file_c.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r grid_cart3d_a_file_f.h5 "..\test_out\grid_cart3d_a_file_c.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r grid_curv2d_a_file_f.h5 "..\test_out\grid_curv2d_a_file_c.h5" 
if errorlevel 1 exit 1
REM ..\test_files\h5diff.exe -r mesh_a_file_f.h5 "..\test_out\mesh_a_file_c.h5" 
REM if errorlevel 1 exit 1
REM ..\test_files\h5diff.exe -r mesh_b_file_f.h5 "..\test_out\mesh_b_file_c.h5" 
REM if errorlevel 1 exit 1
REM ..\test_files\h5diff.exe -r MultiDataSet_f.h5 "..\test_out\MultiDataSet_c.h5" 
REM if errorlevel 1 exit 1
..\test_files\h5diff.exe -r ScalarA_f.h5 "..\test_out\ScalarA_c.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r ScalarB_f.h5 "..\test_out\ScalarB_c.h5" 
if errorlevel 1 exit 1
REM ..\test_files\h5diff.exe -r TT_MultiDataSet_f.h5 "..\test_out\TT_MultiDataSet_c.h5" 
REM if errorlevel 1 exit 1
..\test_files\h5diff.exe -r TT_ScalarA_f.h5 "..\test_out\TT_ScalarA_c.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r TT_Vector2D_A_f.h5 "..\test_out\TT_Vector2D_A_c.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r Vector2D_A_f.h5 "..\test_out\Vector2D_A_c.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r Vector2D_B_f.h5 "..\test_out\Vector2D_B_c.h5" 
if errorlevel 1 exit 1

..\test_files\h5diff.exe -r "..\test_out\geompath_a_file_c.h5" "..\test_out\Geompath_a_file_f.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\grid_cart2d_a_file_c.h5" "..\test_out\grid_cart2d_a_file_f.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\grid_cart3d_a_file_c.h5" "..\test_out\grid_cart3d_a_file_f.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\grid_curv2d_a_file_c.h5" "..\test_out\grid_curv2d_a_file_f.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\mesh_a_file_c.h5" "..\test_out\mesh_a_file_f.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\mesh_b_file_c.h5" "..\test_out\mesh_b_file_f.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\MultiDataSet_c.h5" "..\test_out\MultiDataSet_f.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\ScalarA_c.h5" "..\test_out\ScalarA_f.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\ScalarA_c.h5" "..\test_out\ScalarA_Pieces_c.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\ScalarA_c.h5" "..\test_out\ScalarA_Pieces_f.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "ScalarA_Pieces_alt_c.h5" "..\test_out\ScalarA_Pieces_alt_c.h5"
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\ScalarA_Pieces_alt_f.h5" "..\test_out\ScalarA_Pieces_alt_c.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\ScalarB_c.h5" "..\test_out\ScalarB_f.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\TT_MultiDataSet_c.h5" "..\test_out\TT_MultiDataSet_f.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\TT_ScalarA_c.h5" "..\test_out\TT_ScalarA_f.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\TT_Vector2D_A_c.h5" "..\test_out\TT_Vector2D_A_f.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\Vector2D_A_c.h5" "..\test_out\Vector2D_A_f.h5" 
if errorlevel 1 exit 1
..\test_files\h5diff.exe -r "..\test_out\Vector2D_B_c.h5" "..\test_out\Vector2D_B_f.h5" 
if errorlevel 1 exit 1
REM exit 1
cd ..
