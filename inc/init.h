/* The .h file that will store the function declarations of initialization functions */
#ifndef __INIT_H
#define __INIT_H

#include "main.h"

extern CAN_HandleTypeDef hcan2;  //can structure
extern CanTxMsgTypeDef   TxMessage; //can tx message 	structure
extern CanRxMsgTypeDef   RxMessage; //can rx message structure

extern TIM_OC_InitTypeDef sConfigOC;
extern TIM_HandleTypeDef htim3;

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern UART_HandleTypeDef huart1;

/* function that holds all of the other functions */
void initEverything(void);

/* configures low level settings */
void SystemClock_Config(void);
void Error_Handler(void);

/* Usart initialization */
void MX_GPIO_Init(void);
void MX_USART1_UART_Init(void);

/* configures the DMA nested vector interrupt priorities */
void MX_DMA_Init(void);

/* configures can and sets up an interrupt for received data */
void initCan(void);

/* actual can configuration */
HAL_StatusTypeDef CAN_init(void);

/* initializes the pwm pin to control the camera led */
void initPwm(void);

//initializes nested vector interrupt handlers
void HAL_MspInit(void);

//initialize can with callback interrupt
void initCan(void);

/*function for initializing and using leds */
void initDebugLeds(void);
void LedOn(int ledNum);
void LedOff(int ledNum);
void LedToggle(int ledNum);

#endif