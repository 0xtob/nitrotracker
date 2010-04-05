/*
 * NitroTracker - An FT2-style tracker for the Nintendo DS
 *
 *                                by Tobias Weyand (0xtob)
 *
 * http://nitrotracker.tobw.net
 * http://code.google.com/p/nitrotracker
 */

/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <nds.h>

#include <ntxm/ntxm7.h>

extern "C" {
  #include "ntxm/linear_freq_table.h"
  #include "ntxm/tobmic.h"
}

#define WIFI

#ifdef WIFI
#include <dswifi7.h>
#endif

#define LID_BIT BIT(7)

#define abs(x)	((x)>=0?(x):-(x))

NTXM7 *ntxm7 = 0;

bool lidwasdown = false;
extern bool ntxm_recording;

int vcount;
touchPosition first,tempPos;

// Thanks to LiraNuna for this cool function
void PM_SetRegister(int reg, int control)
{
	SerialWaitBusy();
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER |SPI_BAUD_1MHz | SPI_CONTINUOUS;
	REG_SPIDATA = reg;
	SerialWaitBusy();
	REG_SPICNT = SPI_ENABLE | SPI_DEVICE_POWER |SPI_BAUD_1MHz;
	REG_SPIDATA = control;
}


//---------------------------------------------------------------------------------
void VcountHandler() {
//---------------------------------------------------------------------------------
	if(ntxm_recording == true)
		return;

	inputGetAndSend();
}


void VblankHandler(void)
{
	if((!lidwasdown)&&(REG_KEYXY & LID_BIT))
	{
		PM_SetRegister(0, 0x30);
		lidwasdown = true;
	}

	if((lidwasdown)&&(!(REG_KEYXY & LID_BIT)))
	{
		PM_SetRegister(0, 0x0D);
		lidwasdown = false;
	}

#ifdef WIFI
	if(ntxm_recording == false)
		Wifi_Update(); // update wireless in vblank
#endif
}

// This is the callback for the timer
void ntxmTimerHandler(void) {
	ntxm7->timerHandler();
}

//---------------------------------------------------------------------------------
void enableSound() {
//---------------------------------------------------------------------------------
    powerOn(POWER_SOUND);
    writePowerManagement(PM_CONTROL_REG, ( readPowerManagement(PM_CONTROL_REG) & ~PM_SOUND_MUTE ) | PM_SOUND_AMP );
    REG_SOUNDCNT = SOUND_ENABLE;
    REG_MASTER_VOLUME = 127;
}

//---------------------------------------------------------------------------------
int main(int argc, char ** argv) {
//---------------------------------------------------------------------------------
    readUserSettings(); // read User Settings from firmware
    rtcReset(); // Reset the clock if needed
    irqInit();
    fifoInit();
#ifdef WIFI
    installWifiFIFO();
#endif
    installSystemFIFO();

	irqEnable(IRQ_VBLANK);
	irqSet(IRQ_VBLANK, VblankHandler);

	SetYtrigger(80);
	irqSet(IRQ_VCOUNT, VcountHandler);
	irqEnable(IRQ_VCOUNT);

	irqSet(IRQ_TIMER1, tob_ProcessMicrophoneTimerIRQ);
	irqEnable(IRQ_TIMER1);

#ifdef WIFI
	irqEnable(IRQ_NETWORK);
#endif

	enableSound();

	// Create ntxm player
	ntxm7 = new NTXM7(ntxmTimerHandler);

	// Initialize NTXM update timer
	irqSet(IRQ_TIMER0, ntxmTimerHandler);
	irqEnable(IRQ_TIMER0);

	// Keep the ARM7 out of main RAM
	while (1) {
		swiWaitForVBlank();
	}
}
