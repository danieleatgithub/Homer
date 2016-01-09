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

// Display ON/OFF 0x0
#define DISPLAY_CMD_CTRL                0x08
#define DISPLAY_ON                              0x04
#define DISPLAY_CURSOR                  0x02
#define DISPLAY_CURSOR_BLINK    0x01

// Bias frequency 0x1
#define BIAS_FREQ_CMD                   0x10
#define BIAS_FREQ_1_4                   0x08 // ”High”, 1/4 Low, 1/5
#define BIAS_FREQ_3V_DEFAULT    4        // Default frequency 183 Hz
#define BIAS_FREQ_5V_DEFAULT    4        // Default frequency 192 Hz

// Function Set 0x2
#define FUNCTION_SET_CMD                0x20 // bit 5
#define FUNCTION_SET_DL                 0x10 // bit 4 "High", it means 8-bit bus mode with MPU
#define FUNCTION_SET_2LINES             0x08 // bit 3 High", 2-line display mode is set.
#define FUNCTION_SET_DOUBLE             0x04 // bit 2 Double height font type control bit
#define FUNCTION_SET_EXTENS             0x01 // bit 0 High”, extension instruction be selected (refer extension instruction table)

// Contrast Set 0x7
#define CONTRAST_SET_CMD                0x70 //
#define CONTRAST_DEFAULT                0x02
#define CONTRAST_MASK                   0x0f // Low nibble for value
#define CONTRAST_MAX                    0x3f

// Power Icon 0x5
#define POWER_ICON_CMD                  0x50
#define POWER_ICON_ON                   0x08 // High", ICON display on
#define POWER_ICON_BOOST                0x04 // High", booster circuit is turn on. (for 3.3 V VDD)
#define POWER_ICON_CONTR_C5             0x02 // C5 High bit for contrast value
#define POWER_ICON_CONTR_C4             0x01 // C4 High bit for contrast value
#define POWER_ICON_CONTR_MASK   0x03

// Follower Control 0x6
#define FOLLOWER_CMD                    0x60
#define FOLLOWER_ON                             0x08 // "High", internal follower circuit is turn on.
#define FOLLOWER_DEFAULT                0x07
#define FOLLOWER_MASK                   0x07
#define FOLLOWER_MAX                    0x07

// Set DDRAM Address
#define DDRAM_ADD_CMD                  0x80

union entry_mode_set_u {
	struct entry_mode_set_t {
		uint8_t shift:1;
		uint8_t versus:1;
		uint8_t code:6;
	}reg;
	uint8_t raw;
};

union display_mode_set_u {
	struct display_mode_set_t {
		uint8_t blink_on:1;
		uint8_t cursor_on:1;
		uint8_t display_on:1;
		uint8_t code:5;
	}reg;
	uint8_t raw;
};

union cursor_display_shift_u {
	struct cursor_display_shift_t {
		uint8_t reserved:2;
		uint8_t right_left:1;
		uint8_t screen_cursor:1;
		uint8_t code:4;
	}reg;
	uint8_t raw;
};

union function_set_u {
	struct function_set_t {
		uint8_t extended_instruction_set:1;
		uint8_t reserved:1;
		uint8_t double_height:1;
		uint8_t two_lines:1;
		uint8_t eight_bits:1;
		uint8_t code:4;
	}reg;
	uint8_t raw;
};

union bias_osc_frequency_adj_u {
	struct bias_osc_frequency_adjt_t {
		uint8_t osc_adj:3;
		uint8_t bias:1;
		uint8_t code:4;
	}reg;
	uint8_t raw;
};

union icon_ram_address_u {
	struct icon_ram_address_t {
		uint8_t address:4;
		uint8_t code:4;
	}reg;
	uint8_t raw;
};

union pow_icon_contrast_u {
	struct pow_icon_contrast_t {
		uint8_t contrast_high:2;
		uint8_t booster_on:1;
		uint8_t icon_display_on:1;
		uint8_t code:4;
	}reg;
	uint8_t raw;
};

union follower_u {
	struct follower_t {
		uint8_t v0_amplified_ratio:3;
		uint8_t follower_on:1;
		uint8_t code:4;
	}reg;
	uint8_t raw;
};

union contrast_set_u {
	struct contrast_set_t {
		uint8_t contrast_low:4;
		uint8_t code:4;
	}reg;
	uint8_t raw;
};

#endif /* WINSTAR_H_ */
