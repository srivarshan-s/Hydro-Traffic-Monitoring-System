#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/display.h"
#include "../drivers/keyboard.h"

#include "util.h"

void status()
{
    int i, horizntal_offset = 16;
    int loc_tank = 11, loc_level = 11, loc_status = 11, loc_manual = 11;
    char *tank_no[5] = {"Tank1", "Tank2", "Tank3", "Tank4", "Tank5"};
    char *level[5] = {"Full", "Empty", "Full", "Half", "Half"};
    char *status[5] = {"OFF", "ON", "OFF", "ON", "ON"};
    char *manual[5] = {"-", "-", "-", "-", "-"};
    clear_screen();
    print_string_at("All Motor Status", 30, 5);
    print_string_at("Tank no.", 3 + horizntal_offset, 9);
    for (i = 0; i < 5; i++)
    {
        print_string_at(tank_no[i], 4 + horizntal_offset, loc_tank);
        loc_tank++;
    }
    print_string_at("Level", 15 + horizntal_offset, 9);
    for (i = 0; i < 5; i++)
    {
        print_string_at(level[i], 15 + horizntal_offset, loc_level);
        loc_level++;
    }
    print_string_at("Status", 24 + horizntal_offset, 9);
    for (i = 0; i < 5; i++)
    {
        print_string_at(status[i], 26 + horizntal_offset, loc_status);
        loc_status++;
    }
    print_string_at("Switch", 34 + horizntal_offset, 9);
    for (i = 0; i < 5; i++)
    {
        print_string_at(manual[i], 37 + horizntal_offset, loc_manual);
        loc_manual++;
    }
    print_string_at("Press ENTER on the dash to change the status.", horizntal_offset, 17);
    print_string_at("Press ESC to exit.", horizntal_offset, 18);
    status_control();
    start_kernel();
}

void start_kernel()
{

    clear_screen();
    isr_install();
    init_keyboard();
    print_string_at("Hydro Traffic Management System", 24, 9);
    print_string_at("Welcome", 37, 11);
    print_string_at("/\/\/", 22, 13);
    print_string_at("Press ENTER to see status", 27, 13);
    print_string_at("/\/\/", 54, 13);
    uint8_t input = keyboard_key_return();
    while (input != 0x1c)
    {
        input = keyboard_key_return();
    }
    status();
}
