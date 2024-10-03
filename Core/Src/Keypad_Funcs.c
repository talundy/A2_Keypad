/*
 * Keypad_Funcs.c
 *
 *  Created on: Sep 29, 2024
 *      Author: root
 */


#include "Keypad_Funcs.h"
#include "stm32l4xx_hal.h"

#define POUND_PRESS 15
#define STAR_PRESS 14
#define NO_PRESS -1

// number of columns of keyboard size
#define KEYPAD_SIZE 3

/*
 * Configure GPIO pins for the keypad.
 * Set all columns = 1.
 */
void keypad_init(void){
	// Uses Port C[0-3] for columns, Port A[0-3] for rows. (4x4 matrix)

	//set clocks
	 RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);
	 RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOCEN);

	/*--- Set Port A pins 0-3 ---*/
	// Set MODER to input mode [00]
	GPIOA->MODER 	&= ~(GPIO_MODER_MODE0 |
						GPIO_MODER_MODE1 |
						GPIO_MODER_MODE2 |
						GPIO_MODER_MODE3);

	// Clear PUPDR bits
	GPIOA->PUPDR	&= ~(GPIO_PUPDR_PUPD0 |
						GPIO_PUPDR_PUPD1 |
						GPIO_PUPDR_PUPD2 |
						GPIO_PUPDR_PUPD3);  // Pull Down Resistor not working for my board
	// GPIOD->PUPDR set pull-down mode [10]
	GPIOA->PUPDR 	|= ((2 << GPIO_PUPDR_PUPD0_Pos) |
						(2 << GPIO_PUPDR_PUPD1_Pos) |
						(2 << GPIO_PUPDR_PUPD2_Pos) |
						(2 << GPIO_PUPDR_PUPD3_Pos));


	/*--- Set Port C pins 0-3 ---*/
	// Clear MODER bits
	GPIOC->MODER 	&= ~(GPIO_MODER_MODE0 |
						GPIO_MODER_MODE1 |
						GPIO_MODER_MODE2 |
						GPIO_MODER_MODE3);
	// Set bits to output mode [01]
	GPIOC->MODER 	|= ((1 << GPIO_MODER_MODE0_Pos) |
						(1 << GPIO_MODER_MODE1_Pos) |
						(1 << GPIO_MODER_MODE2_Pos) |
						(1 << GPIO_MODER_MODE3_Pos));
	// Set OTYPER to push-pull [0]
	GPIOC->OTYPER 	&= ~(GPIO_OTYPER_OT0 |
						GPIO_OTYPER_OT1 |
						GPIO_OTYPER_OT2 |
						GPIO_OTYPER_OT3);
	// Set OSPEEDR to high speed [11]
	GPIOC->OSPEEDR	|= (GPIO_OSPEEDR_OSPEED0 |
						GPIO_OSPEEDR_OSPEED1 |
						GPIO_OSPEEDR_OSPEED2 |
						GPIO_OSPEEDR_OSPEED3);
	// Set PUPDR to OFF [00]
	GPIOC->PUPDR 	&= ~(GPIO_PUPDR_PUPD0 |
						GPIO_PUPDR_PUPD1 |
						GPIO_PUPDR_PUPD2 |
						GPIO_PUPDR_PUPD3);
	// Set all column pins high
	GPIOC->ODR 	|= ((GPIO_ODR_OD0) |
					(GPIO_ODR_OD1) |
					(GPIO_ODR_OD2) |
					(GPIO_ODR_OD3));
	// done
}

/*
 * Input: row, col #. Output: integer corresponding to key press outlined in keypad_get_button().
 */
