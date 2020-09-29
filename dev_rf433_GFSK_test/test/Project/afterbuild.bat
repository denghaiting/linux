
::@echo off

::set bin_path=D:\Keil5-arm\ARM\ARMCC\bin
set bin_path=D:\Keil_v5\ARM\ARMCC\bin

set target=stm32f030c8
set image_name=rf_pre_study

::boot变化时 更改此处::
set boot_name=STM32F030C8_IAP_16K_U1_PB6_PB7_B921600_HOLD_P62_P63_V1.0.2
::set boot_name=STM32F030C8_IAP_16K_U1_PA9_PA10_B921600_HOLD_P62_P63_V1.0.2
::set boot_name=STM32F030C8_IAP_16K_U1_PA9_PA10_B115200_HOLD_P62_P63_V1.0.2
::set boot_name=STM32F030C8_IAP_16K_U2_PA2_PA3_B921600_HOLD_P62_P63_V1.0.2
::set boot_name=STM32F030C8_IAP_16K_U2_PA2_PA3_B115200_HOLD_P62_P63_V1.0.2

cd Output

@if exist .\version.h del version.h
@copy ..\..\SourceCode\App\version.h .\version.h  /B/Y
@for /f  %%i in ('rf_read_version.exe version.h APP_VERSION_MAJOR APP_VERSION_MINOR APP_VERSION_REVISION') do (set app_version=%%i)


@copy ..\boot_image\%boot_name%.bin .\boot.bin  /B/Y

::删除旧的bin文件
@if exist ./%image_name%-%target%_v%app_version%.bin del %image_name%-%target%_v%app_version%.bin
@if exist ./%image_name%-%target%_v%app_version%_flash.bin del %image_name%-%target%_v%app_version%_flash.bin

%bin_path%\fromelf.exe --bin -o %target%.bin ./Objects/%target%.axf


copy %target%.bin %target%_tail.bin

FixBin tail %target%_tail.bin

FixBin.exe copy %image_name%-%target%_v%app_version%_flash.bin  boot.bin 0 %target%_tail.bin 16384

copy %target%_tail.bin %image_name%-%target%_v%app_version%.bin


@del %target%_tail.bin %target%.bin
@del boot.bin
@del version.h