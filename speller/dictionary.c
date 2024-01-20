// Implements a dictionary's functionality
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
//length = 45 is defined in dictionary.h
const unsigned int N = 27*45;

// Hash table
node *table[N];

// The number of words in dictionary
unsigned int size_words = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //get index from hash function
    unsigned int index = hash(word);

    //check the word if we can find in the specific index
    for (node *cursor = table[index]; cursor != NULL; cursor = cursor->next)
    {
        //check the word
        if (!strcasecmp(word, cursor->word))
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //count how many letters in the word
    unsigned int len = strlen(word);
    unsigned int output = 0;
    
    
    //if apostrophes
    if ((int) tolower(word[len-1]) == 39)
    {
        return 0 + (len-1)*27;
    }

    //if alphabetical characters
    else
    {
        //compare the ASCII
        int a = (int) tolower(word[len-1]) - 96;
        return a + (len-1)*27;
    }

}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //define variable to read the words in the file
    char file_word[LENGTH +1];

    //open the dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return 1;
    }

    while (EOF != fscanf(file, "%s", file_word))
    {

        //Get index from hash function
        unsigned int index = hash(file_word);

        //plus the number of words in the dictionary
        size_words++;

        //create a new node for each word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return 2;
        }

        //copy the words from the file to dictionary
        strcpy(n->word, file_word);
        n->next =NULL;

        //if the index in the table doesn't point to the next node
        if (table[index] == NULL)
        {
            table[index] = n;
        }

        //if the index in the table points to the next node
        else
        {
            n->next = table[index];
            table[index] = n;
        }
    }

    //close the file
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //counts size of word in the load function
    return size_words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //traverse the talbe array
    for (int i = 0; i < N; i++)
    {
        //traverse the linked list
        for (node *cursor = table[i], *tem = table[i]; cursor != NULL; cursor = tem)
        {
            tem = tem->next;
            free(cursor);
        }
    }
    return true;
}
