#ifndef X8_PWM
#define X8_PWM

/* puts all of the #includes and # defines that you need here */
#include "main.h"


class pwm {


public:


    /*constructor */
    pwm(TIM_HandleTypeDef *htim, TIM_OC_InitTypeDef* sConfig, uint32_t Channel);

    void set(uint8_t dutyCycle);



private:

    TIM_HandleTypeDef *timerHandler;
    TIM_OC_InitTypeDef* sConfig;
    uint32_t channel;


};

#endif
