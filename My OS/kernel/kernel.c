#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/display.h"
#include "../drivers/keyboard.h"

#include "util.h"

int num_of_field;
int motor_output;
int time_slot[100];
int current_time;
struct field
{
    int field_number;
    int water_amount;
    int water_amount_original;
    int time_gap;
    int quota;
    int priority;
} Field[100];

void init()
{
    print_string("Enter the total number of fields: ");
    num_of_field = get_int();
    print_string("Enter the output of the motor: ");
    motor_output = get_int();
    int i;
    for (i = 0; i < num_of_field; i++)
    {
        print_string("Enter the total amount of water required for field ");
        print_int(i + 1);
        print_string(": ");
        Field[i].water_amount = get_int();
        Field[i].water_amount_original = Field[i].water_amount;
        print_string("Enter the time gap between irrigation for field ");
        print_int(i + 1);
        print_string(": ");
        Field[i].time_gap = get_int();
    }
    for (i = 0; i < num_of_field; i++)
    {
        Field[i].field_number = i + 1;
        Field[i].priority = 0;
        Field[i].quota = 24 / Field[i].time_gap;
    }
    for (i = 0; i < 100; i++)
    {
        time_slot[i] = 0;
    }
}

void init_first()
{
    num_of_field = 5;
    motor_output = 60;
    int i;
    Field[0].water_amount = 57;
    Field[1].water_amount = 115;
    Field[2].water_amount = 172;
    Field[3].water_amount = 29;
    Field[4].water_amount = 230;
    Field[0].water_amount_original = Field[0].water_amount;
    Field[1].water_amount_original = Field[1].water_amount;
    Field[2].water_amount_original = Field[2].water_amount;
    Field[3].water_amount_original = Field[3].water_amount;
    Field[4].water_amount_original = Field[4].water_amount;
    Field[0].time_gap = 8;
    Field[1].time_gap = 3;
    Field[2].time_gap = 1;
    Field[3].time_gap = 4;
    Field[4].time_gap = 6;

    for (i = 0; i < num_of_field; i++)
    {
        Field[i].field_number = i + 1;
        Field[i].priority = 0;
        Field[i].quota = 24 / Field[i].time_gap;
    }
    for (i = 0; i < 100; i++)
    {
        time_slot[i] = 0;
    }
}

int not_empty()
{
    int i;
    for (i = 0; i < num_of_field; i++)
    {
        if (Field[i].water_amount > 0)
        {
            return 1;
        }
    }
    return 0;
}

void process()
{
    current_time = 0;
    int i;
    while (not_empty())
    {
        for (i = 0; i < num_of_field; i++)
        {
            if (Field[i].priority <= 0)
            {
                if (Field[i].water_amount > 0)
                {
                    Field[i].water_amount -= motor_output;
                    Field[i].priority = Field[i].time_gap + 1;
                    time_slot[current_time] = Field[i].field_number;
                    break;
                }
            }
        }
        for (i = 0; i < num_of_field; i++)
        {
            Field[i].priority--;
        }
        current_time++;
    }
}

void print_order(int control)
{
    clear_screen();
    int i = 0,loc_time=5;
    while (i < current_time)
    {
        if (time_slot[i])
        {
            print_string_at("Time slot ", 27,loc_time);
            print_int(i + 1);
            print_string(" - ");
            print_string("Field ");
            print_int(time_slot[i]);
            print_nl();
            loc_time++;
        }
        ++i;
    }
    if (current_time > 24)
    {
        print_string_at("WARNING...Motor Output not enough for optimal result",13,18);
        print_nl();
        print_string_at("Try upgrading your motor...",13,19);
        print_nl();
    }
    print_string_at("Press ESC to go back.",13,20);
    esc_to_exit();
    if (control)
    {
        summary_new();
    }
    else
    {
        summary();
    }
    
}

