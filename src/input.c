
#include "input.h"
#include "bullet.h"
#include "io.h"
#include "system.h"
#include "altera_nios2_qsys_irq.h"
#include "sys/alt_irq.h"

#define switches (volatile char *) 0x1001060
#define leds (char *) 0x1001070
#define keys (volatile char *) 0x1001080
#define atariInput (volatile char *) 0x10010b0

void handleKeyInput(void){
	char keyInput;
	short int edgeDetect = 0;

	keyInput = IORD_8DIRECT(KEYS_BASE, 0);

	if ((keyInput & 0x01) && (edgeDetect == 0)) {
		edgeDetect = 1;
	} else if ((keyInput & 0x01) && (edgeDetect == 1)) {
		edgeDetect = 0;
		createBullet(PLAYERBULLET);
	}

}

void handleSwitchInput(void){
	char SWInput;
	short int edgeDetect = 0;

}

void handleAtariInput(void){
	char atariButtons;
	char atariUp;
	char atariDown;
	char atariFire;
	short int edgeDetect = 0;

	atariButtons = (IORD_8DIRECT(atariInput, 0) & 0x0F);
	atariFire = atariButtons & 0x08;
	atariUp = atariButtons & 0x02;
	atariDown = atariButtons & 0x04;
	IOWR_16DIRECT(leds, 0, atariButtons);

	if ((atariFire == 0x00) && (edgeDetect == 0)) {
		edgeDetect = 1;
	} else if ((atariFire != 0x00) && (edgeDetect == 1)) {
		edgeDetect = 0;
		createBullet(PLAYERBULLET);
	}

	//movements - no edge detect
	if (atariUp != 0x00) {
		moveUpPlayer();
	} else if (atariDown != 0x00) {
		moveDownPlayer();
	} else {
		drawPlayer();
	}
}







