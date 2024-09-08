#include "ui/inputs.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int parse_key_input(struct key_event *key_event)
{
    char start = getchar();

    if (start == 033) // escape sequence
    {
        char buf[2];
        buf[0] = getchar();
        buf[1] = getchar();
        if (buf[0] != '[') return INPUT_FAILURE;
        switch (buf[1])
        {
            case 'A':
            {
                key_event->is_key_pressed = 1;
                key_event->id = KEY_UP;
                return INPUT_KEY_SUCCESS;
            }
            case 'B':
            {
                key_event->is_key_pressed = 1;
                key_event->id = KEY_DOWN;
                return INPUT_KEY_SUCCESS;
            }
            case 'C':
            {
                key_event->is_key_pressed = 1;
                key_event->id = KEY_RIGHT;
                return INPUT_KEY_SUCCESS;
            }
            case 'D':
            {
                key_event->is_key_pressed = 1;
                key_event->id = KEY_LEFT;
                return INPUT_KEY_SUCCESS;
            }
            default:
            {
                return INPUT_FAILURE;
            }
        }
    }
    else
    {
        key_event->is_key_pressed = 1;
        key_event->id = start;
        return INPUT_KEY_SUCCESS;
    }
}

#define CURSOR_STDIN_BUFFER_SIZE 16

int parse_mouse_input(struct mouse_event *mouse_event)
{
    char buf[CURSOR_STDIN_BUFFER_SIZE];
    if (read(STDIN_FILENO, buf, CURSOR_STDIN_BUFFER_SIZE) == -1) return INPUT_FAILURE;

    int lower_m = 0;
    char *begin = strstr(buf, "[<");
    if (!begin) return INPUT_FAILURE;
    char *end = begin + 2; // account for '\033' and '[' and '<'
    while (1)
    {
        ++end;
        if (end >= buf + CURSOR_STDIN_BUFFER_SIZE) return INPUT_FAILURE;
        if (*end == 'm')
        {
            lower_m = 1;
            break;
        } else if (*end == 'M') break;
    }
    *end = 0; // create null term string from [begin, end)

    size_t id, x, y;
    if (sscanf(begin, "[<%lu;%lu;%lu", &id, &x, &y) != 3) return INPUT_FAILURE;

    mouse_event->id = id + lower_m;
    mouse_event->x = x;
    mouse_event->y = y;

    return INPUT_MOUSE_SUCCESS;
}

/* int parse_key_mouse_input(struct key_event *key_event, struct mouse_event *mouse_event)
{

    __fpurge(stdin);
    char start = getchar();

    if (start == 033) // escape sequence
    {
        char buf[CURSOR_STDIN_BUFFER_SIZE];
        if (read(STDIN_FILENO, buf, CURSOR_STDIN_BUFFER_SIZE) == -1) return INPUT_FAILURE;

        if (buf[1] != '[') return INPUT_FAILURE;        
        switch (buf[2])
        {
            case 'A':
            {
                key_event->is_key_pressed = 1;
                key_event->id = KEY_UP;
                return INPUT_KEY_SUCCESS;
            }
            case 'B':
            {
                key_event->is_key_pressed = 1;
                key_event->id = KEY_DOWN;
                return INPUT_KEY_SUCCESS;
            }
            case 'C':
            {
                key_event->is_key_pressed = 1;
                key_event->id = KEY_RIGHT;
                return INPUT_KEY_SUCCESS;
            }
            case 'D':
            {
                key_event->is_key_pressed = 1;
                key_event->id = KEY_LEFT;
                return INPUT_KEY_SUCCESS;
            }
            case '<':
            {
                char *begin = &buf[3];
                char *end = begin;
                int button_down = 0;
                while (1)
                {
                    if (end >= buf + CURSOR_STDIN_BUFFER_SIZE) return INPUT_FAILURE;
                    if (*end == 'M') 
                    {
                        button_down = 1;
                        break;
                    } else if (*end == 'm') break;
                    ++end;
                }
                *end = 0;

                size_t id, x, y;
                if (sscanf(begin, "%lu;%lu;%lu", &id, &x, &y) != 3) return INPUT_FAILURE;
                
                mouse_event->id = id + button_down;
                mouse_event->x = x;
                mouse_event->y = y;

                // wprintf(L"SUCCESS\n");

                return INPUT_MOUSE_SUCCESS;
            }
            default:
            {
                return INPUT_FAILURE;
            }
        }
    }
    else
    {
        key_event->is_key_pressed = 1;
        key_event->id = start;
        return INPUT_KEY_SUCCESS;
    }
} */

int parse_key_mouse_input(struct key_event *key_event, struct mouse_event *mouse_event)
{
    char start = getchar();

    if (start == 033) // escape sequence
    {
        char buf[CURSOR_STDIN_BUFFER_SIZE];
        if (read(STDIN_FILENO, buf, CURSOR_STDIN_BUFFER_SIZE) == -1) return INPUT_FAILURE;

        if (buf[1] != '[') return INPUT_FAILURE;

        switch (buf[2])
        {
            case '<':
            {
                char *begin = &buf[3];
                char *end = begin;
                int lower_m = 0;
                while (1)
                {
                    if (end >= buf + CURSOR_STDIN_BUFFER_SIZE) return INPUT_FAILURE;
                    if (*end == 'm') 
                    {
                        lower_m = 1;
                        break;
                    } else if (*end == 'M') break;
                    ++end;
                }
                *end = 0;

                size_t id, x, y;
                if (sscanf(begin, "%lu;%lu;%lu", &id, &x, &y) != 3) return INPUT_FAILURE;
                
                mouse_event->id = id + lower_m;
                mouse_event->x = x;
                mouse_event->y = y;

                return INPUT_MOUSE_SUCCESS;
            }
            case 'A':
            {
                key_event->is_key_pressed = 1;
                key_event->id = KEY_UP;
                return INPUT_KEY_SUCCESS;
            }
            case 'B':
            {
                key_event->is_key_pressed = 1;
                key_event->id = KEY_DOWN;
                return INPUT_KEY_SUCCESS;
            }
            case 'C':
            {
                key_event->is_key_pressed = 1;
                key_event->id = KEY_RIGHT;
                return INPUT_KEY_SUCCESS;
            }
            case 'D':
            {
                key_event->is_key_pressed = 1;
                key_event->id = KEY_LEFT;
                return INPUT_KEY_SUCCESS;
            }
            default:
            {
                return INPUT_FAILURE;
            }
        }
    }
    else
    {
        key_event->is_key_pressed = 1;
        key_event->id = start;
        return INPUT_KEY_SUCCESS;
    }
}