int8_t calculate_button(int8_t row, int8_t col){
	// for the 4x4 matrix #27899
	// assuming 0-3
	// returns -1 on invalid combo

	if (col == 0){
		switch(row){
		case 1: return 1;
		case 2: return 4;
		case 4: return 7;
		case 8: return STAR_PRESS; 	// '*'
		default: return NO_PRESS;
		}
	}
	else if(col == 1){
		switch(row){
		case 1: return 2;
		case 2: return 5;
		case 4: return 8;
		case 8: return 0;
		default: return NO_PRESS;
		}
	}
	else if(col == 2){
		switch(row){
		case 1: return 3;
		case 2: return 6;
		case 4: return 9;
		case 8: return POUND_PRESS; 	// '#'
		default: return NO_PRESS;
		}
	}
	else if (col == 3){ 	// A, B, C, or D
		switch(row){
		case 1: return 10;	// A
		case 2: return 11;	// B
		case 4: return 12;	// C
		case 8: return 13;	// D
		default: return NO_PRESS;
		}

	}
	else return NO_PRESS;
}

/*
 * Turn off all columns, except for one specified.
 * This function is only valid for Port C Pins 0-3
 */
void light_column(int8_t col){

	//turns all columns off temporarily
	GPIOC->ODR &= ~((GPIO_ODR_OD0) |
					(GPIO_ODR_OD1) |
					(GPIO_ODR_OD2) |
					(GPIO_ODR_OD3));

	switch (col){
	case 0:
		GPIOC->ODR |= (GPIO_ODR_OD0);
	 	break;
	case 1:
		GPIOC->ODR |= (GPIO_ODR_OD1);
		break;
	case 2:
		GPIOC->ODR |= (GPIO_ODR_OD2);
		break;
	case 3:
		GPIOC->ODR |= (GPIO_ODR_OD3);
		break;
	default: GPIOC->ODR |= ((GPIO_ODR_OD0) |
							(GPIO_ODR_OD1) |
							(GPIO_ODR_OD2) |
							(GPIO_ODR_OD3)); break; 	// error: lights all columns
	}
}

/*
 * Read keypad for button press.
 * Returns -1 if no button pressed, 0 - 9 for respective digit,
 * returns 11-14 for A-D, 15 for '*' and 16 for '#'.
 */
int8_t keypad_get_button(void){

	// if IDRs are not equal to zero
	//GPIO_IDR_ID0 | GPIO_IDR_ID1 | GPIO_IDR_ID2
	if((GPIOA->IDR & (GPIO_IDR_ID0 | GPIO_IDR_ID1 | GPIO_IDR_ID2 | GPIO_IDR_ID3))){

		int8_t col = 0;
		// keep column 0 on. Turn off 1-3.
		// IF
		//
		while (col < KEYPAD_SIZE){			// while in the valid range of columns
			light_column(col);		// light up the current column
									// check if IDRs are equal to zero

			int8_t row = GPIOA->IDR & (GPIO_IDR_ID0 | GPIO_IDR_ID1 | GPIO_IDR_ID2 | GPIO_IDR_ID3);

			if(row){
				// turn on all columns
				GPIOC->ODR 	|= ((1 << GPIO_ODR_OD0_Pos) |
									(1 << GPIO_ODR_OD1_Pos) |
									(1 << GPIO_ODR_OD2_Pos) |
									(1 << GPIO_ODR_OD3_Pos));
				// calculate & return correct value
					return calculate_button(row, col);

			} else {				// if IDRs = zero
				col++;				// move to next column
			}
		}
		// turn all columns back on, return zero

	// if IDRs are equal to zero at the start of this subroutine
	} else {
		GPIOC->ODR 	|= ((1 << GPIO_ODR_OD0_Pos) |
						(1 << GPIO_ODR_OD1_Pos) |
						(1 << GPIO_ODR_OD2_Pos) |
						(1 << GPIO_ODR_OD3_Pos));
		return NO_PRESS;
	}
	// this code should be unreachable.
	// end of while loop with no column press
	GPIOC->ODR 	|= ((1 << GPIO_ODR_OD0_Pos) |
							(1 << GPIO_ODR_OD1_Pos) |
							(1 << GPIO_ODR_OD2_Pos) |
							(1 << GPIO_ODR_OD3_Pos));
			return NO_PRESS;
}
