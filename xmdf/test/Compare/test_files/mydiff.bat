if NOT EXIST %1 pause
if NOT EXIST %2 pause
h5diff.exe %1 %2 -r
if errorlevel 1 pause
