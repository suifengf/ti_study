#include "TB6612.h"

// 定义电机的参数变量 (方向 1/0, 速度 0~100)
Motor_Control_Args_t motor_a_args = {1, 500}; 
Motor_Control_Args_t motor_b_args = {1, 500}; 

/******************************************************************
 * 函 数 名 称：TB6612_Motor_Stop
 * 函 数 说 明：A端和B端电机停止
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：无
******************************************************************/
void TB6612_Motor_Stop(void)
{
    AIN1_OUT(1);
    AIN2_OUT(1);
    BIN1_OUT(1);
    BIN2_OUT(1);
}

/******************************************************************
 * 函 数 名 称：AO_Control
 * 函 数 说 明：A端口电机控制 (任务框架适用版)
 * 函 数 形 参：arg (指向 Motor_Control_Args_t 结构体的指针)
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：speed 0-1000
******************************************************************/
void AO_Control(void *arg)
{
    // 将 void 指针强转为我们定义的参数结构体指针
    Motor_Control_Args_t *motor_args = (Motor_Control_Args_t *)arg;
    uint8_t dir = motor_args->dir;
    uint32_t speed = motor_args->speed;

    if(speed > 1000 || dir > 1)
    {
        // lc_printf("\nAO_Control parameter error!!!\r\n"); // 移除未声明的lc_printf
        return;
    }

    if( dir == 1 )
    {
        AIN1_OUT(0);
        AIN2_OUT(1);
    }
    else
    {
        AIN1_OUT(1);
        AIN2_OUT(0);
    }

    DL_TimerG_setCaptureCompareValue(PWM_0_INST, 1000-speed, DL_TIMER_CC_0_INDEX);
}



/******************************************************************
 * 函 数 名 称：BO_Control
 * 函 数 说 明：B端口电机控制 (任务框架适用版)
 * 函 数 形 参：arg (指向 Motor_Control_Args_t 结构体的指针)
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：speed 0-100
******************************************************************/
void BO_Control(void *arg)
{
    Motor_Control_Args_t *motor_args = (Motor_Control_Args_t *)arg;
    uint8_t dir = motor_args->dir;
    uint32_t speed = motor_args->speed;

    if(speed > 1000 || dir > 1)
    {
        // lc_printf("\nBO_Control parameter error!!!\r\n");
        return;
    }

    if( dir == 1 )
    {
        BIN1_OUT(0);
        BIN2_OUT(1);
    }
    else
    {
        BIN1_OUT(1);
        BIN2_OUT(0);
    }

    DL_TimerG_setCaptureCompareValue(PWM_0_INST, 1000-speed, DL_TIMER_CC_1_INDEX);
}