#include <cs50.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


int main(void)
{
    string word = "a";
    int s = tolower(word[0]);

    printf("low %i\n", s);
    printf("new %i\n", (int) strlen(word));
    printf("%i\n", word[0]);
}