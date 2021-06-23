#include <stdio.h>
#include <stdlib.h>

int num_of_field;
int motor_output;
int time_slot[100];
int current_time;
struct field
{
    int field_number;
    int water_amount;
    int time_gap;
    int quota;
    int priority;
} Field[100];

void init()
{
    printf("Enter the total number of fields: ");
    scanf("%d", &num_of_field);
    printf("Enter the ouput of the motor: ");
    scanf("%d", &motor_output);
    int i;
    for (i = 0; i < num_of_field; i++)
    {
        printf("Enter the total amount of water required for field %d: ", (i + 1));
        scanf("%d", &Field[i].water_amount);
        printf("Enter the time gap between irrigation for field %d: ", (i + 1));
        scanf("%d", &Field[i].time_gap);
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

void print_order()
{
    int i = 0;
    while (i < current_time)
    {
        if (time_slot[i] != 0)
        {
            printf("Time Slot %d: %d\n", (i + 1), time_slot[i]);
        }
        ++i;
    }
}

int main()
{
    init();
    process();
    print_order();
    return 0;
}
