/**
 * Experiment No. 12: Generate 3-Address Code
 *
 * This program takes a simple arithmetic expression (e.g., a+b*c)
 * and generates the corresponding 3-address code.
 *
 * It uses a Recursive Descent Parser modified to pass temporary
 * variable names up the call stack.
 *
 * Grammar:
 * E -> T E'
 * E' -> + T E' | - T E' | ε
 * T -> F T'
 * T' -> * F T' | / F T' | ε
 * F -> ( E ) | id
 *
 * (We use single letters 'a', 'b', etc. as 'id')
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for exit()
#include <ctype.h>  // for isalpha()

// --- Global Variables ---
char input[100];
int i = 0;          // Current position in the input string
int temp_count = 0; // Counter for new temporary variables (t0, t1, t2...)

// --- Persistent Storage for Names ---
// We need static arrays so the pointers we return remain valid
// after the function exits.
static char temp_buffers[20][5]; // Pool for 20 temp names (e.g., "t0", "t1")
static char id_buffers[20][2];   // Pool for 20 id names (e.g., "a", "b")
static int id_count = 0;         // Counter for id_buffers

// --- Function Prototypes ---
char *E();
char *E_prime(char *left_operand);
char *T();
char *T_prime(char *left_operand);
char *F();

/**
 * @brief Generates a new temporary variable name (e.g., "t0", "t1").
 * @return A pointer to a persistent string (e.g., "t0").
 */
char *new_temp()
{
    if (temp_count >= 20)
    {
        printf("Error: Too many temporary variables.\n");
        exit(1);
    }
    // Create the name "t<number>"
    sprintf(temp_buffers[temp_count], "t%d", temp_count);
    temp_count++;
    // Return the pointer to this new name
    return temp_buffers[temp_count - 1];
}

/**
 * @brief Parses 'F' non-terminal.
 * F -> ( E ) | id
 * @return Pointer to the variable name (e.g., "a" or "t1") holding the result.
 */
char *F()
{
    if (input[i] == '(')
    {
        i++;                // Consume '('
        char *result = E(); // Parse the expression inside
        if (input[i] != ')')
        {
            printf("Error: Missing ')'\n");
            exit(1);
        }
        i++;           // Consume ')'
        return result; // Return the result of the sub-expression
    }
    else if (isalpha(input[i]))
    {
        // It's an id
        if (id_count >= 20)
        {
            printf("Error: Too many identifiers.\n");
            exit(1);
        }
        // Store the id in our persistent buffer
        id_buffers[id_count][0] = input[i];
        id_buffers[id_count][1] = '\0';
        i++; // Consume the id
        id_count++;
        // Return the pointer to this id's name
        return id_buffers[id_count - 1];
    }
    else
    {
        printf("Error: Invalid character '%c'\n", input[i]);
        exit(1);
    }
}

/**
 * @brief Parses 'T_prime' non-terminal.
 * T' -> * F T' | / F T' | ε
 * @param left_operand The name of the variable from the left (e.g., "a").
 * @return Pointer to the variable name holding the final result.
 */
char *T_prime(char *left_operand)
{
    if (input[i] == '*')
    {
        i++; // Consume '*'
        char *right_operand = F();
        char *temp_result = new_temp(); // Get a new temp name (e.g., "t0")

        // --- PRINT 3-ADDRESS CODE ---
        printf("%s = %s * %s\n", temp_result, left_operand, right_operand);

        // Continue parsing for more * or / (e.g., a*b*c)
        return T_prime(temp_result);
    }
    else if (input[i] == '/')
    {
        i++; // Consume '/'
        char *right_operand = F();
        char *temp_result = new_temp();

        // --- PRINT 3-ADDRESS CODE ---
        printf("%s = %s / %s\n", temp_result, left_operand, right_operand);

        return T_prime(temp_result);
    }
    else
    {
        // Epsilon case: No * or / found.
        // The result is just the left operand we were given.
        return left_operand;
    }
}

/**
 * @brief Parses 'T' non-terminal.
 * T -> F T'
 * @return Pointer to the variable name holding the final result.
 */
char *T()
{
    char *left_operand = F();     // Get result from F
    return T_prime(left_operand); // Pass it to T' to handle * or /
}

/**
 * @brief Parses 'E_prime' non-terminal.
 * E' -> + T E' | - T E' | ε
 * @param left_operand The name of the variable from the left (e.g., "t0").
 * @return Pointer to the variable name holding the final result.
 */
char *E_prime(char *left_operand)
{
    if (input[i] == '+')
    {
        i++;                       // Consume '+'
        char *right_operand = T(); // Get result from T
        char *temp_result = new_temp();

        // --- PRINT 3-ADDRESS CODE ---
        printf("%s = %s + %s\n", temp_result, left_operand, right_operand);

        // Continue parsing for more + or - (e.g., a+b+c)
        return E_prime(temp_result);
    }
    else if (input[i] == '-')
    {
        i++; // Consume '-'
        char *right_operand = T();
        char *temp_result = new_temp();

        // --- PRINT 3-ADDRESS CODE ---
        printf("%s = %s - %s\n", temp_result, left_operand, right_operand);

        return E_prime(temp_result);
    }
    else
    {
        // Epsilon case: No + or - found.
        return left_operand;
    }
}

/**
 * @brief Parses 'E' non-terminal (the start symbol).
 * E -> T E'
 * @return Pointer to the variable name holding the final result.
 */
char *E()
{
    char *left_operand = T();     // Get result from T
    return E_prime(left_operand); // Pass it to E' to handle + or -
}

// --- Main Driver ---
int main()
{
    printf("Enter an arithmetic expression (e.g., a+b*c-d): ");
    scanf("%s", input);

    printf("\n--- 3-Address Code ---\n");
    char *final_result = E();

    // Check if we consumed the whole string
    if (input[i] != '\0')
    {
        printf("Error: Trailing characters found.\n");
    }
    else
    {
        printf("-----------------------\n");
        printf("Final result is in: %s\n", final_result);
    }

    return 0;
}