//Import other structs and functions
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
typedef struct card_struct
{
    char Word[50];
    bool used;
} CardType;

typedef struct Player_struct
{
    char Name[50];
    int numWins;
    CardType redCards[7];

} PlayerType;

PlayerType *GetPlayerData(CardType *redCards, int redCardsNum, int *numPLayers, int *goal);

PlayerType GenPlayerHand(CardType *redcards, PlayerType player, int arraySize);

CardType *LoadCards(const char fileName[], int *arraySize);

CardType PickGreenCard(CardType *greenCards, int numGreenCards);

int PickRedCard(CardType greenCard, PlayerType *player);

int Czar(PlayerType *players, int roundCards[], CardType greenCard, int numPlayers);

void GameLoop(CardType *greenCards, CardType *redCards, PlayerType *players, int numPlayers, int numRedCards, int numGreenCards, int goal);

void ShiftCzar(PlayerType *players, int numPlayers);

void PromptContinue(void);

void PromptTurn(PlayerType player);

void PrintRedText(char message[]);

void PrintGreenText(char message[]);

CardType *LoadCards(const char fileName[], int *arraySize)
{
    //Opens the card file
    FILE *inputFile = fopen(fileName, "r");
    //intiliazes the pointer to null
    CardType *cards = NULL;

    //Initializes a variable to store the size of the array
    int counter = 0;

    //Scans the file until it reaches the end
    while (!feof(inputFile))
    {
        counter++;
        CardType temp;
        //Scans the line into the var
        fgets(temp.Word, 50, inputFile);
        //removes the newline character that fgets adds
        strtok(temp.Word, "\n");
        temp.used = false;
        //reallocates the card array size so the array can be any size
        cards = realloc(cards, counter * sizeof(CardType));
        //Adds the current word to the array
        cards[counter - 1] = temp;
    }
    (*arraySize) = counter;
    fclose(inputFile);
    return cards;
}

PlayerType *GetPlayerData(CardType *redCards, int redCardsNum, int *numPlayers, int *goal)
{
    //Creates a pointer to the memory that will be returned
    PlayerType *players = NULL;

    //Gets the number of players
    printf("Please enter the number of players: ");
    scanf("%d", numPlayers);

    printf("Please enter number of green cards needed to Win: ");
    scanf("%d", goal);
    //Creaters and assignes memmory for the number of players
    players = realloc(players, (*numPlayers) * sizeof(PlayerType));

    //Loops through each player and defines the players attributes
    for (int i = 0; i < (*numPlayers); i++)
    {
        //Creates a temport player variable
        PlayerType temp;

        //Prompts the user for the their name
        printf("Please enter playername: ");
        scanf("%s", temp.Name);

        //sets the initial  handsize and number of wins to zero
        temp.numWins = 0;

        //loops through the players red cards and sets them to used
        //This is so that GenPlayerHand will replace each card
        for (int i = 0; i < 7; i++)
        {
            temp.redCards[i].used = true;
        }
        //Generates the players hand
        temp = GenPlayerHand(redCards, temp, redCardsNum);

        //Assignes the current position in the players list to the temporary player variable
        players[i] = temp;
    }
    //return the pointer to the memory
    return players;
}

PlayerType GenPlayerHand(CardType *redcards, PlayerType player, int arraySize)
{
    //Loops through the players red cards
    for (int i = 0; i < 7; i++)
    {
        //Checks if the cards have been used
        if (player.redCards[i].used == true)
        {
            //Loops until a random red card has been found and isnt used
            while (true)
            {
                int index = rand() % arraySize;
                if (redcards[index].used == false)
                {
                    //sets the new card
                    player.redCards[i] = redcards[index];
                    //sets the card used to true for the global list
                    redcards[index].used = true;
                    break;
                }
            }
        }
    }
    //returns the edited player
    return player;
}

CardType PickGreenCard(CardType *greenCards, int numGreenCards)
{
    //Creates a temporary Card Variable
    CardType currentGreenCard;
    while (true)
    {
        //Returns a random index between 0 and the number of green cards
        int index = rand() % (numGreenCards);
        //Checks if the card is used
        if (greenCards[index].used == false)
        {
            //sets the temporary variable to the first unused greencard found
            currentGreenCard = greenCards[index];
            //sets the global green card to used
            greenCards[index].used = true;
            break;
        }
    }
    return currentGreenCard;
}

int PickRedCard(CardType greenCard, PlayerType *player)
{
    //Creates a variable to store the users choice
    int userSelection;
    char message[500];
    //Gives the user general information
    printf("Its %s's turn\n", player->Name);
    sprintf(message, "The green card is %s\n", greenCard.Word);
    PrintGreenText(message);
    printf("Your red Cards are: \n");
    //Loops over and prints out the users redcards
    for (int i = 0; i < 7; i++)
    {
        sprintf(message, "%d.%s\n", (i + 1), player->redCards[i].Word);
        PrintRedText(message);
    }
    //Prompts the user for input
    printf("Enter your selection: ");
    scanf("%d", &userSelection);
    //Sets the players selected redcard to used
    player->redCards[userSelection - 1].used = true;
    //clears the screen
    system("clear");

    return (userSelection - 1);
}

