/*This folder will be used to hold all code used to initialize pins */

#include "main.h"

/*structues used in can */
CAN_HandleTypeDef hcan2;
CanTxMsgTypeDef   TxMessage;
CanRxMsgTypeDef   RxMessage;

/* structures used in pwm */
TIM_OC_InitTypeDef sConfigOC;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

void initEverything(void)
{
	/* GPIO Ports Clock Enable */
 	__GPIOA_CLK_ENABLE();
 	__GPIOB_CLK_ENABLE();
 	__GPIOC_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();

	HAL_Init();
	  
	SystemClock_Config();

	initDebugLeds();

	CAN_init();

	HAL_MspInit();

	MX_DMA_Init();

	MX_GPIO_Init();
	MX_USART1_UART_Init();

	initPwm();
}

/* USART1 init function */
void MX_USART1_UART_Init(void)
{

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart1);

}

//sets up the pins for can to be alternate functions
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if(hcan->Instance==CAN2)
	{

		/* Peripheral clock enable */
		__CAN2_CLK_ENABLE();
		__CAN1_CLK_ENABLE();

		__HAL_RCC_GPIOB_CLK_ENABLE();

		/**CAN1 GPIO Configuration
        PD0     ------> CAN1_RX
        PD1     ------> CAN1_TX
        */
		GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* Peripheral interrupt init*/
		HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
		HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
	}

}

HAL_StatusTypeDef CAN_init(void)
{

	CAN_FilterConfTypeDef  sFilterConfig;


	/*##-1- Configure the CAN peripheral #######################################*/
	hcan2.Instance = CAN2;
	hcan2.pTxMsg = &TxMessage;
	hcan2.pRxMsg = &RxMessage;


	//standard settings for can bus
	hcan2.Init.TTCM = DISABLE;
	hcan2.Init.ABOM = DISABLE;
	hcan2.Init.AWUM = DISABLE;
	hcan2.Init.NART = DISABLE;
	hcan2.Init.RFLM = DISABLE;
	hcan2.Init.TXFP = DISABLE;
	hcan2.Init.Mode = CAN_MODE_NORMAL;
	hcan2.Init.SJW = CAN_SJW_1TQ;
	hcan2.Init.BS1 = CAN_BS1_6TQ;
	hcan2.Init.BS2 = CAN_BS2_8TQ;
	hcan2.Init.Prescaler = 2;

	//if can is unable to initialize
	if(HAL_CAN_Init(&hcan2) != HAL_OK)
	{

	}

	/*##-2- Configure the CAN Filter ###########################################*/
	sFilterConfig.FilterNumber = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = 0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.BankNumber = 0;

	if(HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK)
	{

	}

	//sets up the communication information
	hcan2.pTxMsg->StdId = CAN_ID;  //the id of this microboard


	hcan2.pTxMsg->RTR = CAN_RTR_DATA;
	hcan2.pTxMsg->IDE = CAN_ID_STD;


	HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0);

	return HAL_OK; /* Test Passed */
}

void MX_GPIO_Init(void)
{

	/* GPIO Ports Clock Enable */
	__GPIOA_CLK_ENABLE();

}

void initPwm(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__TIM3_CLK_ENABLE();

	/**TIM3 GPIO Configuration    
    PA6     ------> TIM3_CH1 
    */

    GPIO_InitStruct.Pin = Camera_Led_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(Camera_Led_GPIO_Port, &GPIO_InitStruct);

  	/* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);


  	htim3.Instance = TIM3;
  	htim3.Init.Prescaler = SystemCoreClock;
  	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  	htim3.Init.Period = 1000;
  	htim3.Init.ClockDivision = 0;
  	HAL_TIM_PWM_Init(&htim3);

  	/*sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
 	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  	HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);*/

  	sConfigOC.OCMode = TIM_OCMODE_PWM1;
  	sConfigOC.Pulse = 500;
  	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;


	HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if(huart->Instance==USART1)
	{
				__USART1_CLK_ENABLE();

		/**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
		GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* Peripheral DMA init*/

		hdma_usart1_rx.Instance = DMA2_Stream2;
		hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
		hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart1_rx.Init.Mode = DMA_NORMAL;
		hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
		hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		HAL_DMA_Init(&hdma_usart1_rx);

		__HAL_LINKDMA(huart,hdmarx,hdma_usart1_rx);

		hdma_usart1_tx.Instance = DMA2_Stream7;
		hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
		hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart1_tx.Init.Mode = DMA_NORMAL;
		hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
		hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		HAL_DMA_Init(&hdma_usart1_tx);

		__HAL_LINKDMA(huart,hdmatx,hdma_usart1_tx);

		/* Peripheral interrupt init*/
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}

}

void MX_DMA_Init(void)
{
	//USART DMA initialization
			__DMA2_CLK_ENABLE();

	/* DMA interrupt init */
	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);


}

void HAL_MspInit(void)
{
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}



//configures the system clcok
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;

			__PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
//function used to handle errors
void Error_Handler(void)
{
  while(1)
  {
  }
}
void initDebugLeds(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	//enable the led clock
	 __HAL_RCC_GPIOC_CLK_ENABLE();

	//configures the led pin  
	GPIO_InitStruct.Pin = GPIO_PIN_6; 
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);  

	GPIO_InitStruct.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 

	GPIO_InitStruct.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 

	GPIO_InitStruct.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 


	GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12;  //pin 5
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);  //initializes the pin A5
}
void LedOn(int ledNum)
{
	if(ledNum == 0)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	}
	else if(ledNum == 1)
	{	
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
	}
	else if(ledNum == 2)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	}
	else if(ledNum == 3)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
	}
}
void LedOff(int ledNum)
{
	if(ledNum == 0)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	}
	else if(ledNum == 1)
	{	
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	}
	else if(ledNum == 2)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	}
	else if(ledNum == 3)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
	}
}
void LedToggle(int ledNum)
{
	if(ledNum == 0)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	}
	else if(ledNum == 1)
	{	
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	}
	else if(ledNum == 2)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	}
	else if(ledNum == 3)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
	}
}
