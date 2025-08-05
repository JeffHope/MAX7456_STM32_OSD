#include "main.h"
#include "osd.h"
#include "AT7456E_registers.h"
#include "max7456font.h"
//-- DEFINITIONS --------------------------------------------------------------
#define OSD_CS_PORT GPIOD
#define OSD_CS_PIN	GPIO_PIN_2

#define CS_HIGH() 	{	HAL_GPIO_WritePin(OSD_CS_PORT, OSD_CS_PIN, GPIO_PIN_SET);  }
#define CS_LOW()	{	HAL_GPIO_WritePin(OSD_CS_PORT, OSD_CS_PIN, GPIO_PIN_RESET);  }

#define VM0_VIDEO_SELECT_PAL_BIT (1 << 6)
#define VM0_VIDEO_SELECT_NTSC_BIT (0 << 6)
#define VM0_VIDEO_RESET_BIT (1 << 1)

#define WRITE_NVR        0xA0
#define END_STRING       0xFF
#define STATUS_REG_BUSY  0x20
#define NVM_RAM_SIZE     0x36

#define OSD_ENABLE		 0x01
#define OSD_DISABLE		 0x00
const uint8_t custom_char_T[54] = {
    // Каждый байт кодирует 4 пикселя (2 бита на пиксель: 00-черный, 11-белый)
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // Верхние строки (полностью белые)
    0x3F, 0xFC, 0x3F, 0xFC, 0x3F, 0xFC,  // Вертикальные линии
    0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0,
    0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Пустые строки
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
//-- VARIABLES ----------------------------------------------------------------
extern SPI_HandleTypeDef hspi3;
uint8_t rb_buffer[] = {RB0,RB1,RB2,RB3,RB4,RB5,RB6,RB7,RB8,RB9,RB10,RB11,RB12,RB13,RB14,RB15};
uint8_t rx_buff[10] = {0};
//-- FUNCTIONS ----------------------------------------------------------------
void osd_write(uint8_t reg, uint8_t data)
{
	uint8_t transfer_buffer[2] = {0};
	transfer_buffer[0] = reg;
	transfer_buffer[1] = data;
	CS_LOW();
	HAL_SPI_Transmit(&hspi3, transfer_buffer, 2, HAL_MAX_DELAY);
	CS_HIGH();
}
void osd_upload_custom_char(uint8_t ch, uint8_t* data)
{
	osd_enable(OSD_DISABLE);
	for (uint8_t i = 0; i < 54; i++)
	{
		osd_write(WRITE_ADDRESS(CMAL),i);
		osd_write(WRITE_ADDRESS(CMAL),data[i]);
	}
	osd_write(WRITE_ADDRESS(CMAH),ch);
	osd_write(WRITE_ADDRESS(CMM),0xA0);
	HAL_Delay(15);
	osd_enable(OSD_ENABLE);
}
void osd_print_custom_char(uint8_t x, uint8_t y, uint8_t char_index) {
    uint16_t pos = y * 30 + x;
    osd_write(WRITE_ADDRESS(DMAH), (pos >> 8) & 0x01);
    osd_write(WRITE_ADDRESS(DMAL), pos & 0xFF);
    osd_write(WRITE_ADDRESS(DMDI), char_index);
}
void osd_all_char(void)
{
	osd_write(WRITE_ADDRESS(DMM), 0x04);
	HAL_Delay(1);
	osd_write(WRITE_ADDRESS(DMM), 0x01);
	osd_write(WRITE_ADDRESS(DMAH), 0x00);
	osd_write(WRITE_ADDRESS(DMAL), 0x21);
	for (uint8_t i = 0; i < 255; i++)
	{
		osd_write(WRITE_ADDRESS(DMDI), i);
		if ((i%24) == 23)
		{
			for (uint8_t j = 0; j < 6; j++)
			{
				osd_write(WRITE_ADDRESS(DMDI), 0x00);
			}
		}
	}
	osd_write(WRITE_ADDRESS(DMDI), 0xFF);
}
void osd_wait_writeNVM()
{
	uint8_t temp_buffer[1] = {0};

	while (osd_read(READ_ADDRESS(STAT),temp_buffer) & STATUS_REG_BUSY);
}
void osd_write_one_char_to_font(uint8_t ch, uint8_t* data)
{
	osd_enable(OSD_ENABLE);
	while(osd_read(READ_ADDRESS(STAT), rx_buff) & STATUS_REG_BUSY);
	osd_write(WRITE_ADDRESS(CMAH),ch);
	for (uint8_t i = 0; i < NVM_RAM_SIZE; i++)
	{
		osd_write(WRITE_ADDRESS(CMAL),i);
		osd_write(WRITE_ADDRESS(CMDI),data[i]);
	}
	osd_write(WRITE_ADDRESS(CMM),WRITE_NVR);
	osd_wait_writeNVM();
	osd_enable(OSD_DISABLE);
}
void osd_upload_font()
{
	for (uint16_t i = 0; i < 256; i++)
	{
		osd_write_one_char_to_font(i, &fontdata[i * 64]);
		HAL_Delay(15);
	}
}
uint8_t osd_read(uint8_t address, uint8_t *out_data) {
    uint8_t tx[2] = {0};
    tx[0] = address;
    tx[1] = 0x00;
    CS_LOW();
    HAL_SPI_Transmit(&hspi3, tx, 2, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi3, out_data, 2, HAL_MAX_DELAY);
//    HAL_SPI_TransmitReceive(&hspi3, tx, out_data, 1, 0);
    CS_HIGH();
    return out_data[0];
}
void osd_init()
{
	osd_write(WRITE_ADDRESS(VM0),0x42);
	HAL_Delay(500);
	for (uint8_t i = 0; i < 16; i++)
	{
		osd_write(WRITE_ADDRESS(rb_buffer[i]), 0x05);
	}
	osd_write(WRITE_ADDRESS(VM0),0x44);
}
void osd_enable(uint8_t control)
{
	if (control)
	{
		osd_write(WRITE_ADDRESS(VM0),0x0C);
	} else
	{
		osd_write(WRITE_ADDRESS(VM0),0x00);
	}
}
void osd_ext_video(uint8_t control)
{
	if (!control)
	{
		osd_write(WRITE_ADDRESS(VM0),0x7C);
	}
}
void osd_clear_screen()
{

	osd_write(WRITE_ADDRESS(DMM), 0x04);
	//add waiting cleaning
	HAL_Delay(100);
}

void osd_print_text(uint8_t *str, uint8_t x, uint8_t y)
{

    uint16_t pos = y * 30 + x;
    osd_write(WRITE_ADDRESS(DMM), 0x01);

    osd_write(WRITE_ADDRESS(DMAH), ((pos >> 8) & 0x01) | (0 << 1));
    osd_write(WRITE_ADDRESS(DMAL), pos & 0xFF);

    while(*str) {
        osd_write(WRITE_ADDRESS(DMDI), *str);
        str++;
    }
    osd_write(WRITE_ADDRESS(DMDI), 0xFF);
    osd_write(WRITE_ADDRESS(DMM), 0x00);

}


