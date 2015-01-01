:: set path to yours
set NDK_ROOT=e:/android/android-ndk/android-ndk-r8d
set PROJ=e:/work/myproj/nie3d/build/android

set CleanBinBeforeBuild=1
set Debug=0

cd /d %PROJ%

if %CleanBinBeforeBuild%==1 (
	goto CLEAN_BIN
) else (
	goto BUILD_PROJECT
)

: CLEAN_BIN
@echo CLEAN_BIN
@echo clean bin folder
rmdir /s /q  ..\..\bin\android\armeabi
rmdir /s /q  ..\..\bin\android\armeabi-v7a

: BUILD_PROJECT

@echo -----------------------------------------
@echo # build core
@echo -----------------------------------------
cd ../../src/core/proj.android/jni
call %NDK_ROOT%/ndk-build NDK_NO_WARNINGS=1 NDK_DEBUG=%Debug% NDK_LOG=1 > ndk-log.txt
call :COYPBIN CORE
: CORE

@echo -----------------------------------------
@echo # build oglrenderer
@echo -----------------------------------------
cd ../../src/oglrenderer/proj.android/jni
call %NDK_ROOT%/ndk-build NDK_NO_WARNINGS=1 NDK_DEBUG=%Debug% NDK_LOG=1 > ndk-log.txt
call :COYPBIN OGLRENDERER
: OGLRENDERER

@echo -----------------------------------------
@echo # build engine
@echo -----------------------------------------
cd ../../src/engine/proj.android/jni
call %NDK_ROOT%/ndk-build NDK_NO_WARNINGS=1 NDK_DEBUG=%Debug% NDK_LOG=1 > ndk-log.txt
call :COYPBIN ENGINE
: ENGINE

@echo -----------------------------------------
@echo # build main
@echo -----------------------------------------
cd ../../src/main/proj.android/jni
call %NDK_ROOT%/ndk-build NDK_NO_WARNINGS=1 NDK_DEBUG=%Debug% NDK_LOG=1 > ndk-log.txt
call :COYPBIN MAIN
: MAIN

goto END

: COYPBIN
echo xcopy ..\libs\armeabi\*.so    			..\..\..\..\bin\android\armeabi\ /s/y
echo xcopy ..\libs\armeabi-v7a\*.so    		..\..\..\..\bin\android\armeabi-v7a\ /s/y
echo xcopy ..\obj\local\armeabi\*.a    		..\..\..\..\bin\android\armeabi\ /s/y
echo xcopy ..\obj\local\armeabi-v7a\*.a    	..\..\..\..\bin\android\armeabi-v7a\ /s/y
xcopy ..\libs\armeabi\*.so    				..\..\..\..\bin\android\armeabi\ /s/y
xcopy ..\libs\armeabi-v7a\*.so    			..\..\..\..\bin\android\armeabi-v7a\ /s/y
xcopy ..\obj\local\armeabi\*.a    			..\..\..\..\bin\android\armeabi\ /s/y
xcopy ..\obj\local\armeabi-v7a\*.a   		..\..\..\..\bin\android\armeabi-v7a\  /s/y
cd /d %PROJ%
goto :%1

:END
