EXTRA_CFLAGS += -Idrivers/hwspinlock

obj-$(CONFIG_HUAWEI_THP) += thp.o
thp-objs += huawei_thp_attr.o huawei_thp_core.o huawei_thp_mt_wrapper.o
obj-$(CONFIG_JDI_HOST_TS_KIT) += bu21150_thp.o
EXTRA_CFLAGS += -Idrivers/hisi/tzdriver
EXTRA_CFLAGS += -Idrivers/tzdriver
EXTRA_CFLAGS += -Idrivers/devkit/lcdkit/lcdkit1.0/core/common
ifeq ($(wildcard $(srctree)/drivers/huawei_platform/inputhub/$(TARGET_BOARD_PLATFORM)),)
EXTRA_CFLAGS += -Idrivers/huawei_platform/inputhub/default
else
EXTRA_CFLAGS += -Idrivers/huawei_platform/inputhub/$(TARGET_BOARD_PLATFORM)
endif
