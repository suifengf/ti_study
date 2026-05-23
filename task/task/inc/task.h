#ifndef __TASK_H
#define __TASK_H
#include "stdint.h"
#include "ti_msp_dl_config.h"
typedef struct 
{
    void (*function)(void);
    uint32_t delay;//任务延时时间，单位为ms
    uint32_t period;//任务执行周期，单位为ms
    uint32_t run_count;//任务运行计数器，记录任务需要执行的次数
    uint8_t priority;//任务优先级
}Task_Ctrl;
void Led_Task(void);
void Task_Update(void);
void Task_Dispatch(void);
void TIMER0_INIT(void);
#endif