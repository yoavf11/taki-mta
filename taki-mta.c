#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define RED 'R' // 1
#define BLUE 'B' // 2
#define GREEN 'G' // 3
#define YELLOW 'Y' // 4
#define PLUS '+' // 10
#define STOP "STOP" // 11
#define CHANGE_DIR "<->" // 12
#define COLOR "COLOR" // 13
#define TAKI "TAKI" // 14
#define CARD_ROWS 4
#define MAX_NAME_LEN 20
#define NUMBER_OF_CARDS 14

typedef struct stats {
    int value;
    int count;
}
STATS;

typedef struct card {
    int value; // (1-9 ,+, STOP, <->,COLOR, TAKI )
    char color;
} CARD;

typedef struct player {
    char name[MAX_NAME_LEN];
    int nofCards;
    int pySizeCard;
    CARD* cards;
} PLAYER;

bool isRegularCardMatch(PLAYER* player, CARD* upperCard, int playerAction);
char getColor(int num);
int cardInput(int nofCards, bool isTaki);
int getRandomNumber(int from, int to);
STATS* merge(STATS a1[], int size1, STATS a2[], int size2);
void determineWinner(PLAYER* player, bool* isWinner);
void chooseNewColor(char* color);
void copyArr(STATS dest[], STATS src[], int size);
void freeMemAlloc(PLAYER* players, int nofPlayers);
void gameHandler(PLAYER* players, int nofPlayers, CARD* upperCard, STATS* statistics);
void getNumOfPlayers(int* nofPlayers);
void getPlayerAction(int* playerAction, int nofCards, bool isTaki);
void getPlayersName(PLAYER* players, int nofPlayers);
void getRandomCard(CARD* card, bool isUpperCard, STATS* statistics);
void initGame(PLAYER* players, int nofPlayers, CARD* upperCard, STATS* statistics);
void initPlayers(PLAYER* players, int nofPlayers, STATS* statistics);
void initStatistics(STATS* statistics);
void mergeSort(STATS arr[], int size);
void nextPlayersMove(int* currentTurn, int nofPlayers, bool reverse);
void placeCard(PLAYER* player, int nofPlayers, CARD* upperCard, STATS* statistics, bool* isWinner, bool* isValidMove, bool* isTaki, bool* isReverseTurns, int* turn, int* playerAction);
void playCard(PLAYER* player, CARD* upperCard, int playerAction);
void playTakiWaterfall(PLAYER* player, int nofPlayers, CARD* upperCard, STATS* statistics, bool* isWinner, bool* isTaki, bool* isReverseTurns, int* playerAction);
void printCard(CARD card);
void printCardValue(int cardValue);
void printMemoryAllocFail();
void printPlayersHand(PLAYER* player);
void printSpecialStats(STATS* stats);
void printStatistics(STATS* stats);
void printTopCard(CARD* upperCard);
void printWelcomeMessage();
void takeCard(PLAYER* player, STATS* statistics);
void validateCardsMemAlloc(CARD* cards);
void validatePlayersMemAlloc(PLAYER* players);

void main() {
    int nofPlayers;
    PLAYER* players;
    CARD upperCard;
    STATS statistics[NUMBER_OF_CARDS];

    srand(time(NULL));
    printWelcomeMessage();
    getNumOfPlayers(&nofPlayers);
    players = (PLAYER*)malloc(nofPlayers * sizeof(PLAYER));
    validatePlayersMemAlloc(players);

    initGame(players, nofPlayers, &upperCard, statistics);
    gameHandler(players, nofPlayers, &upperCard, statistics);
    printStatistics(statistics);
}

// This function prints the welcome message
void printWelcomeMessage() {
    printf("************  Welcome to TAKI game !!! *********** \n");
}

// This function prints memory allocation failure message, and exits the program
void printMemoryAllocFail() {
    printf("Oh No! Memory allocation failure. Exiting program.\n");
    exit(1);
}

// This function frees player's and player's cards array memory allocations 
void freeMemAlloc(PLAYER* players, int nofPlayers) {
    for (int i = 0; i < nofPlayers; i++) {
        free((players + i)->cards);
    }
    free(players);
}

