# hisilicon ivp
ccflags-$(CONFIG_IVP_IVPV150) += -Idrivers/hisi/ivp/ivpv150

obj-$(CONFIG_IVP_SMMU)      += ivp_smmu.o
obj-$(CONFIG_HISI_IVP) += ivp_ipc.o
obj-$(CONFIG_HISI_IVP) += ivp_core.o
obj-$(CONFIG_IVP_IVPV150) += ivpv150/

ifneq (, $(filter eng, $(TARGET_BUILD_VARIANT)))
	EXTRA_CFLAGS += -DENG_BUILDMODE
endif

subdir-ccflags-y += -I$(srctree)/drivers/hisi/tzdriver \
                    -I$(srctree)/drivers/hisi/load_image
