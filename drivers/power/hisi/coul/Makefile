obj-$(CONFIG_HISI_COUL_HI6421V500)	+= hi6421v500/

EXTRA_CFLAGS += -Idrivers/hisi/tzdriver/libhwsecurec/
ifeq ($(wildcard $(srctree)/drivers/huawei_platform/inputhub/$(TARGET_BOARD_PLATFORM)),)
ccflags-y  += -I$(srctree)/drivers/huawei_platform/inputhub/default/
else
ccflags-y  += -I$(srctree)/drivers/huawei_platform/inputhub/$(TARGET_BOARD_PLATFORM)/
endif

ifneq ($(TARGET_BUILD_VARIANT),user)
ccflags-y += -DISC_TEST
else
ifeq ($(TARGET_VERSION_MODE),factory)
ccflags-y += -DISC_TEST
endif
endif

obj-$(CONFIG_HISI_COUL)             += hisi_coul_core.o hisi_coul_drv.o
obj-$(CONFIG_HISI_COUL)             += hisi_coul_event.o hisi_coul_imonitor.o
obj-$(CONFIG_HISI_COUL_TEST)        += hisi_coul_drv_test.o

