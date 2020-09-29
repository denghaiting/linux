
::@echo off

set target=stm32f030_iap
set image_name=STM32F030C8_IAP_16K
set usart=U1_PB6_PB7
::set usart=U1_PA9_PA10
::set usart=U2_PA2_PA3
set Baud=921600
set hold=HOLD_P62_P63

cd ..\Output

@if exist .\version.h del version.h
@copy ..\SourceCode\App\version.h .\version.h  /B/Y
@for /f  %%i in ('read_version.exe version.h APP_VERSION_MAJOR APP_VERSION_MINOR APP_VERSION_REVISION') do (set version=%%i)



::删除旧的bin文件
@if exist ./%image_name%_%usart%_B%Baud%_%hold%_V%version%.bin del %image_name%_%usart%_B%Baud%_%hold%_V%version%.bin

copy %target%.bin %image_name%_%usart%_B%Baud%_%hold%_V%version%.bin

@del %target%.bin
@del version.h