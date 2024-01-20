#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool locked[9][9];

int main(void)
{
    locked[1][1] = true;
    locked[2][2] = false;
    printf("locked[1][1] = %d\n", locked[1][1]);
    printf("locked[2][2] = %d\n", locked[2][2]);
    printf("locked[3][3] = %d\n", locked[3][3]);
    
}

