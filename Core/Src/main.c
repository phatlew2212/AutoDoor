#include "main.h"

#include <string.h>
#include <stdio.h>

#include "MFRC522.h"
#include "LCDI2C.h"
#include "Keypad.h"

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi4;

TIM_HandleTypeDef htim2;



u_char status, cardstr[MAX_LEN+1];
u_char UID[5];
//u_char accesscardUID[5] = {0x53,0x17,0x82,0x13,0xD5};
u_char accesscardUID[5] = {0x53, 0x17, 0x82, 0x13, 0xD5};
u_char szBuff[100];

char Key = NULL_CHARACTER;

static int cardhere = 0;
static int defineaccescard = 0;
//static int cardherecnt = 0;
//static int defineaccescardtimer = 0;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI4_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void checkUserInput(void);
void userCard(void);
void userKeypad(void);
void DoorMechanism(void);

int main(void)
{

  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI4_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  LCD_initID(0x4E); //I2C Address
  LCD_Init(); //Initialization

  KEYPAD_Init();

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

	HAL_GPIO_WritePin(RC522_Rst_GPIO_Port, RC522_Rst_Pin, GPIO_PIN_SET);			//Disable reset (HIGH = no reset)
	HAL_Delay(100);																//Delay Function for int (has to be here)
	MFRC522_Init();																//Initialize the RFID Reader
	HAL_Delay(1000);																//Delay Function for int (has to be here)

	status = Read_MFRC522(VersionReg);											//Get information about current version


	while (1)
	{
    LCD_Clear();
    LCD_setCursor(0, 0);
    LCD_sendSTR("Insert Card or");
    LCD_setCursor(1, 0);
    LCD_sendSTR("Enter Password!!");
    LCD_setCursor(0, 0);
    LCD_sendSTR("Insert Card or");
    LCD_setCursor(1, 0);
    LCD_sendSTR("Enter Password!!");
    LCD_setCursor(0, 0);
    LCD_sendSTR("Insert Card or");
    LCD_setCursor(1, 0);
    LCD_sendSTR("Enter Password!!");
    LCD_setCursor(0, 0);
    LCD_sendSTR("Insert Card or");
    LCD_setCursor(1, 0);
    LCD_sendSTR("Enter Password!!");
    LCD_setCursor(0, 0);
    LCD_sendSTR("Insert Card or");
    LCD_setCursor(1, 0);
    LCD_sendSTR("Enter Password!!");
    LCD_setCursor(0, 0);
    LCD_sendSTR("Insert Card or");
    LCD_setCursor(1, 0);
    LCD_sendSTR("Enter Password!!");
    LCD_setCursor(0, 0);
    LCD_sendSTR("Insert Card or");
    LCD_setCursor(1, 0);
    LCD_sendSTR("Enter Password!!");
    LCD_setCursor(0, 0);
    LCD_sendSTR("Insert Card or");
    LCD_setCursor(1, 0);
    LCD_sendSTR("Enter Password!!");

    Key = KEYPAD_Read();
    if (Key != KEYPAD_NOT_PRESSED)
      if (Key == '#')
      {
        userKeypad();
      }

    status = MFRC522_FindCardNonBlock(cardstr);
    if (status == MI_OK)
      userCard();

    HAL_Delay(1);
	}
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }

}


static void MX_SPI4_Init(void)
{

  /* SPI4 parameter configuration*/
  hspi4.Instance = SPI4;
  hspi4.Init.Mode = SPI_MODE_MASTER;
  hspi4.Init.Direction = SPI_DIRECTION_2LINES;
  hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi4.Init.NSS = SPI_NSS_SOFT;
  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi4) != HAL_OK)
  {
    Error_Handler();
  }


}


static void MX_TIM2_Init(void)
{


  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};


  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 900-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000-1;
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
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_MspPostInit(&htim2);

}


