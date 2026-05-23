#ifndef __KEY_H
#define __KEY_H
#include "ti_msp_dl_config.h"
#include <stdint.h>
#include "task.h"
#define KEY_TIM_LONG 1000
#define KEY_TIM_DOUBLE 500
#define KEY_TIM_PRESSING 300


#define KEY_STATE_DOWN 1
#define KEY_STATE_PRESSED 2
#define KEY_STATE_LONG 3
#define KEY_STATE_PERESSING 4
#define KEY_STATE_DOUBLE 5


typedef struct
{
	uint8_t up;
	uint8_t down;
	uint8_t val;
	uint8_t state;
	uint8_t old;
	uint32_t delay;
	uint32_t now;
}Key_t;
extern Key_t key;
extern uint8_t key1;
void Key_Proc (void *arg);
#endif