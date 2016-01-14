//put all of your #includes into main.h file
#include "main.h"
#include "inc/init.h"
#include "inc/pwm.h"
#include "inc/print.h"

/*CAN2 
  PB12  ------> CAN2_RX
  PB13  ------> CAN2_TX
  		* CAN_HandleTypeDef hcan2   		*/

/*TIM3 Main LEDS
  PA6   ------> TIM3_CH1
  		* TIM_HandleTypeDef htim3
  		* TIM_OC_InitTypeDef sConfigOC  
  		* TIM_CHANNEL_1						*/

/*USART1 
        PA9     ------> USART1_TX
        PA10    ------> USART1_RX
       		* UART_HandleTypeDef huart1   	*/


/* 	USEFULL FUNCTIONS

	RED; BLUE; GREEN; ORANGE

	void LedOn(int ledNum);
	void LedOff(int ledNum);
	void LedToggle(int ledNum);
*/

/*	Example how to send can code 
		
	CanHandle.pTxMsg->DLC = 3; //sets the size of the message in bytes. Max 8 bytes per message

	//sets the information that is sent over the message
	CanHandle.pTxMsg->Data[0] = 5;
    CanHandle.pTxMsg->Data[1] = 246;
    CanHandle.pTxMsg->Data[2] = 17;

	HAL_CAN_Transmit(&hcan2, 10);  //sends the message */	


int main(void) {

	//Initializes Everything!
	initEverything();

	while (1) {

	}
}

/* This function is where messages from CAN
   communication are received */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* CanHandle) {

	//example on how to use this in callback function
	if ((CanHandle->pRxMsg)->StdId == 0x11 && (CanHandle->pRxMsg)->IDE == CAN_ID_STD) {

	}

	//Restarts interrupt!
	HAL_CAN_Receive_IT(CanHandle, CAN_FIFO0);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle) {

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) {

}




