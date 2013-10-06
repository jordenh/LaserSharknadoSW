#ifndef INPUT_H_
#define INPUT_H_

#include "bullet.h"
#include "io.h"
#include "system.h"
#include "score.h"
#include "altera_nios2_qsys_irq.h"
#include "sys/alt_irq.h"

#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#include "audio.h"
#include "timer.h"
#include "sd_card.h"
#include "vga.h"
#include "bmp.h"
#include "shark.h"
#include "displacement.h"
#include "collision.h"
#include "io.h"
#include "system.h"
#include "altera_nios2_qsys_irq.h"
#include "sys/alt_irq.h"
#include "score.h"
#include "input.h"

void handleKeyInput(void);

void handleSwitchInput(void);

void handleAtariInput(void);



#endif /* INPUT_H_ */
