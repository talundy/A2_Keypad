/*
 * Keypad_Funcs.h
 *
 *  Created on: Sep 29, 2024
 *      Author: root
 */

#ifndef INC_KEYPAD_FUNCS_H_
#define INC_KEYPAD_FUNCS_H_


/*
 * Configure GPIO pins for the keypad.
 * Set all cols = 1.
 */
void keypad_init(void);

/*
 * Turn off all columns, except for one specified.
 */
void light_column(int8_t col);


/*
 * Read keypad for button press.
 * Returns -1 if no button pressed, 0 - 9 for respective digit,
 * returns 11-14 for A-D, 15 for '*' and 16 for '#'.
 */
int8_t keypad_get_button(void);

/*
 * Input: row, col #. Output: integer corresponding to key press outlined above.
 */
int8_t calculate_button(int8_t row, int8_t col);



#endif /* INC_KEYPAD_FUNCS_H_ */
