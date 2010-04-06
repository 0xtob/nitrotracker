#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

ifeq ($(strip $(LIBDSMI)),)
$(error "Please set LIBDSMI in your environment. export LIBDSMI=<path to>libdsmi")
endif

ifeq ($(strip $(LIBNTXM)),)
$(error "Please set LIBNTXM in your environment. export LIBNTXM=<path to>libntxm")
endif

# Custom targets for copying stuff to the DS
-include mytargets.mk

export TARGET		:=	$(shell basename $(CURDIR))
export TOPDIR		:=	$(CURDIR)

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
