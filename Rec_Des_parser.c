/**
 * Experiment No. 11: Recursive Descent Parser
 * * This program implements a Recursive Descent Parser for the grammar:
 * E  -> T E'
 * E' -> + T E' | #
 * T  -> F T'
 * T' -> * F T' | #
 * F  -> ( E ) | id
 *
 * It checks if a given input string is accepted by the grammar and
 * prints the parsing steps taken.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// --- Global Variables ---
char input[100]; // Stores the input string
int i = 0;       // Current position in the input string (our "lookahead" pointer)
int error = 0;   // Flag to signal if an error has occurred

// --- Function Prototypes for each Non-Terminal ---
void E();
void E_prime();
void T();
void T_prime();
void F();

/**
 * @brief Matches the current input character with an expected terminal.
 * * If the character matches, it advances the input pointer 'i'.
 * If it doesn't match, it sets the global 'error' flag.
 *
 * @param terminal The expected terminal character (e.g., '+', '*', '(', ')', 'i', 'd').
 */
void match(char terminal)
{
    // If an error has already occurred, just stop
    if (error)
        return;

    if (input[i] == terminal)
    {
        // Matched successfully
        printf("  [Match] Matched terminal: %c\n", terminal);
        i++; // Consume the character and advance the pointer
    }
    else
    {
        // Match failed
        printf("  [Error] Expected '%c', but found '%c'\n", terminal, input[i]);
        error = 1; // Set the global error flag
    }
}

/**
 * @brief Parses the 'E' non-terminal.
 * Production: E -> T E'
 */
void E()
{
    if (error)
        return;
    printf("Parsing E  -> T E'\n");
    T();
    E_prime();
}

/**
 * @brief Parses the 'E_prime' non-terminal.
 * Production: E' -> + T E' | #
 */
void E_prime()
{
    if (error)
        return;

    // Check which production to use
    if (input[i] == '+')
    {
        // Use production E' -> + T E'
        printf("Parsing E' -> + T E'\n");
        match('+');
        T();
        E_prime();
    }
    else
    {
        // Use epsilon production E' -> #
        // We don't match or call anything, just return.
        printf("Parsing E' -> # (epsilon)\n");
    }
}

/**
 * @brief Parses the 'T' non-terminal.
 * Production: T -> F T'
 */
void T()
{
    if (error)
        return;
    printf("Parsing T  -> F T'\n");
    F();
    T_prime();
}

/**
 * @brief Parses the 'T_prime' non-terminal.
 * Production: T' -> * F T' | #
 */
void T_prime()
{
    if (error)
        return;

    // Check which production to use
    if (input[i] == '*')
    {
        // Use production T' -> * F T'
        printf("Parsing T' -> * F T'\n");
        match('*');
        F();
        T_prime();
    }
    else
    {
        // Use epsilon production T' -> #
        printf("Parsing T' -> # (epsilon)\n");
    }
}

/**
 * @brief Parses the 'F' non-terminal.
 * Production: F -> ( E ) | id
 */
void F()
{
    if (error)
        return;

    // Check which production to use
    if (input[i] == '(')
    {
        // Use production F -> ( E )
        printf("Parsing F  -> ( E )\n");
        match('(');
        E();
        match(')');
    }
    else if (input[i] == 'i')
    {
        // Use production F -> id
        printf("Parsing F  -> id\n");
        match('i'); // Match the 'i'
        match('d'); // Match the 'd'
    }
    else
    {
        // No valid production found
        printf("  [Error] Expected '(' or 'id', but found '%c'\n", input[i]);
        error = 1;
    }
}

// --- Main Driver Function ---
int main()
{
    printf("Enter the input string (e.g., (id+id)*id): ");
    scanf("%s", input);

    int len = strlen(input);

    printf("\n--- Parsing Steps ---\n");

    // Start parsing from the start symbol 'E'
    E();

    printf("\n--- Result ---\n");

    // Check for success:
    // 1. No error was flagged (error == 0)
    // 2. We have consumed the *entire* input string (i == len)
    if (error == 0 && i == len)
    {
        printf("String successfully parsed. ACCEPTED.\n");
    }
    else
    {
        // Print a more specific error if parsing stopped early
        if (error == 0 && i < len)
        {
            printf("  [Error] Parsing finished, but input string was not fully consumed.\n");
        }
        printf("String parsing failed. REJECTED.\n");
    }

    return 0;
}