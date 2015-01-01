:: set path to yours
set NDK_ROOT=e:/android/android-ndk/android-ndk-r8d

cd /d %~dp0

: BUILD_PROJECT

@echo -----------------------------------------
@echo # build
@echo -----------------------------------------
call %NDK_ROOT%/ndk-build NDK_NO_WARNINGS=1 NDK_LOG=1 > ndk-log.txt
call :COYPBIN MAIN
: MAIN

goto END

: COYPBIN
echo xcopy ..\libs\armeabi\*.so    			..\lib\armeabi\ /s/y
echo xcopy ..\libs\armeabi-v7a\*.so    		..\lib\armeabi-v7a\ /s/y
echo xcopy ..\obj\local\armeabi\*.a    		..\lib\armeabi\ /s/y
echo xcopy ..\obj\local\armeabi-v7a\*.a    	..\lib\armeabi-v7a\ /s/y
xcopy ..\libs\armeabi\*.so    				..\lib\armeabi\ /s/y
xcopy ..\libs\armeabi-v7a\*.so    			..\lib\armeabi-v7a\ /s/y
xcopy ..\obj\local\armeabi\*.a    			..\lib\armeabi\ /s/y
xcopy ..\obj\local\armeabi-v7a\*.a   		..\lib\armeabi-v7a\  /s/y
goto :%1

:END

