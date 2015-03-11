
#include "stm32f10x.h"
#include "NGL.h"

LCD_Typedef *LCD_Struct;

#define GPIO_Pin_7_0          (uint16_t)0x00FF
#define GPIO_Pin_8_15         (uint16_t)0xFF00

void delay_ms(__IO uint32_t count);

int main(void)
{
	/* Configure the LCD Control pins */
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enabling clock for GPIO A,B,C */
	RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;
	RCC->APB2ENR|=RCC_APB2ENR_IOPCEN;

	/* LCD data port, PORTA.0-PORTA.7 Push-pull out 50ÌÃö */
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_7_0;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* LCD command signals */
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	/* LCD Init struct ---------------------------------------------*/
	LCD_Struct = NGL_LCD_GetController_FromIndex(__HX8352__);
	LCD_Struct->DataBusBits = 8;
	LCD_Struct->ColorBits = 16;
	LCD_Struct->ConnectionType = LCD_GPIO_Connect;

	/* LCD command signals init in library */
	LCD_Set_RESET_Pin(GPIOC, GPIO_Pin_4);
	LCD_Set_RS_Pin(GPIOC, GPIO_Pin_1);
	LCD_Set_WR_Pin(GPIOC, GPIO_Pin_2);
	LCD_Set_CS_Pin(GPIOC, GPIO_Pin_0);
	LCD_SetDataPort(GPIOA, GPIO_Pin_7_0);

	/* Reset LCD -----------------------------------------------------------------*/
	GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	delay_ms(50);
	GPIO_SetBits(GPIOC, GPIO_Pin_4);
	delay_ms(50);

	/* Initialization LCD controller and set delay_ms as delay function(for internal LCD library use only) */
	int8_t LCD_Init_Status =  NGL_LCD_ControllerInit(LCD_Struct, delay_ms);

	/* Verify init status */
	if(LCD_Init_Status != 0)
	{
		while(1)
		{
			// ERROR
		}
	}
	else
	{
		/**< Set rotation, clear screen */
//		NGL_LCD_SetRotation(_0_degree);
		NGL_LCD_Clear_AllScreen(0x0000);
	}

	NGL_GP_DrawFillRect(
			50,
			100,
			100,
			150,
			0xf800,
			DRAW,
			0xFFFF);

    while(1)
    {
    }
}


void delay_ms(__IO uint32_t count)
{
	__IO long cnt = count * 9000;
	while(cnt > 0) cnt--;
}
