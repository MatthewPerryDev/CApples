#include <stdio.h>
void GetInt(char message[], int min, int max, int *userInput)
{
    printf("%s", message);
    scanf("%d", userInput);
    if (min > max || max < min)
    {
        return -1;
    }
    while ((*userInput) < min || (*userInput) > max)
    {
        printf("That is not in the range of %d-%d\n", min, max);
        printf("%s", message);
        scanf("%d", userInput);
    }
}

char *GetString(char message[])
{
    
}

int main(void)
{
    int userSelection;
    GetInt("Please enter your selection: ", 1, 7, &userSelection);
    return 0;
}