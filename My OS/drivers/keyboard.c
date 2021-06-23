#include "keyboard.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "display.h"
#include "../kernel/util.h"

void print_letter(uint8_t scancode)
{
    switch (scancode)
    {
    case 0x0:
        print_string("ERROR");
        break;
    case 0x1:
        print_string("ESC");
        break;
    case 0x2:
        print_string("1");
        break;
    case 0x3:
        print_string("2");
        break;
    case 0x4:
        print_string("3");
        break;
    case 0x5:
        print_string("4");
        break;
    case 0x6:
        print_string("5");
        break;
    case 0x7:
        print_string("6");
        break;
    case 0x8:
        print_string("7");
        break;
    case 0x9:
        print_string("8");
        break;
    case 0x0A:
        print_string("9");
        break;
    case 0x0B:
        print_string("0");
        break;
    case 0x0C:
        print_string("-");
        break;
    case 0x0D:
        print_string("+");
        break;
    case 0x0E:
        print_string("Backspace");
        break;
    case 0x0F:
        print_string("Tab");
        break;
    case 0x10:
        print_string("Q");
        break;
    case 0x11:
        print_string("W");
        break;
    case 0x12:
        print_string("E");
        break;
    case 0x13:
        print_string("R");
        break;
    case 0x14:
        print_string("T");
        break;
    case 0x15:
        print_string("Y");
        break;
    case 0x16:
        print_string("U");
        break;
    case 0x17:
        print_string("I");
        break;
    case 0x18:
        print_string("O");
        break;
    case 0x19:
        print_string("P");
        break;
    case 0x1A:
        print_string("[");
        break;
    case 0x1B:
        print_string("]");
        break;
    case 0x1C:
        print_string("ENTER");
        break;
    case 0x1D:
        print_string("LCtrl");
        break;
    case 0x1E:
        print_string("A");
        break;
    case 0x1F:
        print_string("S");
        break;
    case 0x20:
        print_string("D");
        break;
    case 0x21:
        print_string("F");
        break;
    case 0x22:
        print_string("G");
        break;
    case 0x23:
        print_string("H");
        break;
    case 0x24:
        print_string("J");
        break;
    case 0x25:
        print_string("K");
        break;
    case 0x26:
        print_string("L");
        break;
    case 0x27:
        print_string(";");
        break;
    case 0x28:
        print_string("'");
        break;
    case 0x29:
        print_string("`");
        break;
    case 0x2A:
        print_string("LShift");
        break;
    case 0x2B:
        print_string("\\");
        break;
    case 0x2C:
        print_string("Z");
        break;
    case 0x2D:
        print_string("X");
        break;
    case 0x2E:
        print_string("C");
        break;
    case 0x2F:
        print_string("V");
        break;
    case 0x30:
        print_string("B");
        break;
    case 0x31:
        print_string("N");
        break;
    case 0x32:
        print_string("M");
        break;
    case 0x33:
        print_string(",");
        break;
    case 0x34:
        print_string(".");
        break;
    case 0x35:
        print_string("/");
        break;
    case 0x36:
        print_string("Rshift");
        break;
    case 0x37:
        print_string("Keypad *");
        break;
    case 0x38:
        print_string("LAlt");
        break;
    case 0x39:
        print_string("Space");
        break;
    default:
        print_string("Unknown key");
        break;
    }
}

static void keyboard_callback(registers_t *regs)
{
    uint8_t scancode = port_byte_in(0x60);
    print_letter(scancode);
    print_nl();
}

uint8_t keyboard_key_return()
{
    uint8_t scancode = port_byte_in(0x60);
    return scancode;
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
}

