#include <stdio.h>
#include <stdlib.h>

int main()
{
    int pipes[] = {1, 2, 3, 4, 5}, gaps[] = {2, 4, 6, 2, 1}, priority[] = {0, 1, 2, 3, 4};
    int total[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char *times[] = {"6-7", "7-8", "8-9", "9-10", "10-11", "11-12", "12-1", "1-2", "2-3", "3-4", "4-5", "5-6"};
    int i = 0;
    while (i != 12)
    {
        for (int j = 0; j < 5; j++)
        {
            priority[j]--;
            if (priority[j] == -1)
            {
                if (total[i] == 0)
                {
                    total[i] = pipes[j];
                }
                else
                {
                    int z = 1;
                    while (total[i + z] != 0)
                    {
                        z++;
                    }
                    total[i + z] = pipes[j];
                }
                priority[j] = gaps[j];
            }
        }
        i++;
    }
    for (int j = 0; j < 12; j++)
    {
        printf("%s - %d\n", times[j], total[j]);
    }
    return 0;
}
