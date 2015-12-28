/*This folder will be used to hold all code used to initialize pins */

#include "main.h"

/*structues used in can */
CAN_HandleTypeDef hcan2;
CanTxMsgTypeDef   TxMessage;
CanRxMsgTypeDef   RxMessage;

/* structures used in pwm */
TIM_MasterConfigTypeDef sMasterConfig;
TIM_OC_InitTypeDef sConfigOC;
TIM_HandleTypeDef htim3;

void initEverythig(void)
{
	/* GPIO Ports Clock Enable */
 	__GPIOA_CLK_ENABLE();
 	__GPIOB_CLK_ENABLE();
 	__GPIOC_CLK_ENABLE();

	HAL_Init();
	  
	SystemClock_Config();

	initDebugLeds();

	initCan();

	HAL_MspInit();

	initPwm();
}

void HAL_MspInit(void)
{
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void initCan(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__CAN2_CLK_ENABLE();
    __CAN1_CLK_ENABLE();

    /**CAN2 GPIO Configuration    
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX 
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


    hcan2.Instance = CAN2;
  	hcan2.Init.Prescaler = 16;
  	hcan2.Init.Mode = CAN_MODE_NORMAL;
  	hcan2.Init.SJW = CAN_SJW_1TQ;
  	hcan2.Init.BS1 = CAN_BS1_1TQ;
  	hcan2.Init.BS2 = CAN_BS2_1TQ;
  	hcan2.Init.TTCM = DISABLE;
  	hcan2.Init.ABOM = DISABLE;
  	hcan2.Init.AWUM = DISABLE;
  	hcan2.Init.NART = DISABLE;
  	hcan2.Init.RFLM = DISABLE;
  	hcan2.Init.TXFP = DISABLE;
  	
  	HAL_CAN_Init(&hcan2);

  	CAN_FilterConfTypeDef  sFilterConfig;

  	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = 0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.BankNumber = 14;

    HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig);

    //sets up the communication information
    hcan2.pTxMsg->StdId = CAN_ID;  //the id of this microboard
    
    hcan2.pTxMsg->RTR = CAN_RTR_DATA;
    hcan2.pTxMsg->IDE = CAN_ID_STD;


  	HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0);
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

//configures the system clcok
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  
  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
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
}
void LedOn(int ledNum)
{
	if(ledNum == 0)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	}
	else if(ledNum == 1)
	{	
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
	}
	else if(ledNum == 2)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	}
	else if(ledNum == 3)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
	}
}
void LedOff(int ledNum)
{
	if(ledNum == 0)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	}
	else if(ledNum == 1)
	{	
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
	}
	else if(ledNum == 2)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	}
	else if(ledNum == 3)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	}
}
void LedToggle(int ledNum)
{
	if(ledNum == 0)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
	}
	else if(ledNum == 1)
	{	
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
	}
	else if(ledNum == 2)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
	}
	else if(ledNum == 3)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
	}
}
