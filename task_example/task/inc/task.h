#ifndef __TASK_H
#define __TASK_H
#include "stdint.h"
#include "ti_msp_dl_config.h"

#include "LED.h"
#include "KEY.h"
#include "TB6612.h"


typedef struct 
{
    void (*function)(void *arg); // 【关键修改】加上 (void *arg)
    void *task_arg;              // 【新增】用来保存参数指针
    uint32_t delay;              // 任务延时时间
    uint32_t period;             // 任务执行周期
    volatile uint32_t run_count; // 任务运行计数器
    uint8_t run_flag;            // 【新增】是否启动这个任务
    uint8_t priority;            // 任务优先级
} Task_Ctrl;

extern Task_Ctrl Task_Ctrl_List[];

typedef struct {
    uint8_t  led_flag;  //  
} SystemCtrl_t;
void Task_Update(void);
void Task_Dispatch(void);
void TIMER0_INIT(void);
#endif