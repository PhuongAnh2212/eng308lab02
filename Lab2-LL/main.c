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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

uint8_t up_last_state = GPIO_PIN_RESET;
uint8_t up_current_state;

uint8_t down_last_state = GPIO_PIN_RESET;
uint8_t down_current_state;

uint8_t left_last_state = GPIO_PIN_RESET;
uint8_t left_current_state;

uint8_t right_last_state = GPIO_PIN_RESET;
uint8_t right_current_state;

uint8_t select_last_state = GPIO_PIN_RESET;
uint8_t select_current_state;

uint8_t a0_state = 0;
uint8_t a1_state = 0;
uint8_t a2_state = 0;
uint8_t a3_state = 0;
uint8_t leds_locked = 0;
uint8_t select_off = 0;


uint32_t my_initialized_var = 200;
uint32_t my_initialized_var1 = 300;
uint32_t my_initialized_var2 = 400;

void toggle_a0() {
    LL_GPIO_TogglePin(GPIOI, a0_Pin);
    a0_state = !a0_state;
}
void toggle_a1() {
    LL_GPIO_TogglePin(GPIOI, a1_Pin);
    a1_state = !a1_state;
}
void toggle_a2() {
    LL_GPIO_TogglePin(GPIOI, a2_Pin);
    a2_state = !a2_state;
}
void toggle_a3() {
    LL_GPIO_TogglePin(GPIOI, a3_Pin);
    a3_state = !a3_state;
}

void update_led_states(void)
{
	if (a0_state == 0) {
	    LL_GPIO_SetOutputPin(GPIOI, a0_Pin);    // Set to HIGH
	} else {
	    LL_GPIO_ResetOutputPin(GPIOI, a0_Pin);  // Set to LOW
	}

	if (a1_state == 0) {
	    LL_GPIO_SetOutputPin(GPIOI, a1_Pin);    // Set to HIGH
	} else {
	    LL_GPIO_ResetOutputPin(GPIOI, a1_Pin);  // Set to LOW
	}

	if (a2_state == 0) {
	    LL_GPIO_SetOutputPin(GPIOI, a2_Pin);    // Set to HIGH
	} else {
	    LL_GPIO_ResetOutputPin(GPIOI, a2_Pin);  // Set to LOW
	}

	if (a3_state == 0) {
	    LL_GPIO_SetOutputPin(GPIOI, a3_Pin);    // Set to HIGH
	} else {
	    LL_GPIO_ResetOutputPin(GPIOI, a3_Pin);  // Set to LOW
	}
}
void update_input_states()
{
	up_last_state = up_current_state;
	down_last_state = down_current_state;
	left_last_state = left_current_state;
	right_last_state = right_current_state;
}
void initialize_leds(void)
{
	leds_locked = 0;
	a0_state = 0;
	a1_state = 0;
	a2_state = 1;
	a3_state = 0;
	update_led_states();
}

void turn_off_all_leds(void)
{
	a0_state = 0;
	a1_state = 0;
	a2_state = 0;
	a3_state = 0;
	update_led_states();
}

void initialize_inputs(void)
{
    up_current_state = LL_GPIO_IsInputPinSet(GPIOK, up_Pin);
    down_current_state = LL_GPIO_IsInputPinSet(GPIOK, down_Pin);
    left_current_state = LL_GPIO_IsInputPinSet(GPIOK, left_Pin);
    right_current_state = LL_GPIO_IsInputPinSet(GPIOK, right_Pin);
    select_current_state = LL_GPIO_IsInputPinSet(GPIOK, select_Pin);
}
void handle_input(void)
{
    initialize_inputs();
    if (leds_locked == 1) {
        return;
    }
    if (up_last_state == GPIO_PIN_RESET && up_current_state == GPIO_PIN_SET) {
        if (a0_state == 0) {
            toggle_a0();
        } else if (a1_state == 0) {
            toggle_a1();
        } else if (a2_state == 0) {
            toggle_a2();
        } else if (a3_state == 0) {
            toggle_a3();
        }
    }

    if (down_last_state == GPIO_PIN_RESET && down_current_state == GPIO_PIN_SET) {
        if (a3_state == 1) {
            toggle_a3();
        } else if (a2_state == 1) {
            toggle_a2();
        } else if (a1_state == 1) {
            toggle_a1();
        } else if (a0_state == 1) {
            toggle_a0();
        }
    }

    if (left_last_state == GPIO_PIN_RESET && left_current_state == GPIO_PIN_SET) {
        int temp_left = a0_state;
        a0_state = a3_state;
        a3_state = a2_state;
        a2_state = a1_state;
        a1_state = temp_left;
        update_led_states();
    }

    if (right_last_state == GPIO_PIN_RESET && right_current_state == GPIO_PIN_SET) {
        int temp_left = a0_state;
        a0_state = a1_state;
        a1_state = a2_state;
        a2_state = a3_state;
        a3_state = temp_left;
        update_led_states();
    }

    update_input_states();
}

void lock_leds(void) { leds_locked = 1; }
void unlock_leds(void) { leds_locked = 0; }
void reset_leds(void)
{
    a0_state = 0;
    a1_state = 0;
    a2_state = 0;
    a3_state = 0;
    update_led_states();
    leds_locked = 1;
}
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
/* USER CODE BEGIN Boot_Mode_Sequence_0 */
  int32_t timeout;
/* USER CODE END Boot_Mode_Sequence_0 */

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
  Error_Handler();
  }
/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
/* USER CODE BEGIN Boot_Mode_Sequence_2 */
/* When system initialization is finished, Cortex-M7 will release Cortex-M4 by means of
HSEM notification */
/*HW semaphore Clock enable*/
__HAL_RCC_HSEM_CLK_ENABLE();
/*Take HSEM */
HAL_HSEM_FastTake(HSEM_ID_0);
/*Release HSEM in order to notify the CPU2(CM4)*/
HAL_HSEM_Release(HSEM_ID_0,0);
/* wait until CPU2 wakes up from stop mode */
timeout = 0xFFFF;
while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
if ( timeout < 0 )
{
Error_Handler();
}
/* USER CODE END Boot_Mode_Sequence_2 */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  reset_leds();
  initialize_leds();
  turn_off_all_leds();
  lock_leds();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  handle_input();

	 	  select_current_state = LL_GPIO_IsInputPinSet(GPIOK, select_Pin);
	 	  if (select_last_state == GPIO_PIN_RESET && select_current_state == GPIO_PIN_SET) {
	 		  if (select_off == 0)
	 		  {
	 			  turn_off_all_leds();
	 			  select_off = 1;
	 		  } else if (select_off == 1)
	 		  {
	 			  unlock_leds();
	 			  initialize_leds();
	 			  select_off = 2;
	 		  } else if (select_off == 2)
	 		  {
	 			  lock_leds();
	 			  select_off = 0;
	 		  }
	 	  }
	 	  select_last_state = select_current_state;
	 	  LL_mDelay(20);
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

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
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOK);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOI);

  /**/
  LL_GPIO_ResetOutputPin(GPIOI, a0_Pin|a1_Pin|a2_Pin|a3_Pin);

  /**/
  GPIO_InitStruct.Pin = right_Pin|left_Pin|up_Pin|down_Pin
                          |select_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOK, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = a0_Pin|a1_Pin|a2_Pin|a3_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOI, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