// This function validates player's pointer (not NULL)
void validatePlayersMemAlloc(PLAYER* players) {
    if (players == NULL) {
        printMemoryAllocFail();
    }
}


// This function validates card's pointer (not NULL)
void validateCardsMemAlloc(CARD* cards) {
    if (cards == NULL) {
        printMemoryAllocFail();
    }
}

// This function gets player's name as an input (for all players)
void getPlayersName(PLAYER* players, int nofPlayers) {
    for (int i = 0; i < nofPlayers; i++) {
        printf("Please enter the first name of player #%d:\n", i + 1);
        scanf("%s", (players + i)->name);
    }
}

// This function initializes statistics
void initStatistics(STATS* statistics) {
    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        (statistics + i)->count = 0;
        (statistics + i)->value = i + 1;
    }
}

// This function generates and returns a random number
int getRandomNumber(int from, int to) {
    int num;
    num = from + rand() % to;
    return num;
}

// This function returns a card color for a specific number
char getColor(int num) {
    char cardColor;
    switch (num) {
    case 1:
        cardColor = YELLOW;
        break;

    case 2:
        cardColor = RED;
        break;

    case 3:
        cardColor = BLUE;
        break;

    case 4:
        cardColor = GREEN;
        break;

    default:
        break;
    }
    return cardColor;
}

// This function generates a random card
void getRandomCard(CARD* card, bool isUpperCard, STATS* statistics) {
    int num;
    if (isUpperCard) {
        // get card number between 1-9
        num = getRandomNumber(1, 9);
    }
    else {
        // get card number between 1-14
        num = getRandomNumber(1, 14);
    }
    card->value = num;

    if (num != 13) {
        // get cards color
        num = getRandomNumber(1, 4);
        card->color = getColor(num);
    }
    else {
        card->color = ' ';
    }
    statistics[card->value - 1].count++;
}

// This function initializes players and cards
void initPlayers(PLAYER* players, int nofPlayers, STATS* statistics) {
    for (int i = 0; i < nofPlayers; i++) {
        (players + i)->nofCards = 4;
        (players + i)->pySizeCard = 4;
        (players + i)->cards = (CARD*)malloc((players + i)->pySizeCard * sizeof(CARD));
        validateCardsMemAlloc((players + i)->cards);

        for (int j = 0; j < (players + i)->nofCards; j++) {
            getRandomCard(&(players + i)->cards[j], false, statistics);
        }
    }
}

// This function initializes players, cards and statistics
void initGame(PLAYER* players, int nofPlayers, CARD* upperCard, STATS* statistics) {
    getPlayersName(players, nofPlayers);
    initStatistics(statistics);
    // initialize first card
    getRandomCard(upperCard, true, statistics);
    initPlayers(players, nofPlayers, statistics);
}


// This function gets the number of players as an input
void getNumOfPlayers(int* nofPlayers) {
    printf("Please enter the number of players: \n");
    scanf("%d", nofPlayers);
    if (*nofPlayers < 1) {
        exit(1);
    }
}

// This function prints a card value
void printCardValue(int cardValue) {
    switch (cardValue) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        printf("*   %d   *", cardValue);
        break;
    case 10:
        printf("*   %c   *", PLUS);
        break;
    case 11:
        printf("*  %s *", STOP);
        break;
    case 12:
        printf("*  %s  *", CHANGE_DIR);
        break;
    case 13:
        printf("* %s *", COLOR);
        break;
    case 14:
        printf("*  %s *", TAKI);
        break;
    default:
        break;
    }
}

// This function prints a card
void printCard(CARD card) {
    printf("*********\n");
    for (int i = 0; i < CARD_ROWS; i++)
    {
        if (i == 1) {
            printCardValue(card.value);
        }
        else if (i == 2) {
            printf("*   %c   *", card.color);
        }
        else {
            printf("*       *");
        }

        printf("\n");
    }
    printf("*********\n");
    printf("\n");
}

// This function prints the deck's top card
void printTopCard(CARD* upperCard) {
    printf("\nUpper card:\n");
    printCard(*upperCard);
}

