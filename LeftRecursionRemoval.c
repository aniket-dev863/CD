/**
 * Program to remove immediate left recursion from a production.
 *
 * This program expects a production in the format:
 * A -> Aα | β
 *
 * which is entered by the user as:
 * A=Aa|b
 *
 * For example: E=E+T|T
 *
 * It will transform this into:
 * A -> βA'
 * A' -> αA' | #
 *
 * For the example E=E+T|T, the output will be:
 * E = TE'
 * E' = +TE' | #
 */

#include <stdio.h>
#include <string.h>

int main()
{
    char production[100];
    char non_terminal;
    char alpha[100], beta[100];
    int alpha_index = 0, beta_index = 0;
    int i;
    int has_left_recursion = 0;

    printf("Enter the production (e.g., E=E+T|T): ");
    scanf("%s", production);

    // 1. Extract the non-terminal
    non_terminal = production[0];

    // 2. Check if it has immediate left recursion
    // We check if the char after '=' is the same as the non_terminal
    if (production[3] == non_terminal)
    {
        has_left_recursion = 1;
    }
    else
    {
        printf("The production does not have immediate left recursion.\n");
        return 0;
    }

    // 3. Parse the production to find alpha and beta
    // Format is A=Aα|β
    // We start parsing after "A=A"
    i = 4;

    // --- Find alpha ---
    // Read everything from production[4] until the '|'
    while (production[i] != '|' && production[i] != '\0')
    {
        alpha[alpha_index] = production[i];
        alpha_index++;
        i++;
    }
    alpha[alpha_index] = '\0'; // Null-terminate the alpha string

    // --- Find beta ---
    // Check if we stopped at '|'. If not, something is wrong.
    if (production[i] != '|')
    {
        printf("Invalid production format. Expected '|'.\n");
        return 1;
    }

    i++; // Skip the '|' to start reading beta

    while (production[i] != '\0')
    {
        beta[beta_index] = production[i];
        beta_index++;
        i++;
    }
    beta[beta_index] = '\0'; // Null-terminate the beta string

    // 4. Print the new, transformed productions
    if (has_left_recursion)
    {
        printf("\nGrammar after removing left recursion:\n");

        // A -> βA'
        printf("%c = %s%c'\n", non_terminal, beta, non_terminal);

        // A' -> αA' | #
        printf("%c' = %s%c' | #\n", non_terminal, alpha, non_terminal);
    }

    return 0;
}