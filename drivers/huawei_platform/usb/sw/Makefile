#
# Makefile for single wire devices
#

sw-objs :=  sw_device.o \
            sw_core.o \
            sw_hid_vendor.o \
            sw_hid.o \
            sw_debug.o \
            sw_utils.o \
            sw_detect.o \
            sw_detect_kb.o \
            sw_detect_chg.o

sw-objs +=  keyboard/keyboard_cmr.o \
            keyboard/keyboard_scm.o

obj-$(CONFIG_HW_SW_DEVICES) := sw.o
