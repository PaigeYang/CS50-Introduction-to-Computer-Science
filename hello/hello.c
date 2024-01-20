#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //ask for user name
    string name = get_string("What is your name?\n");

    //print the user name
    printf("Hello, %s\n", name);
}