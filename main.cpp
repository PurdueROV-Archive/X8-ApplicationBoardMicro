//put all of your #includes into main.h file
#include "main.h" 

/*CAN2 GPIO Configuration    
  PB12  ------> CAN2_RX
  PB13  ------> CAN2_TX */

/*TIM3 GPIO Configuration    
  PA6   ------> TIM3_CH1 */

int main(void)  
{
	//This does all of the initializations for you!
	initEverythig();

	while (1)
	{
		
	}
}

/* This function is where messages from CAN
   communication are received */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* CanHandle)
{
	//example on how to use this in callback function
	if ((CanHandle->pRxMsg)->StdId == 0x11 && (CanHandle->pRxMsg)->IDE == CAN_ID_STD)
	{


	}
}




