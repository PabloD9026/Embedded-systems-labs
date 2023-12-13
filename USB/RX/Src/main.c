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
#include "spi.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "disp1color.h"
#include "stdlib.h"
#include "usbd_cdc_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define WIDTH 32
#define HEIGHT 16
#define f6x8_MONO_WIDTH 3
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// usb variables
 uint8_t buffer[6]; // buffer of the USB data (raw data, in char)

char *data = "x-56z\n"; // the char that we send to indicate aliveness
// matrix variables
extern uint8_t data1[16], data2[16], data3[16], data4[16]; // matrix divided into 4 because of the multiplexers
int8_t i, j, xm, ym; // matrix values, xm is the x coordinate of the matrix and ym is the y coordinate
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void message_parsing() {
	if (buffer[0] == 120) { //the first letter of the buffer is x
		if (buffer[1] == 45) { // there is a minus
			if (buffer[4] == 122) { //there is a z after 2 numbers, indicating the message has 2 numbers
				xm = 10 * (buffer[2] - 48);
				xm += buffer[3] - 48;
				xm = -xm;

			}
			if (buffer[3] == 122) { //there is a z after 1 number, indicating the message has 1 number
				xm = buffer[2] - 48;
				xm = -xm;

			}

		} else { // there is not a minus
			if (buffer[3] == 122) { //there is a z after 2 numbers, indicating the message has 2 numbers
				xm = 10 * (buffer[1] - 48);
				xm += buffer[2] - 48;

			}
			if (buffer[2] == 122) { //there is a z after 1 numbers, indicating the message has 1 numbers
				xm = buffer[1] - 48;

			}
		}
	} else if (buffer[0] == 121) { // first letter of the buffer is y
		if (buffer[1] == 45) { // it has a minus
			if (buffer[4] == 122) { //there is a z after 2 numbers, indicating the message has 2 numbers
				ym = 10 * (buffer[2] - 48);
				ym += buffer[3] - 48;
				ym = -ym;

			}
			if (buffer[3] == 122) { //there is a z after 1 number, indicating the message has 1 number
				ym += buffer[2] - 48;
				ym = -ym;

			}

		} else { // there is no minus
			if (buffer[3] == 122) { //there is a z after 2 numbers, indicating the message has 2 numbers
				ym = 10 * (buffer[1] - 48);
				ym += buffer[2] - 48;

			}
			if (buffer[2] == 122) { //there is a z after 1 numbers, indicating the message has 1 numbers
				ym = buffer[1] - 48;

			}
		}
	}

}

void disp_row(int row) {

	if (row == 0) {

		HAL_SPI_Transmit(&hspi1, data1, 16, 10);
		while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
			;

		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_RESET);
	}
	if (row == 1) {

		HAL_SPI_Transmit(&hspi1, data2, 16, 10);
		while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
			;

		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_RESET);

	}

	if (row == 2) {

		HAL_SPI_Transmit(&hspi1, data3, 16, 10);
		while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
			;

		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_SET);

	}

	if (row == 3) {

		HAL_SPI_Transmit(&hspi1, data4, 16, 10);
		while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
			;

		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_SET);

	}

	HAL_GPIO_WritePin(nOE_GPIO_Port, nOE_Pin, GPIO_PIN_SET);
	for (uint32_t x = 0; x <= 500; x++) {
	};
	HAL_GPIO_WritePin(nOE_GPIO_Port, nOE_Pin, GPIO_PIN_RESET);
}

void draw_screen(uint8_t state) {
	for (uint16_t i = 0; i < WIDTH; i++) {
		for (uint16_t j = 0; j < HEIGHT; j++) {
			disp1color_DrawPixel(i, j, state);
		}
	}
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	MX_SPI1_Init();
	MX_USB_DEVICE_Init();
	/* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(nOE_GPIO_Port, nOE_Pin, 0);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		HAL_Delay(1000);

		//send a signal to receive a signal indicating aliveness
		CDC_Transmit_FS((uint8_t*) data, strlen(data));
		//decode the message
		message_parsing();
		//clear the screen at start of the sequence
		if(xm == 0 && ym == 0){
			disp1color_FillScreenbuff(0);
		}
		//draw the point without deleting it and then send the data to the matrix
		disp1color_DrawPixel(xm, ym, 1);
		disp1color_UpdateFromBuff();
		prepare_data();
		for (uint8_t i = 0; i < 20; i++) {
			disp_row(0);
			disp_row(1);
			disp_row(2);
			disp_row(3);
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
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
