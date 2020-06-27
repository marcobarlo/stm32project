/*
 * lcd1602.h
 *
 *  Created on: oggi 20/06/2023 ritorno al futuro
 *      Author: ANDREA E MARCO XD
 */

#ifndef INC_LCD1602_H_
#define INC_LCD1602_H_
#include "functions.h"

//hal_gpio ripoff
#define GPIO_PIN_0                 ((uint16_t)0x0001U)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002U)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004U)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008U)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010U)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020U)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040U)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080U)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint16_t)0x0100U)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint16_t)0x0200U)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint16_t)0x0400U)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint16_t)0x0800U)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint16_t)0x1000U)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000U)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000U)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000U)  /* Pin 15 selected   */
#define GPIO_PIN_All               ((uint16_t)0xFFFFU)  /* All pins selected */



//rinomina quando scegliamo i pin
#define RS_Pin GPIO_PIN_2
#define RS_GPIO_Port GPIOE
#define RW_Pin GPIO_PIN_1				//non viene usato perché di fatto non leggiamo mai, scriviamo solo
#define RW_GPIO_Port GPIOE
#define EN_Pin GPIO_PIN_0
#define EN_GPIO_Port GPIOE
#define D4_Pin GPIO_PIN_0
#define D4_GPIO_Port GPIOC
#define D5_Pin GPIO_PIN_1
#define D5_GPIO_Port GPIOC
#define D6_Pin GPIO_PIN_2
#define D6_GPIO_Port GPIOC
#define D7_Pin GPIO_PIN_3
#define D7_GPIO_Port GPIOC





#define tim TIM2				//usiamo timer 2 per i delay

void lcd_init (void);   // initialize lcd

void lcd_send_cmd (char cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear (void);

#endif /* INC_LCD1602_H_ */
