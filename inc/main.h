#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stm32f407xx.h>
#include "init.h"

#define Camera_Led_Pin GPIO_PIN_6
#define Camera_Led_GPIO_Port GPIOA


#define POW_CAN_ID	0x11
#define CAN_ID 0x12
#define MAIN_CAN_ID 0x13

#define GREEN 0
#define ORANGE 1
#define RED 2
#define BLUE 3



#endif /* __MAIN_H */
