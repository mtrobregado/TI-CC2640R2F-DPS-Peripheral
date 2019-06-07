################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-396906753:
	@$(MAKE) --no-print-directory -Onone -f TOOLS/subdir_rules.mk build-396906753-inproc

build-396906753-inproc: ../TOOLS/app_ble.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_51_01_18_core/xs" --xdcpath="C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source;C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/kernel/tirtos/packages;C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.simplelink:CC2640R2F -r release -c "C:/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS" --compileOptions "-mv7M3 --code_state=16 -me -Ooff --opt_for_speed=0 --include_path=\"C:/ti/ccsv9workspace/CC2640R2F_LAUNCPADV310/dps_peripheral_cc2640r2lp_app\" --include_path=\"C:/ti/ccsv9workspace/CC2640R2F_LAUNCPADV310/dps_peripheral_cc2640r2lp_app/Application\" --include_path=\"C:/ti/ccsv9workspace/CC2640R2F_LAUNCPADV310/dps_peripheral_cc2640r2lp_app/Startup\" --include_path=\"C:/ti/ccsv9workspace/CC2640R2F_LAUNCPADV310/dps_peripheral_cc2640r2lp_app/PROFILES\" --include_path=\"C:/ti/ccsv9workspace/CC2640R2F_LAUNCPADV310/dps_peripheral_cc2640r2lp_app/Include\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/controller/cc26xx_r2/inc\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/inc\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/rom\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/common/cc26xx\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/icall/inc\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/profiles/dev_info\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/profiles/roles/cc26xx\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/profiles/roles\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/profiles/simple_profile/cc26xx\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/profiles/simple_profile\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/target\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/hal/src/target/_common\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/hal/src/target/_common/cc26xx\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/hal/src/inc\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/heapmgr\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/icall/src/inc\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/osal/src/inc\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/services/src/saddr\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/blestack/services/src/sdata\" --include_path=\"C:/ti/simplelink_cc2640r2_sdk_2_40_00_32/source/ti/devices/cc26x0r2\" --include_path=\"C:/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/include\" --define=DeviceFamily_CC26X0R2 --define=BOARD_DISPLAY_USE_LCD=0 --define=BOARD_DISPLAY_USE_UART=1 --define=BOARD_DISPLAY_USE_UART_ANSI=1 --define=CC2640R2_LAUNCHXL --define=CC26XX --define=CC26XX_R2 --define=ICALL_EVENTS --define=ICALL_JT --define=ICALL_LITE --define=ICALL_MAX_NUM_ENTITIES=6 --define=ICALL_MAX_NUM_TASKS=3 --define=ICALL_STACK0_ADDR --define=POWER_SAVING --define=STACK_LIBRARY --define=USE_ICALL --define=xdc_runtime_Assert_DISABLE_ALL --define=xdc_runtime_Log_DISABLE_ALL -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-396906753 ../TOOLS/app_ble.cfg
configPkg/compiler.opt: build-396906753
configPkg/: build-396906753


