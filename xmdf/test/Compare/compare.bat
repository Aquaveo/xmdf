echo off

REM ### Run tests

cd "test_out"
REM dir

%1
echo === LOOK FOR ANY ERRORS ABOVE.
if errorlevel 1 pause
REM pause
%2
echo === LOOK FOR ANY ERRORS ABOVE.
if errorlevel 1 pause
REM pause
%1
echo === CHECK FOR ANY ERRORS ABOVE.
if errorlevel 1 pause
REM pause
%2
echo === LOOK FOR ANY ERRORS ABOVE.
if errorlevel 1 pause
REM pause

cd "..\Reference"

REM ### Compare output files.
echo ====================================================================
echo = NEXT, OUTPUT FILES WILL BE COMPARED AGAINST THE REFERENCE FILES
echo = COMPARISONS WILL PAUSE TO DISPLAY ANY DIFFERENCES.
echo ====================================================================
REM pause
echo on
fc Calendar_c.txt "..\test_out\Calendar_c.txt"
if errorlevel 1 pause
fc Calendar_f.txt "..\test_out\Calendar_f.txt"
if errorlevel 1 pause
call mydiff.bat geompath_a_file_c.h5 "..\test_out\geompath_a_file_c.h5" 
if errorlevel 1 pause
fc geompath_a_file_c_out.txt "..\test_out\geompath_a_file_c_out.txt"
if errorlevel 1 pause
call mydiff.bat Geompath_a_file_f.h5 "..\test_out\Geompath_a_file_f.h5" 
if errorlevel 1 pause
fc Geompath_a_file_f_out.txt "..\test_out\Geompath_a_file_f_out.txt"
if errorlevel 1 pause
call mydiff.bat grid_cart2d_a_file_c.h5 "..\test_out\grid_cart2d_a_file_c.h5" 
if errorlevel 1 pause
call mydiff.bat grid_cart2d_a_file_f.h5 "..\test_out\grid_cart2d_a_file_f.h5" 
if errorlevel 1 pause
fc grid_cart2d_a_out_c.txt "..\test_out\grid_cart2d_a_out_c.txt"
if errorlevel 1 pause
fc grid_cart2d_a_out_cf.txt "..\test_out\grid_cart2d_a_out_cf.txt"
if errorlevel 1 pause
fc grid_cart2d_a_out_f.txt "..\test_out\grid_cart2d_a_out_f.txt"
if errorlevel 1 pause
fc grid_cart2d_a_out_fc.txt "..\test_out\grid_cart2d_a_out_fc.txt"
if errorlevel 1 pause
call mydiff.bat grid_cart3d_a_file_c.h5 "..\test_out\grid_cart3d_a_file_c.h5" 
if errorlevel 1 pause
call mydiff.bat grid_cart3d_a_file_f.h5 "..\test_out\grid_cart3d_a_file_f.h5" 
if errorlevel 1 pause
fc grid_cart3d_a_out_c.txt "..\test_out\grid_cart3d_a_out_c.txt"
if errorlevel 1 pause
fc grid_cart3d_a_out_cf.txt "..\test_out\grid_cart3d_a_out_cf.txt"
if errorlevel 1 pause
fc grid_cart3d_a_out_f.txt "..\test_out\grid_cart3d_a_out_f.txt"
if errorlevel 1 pause
fc grid_cart3d_a_out_fc.txt "..\test_out\grid_cart3d_a_out_fc.txt"
if errorlevel 1 pause
call mydiff.bat grid_curv2d_a_file_c.h5 "..\test_out\grid_curv2d_a_file_c.h5" 
if errorlevel 1 pause
call mydiff.bat grid_curv2d_a_file_f.h5 "..\test_out\grid_curv2d_a_file_f.h5" 
if errorlevel 1 pause
fc grid_curv2d_a_out_c.txt "..\test_out\grid_curv2d_a_out_c.txt"
if errorlevel 1 pause
fc grid_curv2d_a_out_cf.txt "..\test_out\grid_curv2d_a_out_cf.txt"
if errorlevel 1 pause
fc grid_curv2d_a_out_f.txt "..\test_out\grid_curv2d_a_out_f.txt"
if errorlevel 1 pause
fc grid_curv2d_a_out_fc.txt "..\test_out\grid_curv2d_a_out_fc.txt"
if errorlevel 1 pause
call mydiff.bat mesh_a_file_c.h5 "..\test_out\mesh_a_file_c.h5" 
if errorlevel 1 pause
fc mesh_a_file_c.txt "..\test_out\mesh_a_file_c.txt"
if errorlevel 1 pause
fc mesh_a_file_cf.txt "..\test_out\mesh_a_file_cf.txt"
if errorlevel 1 pause
call mydiff.bat mesh_a_file_f.h5 "..\test_out\mesh_a_file_f.h5" 
if errorlevel 1 pause
fc mesh_a_file_f.txt "..\test_out\mesh_a_file_f.txt"
if errorlevel 1 pause
fc mesh_a_file_fc.txt "..\test_out\mesh_a_file_fc.txt"
if errorlevel 1 pause
call mydiff.bat mesh_b_file_c.h5 "..\test_out\mesh_b_file_c.h5" 
if errorlevel 1 pause
fc mesh_b_file_c.txt "..\test_out\mesh_b_file_c.txt"
if errorlevel 1 pause
fc mesh_b_file_cf.txt "..\test_out\mesh_b_file_cf.txt"
if errorlevel 1 pause
call mydiff.bat mesh_b_file_f.h5 "..\test_out\mesh_b_file_f.h5" 
if errorlevel 1 pause
fc mesh_b_file_f.txt "..\test_out\mesh_b_file_f.txt"
if errorlevel 1 pause
fc mesh_b_file_fc.txt "..\test_out\mesh_b_file_fc.txt"
if errorlevel 1 pause
call mydiff.bat MultiDataSet_c.h5 "..\test_out\MultiDataSet_c.h5" 
if errorlevel 1 pause
fc MultiDataSet_c.txt "..\test_out\MultiDataSet_c.txt"
if errorlevel 1 pause
call mydiff.bat MultiDataSet_f.h5 "..\test_out\MultiDataSet_f.h5" 
if errorlevel 1 pause
fc MultiDataSet_f.txt "..\test_out\MultiDataSet_f.txt"
if errorlevel 1 pause
call mydiff.bat ScalarA_c.h5 "..\test_out\ScalarA_c.h5" 
if errorlevel 1 pause
fc ScalarA_c.txt "..\test_out\ScalarA_c.txt"
if errorlevel 1 pause
call mydiff.bat ScalarA_edited_c.h5 "..\test_out\ScalarA_edited_c.h5" 
if errorlevel 1 pause
fc ScalarA_edited_c.txt "..\test_out\ScalarA_edited_c.txt"
if errorlevel 1 pause
call mydiff.bat ScalarA_edited_f.h5 "..\test_out\ScalarA_edited_f.h5" 
if errorlevel 1 pause
fc ScalarA_edited_f.txt "..\test_out\ScalarA_edited_f.txt"
if errorlevel 1 pause
fc ScalarA_cf.txt "..\test_out\ScalarA_cf.txt"
if errorlevel 1 pause
call mydiff.bat ScalarA_f.h5 "..\test_out\ScalarA_f.h5" 
if errorlevel 1 pause
fc ScalarA_f.txt "..\test_out\ScalarA_f.txt"
if errorlevel 1 pause
fc ScalarA_fc.txt "..\test_out\ScalarA_fc.txt"
if errorlevel 1 pause
call mydiff.bat ScalarB_c.h5 "..\test_out\ScalarB_c.h5" 
if errorlevel 1 pause
fc ScalarB_c.txt "..\test_out\ScalarB_c.txt"
if errorlevel 1 pause
fc ScalarB_cf.txt "..\test_out\ScalarB_cf.txt"
if errorlevel 1 pause
call mydiff.bat ScalarB_f.h5 "..\test_out\ScalarB_f.h5" 
if errorlevel 1 pause
fc ScalarB_f.txt "..\test_out\ScalarB_f.txt"
if errorlevel 1 pause
fc ScalarB_fc.txt "..\test_out\ScalarB_fc.txt"
if errorlevel 1 pause
call mydiff.bat TT_MultiDataSet_c.h5 "..\test_out\TT_MultiDataSet_c.h5" 
if errorlevel 1 pause
fc TT_MultiDataSet_c.txt "..\test_out\TT_MultiDataSet_c.txt"
if errorlevel 1 pause
call mydiff.bat TT_ScalarA_c.h5 "..\test_out\TT_ScalarA_c.h5" 
if errorlevel 1 pause
fc TT_ScalarA_c.txt "..\test_out\TT_ScalarA_c.txt"
if errorlevel 1 pause
call mydiff.bat TT_Vector2D_A_c.h5 "..\test_out\TT_Vector2D_A_c.h5" 
if errorlevel 1 pause
fc TT_Vector2D_A_c.txt "..\test_out\TT_Vector2D_A_c.txt"
if errorlevel 1 pause
call mydiff.bat Vector2D_A_c.h5 "..\test_out\Vector2D_A_c.h5" 
if errorlevel 1 pause
call mydiff.bat Vector2D_A_c.h5 "..\test_out\Vector2D_A_Pieces_c.h5" 
if errorlevel 1 pause
fc Vector2D_A_c.txt "..\test_out\Vector2D_A_c.txt"
if errorlevel 1 pause
fc Vector2D_A_cf.txt "..\test_out\Vector2D_A_cf.txt"
if errorlevel 1 pause
call mydiff.bat Vector2D_A_f.h5 "..\test_out\Vector2D_A_f.h5" 
if errorlevel 1 pause
fc Vector2D_A_f.txt "..\test_out\Vector2D_A_f.txt"
if errorlevel 1 pause
fc Vector2D_A_fc.txt "..\test_out\Vector2D_A_fc.txt"
if errorlevel 1 pause
call mydiff.bat Vector2D_A_f.h5 "..\test_out\Vector2D_A_Pieces_f.h5" 
if errorlevel 1 pause
call mydiff.bat Vector2D_B_c.h5 "..\test_out\Vector2D_B_c.h5" 
if errorlevel 1 pause
fc Vector2D_B_c.txt "..\test_out\Vector2D_B_c.txt"
if errorlevel 1 pause
fc Vector2D_B_cf.txt "..\test_out\Vector2D_B_cf.txt"
if errorlevel 1 pause
call mydiff.bat Vector2D_B_f.h5 "..\test_out\Vector2D_B_f.h5" 
if errorlevel 1 pause
fc Vector2D_B_f.txt "..\test_out\Vector2D_B_f.txt"
if errorlevel 1 pause
fc Vector2D_B_fc.txt "..\test_out\Vector2D_B_fc.txt"
if errorlevel 1 pause