void status_control()
{
    int inf = 1;
    while (inf)
    {
        uint8_t input = keyboard_key_return();
        if (input == 0x1) // Esc
        {
            port_byte_out(0x60, 0x0);
            break;
        }
        if (input == 0x1c) // Enter key
        {
            int offset = get_cursor();
            uint8_t *vidmem = (uint8_t *)VIDEO_ADDRESS;
            char character = vidmem[offset];
            if (character == '-')
            {
                int row = get_offset_row(offset);
                int col = get_offset_col(offset);
                col = col - 10;
                int new_offset = get_offset(col, row);
                char new_character = vidmem[new_offset];
                if (new_character == 'N')
                {
                    col = col - 1;
                    print_string_at("OFF", col, row);
                }
                else
                {
                    col = col - 1;
                    print_string_at("ON ", col, row);
                }
                set_cursor(offset);
            }
            port_byte_out(0x60, 0x0);
        }
        if (input == 0x48) // UP arrow
        {
            int offset = get_cursor();
            int row = get_offset_row(offset);
            if (row)
            {
                row = row - 1;
            }
            int col = get_offset_col(offset);
            int new_offset = get_offset(col, row);
            set_cursor(new_offset);
            port_byte_out(0x60, 0x0);
        }
        if (input == 0x50) // DOWN arrow
        {
            int offset = get_cursor();
            int row = get_offset_row(offset);
            if (row != 24)
            {
                row = row + 1;
            }
            int col = get_offset_col(offset);
            int new_offset = get_offset(col, row);
            set_cursor(new_offset);
            port_byte_out(0x60, 0x0);
        }
        if (input == 0x4b) // LEFT arrow
        {
            int offset = get_cursor();
            int row = get_offset_row(offset);
            int col = get_offset_col(offset);
            if (col)
            {
                col = col - 1;
            }
            int new_offset = get_offset(col, row);
            set_cursor(new_offset);
            port_byte_out(0x60, 0x0);
        }
        if (input == 0x4d) // RIGHT arrow
        {
            int offset = get_cursor();
            int row = get_offset_row(offset);
            int col = get_offset_col(offset);
            if (col != 79)
            {
                col = col + 1;
            }
            int new_offset = get_offset(col, row);
            set_cursor(new_offset);
            port_byte_out(0x60, 0x0);
        }
    }
}

int summary_control()
{
    int inf = 1;
    int flag;
    port_byte_out(0x60, 0x0);
    while (inf)
    {
        uint8_t input = keyboard_key_return();
        if (input == 0x1) // Esc
        {
            port_byte_out(0x60, 0x0);
            flag = 0;
            break;
        }
        if (input == 0x1c) // Enter key
        {
            port_byte_out(0x60, 0x0);
            flag = 1;
            break;
        }
        if (input == 0x21) // F key
        {
            port_byte_out(0x60, 0x0);
            flag = 2;
            break;
        }
    }
    return flag;
}

int get_int()
{
    port_byte_out(0x60, 0x0);
    int start_offset = get_cursor();
    int start_col = get_offset_col(start_offset);
    uint8_t key[10] = {0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0A, 0x0B};
    char *val_char[10] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    int val_int[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    int ret_val = 0;
    int inf = 1;
    int i;
    while (inf)
    {
        uint8_t input = keyboard_key_return();
        for (i = 0; i < 10; i++)
        {
            if (input == key[i]) // 0-9 keys
            {
                port_byte_out(0x60, 0x0);
                print_string(val_char[i]);
                ret_val = (ret_val * 10) + val_int[i];
            }
        }
        if (input == 0x1C) // ENTER key
        {
            port_byte_out(0x60, 0x0);
            print_nl();
            return ret_val;
        }
        if (input == 0x0E) // Backspace key
        {
            port_byte_out(0x60, 0x0);
            ret_val = ret_val / 10;
            int offset = get_cursor();
            int row = get_offset_row(offset);
            int col = get_offset_col(offset);
            if (col)
            {
                col = col - 1;
            }
            int new_offset = get_offset(col, row);
            if (col == start_col - 1)
                ;
            else
            {
                set_cursor(new_offset);
                set_char_at_video_memory(' ', new_offset);
            }
        }
    }
}

void esc_to_exit()
{
    int inf = 1;
    while (inf)
    {
        uint8_t input = keyboard_key_return();
        if (input == 0x1) // Esc
        {
            port_byte_out(0x60, 0x0);
            break;
        }
    }
}

void enter_to_contn()
{
    int inf = 1;
    while (inf)
    {
        uint8_t input = keyboard_key_return();
        if (input == 0x1C) // Enter Key
        {
            port_byte_out(0x60, 0x0);
            break;
        }
    }
}
