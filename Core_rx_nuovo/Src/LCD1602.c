#include "LCD1602.h"
#include "functions.h"
//#include "stm32f3xx_hal.h"

void writePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t PinState)
{
  if(PinState == 0)
  {
    //GPIOx->BSRR = (uint32_t)GPIO_Pin;
	  GPIOx->ODR &= ~GPIO_Pin;
  }
  else
  {
    //GPIOx->BRR = (uint32_t)GPIO_Pin;
	  GPIOx->ODR |= GPIO_Pin;
  }
}


void send_to_lcd (char data, uint8_t rs)
{
	writePin(RS_GPIO_Port, RS_Pin, rs);  // rs = 1 for data, rs=0 for command
	// write the data to the respective pin
	writePin(D7_GPIO_Port, D7_Pin, ((data>>3)&0x01));
	writePin(D6_GPIO_Port, D6_Pin, ((data>>2)&0x01));
	writePin(D5_GPIO_Port, D5_Pin, ((data>>1)&0x01));
	writePin(D4_GPIO_Port, D4_Pin, ((data>>0)&0x01));

	/* Toggle EN PIN to send the data
	 * if the HCLK > 100 MHz, use the  20 us delay
	 * if the LCD still doesn't work, increase the delay to 50, 80 or 100..
	 */
	writePin(EN_GPIO_Port, EN_Pin, 1);
	tim_delay(3600, tim);

	writePin(EN_GPIO_Port, EN_Pin, 0);
	tim_delay(3600, tim);
	//tempi stretchati oltre il limite descritto dal datasheet (nell'ordine dei nanosecondi...a cosa serve?)
}

void lcd_send_cmd (char cmd)
{
    char data_to_send;

    /* mandiamo prima i 4 bit più significativi */
    data_to_send = ((cmd>>4) & 0x0f);
    send_to_lcd(data_to_send,0);  // RS a 0 per mandare control byte

    /* poi i 4 meno sign*/
    data_to_send = ((cmd)&0x0f);
	send_to_lcd(data_to_send, 0);
}

void lcd_send_data (char data)
{
	char data_to_send;

    /* mandiamo prima i 4 bit più significativi */
	data_to_send = ((data>>4)&0x0f);
	send_to_lcd(data_to_send, 1);  // rs =1 per mandare dati (ref. datasheet)

    /* poi i 4 meno sign*/
	data_to_send = ((data)&0x0f);
	send_to_lcd(data_to_send, 1);
}

void lcd_clear (void)
{
	lcd_send_cmd(0x01); 			//ref datasheet per il comando 1
	tim_delay(144000, tim);				//delay di 2 ms (da datasheet sarebbero 1.53)
}

void lcd_put_cur(int row, int col)
{										//col individua indir base della colonna
    switch (row)
    {
        case 0:
            col |= 0x80;				//set ddram address command  (lo settiamo a 00)  indirizzo della prima colonna inizia da 00 a 27
            break;
        case 1:
            col |= 0xC0;				//set ddram address command (lo settiamo a 40) ... il primo indir della seconda colonna é 40
            break;
    }

    lcd_send_cmd (col);
}


void lcd_init (void)
{
	// 4 bit initialisation
	/*tim_delay(3600000, tim);		//wait for 50 ms (>40)
	lcd_send_cmd (0x30);							// 0x30 come comando = 0011 0000 ----> db5=1 set function, db4=1 4bit di interf, db3=0 2 lines ,db2=0 fontsize 5*11
	tim_delay(360000, tim);  		// wait for >4.1ms
	lcd_send_cmd (0x30);
	tim_delay(72000, tim);  			// wait for >100microsec
	lcd_send_cmd (0x30);
	tim_delay(720000, tim);
	lcd_send_cmd (0x28);  		//4bit
	tim_delay(720000, tim);
	//controlla queste cose strane
	// display initialisation
	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	tim_delay(72000, tim);
	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off con cursore e posizione del cursore spenta
	tim_delay(72000, tim);
	lcd_send_cmd (0x01);  // clear display
	tim_delay(72000, tim);
	tim_delay(72000, tim);
	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (il cursore incrementa verso destra) & S = 0 (no shift del display alla lettura)
	tim_delay(72000, tim);
	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)*/
	tim_delay(7200000, tim);
	//lcd_send_cmd (0x30); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	send_to_lcd(0x03, 0);
	tim_delay(360000, tim);
	send_to_lcd(0x03, 0);
	tim_delay(7200, tim);
	send_to_lcd(0x03, 0);
	tim_delay(7200, tim);
	send_to_lcd(0x02, 0);
	tim_delay(7200, tim);
	send_to_lcd(0x02, 0);
	send_to_lcd(0x08, 0);
	tim_delay(360000, tim);
	send_to_lcd(0x00,0);
	send_to_lcd(0x08,0);
	tim_delay(360000, tim);
	send_to_lcd(0x00,0);
	send_to_lcd(0x01,0);
	tim_delay(360000, tim);
	send_to_lcd(0x00,0);
	send_to_lcd(0x06,0);
	tim_delay(360000, tim);
	send_to_lcd(0x0C,0);
	tim_delay(360000, tim);
/*	send_to_lcd(0x00, 0);
	tim_delay(72000, tim);
	send_to_lcd(0x06, 0);
	send_to_lcd(0x0C, 0);*/
	/*lcd_send_cmd (0x30);
	lcd_send_cmd (0x28);
	lcd_send_cmd (0x08);
	lcd_send_cmd (0x01);
	lcd_send_cmd (0x06);
	lcd_send_cmd (0x0C);*/

}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);			//l'ultimo carattere della stringa é 0
}