static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, RC522_CS_Pin|RC522_Rst_Pin|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
  /*Configure GPIO pins : RC522_CS_Pin RC522_Rst_Pin PE8 PE9
                           PE10 PE11 */
  GPIO_InitStruct.Pin = RC522_CS_Pin|RC522_Rst_Pin|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : UserButton_Pin */
  GPIO_InitStruct.Pin = UserButton_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(UserButton_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PE12 PE13 PE14 PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void userCard(void)
{
  memcpy(UID, cardstr, 5); // Copy it into UID array (is 5 big)
  cardhere = 1;            // now card is here
  // set back read card number
  memset(cardstr, 0, MAX_LEN);

  if (!defineaccescard)
  {
    //    sprintf((char *)szBuff, "ID: 0x%02X%02X%02X%02X%02X", UID[0], UID[1], UID[2], UID[3], UID[4]);
    //      LCD_setCursor(1, 0);
    //      LCD_sendSTR((char *)szBuff);
    LCD_Clear();
    LCD_setCursor(0, 0);
    LCD_sendSTR("Card Detected!");
    HAL_Delay(1000);
    LCD_Clear();
    LCD_setCursor(0, 0);

    if ((UID[0] == accesscardUID[0]) &&
        (UID[1] == accesscardUID[1]) &&
        (UID[2] == accesscardUID[2]) &&
        (UID[3] == accesscardUID[3]) &&
        (UID[4] == accesscardUID[4]))
    {
  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
  	  HAL_Delay(100);
  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
  	  HAL_Delay(100);
  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
  	  HAL_Delay(100);
  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
  	  HAL_Delay(100);
  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
  	  HAL_Delay(100);
  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
  	  HAL_Delay(100);
      LCD_sendSTR("Access Granted!");
      DoorMechanism();
    }
    else
    {
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
    	HAL_Delay(500);
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
    	HAL_Delay(100);
      LCD_sendSTR("Access Denied!");
      HAL_Delay(2000);
    }
  }
}

void userKeypad(void)
{
  char inputPassword[5] = {0};   // Array to store 4-digit password, +1 for null terminator
  char AuthPassword[5] = "1111"; // Predefined authorized password
  int inputIndex = 0;

  LCD_Clear();
  LCD_setCursor(0, 0);
  LCD_sendSTR("Enter Password:");
  LCD_setCursor(1, 0);

  while (inputIndex < 4)
  {
    Key = KEYPAD_Read();

    if (Key != KEYPAD_NOT_PRESSED)
    {
      // Ensure the input is a digit
      if ((Key >= '0' && Key <= '9') || (Key >= 'A' && Key <= 'D'))
      {
        inputPassword[inputIndex] = Key;
        LCD_sendSTR("*"); // Display asterisk for each entered digit
        inputIndex++;
      }
    }
    HAL_Delay(100); // Debounce delay
  }

  inputPassword[4] = '\0'; // Null-terminate the input string

  HAL_Delay(500);

  LCD_Clear();
  LCD_setCursor(0, 0);

  // Compare the input password with the authorized password
  if (strcmp(inputPassword, AuthPassword) == 0)
  {
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
	  HAL_Delay(100);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	  HAL_Delay(100);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
	  HAL_Delay(100);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	  HAL_Delay(100);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
	  HAL_Delay(100);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	  HAL_Delay(100);
    LCD_sendSTR("Access Granted!");
    DoorMechanism();
  }
  else
  {
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
	      	HAL_Delay(500);
	      	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	      	HAL_Delay(100);
    LCD_sendSTR("Access Denied!");
    HAL_Delay(2000);
  }
}

void DoorMechanism(void)
{
  HAL_Delay(1000);

  LCD_Clear();
  LCD_setCursor(0, 0);
  LCD_sendSTR("Door opened.");
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 175);
  HAL_Delay(2000);

  LCD_Clear();
  LCD_setCursor(0, 0);
  LCD_sendSTR("Door closed.");
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 75);
  HAL_Delay(1000);
}

void Error_Handler(void)
{
	__disable_irq();
	while (1)
	{
	}
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif
