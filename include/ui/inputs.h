#ifndef UI_INPUTS_H
#define UI_INPUTS_H

#include <pthread.h>

enum key_event_id 
{
    KEY_LOWERCASE_Q = 'q',
    KEY_LOWERCASE_W = 'w',
    KEY_LOWERCASE_E = 'e',
    KEY_LOWERCASE_R = 'r',
    KEY_LOWERCASE_T = 't',
    KEY_LOWERCASE_Y = 'y',
    KEY_LOWERCASE_U = 'u',
    KEY_LOWERCASE_I = 'i',
    KEY_LOWERCASE_O = 'o',
    KEY_LOWERCASE_P = 'p',
    KEY_LOWERCASE_A = 'a',
    KEY_LOWERCASE_S = 's',
    KEY_LOWERCASE_D = 'd',
    KEY_LOWERCASE_F = 'f',
    KEY_LOWERCASE_G = 'g',
    KEY_LOWERCASE_H = 'h',
    KEY_LOWERCASE_J = 'j',
    KEY_LOWERCASE_K = 'k',
    KEY_LOWERCASE_L = 'l',
    KEY_LOWERCASE_Z = 'z',
    KEY_LOWERCASE_X = 'x',
    KEY_LOWERCASE_C = 'c',
    KEY_LOWERCASE_V = 'v',
    KEY_LOWERCASE_B = 'b',
    KEY_LOWERCASE_N = 'n',
    KEY_LOWERCASE_M = 'm',
    KEY_1 = '1',
    KEY_2 = '2',
    KEY_3 = '3',
    KEY_4 = '4',
    KEY_5 = '5',
    KEY_6 = '6',
    KEY_7 = '7',
    KEY_8 = '8',
    KEY_9 = '9',
    KEY_0 = '0',
    KEY_DASH = '-',
    KEY_EQUALS = '=',
    KEY_LEFT_SQUARE_BRACKET = '[',
    KEY_RIGHT_SQUARE_BRACKET = ']',
    KEY_BACKSLASH = '\\',
    KEY_SEMICOLON = ';',
    KEY_APOSTROPHE = '\'',
    KEY_COMMA = ',',
    KEY_PERIOD = '.',
    KEY_SLASH = '/',
    KEY_UP = 254,
    KEY_DOWN = 253,
    KEY_RIGHT = 252,
    KEY_LEFT = 251,
    KEY_UPPERCASE_Q = 'Q',
    KEY_UPPERCASE_W = 'W',
    KEY_UPPERCASE_E = 'E',
    KEY_UPPERCASE_R = 'R',
    KEY_UPPERCASE_T = 'T',
    KEY_UPPERCASE_Y = 'Y',
    KEY_UPPERCASE_U = 'U',
    KEY_UPPERCASE_I = 'I',
    KEY_UPPERCASE_O = 'O',
    KEY_UPPERCASE_P = 'P',
    KEY_UPPERCASE_A = 'A',
    KEY_UPPERCASE_S = 'S',
    KEY_UPPERCASE_D = 'D',
    KEY_UPPERCASE_F = 'F',
    KEY_UPPERCASE_G = 'G',
    KEY_UPPERCASE_H = 'H',
    KEY_UPPERCASE_J = 'J',
    KEY_UPPERCASE_K = 'K',
    KEY_UPPERCASE_L = 'L',
    KEY_UPPERCASE_Z = 'Z',
    KEY_UPPERCASE_X = 'X',
    KEY_UPPERCASE_C = 'C',
    KEY_UPPERCASE_V = 'V',
    KEY_UPPERCASE_B = 'B',
    KEY_UPPERCASE_N = 'N',
    KEY_UPPERCASE_M = 'M',
    KEY_UNDERSCORE = '_',
    KEY_PLUS_SIGN = '+',
    KEY_LEFT_CURLY_BRACKET = '{',
    KEY_RIGHT_CURLY_BRACKET = '}',
    KEY_VERTICAL_LINE = '|',
    KEY_COLON = ':',
    KEY_QUOTATION_MARK = '"',
    KEY_LEFT_ANGLED_BRACKET = '<',
    KEY_RIGHT_ANGLED_BRACKET = '>',
    KEY_QUESTION_MARK = '?',
    KEY_EXCLAMATION_MARK = '!',
    KEY_AT_SYMBOL = '@',
    KEY_HASH_SIGN = '#',
    KEY_DOLLAR_SIGN = '$',
    KEY_PERCENT_SIGN = '%',
    KEY_CARROT_SIGN = '^',
    KEY_AMPERSAND = '&',
    KEY_ASTERISK = '*',
    KEY_LEFT_PARENTHESIS = '(',
    KEY_RIGHT_PARENTHESIS = ')',
    KEY_NONE = -1
};

enum mouse_event_id
{
    MOUSE_LEFT_BUTTON_DOWN = 0,
    MOUSE_LEFT_BUTTON_UP = 1,
    MOUSE_RIGHT_BUTTON_DOWN = 2,
    MOUSE_RIGHT_BUTTON_UP = 3,
    MOUSE_SCROLL_DOWN = 65,
    MOUSE_SCROLL_UP = 64,
    MOUSE_MOVE = 36,
    MOUSE_LEFT_DRAG = 32,
    MOUSE_RIGHT_DRAG = 34,
    MOUSE_NONE = -1
};

struct key_event
{
    unsigned char is_key_pressed;
    enum key_event_id id;
};

struct mouse_event
{
    enum mouse_event_id id;
    size_t x;
    size_t y;
};

#define INPUT_FAILURE 0
#define INPUT_KEY_SUCCESS 1
#define INPUT_MOUSE_SUCCESS 2

int parse_key_input(struct key_event *key_event);

int parse_mouse_input(struct mouse_event *mouse_event);

int parse_key_mouse_input(struct key_event *key_event, struct mouse_event *mouse_event);

#endif