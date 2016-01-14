/* Put all of your #includes and #defines up here   */

#include "pwm.h"


pwm::pwm(TIM_HandleTypeDef* HTIM, TIM_OC_InitTypeDef* SCONFIG, uint32_t CHANNEL)
{
    timerHandler = HTIM;
    sConfig = SCONFIG;
    channel = CHANNEL;


}

void pwm::set(uint8_t dutyCycle)
{
    sConfig->Pulse = timerHandler->Init.Period * dutyCycle / 255;

    HAL_TIM_PWM_ConfigChannel(timerHandler, sConfig, channel);
    HAL_TIM_PWM_Start(timerHandler, channel) ;
}



