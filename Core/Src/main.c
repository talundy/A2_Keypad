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
#include "Keypad_Funcs.h"


void SystemClock_Config(void);
void LED_config(void);


int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();


  /* Configure the system clock */
  SystemClock_Config();

  // GPIO clock
  RCC->AHB2ENR = (1 << 0);

  // Initialize keypad
  keypad_init();

  // configs LED pins
  LED_config();

  int count;

    /* USER CODE BEGIN WHILE */
    while (1)
    {
  	  count = keypad_get_button();
	  //GPIOC->ODR |= count;
  	  if(count != -1){
		  GPIOB->ODR &= count; //sets LEDS to only light on count
		  GPIOB->ODR |= count; // Not actually needed, but a fail safe just in case
  	  }
    }
}

void LED_config(void){
	//
	// sets GPIOB Clock to high
	// USES PORTS GPIO B [0-3] for LEDS

	    RCC->AHB2ENR |= (1<<RCC_AHB2ENR_GPIOBEN);


	    //Sets first 8 bits to 10101010,
	    GPIOB->MODER &= ~(GPIO_MODER_MODE0 |
	  		  	 	 	GPIO_MODER_MODE1 |
	  					GPIO_MODER_MODE2 |
	  					GPIO_MODER_MODE3);

	    GPIOB->MODER |= ((2 << GPIO_MODER_MODE0_Pos) |
	  		  	  	   (2 << GPIO_MODER_MODE1_Pos) |
	  				   (2 << GPIO_MODER_MODE2_Pos) |
	  				   (2 << GPIO_MODER_MODE3_Pos));

	    // Sets all OTYPE bits to 0
	    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT0 |
	  		  	  	     GPIO_OTYPER_OT1 |
	  					 GPIO_OTYPER_OT2 |
	  					 GPIO_OTYPER_OT3);

	    // Sets all SPEED and PUPD bits to 0
	    GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED0 |
	  		  	  	  	  GPIO_OSPEEDR_OSPEED1 |
	  					  GPIO_OSPEEDR_OSPEED2 |
	  					  GPIO_OSPEEDR_OSPEED3);

	    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD0 |
	  		  	  	  	GPIO_PUPDR_PUPD1 |
	  					GPIO_PUPDR_PUPD2 |
	  					GPIO_PUPDR_PUPD3);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
