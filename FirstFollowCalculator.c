/**
 * Program to dynamically calculate FIRST and FOLLOW sets for a given grammar.
 *
 * Input Format:
 * 1. Enter the number of productions.
 * 2. Enter each production on a new line (e.g., E->TR).
 * 3. Use uppercase letters for non-terminals (E, T, R, ...).
 * 4. Use lowercase letters or symbols for terminals (a, b, +, *, ...).
 * 5. Use '#' for epsilon (ε).
 * 6. DO NOT use the '|' symbol. Split productions into separate lines.
 * (e.g., Enter "R->+TR" and "R->#" on two lines, NOT "R->+TR|#").
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// --- Configuration ---
#define MAX_PRODUCTIONS 20
#define MAX_SYMBOLS 30 // Max length of a production RHS
#define MAX_NON_TERMINALS 10
#define MAX_SET_SIZE 20
#define EPSILON '#'
#define END_MARKER '$'

// --- Data Structures ---
struct Production
{
    char lhs;
    char rhs[MAX_SYMBOLS];
};

struct Production grammar[MAX_PRODUCTIONS];
int num_productions = 0;

char non_terminals[MAX_NON_TERMINALS];
int num_non_terminals = 0;

char first_sets[MAX_NON_TERMINALS][MAX_SET_SIZE];
char follow_sets[MAX_NON_TERMINALS][MAX_SET_SIZE];

// --- Helper Functions ---

/**
 * @brief Adds a character to a set (string) if it's not already present.
 * @param set The set to add to.
 * @param c The character to add.
 * @param changed Pointer to a flag, set to 1 if the set was modified.
 */
void add_to_set(char *set, char c, int *changed)
{
    if (strchr(set, c) == NULL)
    {
        int len = strlen(set);
        set[len] = c;
        set[len + 1] = '\0';
        *changed = 1; // Mark that we made a change
    }
}

/**
 * @brief Adds all elements from src set to dest set, excluding epsilon.
 */
void add_set_minus_epsilon(char *dest, const char *src, int *changed)
{
    for (int i = 0; src[i] != '\0'; i++)
    {
        if (src[i] != EPSILON)
        {
            add_to_set(dest, src[i], changed);
        }
    }
}

/**
 * @brief Checks if a set (string) contains epsilon.
 */
int set_has_epsilon(const char *set)
{
    return strchr(set, EPSILON) != NULL;
}

/**
 * @brief Finds the index of a non-terminal in our global array.
 * This maps a char like 'E' to an index like 0.
 */
int get_nt_index(char nt)
{
    for (int i = 0; i < num_non_terminals; i++)
    {
        if (non_terminals[i] == nt)
        {
            return i;
        }
    }
    return -1; // Should not happen if input is correct
}

/**
 * @brief Adds a non-terminal to our global list if it's new.
 */
void add_non_terminal(char nt)
{
    if (isupper(nt) && get_nt_index(nt) == -1)
    {
        non_terminals[num_non_terminals++] = nt;
    }
}

// --- Core Logic: FIRST Sets ---

/**
 * @brief Calculates all FIRST sets using a fixed-point algorithm.
 * It loops until no new terminals can be added to any FIRST set.
 */
void calculate_first_sets()
{
    int changed;
    do
    {
        changed = 0;
        // Iterate over every production
        for (int i = 0; i < num_productions; i++)
        {
            char lhs = grammar[i].lhs;
            char *rhs = grammar[i].rhs;
            int lhs_index = get_nt_index(lhs);

            // Rule 1: RHS starts with a terminal
            if (!isupper(rhs[0]) && rhs[0] != EPSILON)
            {
                add_to_set(first_sets[lhs_index], rhs[0], &changed);
            }
            // Rule 2: RHS is epsilon
            else if (rhs[0] == EPSILON)
            {
                add_to_set(first_sets[lhs_index], EPSILON, &changed);
            }
            // Rule 3: RHS starts with a non-terminal (X Y Z...)
            else
            {
                int all_nullable = 1;
                for (int j = 0; rhs[j] != '\0'; j++)
                {
                    char symbol = rhs[j];

                    if (!isupper(symbol))
                    { // It's a terminal
                        add_to_set(first_sets[lhs_index], symbol, &changed);
                        all_nullable = 0;
                        break;
                    }

                    // It's a non-terminal (e.g., 'X')
                    int symbol_index = get_nt_index(symbol);
                    // Add FIRST(X) - {#} to FIRST(A)
                    add_set_minus_epsilon(first_sets[lhs_index], first_sets[symbol_index], &changed);

                    if (!set_has_epsilon(first_sets[symbol_index]))
                    {
                        all_nullable = 0; // X is not nullable, so stop
                        break;
                    }
                    // If X is nullable, continue to the next symbol (e.g., 'Y')
                }

                // If we went through all symbols and all were nullable
                if (all_nullable)
                {
                    add_to_set(first_sets[lhs_index], EPSILON, &changed);
                }
            }
        }
    } while (changed); // Repeat until no changes are made
}

