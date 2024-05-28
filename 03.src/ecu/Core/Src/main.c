/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticQueue_t osStaticMessageQDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart1;

/* Definitions for EcoTaskMain */
osThreadId_t EcoTaskMainHandle;
const osThreadAttr_t EcoTaskMain_attributes = {
  .name = "EcoTaskMain",
  .stack_size = 4096 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for EcoTaskUART */
osThreadId_t EcoTaskUARTHandle;
const osThreadAttr_t EcoTaskUART_attributes = {
  .name = "EcoTaskUART",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for EcoTaskNMEA2KRx */
osThreadId_t EcoTaskNMEA2KRxHandle;
const osThreadAttr_t EcoTaskNMEA2KRx_attributes = {
  .name = "EcoTaskNMEA2KRx",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for EcoTaskNMEA2KTx */
osThreadId_t EcoTaskNMEA2KTxHandle;
const osThreadAttr_t EcoTaskNMEA2KTx_attributes = {
  .name = "EcoTaskNMEA2KTx",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for EcoQueueUART1 */
osMessageQueueId_t EcoQueueUART1Handle;
uint8_t EcoQueueUART1Buffer[ 256 * sizeof( uint8_t ) ];
osStaticMessageQDef_t EcoQueueUART1CtrlBlock;
const osMessageQueueAttr_t EcoQueueUART1_attributes = {
  .name = "EcoQueueUART1",
  .cb_mem = &EcoQueueUART1CtrlBlock,
  .cb_size = sizeof(EcoQueueUART1CtrlBlock),
  .mq_mem = &EcoQueueUART1Buffer,
  .mq_size = sizeof(EcoQueueUART1Buffer)
};
/* Definitions for EcoQueueNMEA2KRX1 */
osMessageQueueId_t EcoQueueNMEA2KRX1Handle;
uint8_t EcoQueueNMEA2KRX1Buffer[ 32 * sizeof( uint8_t ) ];
osStaticMessageQDef_t EcoQueueNMEA2KRX1CtrlBlock;
const osMessageQueueAttr_t EcoQueueNMEA2KRX1_attributes = {
  .name = "EcoQueueNMEA2KRX1",
  .cb_mem = &EcoQueueNMEA2KRX1CtrlBlock,
  .cb_size = sizeof(EcoQueueNMEA2KRX1CtrlBlock),
  .mq_mem = &EcoQueueNMEA2KRX1Buffer,
  .mq_size = sizeof(EcoQueueNMEA2KRX1Buffer)
};
/* Definitions for EcoQueueNMEA2KTX1 */
osMessageQueueId_t EcoQueueNMEA2KTX1Handle;
uint8_t EcoQueueNMEA2KTX1Buffer[ 384 * sizeof( uint8_t ) ];
osStaticMessageQDef_t EcoQueueNMEA2KTX1CtrlBlock;
const osMessageQueueAttr_t EcoQueueNMEA2KTX1_attributes = {
  .name = "EcoQueueNMEA2KTX1",
  .cb_mem = &EcoQueueNMEA2KTX1CtrlBlock,
  .cb_size = sizeof(EcoQueueNMEA2KTX1CtrlBlock),
  .mq_mem = &EcoQueueNMEA2KTX1Buffer,
  .mq_size = sizeof(EcoQueueNMEA2KTX1Buffer)
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_CAN1_Init(void);
static void MX_TIM7_Init(void);
void runEcoTaskMain(void *argument);
void runEcoTaskUART(void *argument);
void runEcoTaskNMEA2KRx(void *argument);
void runEcoTaskNMEA2KTx(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
/* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_CAN1_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of EcoQueueUART1 */
  EcoQueueUART1Handle = osMessageQueueNew (256, sizeof(uint8_t), &EcoQueueUART1_attributes);

  /* creation of EcoQueueNMEA2KRX1 */
  EcoQueueNMEA2KRX1Handle = osMessageQueueNew (32, sizeof(uint8_t), &EcoQueueNMEA2KRX1_attributes);

  /* creation of EcoQueueNMEA2KTX1 */
  EcoQueueNMEA2KTX1Handle = osMessageQueueNew (384, sizeof(uint8_t), &EcoQueueNMEA2KTX1_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of EcoTaskMain */
  EcoTaskMainHandle = osThreadNew(runEcoTaskMain, NULL, &EcoTaskMain_attributes);

  /* creation of EcoTaskUART */
  EcoTaskUARTHandle = osThreadNew(runEcoTaskUART, NULL, &EcoTaskUART_attributes);

  /* creation of EcoTaskNMEA2KRx */
  EcoTaskNMEA2KRxHandle = osThreadNew(runEcoTaskNMEA2KRx, NULL, &EcoTaskNMEA2KRx_attributes);

  /* creation of EcoTaskNMEA2KTx */
  EcoTaskNMEA2KTxHandle = osThreadNew(runEcoTaskNMEA2KTx, NULL, &EcoTaskNMEA2KTx_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 12;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_15TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = ENABLE;
  hcan1.Init.AutoWakeUp = ENABLE;
  hcan1.Init.AutoRetransmission = ENABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = ENABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 10800-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 10000-1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_ON_GPIO_Port, LED_ON_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(WDI_GPIO_Port, WDI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_ON_Pin */
  GPIO_InitStruct.Pin = LED_ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_ON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY_DN_Pin */
  GPIO_InitStruct.Pin = KEY_DN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY_DN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : WDI_Pin */
  GPIO_InitStruct.Pin = WDI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(WDI_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_runEcoTaskMain */
/**
  * @brief  Function implementing the EcoTaskMain thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_runEcoTaskMain */
__weak void runEcoTaskMain(void *argument)
{
  /* USER CODE BEGIN 5 */

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_runEcoTaskUART */
/**
* @brief Function implementing the EcoTaskUART thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runEcoTaskUART */
__weak void runEcoTaskUART(void *argument)
{
  /* USER CODE BEGIN runEcoTaskUART */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END runEcoTaskUART */
}

/* USER CODE BEGIN Header_runEcoTaskNMEA2KRx */
/**
* @brief Function implementing the EcoTaskNMEA2KRx thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runEcoTaskNMEA2KRx */
__weak void runEcoTaskNMEA2KRx(void *argument)
{
  /* USER CODE BEGIN runEcoTaskNMEA2KRx */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END runEcoTaskNMEA2KRx */
}

/* USER CODE BEGIN Header_runEcoTaskNMEA2KTx */
/**
* @brief Function implementing the EcoTaskNMEA2KTx thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runEcoTaskNMEA2KTx */
__weak void runEcoTaskNMEA2KTx(void *argument)
{
  /* USER CODE BEGIN runEcoTaskNMEA2KTx */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END runEcoTaskNMEA2KTx */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
