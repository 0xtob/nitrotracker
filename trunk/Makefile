#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM)
endif

include $(DEVKITARM)/ds_rules

export TARGET		:=	$(shell basename $(CURDIR))
export TOPDIR		:=	$(CURDIR)

LIBNTXM     := ~/coding/dsdev/tob/libntxm/libntxm/libntxm
LIBDSMI     := ~/coding/dsdev/tob/dsmi/dsmi/ds/libdsmi

#---------------------------------------------------------------------------------
# path to tools - this can be deleted if you set the path in windows
#---------------------------------------------------------------------------------
export PATH		:=	$(DEVKITARM)/bin:$(PATH)

.PHONY: libdsmi libntxm tobkit $(TARGET).arm7 $(TARGET).arm9

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
all: $(TARGET).ds.gba $(TARGET).gba.nds

libntxm:
	@make -C $(LIBNTXM)

tobkit:
	@make -C tobkit

libdsmi:
	@make -C $(LIBDSMI)

debug: $(TARGET).dbg.nds

cp: all
	dlditool ~/coding/dsdev/tools/dldi/mpcf.dldi $(TARGET).nds
	cp $(TARGET).nds /media/GBAMP/NitroTracker.nds
	pumount /media/GBAMP

r4: all
	dlditool ~/coding/dsdev/tools/dldi/r4tf.dldi $(TARGET).nds
	cp $(TARGET).nds /media/R4
	pumount /media/R4

/media/DSX:
	pmount /dev/sda /media/DSX

dsx: all /media/DSX
	dlditool ~/coding/dsdev/tools/dldi/dsx.dldi $(TARGET).nds
	cp $(TARGET).nds /media/DSX
	pumount /media/DSX

scl: all
	dlditool ~/coding/dsdev/tools/dldi/sclt.dldi $(TARGET).nds
	cp $(TARGET).nds /media/MICROSD
	pumount /media/MICROSD

fcsr:
	~/coding/dsdev/tools/fcsr/build.sh nt.img wav
	padbin 512 $(TARGET).ds.gba
	cat $(TARGET).ds.gba nt.img > $(TARGET)_fcsr.ds.gba
	dlditool ~/coding/dsdev/tools/dldi/fcsr.dldi $(TARGET)_fcsr.ds.gba

$(TARGET).ds.gba	: $(TARGET).nds
	

$(TARGET).gba.nds: $(TARGET).nds
	cat ndsloader.bin $(TARGET).nds > $(TARGET).gba.nds

#---------------------------------------------------------------------------------
$(TARGET).nds	:	libdsmi libntxm tobkit $(TARGET).arm7 $(TARGET).arm9
	ndstool -c $(TARGET).nds -7 $(TARGET).arm7 -9 $(TARGET).arm9 -b icon.bmp "NitroTracker"

#---------------------------------------------------------------------------------
$(TARGET).arm7	: arm7/$(TARGET).elf
	
$(TARGET).arm9	: arm9/$(TARGET).elf
	
#---------------------------------------------------------------------------------
arm7/$(TARGET).elf:
	$(MAKE) -C arm7
	
#---------------------------------------------------------------------------------
arm9/$(TARGET).elf:
	$(MAKE) -C arm9

clean:
	$(MAKE) -C arm9 clean
	$(MAKE) -C arm7 clean
	rm -f $(TARGET).ds.gba $(TARGET).nds $(TARGET).arm7 $(TARGET).arm9