// --- Core Logic: FOLLOW Sets ---

/**
 * @brief Calculates all FOLLOW sets using a fixed-point algorithm.
 * Must be called *after* calculate_first_sets().
 */
void calculate_follow_sets()
{
    int changed;

    // Rule 1: Add $ to FOLLOW(StartSymbol)
    int start_index = get_nt_index(grammar[0].lhs);
    add_to_set(follow_sets[start_index], END_MARKER, &changed); // 'changed' is a dummy here

    do
    {
        changed = 0;
        // Iterate over every production (A -> αBβ)
        for (int i = 0; i < num_productions; i++)
        {
            char lhs = grammar[i].lhs; // This is 'A'
            char *rhs = grammar[i].rhs;
            int lhs_index = get_nt_index(lhs);
            int rhs_len = strlen(rhs);

            // Iterate through RHS (to find 'B')
            for (int j = 0; j < rhs_len; j++)
            {
                char B = rhs[j];
                if (!isupper(B))
                    continue; // We only care about non-terminals

                int B_index = get_nt_index(B);

                // --- Rule 2: A -> αBβ ---
                // Find FIRST(β) and add it to FOLLOW(B)
                int all_beta_nullable = 1;
                for (int k = j + 1; k < rhs_len; k++)
                {
                    char beta_symbol = rhs[k]; // A symbol in β

                    if (!isupper(beta_symbol))
                    { // Terminal
                        add_to_set(follow_sets[B_index], beta_symbol, &changed);
                        all_beta_nullable = 0;
                        break;
                    }

                    // Non-terminal
                    int beta_index = get_nt_index(beta_symbol);
                    add_set_minus_epsilon(follow_sets[B_index], first_sets[beta_index], &changed);

                    if (!set_has_epsilon(first_sets[beta_index]))
                    {
                        all_beta_nullable = 0;
                        break;
                    }
                }

                // --- Rule 3: A -> αB or A -> αBβ (where FIRST(β) contains #) ---
                if (all_beta_nullable)
                {
                    // Add all of FOLLOW(A) to FOLLOW(B)
                    for (int k = 0; follow_sets[lhs_index][k] != '\0'; k++)
                    {
                        add_to_set(follow_sets[B_index], follow_sets[lhs_index][k], &changed);
                    }
                }
            }
        }
    } while (changed); // Repeat until no changes are made
}

// --- Main Driver ---

/**
 * @brief Reads the grammar from the user.
 */
void get_grammar()
{
    printf("Enter the number of productions: ");
    scanf("%d", &num_productions);
    getchar(); // Consume the newline

    printf("Enter the productions (e.g., E->TR, R->#, F->a):\n");
    for (int i = 0; i < num_productions; i++)
    {
        char buffer[100];
        printf("  %d: ", i + 1);
        fgets(buffer, 100, stdin);

        // sscanf parses "L->RHS\n"
        sscanf(buffer, "%c->%s", &grammar[i].lhs, grammar[i].rhs);

        // Register the non-terminals
        add_non_terminal(grammar[i].lhs);
        for (int j = 0; grammar[i].rhs[j] != '\0'; j++)
        {
            add_non_terminal(grammar[i].rhs[j]);
        }
    }
}

/**
 * @brief Initializes all set strings to empty.
 */
void initialize_sets()
{
    for (int i = 0; i < MAX_NON_TERMINALS; i++)
    {
        strcpy(first_sets[i], "");
        strcpy(follow_sets[i], "");
    }
}

/**
 * @brief Prints the final calculated sets.
 */
void print_sets()
{
    printf("\n--- CALCULATED SETS ---\n");
    for (int i = 0; i < num_non_terminals; i++)
    {
        char nt = non_terminals[i];
        printf("FIRST(%c)  = { %s }\n", nt, first_sets[i]);
    }
    printf("\n");
    for (int i = 0; i < num_productions; i++)
    {
        char nt = non_terminals[i];
        printf("FOLLOW(%c) = { %s }\n", nt, follow_sets[i]);
    }
}

int main()
{
    initialize_sets();
    get_grammar();

    calculate_first_sets();
    calculate_follow_sets();

    print_sets();

    return 0;
}