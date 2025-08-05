/*
 * osd.h
 *
 *  Created on: Jun 3, 2025
 *      Author: sokolov
 */

#ifndef INC_OSD_H_
#define INC_OSD_H_
#include "AT7456E_registers.h"

void osd_write(uint8_t reg, uint8_t data);
uint8_t osd_read(uint8_t address, uint8_t *out_data);
void osd_init(void);
void osd_clear_screen(void);
void osd_print_text(uint8_t *str, uint8_t x, uint8_t y);
void osd_enable(uint8_t control);
void osd_all_char(void);
void osd_upload_font();
void osd_print_custom_char(uint8_t x, uint8_t y, uint8_t char_index);
void osd_upload_custom_char(uint8_t ch, uint8_t* data);
void osd_write_one_char_to_font(uint8_t ch, const uint8_t* data);
#endif /* INC_OSD_H_ */
