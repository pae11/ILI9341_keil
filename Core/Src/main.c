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
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "string.h"
#include "stdio.h"


#include "ILI9341_GFX.h"
#include "fonts.h"
#include "img.h"
//#include "xpt2046_touch.h"
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* ARM Compiler 5/6*/
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define DWT_CONTROL *(volatile unsigned long *)0xE0001000
#define SCB_DEMCR   *(volatile unsigned long *)0xE000EDFC

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/*ADC Var*/
char trans_str[64] = {0,};
volatile uint16_t adc = 0;
//volatile uint16_t adc[20] = {0,};

volatile uint8_t flag = 0;


typedef struct
{
	float q;  // Процессный шум (ожидаемые изменения сигнала)
	float r;  // Измерительный шум (шум измерений)
	float x;  // Оценка (фильтрованное значение)
	float p;  // Ошибка оценки
	float k;  // Калмановский коэффициент
} KalmanFilter;
uint32_t adcValue = 0;
float filteredValue = 0;

/*Display Var*/
char txt_buf[] = "ADC Data";
char buf[64] = {0,};


bool btnClick = false;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance == ADC1)
	{
		flag = 1;
		//	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim3.Instance)
	{

		// если кнопка была нажата, меняем ее состояние на противоположное и сбрасываем флаг нажатия
		if(HAL_GPIO_ReadPin(PUMP_ON_GPIO_Port, PUMP_ON_Pin))
		{
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
			btnClick = false;
		}
	}
}

// Инициализация фильтра Калмана
void Kalman_Init(KalmanFilter *kf, float process_noise, float measurement_noise, float estimated_error, float initial_value)
{
	kf->q = process_noise;
	kf->r = measurement_noise;
	kf->x = initial_value;
	kf->p = estimated_error;
	kf->k = 0;
}

// Обновление состояния фильтра Калмана при новом измерении
float Kalman_Update(KalmanFilter *kf, float measurement)
{
	// Шаг предсказания
	kf->p = kf->p + kf->q;

	// Шаг обновления
	kf->k = kf->p / (kf->p + kf->r);  // Вычисляем коэффициент Калмана
	kf->x = kf->x + kf->k * (measurement - kf->x);  // Обновляем оценку
	kf->p = (1 - kf->k) * kf->p;  // Обновляем ошибку оценки

	return kf->x;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

	/* USER CODE BEGIN 1 */
	KalmanFilter adcKalman;
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
	MX_DMA_Init();
	MX_USART1_UART_Init();
	MX_SPI1_Init();
	MX_ADC1_Init();
	MX_TIM4_Init();
	MX_TIM3_Init();
	/* USER CODE BEGIN 2 */

	Kalman_Init(&adcKalman, 0.01, 1.0, 0.8, 0);


	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc, 20);
	HAL_TIM_OC_Start(&htim4, TIM_CHANNEL_4);
	HAL_TIM_Base_Start_IT(&htim3);

	__HAL_SPI_ENABLE(DISP_SPI_PTR); // включаем SPI

	DISP_CS_UNSELECT;
// TOUCH_CS_UNSELECT; // это нужно только если есть тач

	ILI9341_Init(); // инициализация дисплея
	ILI9341_Set_Rotation(SCREEN_VERTICAL_2); // установка ориентации экрана (варианты в файле ILI9341_GFX.h)
	ILI9341_Fill_Screen(BLUE); // заливка всего экрана цветом (цвета в файле ILI9341_GFX.h)
	ILI9341_WriteString(40, 46, txt_buf, Font_16x26, YELLOW, BLUE); // можно передавать массив

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while(1)
	{
		if(flag & (adc >= 4050))
		{
			flag = 0;
			GPIOA->BSRR = GPIO_BSRR_BR4;
			//HAL_ADC_Stop_DMA(&hadc1); // это необязательно
			ADC1->CR2 &= ~ADC_CR2_DMA;  // Отключить DMA для ADC1
			ADC1->CR2 &= ~ADC_CR2_ADON; // Остановить ADC1

			// Обновляем фильтр Калмана с новым значением АЦП
			filteredValue = Kalman_Update(&adcKalman, (float)adc);

			snprintf(trans_str, 10, "ADC%d\r\n", (uint16_t)filteredValue);
			ILI9341_WriteString(40, 96, trans_str, Font_16x26, YELLOW, BLUE);
		//	HAL_UART_Transmit(&huart1, (uint8_t*)trans_str, strlen(trans_str), 100);

			HAL_Delay(500);
			printf("hello\r\n");
			adc = 0;
			HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc, 20);
		}

		if(flag & (adc < 4050))
		{
			flag = 0;
			GPIOA->BSRR = GPIO_BSRR_BS4;
			//HAL_ADC_Stop_DMA(&hadc1); // это необязательно
			ADC1->CR2 &= ~ADC_CR2_DMA;  // Отключить DMA для ADC1
			ADC1->CR2 &= ~ADC_CR2_ADON; // Остановить ADC1
			// Обновляем фильтр Калмана с новым значением АЦП
			filteredValue = Kalman_Update(&adcKalman, (float)adc);

			snprintf(trans_str, 10, "ADC %d \r\n", (uint16_t)filteredValue);
//           snprintf(trans_str, 10, "ADC %d \r\n", adc);
			ILI9341_WriteString(40, 96, trans_str, Font_16x26, YELLOW, BLUE);
		//	HAL_UART_Transmit(&huart1, (uint8_t*)trans_str, strlen(trans_str), 100);

			HAL_Delay(500);
						printf("hello\r\n");
			adc = 0;
			HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc, 20);
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
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
	while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
	{
	}
	LL_RCC_HSE_Enable();

	/* Wait till HSE is ready */
	while(LL_RCC_HSE_IsReady() != 1)
	{

	}
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
	LL_RCC_PLL_Enable();

	/* Wait till PLL is ready */
	while(LL_RCC_PLL_IsReady() != 1)
	{

	}
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	/* Wait till System clock is ready */
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
	{

	}
	LL_SetSystemCoreClock(72000000);

	/* Update the time base */
	if(HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
	{
		Error_Handler();
	}
	LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_6);
}

/* USER CODE BEGIN 4 */

int fputc(int ch, FILE *f) {
    // Ожидание, пока буфер передачи не будет пуст (TXE = 1)
    while (!(USART1->SR & USART_SR_TXE));

    // Отправляем символ
    USART1->DR = (uint8_t)ch;

    // Возвращаем символ
    return ch;
}

//int fputc(int ch, FILE *f) {
//    // Отправка символа через UART (например, huart1)
//    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
//    return ch;
//}

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
	while(1)
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
