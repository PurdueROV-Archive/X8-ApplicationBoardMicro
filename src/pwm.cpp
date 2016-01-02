/* Put all of your #includes and #defines up here   */

#include "pwm.h"


/*Put all function definitions here. Do not forget the 
  classname:: part before the function name! */

/* Second note, each class must have a constructor. This
   is a function that is called when you create the object 
   (just like creating a variable) of the class. This function
   will have no return type. */

//HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim, TIM_OC_InitTypeDef* sConfig, uint32_t Channel)

//HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel)


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