void ShiftCzar(PlayerType *players, int numPlayers)
//shifts the first player in the array to the last position
{
    //creates a temp variable to store the first player in the array
    PlayerType temp = players[0];
    //loops over all players
    for (int i = 0; i < numPlayers; i++)
    {
        if (i != numPlayers - 1)
        {
            //swaps two players in the array
            PlayerType temp1 = players[i];
            players[i] = players[i + 1];
            players[i + 1] = temp1;
        }
        else
        {
            //sets the last player in the array to the temp player
            players[numPlayers - 1] = temp;
        }
    }
}

int Czar(PlayerType *players, int roundCards[], CardType greenCard, int numPlayers)
{
    //Creates a variable to store the Czars choice
    int userSelection;
    char message[500];
    //Gives the czar general information
    printf("You are the Czar\n");
    sprintf(message, "The green card is %s", greenCard.Word);
    PrintGreenText(message);
    printf("\nYour red Cards are:\n");
    //prints all the red cards that have been played
    for (int i = 0; i < (numPlayers - 1); i++)
    {

        sprintf(message, "%d.%s\n", (i + 1), players[i].redCards[roundCards[i]].Word);
        PrintRedText(message);
    }
    //Prompts the czar for their selection
    printf("Enter your selection: ");
    scanf("%d", &userSelection);
    players[userSelection - 1].numWins++;

    return userSelection;
}
void PrintRedText(char message[])
{
    printf("\033[1;31m");
    printf("%s", message);
    printf("\033[0m");
}
void PrintGreenText(char message[])
{
    printf("\033[32;1m");
    printf("%s", message);
    printf("\033[0m");
}

void GetInt(char message[],int min,int max, int* userInput){
    printf("%s",message);
    scanf("%d",userInput);
    while ((*userInput)<min||(*userInput)>max)
    {
        printf("That is not in the range of %d - %d\n",min,max);
        printf("%s",message);
        scanf("%d",userInput);
    }   
}

void GameLoop(CardType *greenCards, CardType *redCards, PlayerType *players, int numPlayers, int numRedCards, int numGreenCards, int goal)
{
    //Clears the Screen of any inital text
    system("clear");
    
    //Clears the initial input buffer
    getchar();

    //Initializes the Game
    //Will note be exited till the game has been won
    while (true)
    {
        //Loops over each players hand at the Beginning of the round and replaces any used cards
        for (int i = 0; i < numPlayers; i++)
        {
            players[i] = GenPlayerHand(redCards, players[i], numRedCards);
        }

        //Moves the Czar to the end of the array
        ShiftCzar(players, numPlayers);

        //Picks the green card for the round
        CardType greenCard = PickGreenCard(greenCards, numGreenCards);
        
        //Creates an array for the index of the card in the user red cards
        //The index of in this array matches the index of the player in the players array
        int roundCards[numPlayers - 1];

        //Loops over each player
        for (int i = 0; i < (numPlayers - 1); i++)
        {

            //tells the user who's turn is next and prompts them to press enter to continue
            printf("%s's turn is next\n", players[i].Name);
            printf("Press any key to continue: ");
            //Gets the users input
            getchar();
            //Clears the screen
            system("clear");
            //assignes the index of the redcard the player picked to the round cards
            roundCards[i] = PickRedCard(greenCard, &players[i]);
            getchar();
        }

        //Tells the user the who the czar is and prompts the user to press enter to continue
        printf("The czar's turn is next.\n");
        printf("%s is the czar\n", players[numPlayers - 1].Name);
        printf("Press any key to continue: ");
        getchar();
        system("clear");

        //Returns the index of the player that one
        int winner = Czar(players, roundCards, greenCard, numPlayers);
        getchar();
        //Prints the winner and the card that one

        printf("%s won the round with %s\n", players[winner - 1].Name, players[winner - 1].redCards[roundCards[winner - 1]].Word);

        //Checks if the game has been won
        if (players[winner - 1].numWins == goal)
        {
            printf("%s won the game\n", players[winner - 1].Name);
            printf("Press any key to continue: ");
            getchar();
            break;
        }
        //Prompts the user to press enter to continue to the next line
        printf("Press any key to continue: ");
        getchar();
        system("clear");
    }
}

int main(void)
{
    //Seeds the random function so the cards are never the same
    srand(time(NULL));
    
    int greenCardsNum;
    int redCardsNum;
    int numPlayers;
    int goal;

    //Retrive the Green and redcards
    CardType *greenCards = LoadCards("greencards.txt", &greenCardsNum);

    CardType *redCards = LoadCards("redcards.txt", &redCardsNum);
    //Loads in the player
    PlayerType *players = GetPlayerData(redCards, redCardsNum, &numPlayers, &goal);
    //Starts the game
    GameLoop(greenCards, redCards, players, numPlayers, greenCardsNum, greenCardsNum, goal);
    
    //Release the memory
    free(greenCards);
    free(redCards);
    free(players);
    
    return 0;
}