// This function prints all player's cards
void printPlayersHand(PLAYER* player) {
    printf("%s's turn:\n\n", (player)->name);
    for (int i = 0; i < (player)->nofCards; i++) {
        printf("Card #%d:\n", i + 1);
        printCard((player)->cards[i]);
    }
}

// This function gets player's card choice
int cardInput(int nofCards, bool isTaki) {
    int number;
    if (!isTaki) {
        printf("Please enter 0 if you want to take a card from the deck\n");
    }
    else {
        printf("Please enter 0 if you want to finish your turn\n");
    }
    printf("or 1-%d if you want to put one of your cards in the middle:\n", nofCards);
    scanf("%d", &number);

    return number;
}

// function that gets the card number input and checks if it is valid
void getPlayerAction(int* playerAction, int nofCards, bool isTaki) {
    int number;
    number = cardInput(nofCards, isTaki);

    while (number < 0 || number > nofCards) {
        if (!isTaki) {
            printf("Invalid choice! Try again.\n");
        }
        else {
            printf("Invalid card! Try again.\n");
        }
        number = cardInput(nofCards, isTaki);
    }
    *playerAction = number;
}

// This function handles player's 'take card for deck' action and changes the cards array
void takeCard(PLAYER* player, STATS* statistics) {
    CARD* temp;

    if (player->pySizeCard == player->nofCards) {
        player->pySizeCard *= 2;
        temp = (CARD*)malloc(player->pySizeCard * sizeof(CARD));
        validateCardsMemAlloc(temp);
        for (int i = 0; i < player->nofCards; i++) {
            temp[i] = player->cards[i];
        }
        free(player->cards);
        player->cards = temp;
    }
    getRandomCard(&((player)->cards[((player)->nofCards)++]), false, statistics);
    temp = (CARD*)malloc(player->nofCards * sizeof(CARD));
    validateCardsMemAlloc(temp);

    player->pySizeCard = player->nofCards;
    for (int i = 0; i < player->nofCards; i++) {
        temp[i] = player->cards[i];
    }

    free(player->cards);
    player->cards = temp;
}

// This function handles next player's move
void nextPlayersMove(int* currentTurn, int nofPlayers, bool reverse) {

    if (reverse) {
        if (*currentTurn >= 1) {
            *currentTurn -= 1;
        }
        else {
            *currentTurn = nofPlayers - 1;
        }
    }
    else {
        if (*currentTurn < nofPlayers - 1) {
            *currentTurn += 1;
        }
        else {
            *currentTurn = 0;
        }
    }

}

// This function checks if the player chose a regular card and matches to the upper card's color or value
bool isRegularCardMatch(PLAYER* player, CARD* upperCard, int playerAction) {
    return ((player)->cards[playerAction - 1].value >= 1 &&
        (player)->cards[playerAction - 1].value <= 9) &&
        ( (player)->cards[playerAction - 1].color == upperCard->color ||
            (player)->cards[playerAction - 1].value == upperCard->value);
}

// This function updates the upper card to the player's chooice and updates player's cards array
void playCard(PLAYER* player, CARD* upperCard, int playerAction) {
    *upperCard = (player)->cards[playerAction - 1];
    (player)->cards[playerAction - 1] = (player)->cards[(player)->nofCards - 1];
    (player)->nofCards--;
}

// This function hanldes player's color pick input
void chooseNewColor(char* color) {
    int colorNum;
    printf("Please enter your color choice:\n"
        "1 - Yellow\n"
        "2 - Red\n"
        "3 - Blue\n"
        "4 - Green\n");
    scanf("%d", &colorNum);
    *color = getColor(colorNum);
}

