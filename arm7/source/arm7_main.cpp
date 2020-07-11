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
}

#define WIFI

#ifdef WIFI
#include <dswifi7.h>
#endif

#define abs(x)	((x)>=0?(x):-(x))

NTXM7 *ntxm7 = 0;

static volatile bool exitflag = false;
extern bool ntxm_recording;

int vcount;
touchPosition first,tempPos;

//---------------------------------------------------------------------------------
void VcountHandler() {
//---------------------------------------------------------------------------------
	if(ntxm_recording == true)
		return;

	inputGetAndSend();
}


void VblankHandler(void)
{
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

void powerButtonHandler(void) {
	exitflag = true;
}

//---------------------------------------------------------------------------------
int main(int argc, char ** argv) {
//---------------------------------------------------------------------------------
    readUserSettings(); // read User Settings from firmware
    rtcReset(); // Reset the clock if needed
    irqInit();
    fifoInit();
	touchInit();
#ifdef WIFI
    installWifiFIFO();
#endif
    installSystemFIFO();

	irqEnable(IRQ_VBLANK);
	irqSet(IRQ_VBLANK, VblankHandler);

	SetYtrigger(80);
	irqSet(IRQ_VCOUNT, VcountHandler);
	irqEnable(IRQ_VCOUNT);

#ifdef WIFI
	irqEnable(IRQ_NETWORK);
#endif

	enableSound();

	// Create ntxm player
	ntxm7 = new NTXM7(ntxmTimerHandler);

	// Initialize NTXM update timer
	irqSet(IRQ_TIMER0, ntxmTimerHandler);
	irqEnable(IRQ_TIMER0);

	setPowerButtonCB(powerButtonHandler);

	// Keep the ARM7 out of main RAM
	while (!exitflag) {
		swiWaitForVBlank();
	}

	return 0;
}