REM Check selected Fortran == C output files

REM call mydiff.bat Geompath_a_file_f.h5 "..\test_out\Geompath_a_file_c.h5" 
REM if errorlevel 1 pause
call mydiff.bat grid_cart2d_a_file_f.h5 "..\test_out\grid_cart2d_a_file_c.h5" 
if errorlevel 1 pause
call mydiff.bat grid_cart3d_a_file_f.h5 "..\test_out\grid_cart3d_a_file_c.h5" 
if errorlevel 1 pause
call mydiff.bat grid_curv2d_a_file_f.h5 "..\test_out\grid_curv2d_a_file_c.h5" 
if errorlevel 1 pause
REM call mydiff.bat mesh_a_file_f.h5 "..\test_out\mesh_a_file_c.h5" 
REM if errorlevel 1 pause
REM call mydiff.bat mesh_b_file_f.h5 "..\test_out\mesh_b_file_c.h5" 
REM if errorlevel 1 pause
REM call mydiff.bat MultiDataSet_f.h5 "..\test_out\MultiDataSet_c.h5" 
REM if errorlevel 1 pause
call mydiff.bat ScalarA_f.h5 "..\test_out\ScalarA_c.h5" 
if errorlevel 1 pause
call mydiff.bat ScalarB_f.h5 "..\test_out\ScalarB_c.h5" 
if errorlevel 1 pause
REM call mydiff.bat TT_MultiDataSet_f.h5 "..\test_out\TT_MultiDataSet_c.h5" 
REM if errorlevel 1 pause
call mydiff.bat TT_ScalarA_f.h5 "..\test_out\TT_ScalarA_c.h5" 
if errorlevel 1 pause
call mydiff.bat TT_Vector2D_A_f.h5 "..\test_out\TT_Vector2D_A_c.h5" 
if errorlevel 1 pause
call mydiff.bat Vector2D_A_f.h5 "..\test_out\Vector2D_A_c.h5" 
if errorlevel 1 pause
call mydiff.bat Vector2D_B_f.h5 "..\test_out\Vector2D_B_c.h5" 
if errorlevel 1 pause