// This function handles TAKI waterfall card logic
void playTakiWaterfall(PLAYER* player, int nofPlayers, CARD* upperCard, STATS* statistics, bool* isWinner, bool* isTaki,
    bool* isReverseTurns, int* playerAction) {
    while ((*isTaki)) {
        printTopCard(upperCard);
        printPlayersHand(player);
        getPlayerAction(playerAction, (player)->nofCards, (*isTaki));

        while ((player)->cards[(*playerAction) - 1].color != upperCard->color && (*playerAction) != 0) {
            printf("Invalid choice! Try again.\n");
            getPlayerAction(playerAction, (player)->nofCards, (*isTaki));
        }

        if ((player)->cards[(*playerAction) - 1].color == upperCard->color) {
            if ((*playerAction) == 13) {
                chooseNewColor(&(player)->cards[(*playerAction) - 1].color);
            }
            playCard(player, upperCard, (*playerAction));
            (*isTaki) = false;
        }

        if ((*playerAction) == 0 || (player)->nofCards == 0) {
            if (upperCard->value == 10) {
                takeCard(player, statistics);
            }
            if (upperCard->value == 11 && nofPlayers == 2 && (player)->nofCards == 0) {
                takeCard(player, statistics);
            }
            if (upperCard->value == 12) {
                (*isReverseTurns) = !(*isReverseTurns);
            }
            if (upperCard->value == 13) {
                chooseNewColor(&(player)->cards[(*playerAction) - 1].color);
            }

            if ((player)->nofCards == 0) {
                (*isWinner) = true;
            }
            (*isTaki) = false;
        }
    }
}

// This function handles player's card action
void placeCard(PLAYER* player, int nofPlayers, CARD* upperCard, STATS* statistics, bool* isWinner, bool* isValidMove,
    bool* isTaki, bool* isReverseTurns, int* turn, int* playerAction) {
    // regular card
    if (isRegularCardMatch(player, upperCard, (*playerAction))) {
        playCard(player, upperCard, (*playerAction));
        nextPlayersMove(turn, nofPlayers, (*isReverseTurns));
        (*isValidMove) = !(*isValidMove);
    }
    // plus
    else if ((player)->cards[(*playerAction) - 1].value == 10 &&
        (player)->cards[(*playerAction) - 1].color == upperCard->color) {
        playCard(player, upperCard, (*playerAction));
        if ((player)->nofCards == 1) {
            takeCard(player, statistics);
        }
        (*isValidMove) = !(*isValidMove);
    }
    // stop card
    else if ((player)->cards[(*playerAction) - 1].value == 11 &&
        (player)->cards[(*playerAction) - 1].color == upperCard->color) {
        if (nofPlayers == 2 && (player)->nofCards == 1) {
            takeCard(player, statistics);
        }
        playCard(player, upperCard, (*playerAction));
        (*turn)++;
        nextPlayersMove(turn, nofPlayers, (*isReverseTurns));
        (*isValidMove) = !(*isValidMove);
    }
    // change-dir
    else if ((player)->cards[(*playerAction) - 1].value == 12 &&
        (player)->cards[(*playerAction) - 1].color == upperCard->color) {
        playCard(player, upperCard, (*playerAction));
        (*isReverseTurns) = !(*isReverseTurns);
        nextPlayersMove(turn, nofPlayers, (*isReverseTurns));
        (*isValidMove) = !(*isValidMove);
    }
    // change color
    else if ((player)->cards[(*playerAction) - 1].value == 13) {
        (*isTaki) = false;
        chooseNewColor(&(player)->cards[(*playerAction) - 1].color);
        playCard(player, upperCard, (*playerAction));
        nextPlayersMove(turn, nofPlayers, (*isReverseTurns));
        (*isValidMove) = !(*isValidMove);
    }
    // Taki
    else if ((player)->cards[(*playerAction) - 1].value == 14 &&
        (player)->cards[(*playerAction) - 1].color == upperCard->color) {
        (*isTaki) = true;
        playCard(player, upperCard, (*playerAction));
        if ((player)->nofCards == 0) {
            *isWinner = true;
        }
        else {
            playTakiWaterfall(player, nofPlayers, upperCard, statistics, isWinner, isTaki, isReverseTurns, playerAction);
        }

        if (!(*isWinner)) {
            nextPlayersMove(turn, nofPlayers, (*isReverseTurns));
        }
        (*isValidMove) = !(*isValidMove);
    }
    else {
        printf("Invalid choice! Try again.\n");
    }
}

