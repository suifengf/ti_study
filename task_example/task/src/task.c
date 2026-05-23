#include "task.h"
#define TASK_LIST_NUM_MAX (sizeof(Task_Ctrl_List)/sizeof(Task_Ctrl_List[0]))
#include "KEY.h"
#define PRIORITY_HIGH 1  //高优先级
#define PRIORITY_LOW  0  //低优先级
#define ON  1
#define OFF 0
/**
*@brief 任务控制结构体数组
*用于存储系统中所有任务的控制信息
*每个元素包含一个任务的控制参数和状态信息
*@details 组成元素为 任务函数，任务延时时间ms，任务执行周期ms，任务运行计数器（防止卡死累计任务，导致漏执行），任务优先级（优先级高在中断中执行）
*@example {function,NULL,0,200,0,ON,PRIORITY_HIGH} 函数function，需要传入这个函数的参数，每次0ms延时，200ms为周期执行一次，累计需要运行次数为0（初始化），开始时启用，任务优先级为高
*/

// 初始化任务列表
Task_Ctrl Task_Ctrl_List[] = {
    // 函数名,      参数指针,          delay, period, run_count,run_flag, priority
    {Led_Task,     NULL,              0,     200,    0,         OFF,      PRIORITY_LOW},
    {Key_Proc,     (void *)&key1,     0,     1,      0,         ON,      PRIORITY_HIGH},
    {AO_Control,   (void *)&motor_a_args, 0, 10,     0,         ON,      PRIORITY_LOW},
    {BO_Control,   (void *)&motor_b_args, 0, 10,     0,         ON,      PRIORITY_LOW},
};

/**
 *@brief 任务更新函数
 *@details 遍历任务列表，更新每个任务的状态和延时计数器
 */

void Task_Update(void)
{
    uint8_t task_num = 0;
    for(task_num = 0; task_num < TASK_LIST_NUM_MAX; task_num++)
    {
        //如果任务被暂停 (run_flag == OFF)，直接跳过，不管它
        if(Task_Ctrl_List[task_num].run_flag == ON)
        {
            if(Task_Ctrl_List[task_num].delay == 0)
            {
                if(Task_Ctrl_List[task_num].priority > 0) {
                    (*Task_Ctrl_List[task_num].function)(Task_Ctrl_List[task_num].task_arg);
                } else {
                    Task_Ctrl_List[task_num].run_count++;
                }
                Task_Ctrl_List[task_num].delay = Task_Ctrl_List[task_num].period - 1;
            }
            else 
            {
                Task_Ctrl_List[task_num].delay -= 1;
            }
        }
    }
}

/**
 * @brief 任务调度函数
 * @details 遍历任务列表，按任务列表的顺序完成任务
 */
void Task_Dispatch(void)
{
    uint8_t task_num = 0;
    for(task_num = 0; task_num < TASK_LIST_NUM_MAX; task_num++)
    {
        if(Task_Ctrl_List[task_num].run_count > 0)
        {
            (*Task_Ctrl_List[task_num].function)(Task_Ctrl_List[task_num].task_arg);
            Task_Ctrl_List[task_num].run_count -= 1;
        }
    }
}
/**
 * @brief 定时器TIMG0中断初始化
 * 
 */
void TIMER0_INIT(void)
{
    //清除定时器中断标志
    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
    //使能定时器中断
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
}

void TIMER_0_INST_IRQHandler(void)
{
    //如果产生了定时器中断
    switch( DL_TimerG_getPendingInterrupt(TIMER_0_INST))
    {
        case DL_TIMER_IIDX_ZERO://如果是0溢出中断
            {
                Task_Update();
                key.delay++;
                break;
            }
            
        default://其他的定时器中断
            break;
    }
}