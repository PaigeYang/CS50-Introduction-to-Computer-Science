#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner
    if (score1 > score2)
    {
        printf ("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf ("Player 2 wins!\n");
    }
    else
    {
        printf ("Tie!\n");
    }
}

int compute_score(string word)
{
    // test the length of the word
    int lenth = strlen(word);
    int total = 0;

    for (int i = 0; i < lenth; i++)
    {
        int n = tolower(word[i]);

        if (n >=97 && n <= 122 )
        {
            total = total + (int) POINTS[n-97];
        }
        else
        {
            total = total + 0;
        }
    }

    return total;

}
