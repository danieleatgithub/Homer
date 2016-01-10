/*
 * winstar.h
 *
 *  Created on: 06/gen/2016
 *      Author: daniele
 */

#ifndef WINSTAR_H_
#define WINSTAR_H_
#include <stdint.h>


#define WINSTAR_I2C_ADD                 0x3e

// Clear Display & Home
#define CLEAR_DISPLAY_CMD               0x01
#define RETURN_HOME_CMD                 0x02

#define CONTRAST_DEFAULT                0x02
#define FOLLOWER_DEFAULT                0x07
#define CONTRAST_MAX					0x3f
union entry_mode_set_u {
	struct entry_mode_set_t {
		uint8_t shift:1;
		uint8_t versus:1;
		uint8_t code:6;
	}reg;
	unsigned char raw;
};

union display_mode_set_u {
	struct display_mode_set_t {
		uint8_t blink_on:1;
		uint8_t cursor_on:1;
		uint8_t display_on:1;
		uint8_t code:5;
	}reg;
	unsigned char raw;
};

union cursor_display_shift_u {
	struct cursor_display_shift_t {
		uint8_t reserved:2;
		uint8_t right_left:1;
		uint8_t screen_cursor:1;
		uint8_t code:4;
	}reg;
	unsigned char raw;
};

union function_set_u {
	struct function_set_t {
		uint8_t extended_instruction_set:1;
		uint8_t reserved:1;
		uint8_t double_height:1;
		uint8_t two_lines:1;
		uint8_t eight_bits:1;
		uint8_t code:3;
	}reg;
	unsigned char raw;
};

union bias_osc_frequency_adj_u {
	struct bias_osc_frequency_adjt_t {
		uint8_t osc_adj:3;
		uint8_t bias:1;
		uint8_t code:4;
	}reg;
	unsigned char raw;
};

union icon_ram_address_u {
	struct icon_ram_address_t {
		uint8_t address:4;
		uint8_t code:4;
	}reg;
	unsigned char raw;
};

union pow_icon_contrast_u {
	struct pow_icon_contrast_t {
		uint8_t contrast_high:2;
		uint8_t booster_on:1;
		uint8_t icon_display_on:1;
		uint8_t code:4;
	}reg;
	unsigned char raw;
};

union follower_u {
	struct follower_t {
		uint8_t v0_amplified_ratio:3;
		uint8_t follower_on:1;
		uint8_t code:4;
	}reg;
	unsigned char raw;
};

union contrast_set_u {
	struct contrast_set_t {
		uint8_t contrast_low:4;
		uint8_t code:4;
	}reg;
	unsigned char raw;
};

#endif /* WINSTAR_H_ */
