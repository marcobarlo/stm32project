#include "stm32f3xx.h"
//#include "stm32f30x.h"
#include "functions.h"
#include "dht11.h"
#include "LCD1602.h"

void portsStartup();
void devicesStartup();
void systemStartup();

//variabili globali
uint8_t response;         //variabili globali perché non posso farle locali alla interruzione se voglio utilizzare le stesse ogni volta
uint8_t data_dht11[5];
int8_t count_bytes = 4;
uint8_t integrity = 0, status=0;
int main()
{
	systemStartup();
	while (1)
	{
	}
}
//primo invio nella isr del pulsante, gli altri guidati dalla txie
/*void EXTI0_IRQHandler(void)
{
	uint8_t integrity;
	// GPIOE->MODER |= 0x5555<<16;
	// GPIOE->ODR |= 255<<8;
	EXTI->PR |= EXTI_PR_PR0;                                      // cancello pending request per segnalare l'interrupt servito
	response = handshake_dht11();
	if (response == 0)	//se il sensore ha risposto correttamentE
	{
		for(int i=0; i<5; i++)
			data_dht11[i] = read_data_dht11();
		dht_status = 0;
		integrity = check_payload(data_dht11[0], data_dht11[1], data_dht11[2], data_dht11[3], data_dht11[4]);
		//float temp= (float) T_int;
		if (integrity == 1)
		{
			count_bytes = 4;
			USART1_send(data_dht11[count_bytes]);
			USART1->CR1 |= USART_CR1_TXEIE;
		}
		else dht_status = 2;              //secondo bit alto se la comunicazione è avvenuta ma con errore di integrità
		//oh noooo!
	}
	else
		dht_status = 1;                       //primo bit alto se il sensore non completa l'handhsake
}*/
void portsStartup()             // pc4 pc5 usart, pa11 pa12 usart rts cts, pa0 bottone
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOEEN | RCC_AHBENR_GPIOCEN;             //abilito porta E per sensori, porta A per usart handshake, porta C per usart
	//moder 01 output, 00 input, 11 analog,10 alternate
	GPIOA->MODER &= ~(GPIO_MODER_MODER11_0 | GPIO_MODER_MODER12_0);                //alternate handshake
	GPIOE->MODER |= 0x5555<<16;			//led per effetti scenici
	GPIOA->MODER |= GPIO_MODER_MODER11_1 | GPIO_MODER_MODER12_1;
	GPIOA->AFR[1] &= ~(GPIO_AFRH_AFRH3 | GPIO_AFRH_AFRH4);      // il reset di porta a non è 0
	GPIOA->AFR[1] |= 7 << 12;             //afr[1] dovrebbe essere il registro per le linee da 9 a 16
	GPIOA->AFR[1] |= 7 << 16;             //alternate f for hanshake sempre funzione 7
	//GPIOA->MODER &= ~(GPIO_MODER_MODER0);       //input digitale per pa0, bottone
	GPIOC->MODER |=  GPIO_MODER_MODER5_1 | GPIO_MODER_MODER4_1;			//usart in alternate function
	GPIOC->AFR[0] |= 7<<20;						 //usart tx pc5 alternate f 7   afr[0] è il reg alternate function per linee da 0 a 8
	GPIOC->AFR[0] |= 7<<16;
}

void devicesStartup()
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_SYSCFGEN;                                //abilito usart1 e l'alimentazione per il sys config     apb2 full speed bus, apb1 36 mhz (ma il timer va sempre a 72)
	USART1->CR1 &= ~USART_CR1_M;   //azzero M per trasmissione di 8 bit
	USART1->BRR = 7500;                 //72000000 / 9600(bit/s) = 7500
	USART1->CR1 &= ~USART_CR1_OVER8;    //azzeriamo il bit over 8 per avere un sovracampionamento di 16
	//checksum?? potrei farlo a livello applicativo(mando la checksum come 5 byte) o a livello di byte(bit di parità)
	// USART1->CR1 |= USART_CR1_PCE;            //decommentare per attivare bit di parità
	// USART1->CR1 |= USART_CR1_PS;             //decommentare per parità dispari
	// USART1->CR1 &= ~USART_CR1_PS;            //decommentare per parità pari
	USART1->CR2 &= ~USART_CR2_STOP;              // 00 per 1 stop bit
	//USART1->CR3 |= USART_CR3_RTSE;              //abilito il flow control in ricezione

	USART1->CR1 |= USART_CR1_UE;                //abilitiamo la periferica
	USART1->CR1 |= USART_CR1_RE;                //abilitiamo la sezione del ricevitore
	USART1->CR1 |= USART_CR1_TE;                //abilitiamo la sezione del trasmettitore

	//config per interruzione di linea A0 (bottone)
	//SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0);    //per settare la porta a della linea 0
	NVIC->ISER[1] |= 1 << 5;      //linea 37... interruzione della uart
	//EXTI->IMR |= EXTI_IMR_MR0;    // smaschero linea 0
	//EXTI->RTSR |= EXTI_RTSR_TR0;   //abilito fronte di salita linea0
	//NVIC->ISER[0] |= 1 << 6;      //per linea 0
	//faccio in modo che la linea possa generare l'evento (smaschero) e accendo l nvic corrispondnete per farglielo ascoltare
	for(int i=0 ; i< 90000; i++);				//aspetto per mandare il sistema a regime
	USART1->RQR |= USART_RQR_RXFRQ;
	USART1->CR1 |= USART_CR1_RXNEIE;			//abilito a generare interruzioni alla ricezione
}
void systemStartup()
{
	portsStartup();
	//timer2Startup();
	devicesStartup();
}

void USART1_IRQHandler(void)
{
	if ((USART1->ISR &= USART_ISR_RXNE) == USART_ISR_RXNE)     //verificare che l'interruzione sia a causa di rxne
	{
		data_dht11[count_bytes--]=USART1->RDR;
		if(count_bytes<0)
		{
			count_bytes=4;
			status= (USART1->ISR & 0x0000000F);					// ci prendiamo gli ultimi 4 bit dell'isr per controllare errori di trasm
			integrity = check_payload(data_dht11[4], data_dht11[3], data_dht11[2], data_dht11[1], data_dht11[0]);
			GPIOE->ODR &= 0x00FF;			//abbasso i led di status
			GPIOE->ODR |= integrity<<8;		//bit 8 di ODR è integrità
			GPIOE->ODR |= status<<9;		//bit 9 in poi error vari
			if(integrity != 1)
				USART1->TDR = NACK;			//se il pacchetto non è integro mando un nack per il resend
		}

	}

}