call mydiff.bat "..\test_out\geompath_a_file_c.h5" "..\test_out\Geompath_a_file_f.h5" 
if errorlevel 1 pause
call mydiff.bat "..\test_out\grid_cart2d_a_file_c.h5" "..\test_out\grid_cart2d_a_file_f.h5" 
if errorlevel 1 pause
call mydiff.bat "..\test_out\grid_cart3d_a_file_c.h5" "..\test_out\grid_cart3d_a_file_f.h5" 
if errorlevel 1 pause
call mydiff.bat "..\test_out\grid_curv2d_a_file_c.h5" "..\test_out\grid_curv2d_a_file_f.h5" 
if errorlevel 1 pause
call mydiff.bat "..\test_out\mesh_a_file_c.h5" "..\test_out\mesh_a_file_f.h5" 
if errorlevel 1 pause
call mydiff.bat "..\test_out\mesh_b_file_c.h5" "..\test_out\mesh_b_file_f.h5" 
if errorlevel 1 pause
call mydiff.bat "..\test_out\MultiDataSet_c.h5" "..\test_out\MultiDataSet_f.h5" 
if errorlevel 1 pause
call mydiff.bat "..\test_out\ScalarA_c.h5" "..\test_out\ScalarA_f.h5" 
if errorlevel 1 pause
call mydiff.bat "..\test_out\ScalarA_c.h5" "..\test_out\ScalarA_Pieces_c.h5" 
if errorlevel 1 pause
call mydiff.bat "..\test_out\ScalarA_c.h5" "..\test_out\ScalarA_Pieces_f.h5" 
if errorlevel 1 pause
call mydiff.bat "ScalarA_Pieces_alt_c.h5" "..\test_out\ScalarA_Pieces_alt_c.h5"
if errorlevel 1 pause
call mydiff.bat "..\test_out\ScalarA_Pieces_alt_f.h5" "..\test_out\ScalarA_Pieces_alt_c.h5" 
if errorlevel 1 pause
call mydiff.bat "..\test_out\ScalarB_c.h5" "..\test_out\ScalarB_f.h5" 
if errorlevel 1 pause
call mydiff.bat "..\test_out\TT_MultiDataSet_c.h5" "..\test_out\TT_MultiDataSet_f.h5" 
if errorlevel 1 pause
call mydiff.bat "..\test_out\TT_ScalarA_c.h5" "..\test_out\TT_ScalarA_f.h5" 
if errorlevel 1 pause
call mydiff.bat "..\test_out\TT_Vector2D_A_c.h5" "..\test_out\TT_Vector2D_A_f.h5" 
if errorlevel 1 pause
call mydiff.bat "..\test_out\Vector2D_A_c.h5" "..\test_out\Vector2D_A_f.h5" 
if errorlevel 1 pause
call mydiff.bat "..\test_out\Vector2D_B_c.h5" "..\test_out\Vector2D_B_f.h5" 
if errorlevel 1 pause
REM pause
cd ..
