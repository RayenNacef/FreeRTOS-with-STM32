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
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include "task.h"
#include "semphr.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
// Task and semaphore handles
osThreadId_t taskAHandle, taskBHandle, taskACtrlHandle;
osSemaphoreId_t myBinarySem01Handle; // Semaphore for taskACtrl signaling taskA

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void taskA(void *argument);
void taskB(void *argument);
void taskACtrl(void *argument);

/* USER CODE BEGIN PFP */
#define BLUE_LED_GPIO    GPIOD
#define BLUE_LED_PIN     GPIO_PIN_15
#define GREEN_LED_GPIO   GPIOD
#define GREEN_LED_PIN    GPIO_PIN_12
#define RED_LED_GPIO     GPIOD
#define RED_LED_PIN      GPIO_PIN_14

/* LED Control Helpers */

/* USER CODE END PFP */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  // Create semaphore
  myBinarySem01Handle = osSemaphoreNew(1, 0, NULL);
  if (myBinarySem01Handle == NULL) {
    // Handle semaphore creation failure
    Error_Handler();
  }
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_THREADS */
  // Create tasks
  /* creation of A */

	const osThreadAttr_t taskA_attributes = {
    .name = "TaskA",
    .priority = osPriorityLow,
    .stack_size = 128 * 4
  };
  taskAHandle = osThreadNew(taskA, NULL, &taskA_attributes);

  const osThreadAttr_t taskB_attributes = {
    .name = "TaskB",
    .priority = osPriorityNormal,
    .stack_size = 128 * 4
  };
  taskBHandle = osThreadNew(taskB, NULL, &taskB_attributes);

  const osThreadAttr_t taskACtrl_attributes = {
    .name = "TaskACtrl",
    .priority = osPriorityHigh,
    .stack_size = 128 * 4
  };
  taskACtrlHandle = osThreadNew(taskACtrl, NULL, &taskACtrl_attributes);
  /* USER CODE END RTOS_THREADS */

  osKernelStart();

  /* USER CODE BEGIN WHILE */
  while (1) {
    // Control is now under the scheduler
  }
  /* USER CODE END WHILE */
}

/* Task Implementations */
void taskA(void *argument) {
  for (;;) {
    // Wait for semaphore from taskACtrl
    if (osSemaphoreAcquire(myBinarySem01Handle, osWaitForever) == osOK) {
      // Turn on Blue LED
      HAL_GPIO_WritePin(GPIOD, BLUE_LED_PIN, GPIO_PIN_SET);
			
      osDelay(300); // Simulate 300ms of work
      // Turn off Blue LED
      HAL_GPIO_WritePin(GPIOD, BLUE_LED_PIN, GPIO_PIN_RESET);
			
    }
  }
}

void taskB(void *argument) {
  for (;;) {
    // Turn on Green LED
    HAL_GPIO_WritePin(GPIOD, GREEN_LED_PIN, GPIO_PIN_SET);
    osDelay(100); // Simulate 100ms of work
    // Turn off Green LED
    HAL_GPIO_WritePin(GPIOD, GREEN_LED_PIN, GPIO_PIN_RESET);
    osDelay(900); // Complete 1000ms period
  }
}

void taskACtrl(void *argument) {
  for (;;) {
    // Signal taskA
    osSemaphoreRelease(myBinarySem01Handle);
    // Turn on Red LED
    HAL_GPIO_WritePin(GPIOD, RED_LED_PIN, GPIO_PIN_SET);
    osDelay(50); // Indicate release time
    // Turn off Red LED
    HAL_GPIO_WritePin(GPIOD, GREEN_LED_PIN, GPIO_PIN_RESET);
    // Wait for the next release period
    osDelay(3000); // 3000ms period
  }
}

/* System Clock Configuration */
void SystemClock_Config(void) {
  // Clock configuration here (already correct in your code)
}

/* GPIO Initialization */
static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitStruct.Pin = BLUE_LED_PIN | GREEN_LED_PIN | RED_LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  // Reset LEDs
  HAL_GPIO_WritePin(GPIOD, BLUE_LED_PIN | GREEN_LED_PIN | RED_LED_PIN, GPIO_PIN_RESET);
}
