@echo off

echo.
echo ################################################
echo ####
echo #### migrationToVS2012 - command-line script
echo #### Upgrade oF projects for Visual Studio 2012
rem  #### 04/2013 - Leo Colomb
echo ####
echo ################################################
echo.
echo.

rem //////////////
rem / Setup Part /
rem //////////////

rem Manual setup
set PROJECTNAME=
set VSOLD=vs2010
set VSPROJTYPE=vcxproj

rem Dynamic setup
if "%PROJECTNAME%"=="" (
    echo ++++ Setting up +++++++++
    set /p PROJECTNAME="Type the project name which is going to be upgraded: "
    echo.
)

rem ///////////////
rem / Script Part /
rem ///////////////

echo ++++ Migration start ++++
echo --- Keep old files accessible -------
PowerShell.exe -Command "Rename-Item %CD%\%PROJECTNAME%.sln %PROJECTNAME%.sln.old"
PowerShell.exe -Command "Rename-Item %CD%\%PROJECTNAME%.%VSPROJTYPE% %PROJECTNAME%.%VSPROJTYPE%.old"

echo --- Reorganize structure for oF -----
PowerShell.exe -Command "Get-Content %CD%\%PROJECTNAME%.sln.old | ForEach-Object { $_ -replace '%VSOLD%', 'vs' } | Set-Content %CD%\%PROJECTNAME%.sln"
PowerShell.exe -Command "Get-Content %CD%\%PROJECTNAME%.%VSPROJTYPE%.old | ForEach-Object { $_ -replace '%VSOLD%', 'vs' } | Set-Content %CD%\%PROJECTNAME%.%VSPROJTYPE%"

echo --- Upgrade Visual Studio project ---
if not defined VCINSTALLDIR (
    call "%VS110COMNTOOLS%vsvars32.bat"
)
devenv "%PROJECTNAME%.sln" /upgrade

echo.
echo --- Clean up temp files -------------
move %CD%\%PROJECTNAME%.sln.old %CD%\Backup\
move %CD%\%PROJECTNAME%.vcxproj.old %CD%\Backup\
move %CD%\UpgradeLog.htm %CD%\Backup\
move %CD%\UpgradeLog.XML %CD%\Backup\
move %CD%\_UpgradeReport_Files %CD%\Backup\

echo.
echo ++++ Migration done +++++
pause