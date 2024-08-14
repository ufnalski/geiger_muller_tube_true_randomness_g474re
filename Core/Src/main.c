/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "usart.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "ssd1306.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define DETECTION_EVENT_DEADTIME 10  //ms

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint32_t SoftTimerGeiger;

volatile uint8_t meas_id = 0;
volatile uint8_t bit_id = 0;
volatile uint32_t time_instance[4];
volatile uint8_t random_byte;
volatile uint8_t byte_ready_flag = 0;
uint32_t number_of_events = 0;

char lcd_line[32];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	if ((GPIO_Pin == GEIGER_RX_Pin)
			&& (HAL_GetTick() - SoftTimerGeiger > DETECTION_EVENT_DEADTIME)
			&& (byte_ready_flag == 0))
	{
		// In my case the resolution of HAL_GetTick() proved to be sufficient to pass selected tests for randomness
		// collected by NIST and described in "A Statistical Test Suite for Random and Pseudorandom Number Generators for Cryptographic Applications".
		// If the resolution of 1 ms is not sufficient in your case, go for a timer in the input capture mode.
		SoftTimerGeiger = HAL_GetTick();
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		time_instance[meas_id] = SoftTimerGeiger;
		meas_id++;
		number_of_events++;

		if (meas_id == 4)
		{
			meas_id = 0;
			if (bit_id < 8)
			{
				random_byte |= (time_instance[1] - time_instance[0]
						> time_instance[3] - time_instance[2]) << bit_id;
				bit_id++;
			}
			else
			{
				byte_ready_flag = 1;
				bit_id = 0;
			}
		}
	}

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
	MX_LPUART1_UART_Init();
	MX_SPI2_Init();
	/* USER CODE BEGIN 2 */
	SoftTimerGeiger = HAL_GetTick();

	ssd1306_Init();
	ssd1306_Fill(Black);
	ssd1306_SetCursor(17, 0);
	ssd1306_WriteString("ufnalski.edu.pl", Font_6x8, White);
	ssd1306_SetCursor(10, 11);
	ssd1306_WriteString("Geiger-Muller tube", Font_6x8, White);
	ssd1306_SetCursor(12, 22);
	ssd1306_WriteString("True/Hardware RNG", Font_6x8, White);

	ssd1306_UpdateScreen();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		if (byte_ready_flag == 1)
		{
			for (uint8_t i = 0; i < 8; i++)
			{
				if (((random_byte >> i) & 0x01) == 0x01)
				{
					uint8_t bit1 = '1';
					HAL_UART_Transmit(&hlpuart1, &bit1, 1, 10);
				}
				else
				{
					uint8_t bit0 = '0';
					HAL_UART_Transmit(&hlpuart1, &bit0, 1, 10);
				}
			}

			ssd1306_SetCursor(0, 35);
			sprintf(lcd_line, "CPM: %lu   ",
					number_of_events * 60 * 1000 / HAL_GetTick());
			ssd1306_WriteString(lcd_line, Font_6x8, White);
			ssd1306_SetCursor(0, 45);
			sprintf(lcd_line, "Rnd byte: %d [DEC]  ", random_byte);
			ssd1306_WriteString(lcd_line, Font_6x8, White);
			ssd1306_SetCursor(0, 55);
			sprintf(lcd_line, "Bytes per min: %lu  ",
					number_of_events * 60 * 1000 / 32 / HAL_GetTick());
			ssd1306_WriteString(lcd_line, Font_6x8, White);
			ssd1306_UpdateScreen();

			meas_id = 0;
			bit_id = 0;
			random_byte = 0;
			byte_ready_flag = 0;

		}

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
	RCC_OscInitTypeDef RCC_OscInitStruct =
	{ 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct =
	{ 0 };

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
	RCC_OscInitStruct.PLL.PLLN = 85;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
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
