#ifndef TIMER_H
#define TIMER_H

// More info about the hardware timer can be found in the embedded ip guide pages 28-6 and 28-7

#define CLOCK_FREQ 50000000

void timer_test(void);
void setHardwareTimerPeriod(int period);
int isHardwareTimerRunning(void);
int hasHardwareTimerExpired(void);
void startHardwareTimer(void);

#endif
