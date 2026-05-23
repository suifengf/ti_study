#include "task.h"
#define TASK_LIST_NUM_MAX (sizeof(Task_Ctrl_List)/sizeof(Task_Ctrl_List[0]))

#define PRIORITY_HIGH 1  //高优先级
#define PRIORITY_LOW  0  //低优先级

/**
*@brief 任务控制结构体数组
*用于存储系统中所有任务的控制信息
*每个元素包含一个任务的控制参数和状态信息
*@details 组成元素为 任务函数，任务延时时间ms，任务执行周期ms，任务运行计数器（防止卡死累计任务，导致漏执行），任务优先级（优先级高在中断中执行）
*@example {function,0,200,0,PRIORITY_HIGH} 函数function，每次0ms延时，200ms为周期执行一次，累计需要运行次数为0（初始化），任务优先级为高
*/
Task_Ctrl Task_Ctrl_List[] = 
{
    {Led_Task,0,200,0,PRIORITY_HIGH},
};

/**
 *@brief 任务更新函数
 *@details 遍历任务列表，更新每个任务的状态和延时计数器
 */
void Task_Update(void)
{
    uint8_t task_num = 0;//初始化任务数量
    for(task_num = 0; task_num < TASK_LIST_NUM_MAX; task_num++)
    {
        if(Task_Ctrl_List[task_num].delay == 0)
        {
            //如果是高优先级任务，在中断中执行
            if(Task_Ctrl_List[task_num].priority > 0)
            {
                (*Task_Ctrl_List[task_num].function)();
            }
            else//低优先级任务，在主循环中执行
            {
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

//任务一：小灯闪烁
void Led_Task(void)
{
    DL_GPIO_togglePins(GPIO_LED_PORT,GPIO_LED_USER_LED_1_PIN);
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
        if(Task_Ctrl_List[task_num].run_count > 0)//如果有任务要完整，则执行，如果有堆积，则会一直执行，直到将堆积的任务完成
        {
            (*Task_Ctrl_List[task_num].function)();
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
                break;
            }
            
        default://其他的定时器中断
            break;
    }
}