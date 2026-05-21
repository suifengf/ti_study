#include "Key.h"
#include "ti_msp_dl_config.h"

Key_t key;
uint32_t falg;
uint8_t Key_Scan(uint8_t* State)
{
	key.val = 0;
	key.up = 0;
	key.down = 0;
	key.state = 0;
	
	if (DL_GPIO_readPins(GPIO_Key_PORT, GPIO_Key_PIN_Key_1_PIN) == 0)
	{
		key.val = 1;
	}
	
	key.down = key.val & (key.val ^key.old);
	key.up = ~key.val & (key.val ^ key.old);
	key.old = key.val;
	
	if (key.down)
	{
		key.state = KEY_STATE_DOWN;
		key.delay = 0;
	}
	
	else if (key.up)
	{
		key.val = key.up;
		if (key.delay < KEY_TIM_LONG)
		{
			key.state = KEY_STATE_PRESSED; // 小于1秒松手，正常触发短按
		}
		else
		{
			// 如果按的时间超过了长按阈值，松手时就不应该再触发任何状态了
			key.state = 0; 
		}
	}
	else // 在按住不放的期间
	{
		if (key.val)
		{
			if (key.delay > KEY_TIM_LONG)
			{
				// 只要按住时间一超过 1000ms，立刻、持续地触发长按状态
				key.state = KEY_STATE_LONG;
			}
			else if (key.delay > KEY_TIM_PRESSING)
			{
				// 在 300ms 到 1000ms 之间，触发“持续按压”状态（可选）
				key.state = KEY_STATE_PERESSING;
			}
		}
	}
	
	*State = key.state;
	return key.val;
}

void Key_Proc (void)
{
	uint8_t KeyState;
	switch (Key_Scan(&KeyState))
	{
		case 1:
		{
			if (KeyState == KEY_STATE_PRESSED)
			{
				DL_GPIO_togglePins(GPIO_Led_PORT, GPIO_Led_PIN_Led_PIN);
			}
			break;
		}
		case 2:
		{

			break;
		}
		case 3:
		{
			
			break;
		}
		case 4:
		{
			
			break;
		}
		default:break;
	}
}

void TIMER_0_INST_IRQHandler(void)
{
    //如果产生了定时器中断
    switch( DL_TimerG_getPendingInterrupt(TIMER_0_INST) )
    {
        case DL_TIMER_IIDX_ZERO://如果是0溢出中断
            key.delay++;
            break;

        default://其他的定时器中断
            break;
    }
}