#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;
bool recursion(int i, int j);

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
        
    }
    
    for (int k = 0; k < candidate_count; k++)
    {
        for (int c = 0; c < candidate_count; c++)
        {
            printf("preferences[%i][%i] = %i\n", k, c, preferences[k][c]);
        }
    }
     
    
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] == preferences[j][i])
            {
                //no pair
            }
            
            else if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                printf("pairs[%i].winner = %i\n", pair_count, pairs[pair_count].winner);
                printf("pairs[%i].loser = %i\n", pair_count, pairs[pair_count].loser); 
                pair_count++;
            }
            else
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                printf("pairs[%i].winner = %i\n", pair_count, pairs[pair_count].winner);
                printf("pairs[%i].loser = %i\n", pair_count, pairs[pair_count].loser); 
                pair_count++;
            }
        }
         
    }
    printf("pair_count = %i\n", pair_count);
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count - 1; i++)
    {
        
        for (int j = i + 1 ; j < pair_count; j++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[j].winner][pairs[j].loser])
            {
                //printf("   原本pairs[%i].winner = %i ; 原本pairs[%i].loser = %i\n", i, pairs[i].winner, i, pairs[i].loser );
                //printf("   要交換的pairs[%i].winner = %i ; 要交換的pairs[%i].loser = %i\n", j, pairs[j].winner, j, pairs[j].loser );
                int tem = pairs[i].winner;
                pairs[i].winner = pairs[j].winner;
                pairs[j].winner = tem;

                int tem2 = pairs[i].loser;
                pairs[i].loser = pairs[j].loser;
                pairs[j].loser = tem2;
                //printf("   交換過後的pairs[%i].winner = %i ; 交換過後的pairs[%i].loser = %i\n", i, pairs[i].winner, i, pairs[i].loser );
                //printf("   交換過後的pairs[%i].winner = %i ; 交換過後的pairs[%i].loser = %i\n", j, pairs[j].winner, j, pairs[j].loser );
            }
        }
        printf("pairs[%i].winner = %i ; pairs[%i].loser = %i\n", i, pairs[i].winner, i, pairs[i].loser);
        
        if (i == pair_count - 2)
        {
            printf("pairs[%i].winner = %i ; pairs[%i].loser = %i\n", i + 1, pairs[i + 1].winner, i + 1, pairs[i + 1].loser);
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        printf("**** i=%i\n", i);
        
        locked[pairs[i].winner][pairs[i].loser] = true;
        
        if (recursion(i, pairs[i].winner) == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
        printf("locked[%i][%i] = %d\n", pairs[i].winner, pairs[i].loser, locked[pairs[i].winner][pairs[i].loser]);
    }
    
    return;
}

bool recursion(int i, int j)
{
    if (i == 0)
    {
        for (int a = 0; a < pair_count; a++)
        {
            if (a != j)
            {
                if (locked[a][j] == true)
                {
                    printf("i= %i and a =%i and j=%i\n", i, a, j);
                    return false;
                }
            }
        }
        return true;
    }
    
    else
    {
        for (int a = 0; a < pair_count; a++)
        {
            if (a != j)
            {
                if (locked[a][j] == true)
                {
                    printf("i= %i and a =%i and j=%i\n", i, a, j);
                    return recursion(i - 1, a);
                }
            }
        }
        return true;
    }
    
}


// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        int winner_count = 0;
        
        for (int a = 0; a < candidate_count; a++)
        {
            if (locked[a][i] == false)   
            {
                winner_count++;
            }
        }
        
        if (winner_count == candidate_count)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

