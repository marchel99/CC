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
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// #include "lcd.h"
// #include "logo.c"

#include <wchar.h>
#include "hagl.h"
#include "font6x9.h"
#include "rgb565.h"
#include "max.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t test_image[64 * 64];

int __io_putchar(int ch)
{
  if (ch == '\n')
  {
    uint8_t ch2 = '\r';
    HAL_UART_Transmit(&huart2, &ch2, 1, HAL_MAX_DELAY);
  }

  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return 1;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  HAL_StatusTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef * hrtc, RTC_TimeTypeDef * sTime, uint32_t Format);
  HAL_StatusTypeDef HAL_RTC_GetDate(RTC_HandleTypeDef * hrtc, RTC_DateTypeDef * sDate, uint32_t Format);
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
  MX_SPI2_Init();
  MX_RTC_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  /* RTC_TimeTypeDef time;
  RTC_DateTypeDef date;

  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

   */

  lcd_init();

  /* // RECTANGLE
  for (int i = 0; i < 8; i++) {
    hagl_draw_rounded_rectangle(2+i, 2+i, 158-i, 126-i, 8-i, rgb565(0, 0, i*16));
  }

   */

  wchar_t buffer[32]; // Bufor na ciąg znaków, musi być wystarczająco duży

  // Konwersja int na ciąg znaków
  // swprintf(buffer, 32, L"Pomiar prądu: %.2f mA", your_variable);

  // hagl_put_text(L"Godzina: ", 15, 20, WHITE, font6x9);
  //-l_+p , -g|+d

  // Teraz możesz wywołać funkcję hagl_put_text z tym buforem
  hagl_put_text(buffer, 15, 40, WHITE, font6x9);

  wchar_t time_buffer[32]; // Bufor na ciąg znaków, musi być wystarczająco duży

  while (1)
  {

    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN); // Funkcja musi być wywołana po GetTime
    swprintf(time_buffer, sizeof(time_buffer), L"Czas: %02d:%02d:%02d", time.Hours, time.Minutes, time.Seconds);

    hagl_put_text(time_buffer, 15, 20, WHITE, font6x9);

    float battery_voltage = read_voltage(&hi2c1);
    wchar_t voltage_buffer[32]; // Bufor na napięcie baterii

    if (battery_voltage > 0)
    {
      // Formatowanie i wyświetlanie napięcia baterii
      swprintf(voltage_buffer, sizeof(voltage_buffer), L"Napięcie: %.4f V", battery_voltage);
      hagl_put_text(voltage_buffer, 15, 50, WHITE, font6x9); // Zmienić położenie tekstu w razie potrzeby
    }
    else
    {
      // Wyświetlanie informacji o błędzie
      hagl_put_text(L"Błąd odczytu napięcia!", 15, 50, WHITE, font6x9);
    }

    float battery_soc = read_soc(&hi2c1);
    wchar_t soc_buffer[32]; // Bufor na procent naładowania baterii

    if (battery_soc >= 0)
    {
      // Formatowanie i wyświetlanie stanu naładowania baterii
      swprintf(soc_buffer, sizeof(soc_buffer), L"Naładowanie: %.2f%%", battery_soc);
      hagl_put_text(soc_buffer, 15, 40, WHITE, font6x9); // Zmienić położenie tekstu w razie potrzeby
    }
    else
    {
      // Wyświetlanie informacji o błędzie
      hagl_put_text(L"Błąd odczytu SoC!", 15, 40, WHITE, font6x9);
    }

    uint8_t ic_version = read_ic_version(&hi2c1);
    wchar_t version_buffer[32]; // Bufor na wersję układu IC

    if (ic_version != 0xFF)
    {
      // Formatowanie i wyświetlanie wersji układu IC
      swprintf(version_buffer, sizeof(version_buffer), L"Wersja IC: 0x%02X", ic_version);
      hagl_put_text(version_buffer, 15, 80, WHITE, font6x9); // Zmienić położenie tekstu w razie potrzeby
    }
    else
    {
      // Wyświetlanie informacji o błędzie
      hagl_put_text(L"Błąd odczytu wersji IC!", 15, 80, WHITE, font6x9);
    }




    float resistance = 1000.0f; // Wartość bocznika: 0 Ohm, co oznacza brak bocznika
    wchar_t current_buffer[64]; // Bufor na prąd
    wchar_t shunt_buffer[32];   // Bufor na opis bocznika

    // Sprawdzanie czy bocznik jest obecny
    if (resistance > 0.0f)
    {
      // Formatowanie opisu bocznika
      if (resistance < 1000.0f)
      {
        swprintf(shunt_buffer, sizeof(shunt_buffer) / sizeof(wchar_t), L"Bocznik: %.0f Ω", resistance);
      }
      else
      {
        swprintf(shunt_buffer, sizeof(shunt_buffer) / sizeof(wchar_t), L"Bocznik: %.1f kΩ", resistance / 1000.0f);
      }
    }
    else
    {
      // Jeśli bocznik jest równy 0, wyświetla "BRAK"
      swprintf(shunt_buffer, sizeof(shunt_buffer) / sizeof(wchar_t), L"Bocznik: BRAK");
    }

    float battery_current = read_current(&hi2c1, resistance);

    // Sprawdzanie wartości prądu i formatowanie wyniku
    if (battery_current != 0 && resistance > 0.0f)
    {
      if (battery_current < 1.0)
      { // Jeśli prąd jest mniejszy niż 1 A, wyświetl w mA
        swprintf(current_buffer, sizeof(current_buffer) / sizeof(wchar_t), L"Prąd: %.2f mA", battery_current * 1000);
      }
      else
      { // W przeciwnym razie wyświetl w A
        swprintf(current_buffer, sizeof(current_buffer) / sizeof(wchar_t), L"Prąd: %.2f A", battery_current);
      }
    }
    else
    {
      // Jeśli bocznik jest równy 0 lub prąd wynosi 0, wyświetla "BRAK BOCZNIKA"
      swprintf(current_buffer, sizeof(current_buffer) / sizeof(wchar_t), L"Prąd: BRAK BOCZNIKA");
    }

    hagl_put_text(current_buffer, 15, 60, WHITE, font6x9);
    hagl_put_text(shunt_buffer, 15, 70, WHITE, font6x9); // Wyświetlanie informacji o boczniku

    printf("Czas: %02d:%02d:%02d", time.Hours, time.Minutes, time.Seconds);
    printf(" | Voltage: %.8f\n", battery_voltage);

    lcd_copy();
    HAL_Delay(1000);

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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

#ifdef USE_FULL_ASSERT
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