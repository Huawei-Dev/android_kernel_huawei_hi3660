#=============================================================================
#=============================================================================

obj-$(CONFIG_HW_ANTENNA_BOARD_MATCH)  += antenna_board_match/
obj-$(CONFIG_ANTENNA_FPGA) += antenna_fpga/
obj-$(CONFIG_HUAWEI_AUDIO) += audio/
obj-$(CONFIG_CHR_NETLINK_MODULE)   += chr/
obj-y   += connectivity/
obj-$(CONFIG_HUAWEI_HW_DEV_DCT)  += devdetect/

ifneq ($(strip $(OBB_SEPARATE)),true)
obj-$(CONFIG_VIACBP82D_3RD_MODEM) += ext_modem/
endif
obj-y   += inputhub/

obj-$(CONFIG_MAXQ616_POWER_SWITCH) += irda/
obj-$(CONFIG_NFC_DEVICES)   += nfc/
obj-$(CONFIG_HW_WIFI)   += net/
obj-y   += power/
obj-$(CONFIG_HUAWEI_DSM) += recovery/
obj-$(CONFIG_HUAWEI_SENSORS_INPUT_INFO)   += sensor/
obj-y   += storage/
obj-$(CONFIG_HUAWEI_TS)   += touthscreen/
obj-y   += touch_key/
obj-y   += usb/
obj-$(CONFIG_HW_VOLTAGE_DETECTION) += voltage_detection/
obj-$(CONFIG_ANTENNA_BOARDID_GPIO_DETECT) += antenna_board_gpio_match/
obj-$(CONFIG_HUAWEI_VIBRATOR) += vibrator/
obj-$(CONFIG_MADAPT) += madapt/
obj-$(CONFIG_HUAWEI_IO_TRACING) += iotrace/
obj-$(CONFIG_HW_LED_ALWAYSON) += led_alwayson/
obj-$(CONFIG_HUAWEI_EMCOM) += emcom/
obj-$(CONFIG_DP_AUX_SWITCH) += dp_aux_switch/
obj-$(CONFIG_HUAWEI_VENDOR_EXCEPTION) += vendor_exception/
obj-$(CONFIG_HWAA) += hwaa/
