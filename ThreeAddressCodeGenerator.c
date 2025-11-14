#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
char input[100];
int i = 0;
int temp_count = 0;

static char temp_buffers[20][5];
static char id_buffers[20][2];
static int id_count = 0;

char *E();
char *E_prime(char *left_operand);
char *T();
char *T_prime(char *left_operand);
char *F();

char *new_temp()
{
    if (temp_count >= 20)
    {
        printf("Error: Too many temporary variables.\n");
        exit(1);
    }
    sprintf(temp_buffers[temp_count], "t%d", temp_count);
    temp_count++;
    return temp_buffers[temp_count - 1];
}

char *F()
{
    if (input[i] == '(')
    {
        i++;
        char *result = E();
        if (input[i] != ')')
        {
            printf("Error: Missing ')'\n");
            exit(1);
        }
        i++;
        return result;
    }
    else if (isalpha(input[i]))
    {
        if (id_count >= 20)
        {
            printf("Error: Too many identifiers.\n");
            exit(1);
        }
        id_buffers[id_count][0] = input[i];
        id_buffers[id_count][1] = '\0';
        i++;
        id_count++;
        return id_buffers[id_count - 1];
    }
    else
    {
        printf("Error: Invalid character '%c'\n", input[i]);
        exit(1);
    }
}

char *T_prime(char *left_operand)
{
    if (input[i] == '*')
    {
        i++;
        char *right_operand = F();
        char *temp_result = new_temp();

        printf("%s = %s * %s\n", temp_result, left_operand, right_operand);
        return T_prime(temp_result);
    }
    else if (input[i] == '/')
    {
        i++;
        char *right_operand = F();
        char *temp_result = new_temp();
        printf("%s = %s / %s\n", temp_result, left_operand, right_operand);

        return T_prime(temp_result);
    }
    else
    {
        return left_operand;
    }
}

char *T()
{
    char *left_operand = F();
    return T_prime(left_operand);
}

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

char *E()
{
    char *left_operand = T();     // Get result from T
    return E_prime(left_operand); // Pass it to E' to handle + or -
}

int main()
{
    printf("Enter an arithmetic expression (e.g., a+b*c-d): ");
    scanf("%s", input);

    printf("\n--- 3-Address Code ---\n");
    char *final_result = E();
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