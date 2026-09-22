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


#define PWM_STEPS          100U   /* Ò»¸öÈí¼þPWMÖÜÆÚ´ÎÊý¼ÆÊý*/
#define COLOR_COUNT        7U     /* ºì,ÂÌ,À¶,»Æ,Çà£¬Æ·ºì£¬°×ì*/
#define SPEED_COUNT        3U     /* ºôÎüËÙ¶È */
#define KEY_DEBOUNCE_MS    30U    /* °¸¼þÈí¼þÈ¥¶¶Ê±¼ä */

#define RGB_R_PIN          GPIO_PIN_0
#define RGB_G_PIN          GPIO_PIN_1
#define RGB_B_PIN          GPIO_PIN_2
#define RGB_PORT           GPIOA



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

volatile uint8_t  g_pwm_counter = 0U;          /* 0 ~ PWM_STEPS-1 */
volatile uint8_t  g_pwm_duty[3] = {0U, 0U, 0U};/* R/G/B µ±Ç°Õ¼¿Õ±È  */
volatile int16_t  g_breath_level = 0;          /* ???? 0 ~ 100 */
volatile int8_t   g_breath_dir = 1;            /* 1 ??,-1 ?? */

volatile uint8_t  g_color_index = 0U;          /* ?????? */
volatile uint8_t  g_speed_index = 0U;          /* ?????? */
volatile uint16_t g_speed_frames = 8U;         /* ?????????? */
volatile uint16_t g_frame_counter = 0U
;

static const uint8_t
 g_color_masks[COLOR_COUNT] = {
    0x01U,  /* R */
    0x02U,  /* G */
    0x04U,  /* B */
    0x03U,  /* R+G:? */
    0x06U,  /* G+B:? */
    0x05U,  /* R+B:?? */
    0x07U   /* R+G+B:? */
};

static const uint16_t
 g_speed_frames_table[SPEED_COUNT] = {
    8U,     /* ? 1.6 s ?????-?-? */
    16U,    /* ? 3.2 s */
    32U     /* ? 6.4 s */
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

static void RGB_ApplyLevel(uint8_t level);

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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

HAL_TIM_Base_Start_IT(&htim2);


  /* USER CODE END 2 */

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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 72-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */




/* ?????????,?????????? */
static void RGB_ApplyLevel(uint8_t level)
{
    uint8_t mask = g_color_masks[g_color_index];

    g_pwm_duty[0] = (mask & 0x01U) ? level : 0U;  /* R */
    g_pwm_duty[1] = (mask & 0x02U) ? level : 0U;  /* G */
    g_pwm_duty[2] = (mask & 0x04U) ? level : 0U;  /* B */
}

/* TIM2 ??????:? 10 us ???? */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance != TIM2)
    {
        return;
    }

    g_pwm_counter++;

    /* 100 ? 10 us ?????? 1 ms ??? PWM ?? */
    if (g_pwm_counter >= PWM_STEPS)
    {
        g_pwm_counter = 0U;
        g_frame_counter++;

        /* ????? PWM ???????,?????? */
        if (g_frame_counter >= g_speed_frames)
        {
            g_frame_counter = 0U;

            g_breath_level += g_breath_dir;

            if (g_breath_level >= (int16_t)PWM_STEPS)
            {
                g_breath_level = (int16_t)PWM_STEPS;
                g_breath_dir = -1;
            }
            else if (g_breath_level <= 0)
            {
                g_breath_level = 0;
                g_breath_dir = 1;
            }

            RGB_ApplyLevel((uint8_t)g_breath_level);
        }
    }

    /*
     * ?? RGB ????????? + 1 kO ??? +5 V:
     * GPIO_PIN_SET   -> ??,??????????,LED ?;
     * GPIO_PIN_RESET -> ??,?????,LED ??
     *
     * ??????“PA8 -> ?? -> LED -> 3.3 V??????”?
     * ???????,???????????
     */
    HAL_GPIO_WritePin(RGB_PORT, RGB_R_PIN,
        (g_pwm_counter < g_pwm_duty[0]) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(RGB_PORT, RGB_G_PIN,
        (g_pwm_counter < g_pwm_duty[1]) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(RGB_PORT, RGB_B_PIN,
        (g_pwm_counter < g_pwm_duty[2]) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* PB12?PB13 ?????? */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static uint32_t last_speed_tick = 0U;
    static uint32_t last_color_tick = 0U;
    uint32_t now = HAL_GetTick();

    if (GPIO_Pin == GPIO_PIN_12)
    {
        if ((now - last_speed_tick) >= KEY_DEBOUNCE_MS)
        {
            last_speed_tick = now;

            /* ??????:8 -> 16 -> 32 -> 8 */
            g_speed_index = (uint8_t)((g_speed_index + 1U) % SPEED_COUNT);
            g_speed_frames = g_speed_frames_table[g_speed_index];
        }
    }
    else if (GPIO_Pin == GPIO_PIN_13)
    {
        if ((now - last_color_tick) >= KEY_DEBOUNCE_MS)
        {
            last_color_tick = now;

            /* ?????? */
            g_color_index = (uint8_t)((g_color_index + 1U) % COLOR_COUNT);
        }
    }
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
