#ifndef __TB6612_H
#define __TB6612_H
#include "ti_msp_dl_config.h"

#define AIN1_OUT(X)  ( (X) ? (DL_GPIO_setPins(GPIO_TB6612_AIN1_PORT,GPIO_TB6612_AIN1_PIN)) : (DL_GPIO_clearPins(GPIO_TB6612_AIN1_PORT,GPIO_TB6612_AIN1_PIN)) )
#define AIN2_OUT(X)  ( (X) ? (DL_GPIO_setPins(GPIO_TB6612_AIN2_PORT,GPIO_TB6612_AIN2_PIN)) : (DL_GPIO_clearPins(GPIO_TB6612_AIN2_PORT,GPIO_TB6612_AIN2_PIN)) )

#define BIN1_OUT(X)  ( (X) ? (DL_GPIO_setPins(GPIO_TB6612_BIN1_PORT,GPIO_TB6612_BIN1_PIN)) : (DL_GPIO_clearPins(GPIO_TB6612_BIN1_PORT,GPIO_TB6612_BIN1_PIN)) )
#define BIN2_OUT(X)  ( (X) ? (DL_GPIO_setPins(GPIO_TB6612_BIN2_PORT,GPIO_TB6612_BIN2_PIN)) : (DL_GPIO_clearPins(GPIO_TB6612_BIN2_PORT,GPIO_TB6612_BIN2_PIN)) )

// 定义AO_Control和BO_Control需要使用的任务参数结构体
typedef struct {
    uint8_t dir;
    uint32_t speed;
} Motor_Control_Args_t;

extern Motor_Control_Args_t motor_a_args;
extern Motor_Control_Args_t motor_b_args;

void TB6612_Motor_Stop(void);
void AO_Control(void *arg);
void BO_Control(void *arg);

#endif