// This function checks for a winner and prints a message and changes the isWinner flag to true
void determineWinner(PLAYER* player, bool* isWinner) {
    if ((player)->nofCards == 0) {
        printf("The winner is... %s! Congratulations!\n", (player)->name);
        *isWinner = true;
    }
}

// This fnuction handels the game's session
void gameHandler(PLAYER* players, int nofPlayers, CARD* upperCard, STATS* statistics) {
    bool isWinner = false, isValidMove = false, isTaki = false, isReverseTurns = false;
    int turn = 0, playerAction;

    while (!isWinner) {
        printTopCard(upperCard);
        printPlayersHand(players + turn);
        // player has to pick a valid card to make a move
        while (!isValidMove) {
            getPlayerAction(&playerAction, (players + turn)->nofCards, isTaki);

            if (playerAction == 0) {
                takeCard(players + turn, statistics);
                nextPlayersMove(&turn, nofPlayers, isReverseTurns);
                isValidMove = !isValidMove;
            }
            else {
                placeCard(players + turn, nofPlayers, upperCard, statistics, &isWinner, &isValidMove, &isTaki, &isReverseTurns, &turn, &playerAction);
            }
        }
        if (!isWinner) {
            isValidMove = !isValidMove;
        }
        determineWinner(players + turn, &isWinner);
    }

    freeMemAlloc(players, nofPlayers);
}

// This function merges and sorts two arrays to one
STATS* merge(STATS a1[], int size1, STATS a2[], int size2) {
    int ind1, ind2, resInd;
    STATS* res = (STATS*)malloc((size1 + size2) * sizeof(STATS));

    if (res)
    {
        ind1 = ind2 = resInd = 0;

        while ((ind1 < size1) && (ind2 < size2)) {
            if (a1[ind1].count > a2[ind2].count) {
                res[resInd] = a1[ind1];
                ind1++;
            }
            else {
                res[resInd] = a2[ind2];
                ind2++;
            }
            resInd++;
        }

        while (ind1 < size1) {
            res[resInd] = a1[ind1];
            ind1++;
            resInd++;
        }
        while (ind2 < size2) {
            res[resInd] = a2[ind2];
            ind2++;
            resInd++;
        }
    }
    return res;
}

// This function sorts the statistics array
void mergeSort(STATS arr[], int size)
{
    STATS* tmpArr = NULL;
    if (size <= 1)
        return;

    mergeSort(arr, size / 2);
    mergeSort(arr + size / 2, size - size / 2);
    tmpArr = merge(arr, size / 2, arr + size / 2, size - size / 2);

    if (tmpArr)
    {
        copyArr(arr, tmpArr, size); // copy values from tmpArr to arr
        free(tmpArr);
    }
    else
    {
        printf("Memory allocation failure!!!\n");
        exit(1);
    }
}

// This function copies an array to another
void copyArr(STATS dest[], STATS src[], int size)
{
    for (int i = 0; i < size; i++)
        dest[i] = src[i];
}

// This function prints specical card's stats (for each row)
void printSpecialStats(STATS* stats) {
    switch ((stats)->value) {
    case 10:
        printf("   +   |    %d  \n", (stats)->count);
        break;
    case 11:
        printf(" STOP  |    %d  \n", (stats)->count);
        break;
    case 12:
        printf("  <->  |    %d  \n", (stats)->count);
        break;
    case 13:
        printf(" COLOR |    %d  \n", (stats)->count);
        break;
    case 14:
        printf(" TAKI  |    %d  \n", (stats)->count);
        break;
    }
}

// This function prints the game's stats
void printStatistics(STATS* stats) {
    mergeSort(stats, NUMBER_OF_CARDS);
    printf("\n************ Game Statistics ************ \n"
        "Card # | Frequency\n"
        "__________________\n");

    for (int i = 0; i < NUMBER_OF_CARDS; i++) {
        if ((stats + i)->value < 10) {
            printf("   %d   |    %d \n", (stats + i)->value, (stats + i)->count);
        }
        else {
            printSpecialStats(stats + i);
        }
    }
}
