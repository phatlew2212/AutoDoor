/**
 **************************************************************************************
 * @author
 * @file    Keypad.h
 * @brief   Header file for interfacing 4*4 Matrix Keypad with STM32F429ZI
 **************************************************************************************
**/ 


#ifndef Keypad_H
#define Keypad_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/*------------------------Define your GPIO pins here - START----------------------------*/

/** 
  Keypad	STM32F4xx	 Description      I/O

     R1			PE8				Row 1           Output
     R2			PE9				Row 2           Output
     R3			PE10			Row 3           Output
     R4			PE11			Row 4           Output
		 
     C1			PE12			Column 1        Input
     C2			PE13			Column 2        Input
     C3			PE14			Column 3        Input
     C4			PE15			Column 4        Input
**/


/* Rows */
/* Row 1 default */
#define KEYPAD_ROW_1_PORT			  GPIOE
#define KEYPAD_ROW_1_PIN			  GPIO_PIN_8

/* Row 2 default */
#define KEYPAD_ROW_2_PORT			  GPIOE
#define KEYPAD_ROW_2_PIN			  GPIO_PIN_9

/* Row 3 default */
#define KEYPAD_ROW_3_PORT			  GPIOE
#define KEYPAD_ROW_3_PIN			  GPIO_PIN_10

/* Row 4 default */
#define KEYPAD_ROW_4_PORT			  GPIOE
#define KEYPAD_ROW_4_PIN			  GPIO_PIN_11


/* Columns */
/* Column 1 default */
#define KEYPAD_COLUMN_1_PORT		GPIOE
#define KEYPAD_COLUMN_1_PIN			GPIO_PIN_12

/* Column 2 default */
#define KEYPAD_COLUMN_2_PORT		GPIOE
#define KEYPAD_COLUMN_2_PIN			GPIO_PIN_13

/* Column 3 default */
#define KEYPAD_COLUMN_3_PORT		GPIOE
#define KEYPAD_COLUMN_3_PIN			GPIO_PIN_14

/* Column 4 default */
#define KEYPAD_COLUMN_4_PORT		GPIOE
#define KEYPAD_COLUMN_4_PIN			GPIO_PIN_15

/* ROW port clock enable */
#define KEYPAD_ROW_1_PORT_CLK_ENABLE        __HAL_RCC_GPIOC_CLK_ENABLE()
#define KEYPAD_ROW_2_PORT_CLK_ENABLE        __HAL_RCC_GPIOC_CLK_ENABLE()
#define KEYPAD_ROW_3_PORT_CLK_ENABLE        __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEYPAD_ROW_4_PORT_CLK_ENABLE        __HAL_RCC_GPIOA_CLK_ENABLE()

/* COLUMN Port clock enable */
#define KEYPAD_COLUMN_1_PORT_CLK_ENABLE     __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEYPAD_COLUMN_2_PORT_CLK_ENABLE     __HAL_RCC_GPIOA_CLK_ENABLE() 
#define KEYPAD_COLUMN_3_PORT_CLK_ENABLE     __HAL_RCC_GPIOC_CLK_ENABLE() 
#define KEYPAD_COLUMN_4_PORT_CLK_ENABLE     __HAL_RCC_GPIOB_CLK_ENABLE() 


/*------------------------Define your GPIO pins here - END----------------------------*/
/* Keypad NOT pressed */
#define NULL_CHARACTER              '\0'
#define KEYPAD_NOT_PRESSED			NULL_CHARACTER 

/**
 * @brief  Initializes keypad functionality
 * @param  none
 * @retval None
 */
void KEYPAD_Init(void);

/**
 * @brief  Reads keypad data
 * @param  None
 * @retval Button status. This parameter will be a value of KEYPAD_Button_t enumeration
 */
char KEYPAD_Read(void);

#endif