void summary()
{
    init_first();
    process();
    int i, horizntal_offset = 16;
    int loc_pipe = 5, loc_plant = 5, loc_water_per_day = 5, loc_time_gap = 5;
    char *pipe_no[5] = {"Pipe1", "Pipe2", "Pipe3", "Pipe4", "Pipe5"};
    char *plant[5] = {"Carrot", "Onions", "Pepper", "Rosemary", "Tomatoes"};
    char *water_per_day[5] = {"57L", "115L", "172L", "29L", "230L"};
    char *time_gap[5] = {"8hr", "3hr", "1hr", "4hr", "6hr"};
    clear_screen();
    print_string_at("Field Summary", 30, 1);
    print_string_at("Pipe no.", 3 + horizntal_offset, 3);
    for (i = 0; i < 5; i++)
    {
        print_string_at(pipe_no[i], 4 + horizntal_offset, loc_pipe);
        loc_pipe++;
    }
    print_string_at("Plant", 15 + horizntal_offset, 3);
    for (i = 0; i < 5; i++)
    {
        print_string_at(plant[i], 15 + horizntal_offset, loc_plant);
        loc_plant++;
    }
    print_string_at("Litre", 25 + horizntal_offset, 3);
    for (i = 0; i < 5; i++)
    {
        print_string_at(water_per_day[i], 26 + horizntal_offset, loc_water_per_day);
        loc_water_per_day++;
    }
    print_string_at("Time Gap", 34 + horizntal_offset, 3);
    for (i = 0; i < 5; i++)
    {
        print_string_at(time_gap[i], 37 + horizntal_offset, loc_time_gap);
        loc_time_gap++;
    }
    print_string_at("Press ENTER to update the values.", horizntal_offset, 17);
    print_string_at("Press F to view order.", horizntal_offset, 18);
    print_string_at("Press ESC to exit.", horizntal_offset, 19);
    int flag = summary_control();
    if (flag == 1)
    {
        my_func();
    }
    if (flag == 0)
    {
        start_kernel();
    }
    else
    {
        print_order(0);
    }
}

void summary_new()
{
    int i, horizntal_offset = 16;
    int loc_pipe = 5, loc_plant = 5, loc_water_per_day = 5, loc_time_gap = 5;
    char *pipe_no[5] = {"Pipe1", "Pipe2", "Pipe3", "Pipe4", "Pipe5"};
    char *plant[5] = {"Carrot", "Onions", "Pepper", "Rosemary", "Tomatoes"};
    char *water_per_day[5] = {"57L", "115L", "172L", "29L", "230L"};
    char *time_gap[5] = {"8hr", "3hr", "1hr", "4hr", "6hr"};
    clear_screen();
    print_string_at("Field Summary", 30, 1);
    print_string_at("Pipe no.", 3 + horizntal_offset, 3);
    for (i = 0; i < num_of_field; i++)
    {
        print_string_at("Pipe", 4 + horizntal_offset, loc_pipe);
        print_int(Field[i].field_number);
        loc_pipe++;
    }
    print_string_at("Plant", 15 + horizntal_offset, 3);
    for (i = 0; i < num_of_field; i++)
    {
        print_string_at(plant[i], 15 + horizntal_offset, loc_plant);
        loc_plant++;
    }
    print_string_at("Litre", 25 + horizntal_offset, 3);
    for (i = 0; i < num_of_field; i++)
    {
        print_int_at(Field[i].water_amount_original, 26 + horizntal_offset, loc_water_per_day);
        print_string("L");
        loc_water_per_day++;
    }
    print_string_at("Time Gap", 34 + horizntal_offset, 3);
    for (i = 0; i < num_of_field; i++)
    {
        print_int_at(Field[i].time_gap, 37 + horizntal_offset, loc_time_gap);
        print_string("H");
        loc_time_gap++;
    }
    print_string_at("Press ENTER to update the values.", horizntal_offset, 17);
    print_string_at("Press F to view order.", horizntal_offset, 18);
    print_string_at("Press ESC to exit.", horizntal_offset, 19);
    int flag = summary_control();
    if (flag == 1)
    {
        my_func();
    }
    if (flag == 0)
    {
        start_kernel();
    }
    else
    {
        print_order(1);
    }
}

void my_func()
{
    clear_screen();
    init();
    process();
    print_nl();
    print_string("Press ENTER to continue.");
    enter_to_contn();
    summary_new();
}

void start_kernel()
{

    clear_screen();
    isr_install();
    init_keyboard();
    print_string_at("Hydro Traffic Management System", 24, 9);
    print_string_at("Welcome", 37, 11);
    print_string_at("///", 22, 13);
    print_string_at("Press ENTER to see Summary", 27, 13);
    print_string_at("///", 54, 13);
    uint8_t input = keyboard_key_return();
    while (input != 0x1c)
    {
        input = keyboard_key_return();
    }
    summary();
}
