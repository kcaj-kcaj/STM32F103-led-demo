/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//???????????,????1:
uint8_t key_pre_mode = 1;
//???????????,????1:
uint8_t key_cur_mode =1;
//???????????,????1:
uint8_t key_pre_speed = 1;
//???????????,????1:
uint8_t key_cur_speed = 1;
//???????????????,????0
uint8_t ledMode_step_cur=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

//??????
uint8_t LED_mode(uint8_t mode,uint8_t step_cur);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//??????ledMode,????0
uint8_t ledMode=0;
int TperLED = 100; //ms ,???????????????

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
				//1. ????,????????
		key_cur_speed = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);
		if(key_cur_speed==0 && key_pre_speed==1)
		{
		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_11|GPIO_PIN_12); //?????????
				switch(TperLED)
				{
					case 100:
					TperLED = 200;
					break;
					case 200:
					TperLED = 500;
					break;
					case 500:
					TperLED = 100;
					break;
				}
		}
key_pre_speed=key_cur_speed;
//2. ????,????????
			key_cur_mode=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);
			if(key_cur_mode==0 && key_pre_mode==1)
			{
			//
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9); //?????????
			//ledMode=(ledMode+1)%3;
			if(ledMode == 0)
			{
			ledMode = 1;
			}
			else if(ledMode==1){
			ledMode=2;
			}
			else
			{
			ledMode = 0;
			}
			ledMode_step_cur=0;//?????????,?????????????,??Bug
			//?????,??LED ??
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9| GPIO_PIN_10 \
			|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_SET);
			}
			key_pre_mode=key_cur_mode;
			//3. ???,??????????
			ledMode_step_cur=LED_mode(ledMode,ledMode_step_cur);
		
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12, GPIO_PIN_SET);

  /*Configure GPIO pins : PB12 PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 PA10 PA11
                           PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
* @brief ??: ????,????????????
* @param
??????:
mode:????
step_cur:??????????
* @retval ??????:???
*/
uint8_t LED_mode(uint8_t mode,uint8_t step_cur)
{
switch(mode)
{
case 0://??0,?????????LED
switch(step_cur)
{
case 0:
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9| GPIO_PIN_10\
|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_RESET);
HAL_Delay(TperLED);
return step_cur+1;
case 1:
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9| GPIO_PIN_10 \
|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);
HAL_Delay(TperLED);
return step_cur+1;
case 2:
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9| GPIO_PIN_10 \
|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,GPIO_PIN_RESET);
HAL_Delay(TperLED);
return step_cur+1;
case 3:
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9| GPIO_PIN_10 \
|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11,GPIO_PIN_RESET);
HAL_Delay(TperLED);
return step_cur+1;
case 4:
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9| GPIO_PIN_10 \
|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12,GPIO_PIN_RESET);
HAL_Delay(TperLED);
return 0;
} //end ??0
break;//??break ??????,????switch ??return ??????
//?,?????switch-case ???????break
case 1: //??1 ,??LED ???,??2*TperLED ???,???,?????
switch(step_cur)
{
case 0:
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9| GPIO_PIN_10 \
|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_SET);
HAL_Delay(2*TperLED);
return step_cur+1;
case 1:
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9| GPIO_PIN_10 \
|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_RESET);
HAL_Delay(2*TperLED);
return 0;


}

case 2://??0,?????????LED
switch(step_cur)
{
case 4:
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9| GPIO_PIN_10\
|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_RESET);
HAL_Delay(TperLED);
return 0;
case 3:
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9| GPIO_PIN_10 \
|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);
HAL_Delay(TperLED);
return step_cur+1;
case 2:
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9| GPIO_PIN_10 \
|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,GPIO_PIN_RESET);
HAL_Delay(TperLED);
return step_cur+1;
case 1:
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9| GPIO_PIN_10 \
|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11,GPIO_PIN_RESET);
HAL_Delay(TperLED);
return step_cur+1;
case 0:
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9| GPIO_PIN_10 \
|GPIO_PIN_11|GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12,GPIO_PIN_RESET);
HAL_Delay(TperLED);
return step_cur+1;
} //end ??0
break;

} //end switch mode
//????return,????????return;??,????break,??return
}

/* USER CODE END 4 */

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
