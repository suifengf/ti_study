#include "LED.h"

/**
 * @brief 翻转LED1状态
 * 
 */
void Led_Task(void *arg)
{
    (void)arg; // 告诉编译器我故意不用这个变量，防止报警告
    DL_GPIO_togglePins(GPIO_LED_PORT,GPIO_LED_USER_LED_1_PIN);
}