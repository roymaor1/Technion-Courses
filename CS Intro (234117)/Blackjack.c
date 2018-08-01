/*----------------------------------------------------------------
  This program is a Blackjack Game Simulator.
  Player vs. Computer.
  If you get over 21, you lose.
  Otherwise, if your tally is larger than the computers', you WIN!
  Enjoy!
 ------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#define min_card 1
#define max_card 13
#define start 1000
#define min_bet 1

int rand_range (int,int);
void srand_range (unsigned int);
int Card_Dealer (int*);
void Enter_Seed ();
int Bet_Check (int*,int*);
void Enter_Bet (int*,int*);
void Side_Simulation (int*,int*,int*,int*,int*,int);
int Is_Card_Unique (int);
char Unique_Int_To_Letter (int);
void Print_Draw (int*,int*,int);
int Find_Rare_Card (int*);
void Print_Extra_Card (int);
void Update_Sum_Extra (int,int*,int*,int*);
char Ask_Player_For_Extra (int*);
void Extra_Card(int,int*,int*,int*);
void Player_Simulation (int*,int*,int*,int*,int*);
void Sudden_Death (int*,int*,int*);
int Decide_Winner (int*,int*,int*);
int Calc_Bonus (int*,int*);
void Print_Winner (int*,int*,int*,int*,int*);
void In_Game_Operations (int*,int*,int*,int*,int*,int*);
void Game_Simulation(int*,int*,int*,int*,int*,int*,int*,int*,int*,int*);

// Returns random integers between 2 given numbers, including edges
// Function Length: 3 Rows
int rand_range (int low, int high)
{
    static unsigned int next = 1;
    next = next * 1103515245 + 12345;
    return ((unsigned int)(next/65536) % 32768) % (high - low + 1) + low;
}

// Use seed for random values
// Function Length: 2 Rows
void srand_range (unsigned int seed)
{
    for (int i = 0; (unsigned int)i < seed; i++)
    {
        rand_range(i,i+1);
    }
}

// Draw new card, return it, and update the frequency array
// Function Length: 3 Rows
int Card_Dealer (int* Frequency)
{
    int card=rand_range(min_card,max_card);
    Frequency[card-1]++; // update frequency array for rare card
    return card;
}

// Welcomes the player and asks for seed input
// Function Length: 5 Rows
void Enter_Seed ()
{
    int seed;
    printf("Welcome to CS intro Blackjack!\n");
    printf("Start by entering a random seed:\n");
    scanf("%d", &seed);
    srand_range(seed);
}

// Return 1 if players bet input is legal, 0 if not
// Function Length: 5 Rows
int Bet_Check (int* bet, int* amount)
{
  if(*bet>*amount || *bet<min_bet) // if bet is illegal
    {
        printf("Your bet must be at least ");
        printf("%d and at most %d, try again:\n", min_bet, *amount);
        return 0;
    }
  else
    {
        return 1;
    }
}

// Ask player for bet amount
// Function Length: 5 Rows
void Enter_Bet (int* bet, int* amount)
{
  printf("You currently have %d fake dollars.", *amount);
  printf(" How much do you want to bet?\n");
  scanf("%d", bet);
  while(!Bet_Check(bet, amount)) //as long as bet is illegal
  {
    scanf("%d", bet);
  }
}

// Simulates card draws for computer and player, updates sums
// Function Length: 7 Rows
void Side_Simulation (int* card1, int* card2, int* freq,int* sumpc,
                      int* sumplayer,int id)
{
    *card1=Card_Dealer(freq); // draw 1st card
    *card2=Card_Dealer(freq); // draw 2nd card
    if(id==0) // if its computers turn
    {
    *sumpc=0; //zero sum from last round
    *sumpc=*card1+*card2; // update computers sum
    }
    else // if its players turn
    {
    *sumplayer=0; //zero sum from last round
    *sumplayer=*card1+*card2; // update players sum
    }
}

// Returns 1 if drawn card is ace/prince/queen/king, 0 otherwise
// Function Length: 3 Rows
int Is_Card_Unique (int card)
{
    if(card>1 && card<11)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// Return unique cards to letters
// Function Length: 7 Rows
char Unique_Int_To_Letter (int card)
{
    switch(card)
    {
        case 1: return 'A'; break;
        case 11: return 'J'; break;
        case 12: return 'Q'; break;
        case 13: return 'K'; break;
    }
    return 'Z'; /* just to return something. this value will never be
                returned (function is called only when card is unique)*/
}

// Prints results of the draw (both cards) for computer and player
// Function Length: 11 Rows
void Print_Draw (int* c1, int* c2, int id)
{
    if(id==0) //if its computers turn (first card print)
    {
        if(Is_Card_Unique(*c1))
        {
            printf("The computer got %c", Unique_Int_To_Letter(*c1));
        }
        else
        {
            printf("The computer got %d", *c1);
        }
    }
    else // if its players turn (first card print)
    {
        if(Is_Card_Unique(*c1))
        {
            printf("You got %c", Unique_Int_To_Letter(*c1));
        }
        else
        {
            printf("You got %d", *c1);
        }
    }
    // second card print (for both computer and player)
    if(Is_Card_Unique(*c2))
        {
            printf(" and %c.\n", Unique_Int_To_Letter(*c2));
        }
    else
        {
            printf(" and %d.\n", *c2);
        }
}

/* Returns the rare card (card that has been drawed in the game the
   least amount of times). If there are several, returns the maximum */
// Function Length: 6 Rows
int Find_Rare_Card (int* freq)
{
    int rare_card=min_card-1; // define rare card as the 1st index
    for (int i=min_card; i<max_card; i++)
    {
        if(*(freq+i)<=*(freq+rare_card)) /* if current card has been
        drawn less times than previous rare card */
        {
            rare_card=i; // define rare card as current index
        }
    }
    return rare_card+1; // add 1 to index to get valie of rare card
}

// Prints extra card
// Function Length: 3 Rows
void Print_Extra_Card (int card)
{
    if(Is_Card_Unique(card))
    {
        printf("Extra card:%c.\n", Unique_Int_To_Letter(card));
    }
    else
    {
        printf("Extra card:%d.\n", card);
    }
}

// Adds the extra card to the relevant sum
// Function Length: 6 Rows
void Update_Sum_Extra (int id, int* freq, int* sumpc, int* sumplayer)
{
    int extra;
    extra=Card_Dealer(freq);
    if(id==0) // if its computers turn
    {
        *sumpc+=extra; // add to computer sum
    }
    else // if its players turn
    {
        *sumplayer+=extra; // add to players sum
    }
    Print_Extra_Card(extra);
}

// Asks player if he wants extra cards, returns his one letter answer
// Function Length: 5 Rows
char Ask_Player_For_Extra (int* sumplayer)
{
            char letter;
            printf("The current sum is %d.", *sumplayer);
            printf(" Do you want another card? (Y / N)\n");
            scanf(" %c", &letter);
            return letter;
}

// Draws extra card (if necessary) for computer and player
// Function Length: 11 Rows
void Extra_Card(int id, int* freq, int* sumpc, int* sumplayer)
{
    if(id==0) // if its computers turn
    {
        //as long as projected card to be drawn doesnt put sum over 21
        while(*sumpc+Find_Rare_Card(freq)<22)
            {
                Update_Sum_Extra(0,freq,sumpc,sumplayer); // take extra
            }
    }
    else // if its players turn
    {
        // as long as extra card can help to raise sum not passed 21
        while(*sumplayer<21)
        {
            char let;
            let=Ask_Player_For_Extra(sumplayer);
            if(let=='Y') // if answer is positive, take card
                {
                    Update_Sum_Extra(1,freq,sumpc,sumplayer);
                }
            else
                {
                    break; // finish asking for extra card
                }
        }
    }
}

// Runs Side_Simulation for player and prints his draw
// Function Length: 2 Rows
void Player_Simulation (int* c1, int* c2, int* freq, int* sumpc,
                        int* sumplayer)
{
           Side_Simulation(c1,c2,freq,sumpc,sumplayer,1);
           Print_Draw(c1,c2,1);
}

// Call winner when no side passed 21
// Function Length: 3 Rows
void Sudden_Death (int* sumpc,int* sumplayer, int* winner)
{
    if(*sumplayer>=*sumpc)
    {
        *winner=1; //player wins
    }
    else
    {
        *winner=0; //comp wins
    }
}

// Decides who wins by opponent passing 21, or if no one past it.
// Function Length: 9 Rows
int Decide_Winner (int* sumpc,int* sumplayer, int* winner) //0 if pc wins
{
    if(*sumpc>21 || *sumplayer>21) //someone lost before sudden death
    {
        if(*sumpc>21)
        {
            if(*winner==-1) // so message will be printed only once
            {
                printf("The computer got more than 21!\n");
            }
            *winner=1; //winner is player
            return 1;
        }
        else //player lost
        {
            *winner=0; //winner is computer
            return 0;
        }
    }
    return 2; //if nobody passed 21
}

// Calculates bonus for player
// Function Length: 3 Rows
int Calc_Bonus (int* wins_in_a_row, int* winning_bets_sum)
{
    int bonus;
    bonus=0.1*((*winning_bets_sum)/(*wins_in_a_row));
    return bonus;
}

// Prints winner and updates game values (money amount, wins, etc.)
// Function Length: 13 Rows
void Print_Winner (int* winner, int* wins_in_a_row,
                   int* winning_bets_sum, int* c_amount, int* c_bet)
{
    if(*winner==1) //if player won
    {
        printf("You have won %d fake dollars!\n", *c_bet);
        *c_amount+=*c_bet; //add winning prize to player tally
        *wins_in_a_row+=1; //add win to streak
        *winning_bets_sum+=*c_bet; // add bet to sum of winning bets
    }
    else //if computer won
    {
        printf("The computer has won this round... you lose ");
        printf("%d fake dollars!\n", *c_bet);
        if(*wins_in_a_row>2)
        {
            int bonus=Calc_Bonus(wins_in_a_row,winning_bets_sum);
            printf("You have won %d times in a row!\n", *wins_in_a_row);
            printf("You deserve a bonus of %d fake dollars!\n", bonus);
            *c_amount+=bonus-(*c_bet);
        }
        else
        {
            *c_amount-=*c_bet;
        }
    }
}


// Operates draws for both sides and checks if a side has passed 21
// Function Length: 9 Rows
void In_Game_Operations (int* c1,int* c2, int* sumpc,int* sumplayer,
                         int* freq, int* winner)
{
    // if computer hasnt lost yet, try taking extra card
    if(Decide_Winner(sumpc,sumplayer,winner)!=1)
       {
           Extra_Card(0,freq,sumpc,sumplayer);
           // if computer hasnt lost even after extra cards
           if(Decide_Winner(sumpc,sumplayer,winner)!=1)
               {
                   Player_Simulation(c1,c2,freq,sumpc,sumplayer);
                   // if player hasnt lost yet, ask him for extra cards
                   if(Decide_Winner(sumpc,sumplayer,winner)!=0)
                   {
                       Extra_Card(1,freq,sumpc,sumplayer);
                   }
               }
       }
}

// Simulates occurence of a round in the Blackjack game
// Function Length: 13 Rows
void Game_Simulation(int* c1,int* c2,int* sumpc,int* sumplayer, int* freq,
                     int* winner,int* wins_in_a_row,
                     int* winning_bets_sum, int* bet, int* amount)
{
       Enter_Bet(bet,amount); // player enters his bet
       Side_Simulation(c1,c2,freq,sumpc,sumplayer,0); //draw cards
       Print_Draw(c1,c2,0); // prints computer drawn cards
       In_Game_Operations(c1,c2,sumpc,sumplayer,freq,winner);
       if(Decide_Winner(sumpc,sumplayer,winner)==2) //if no one passed 21
       {
            Sudden_Death(sumpc,sumplayer,winner); // who has biggest sum
       }
       Print_Winner(winner,wins_in_a_row,winning_bets_sum,amount,bet);
       if(*winner==0) // if computer won, zero this rounds variables
       {
            *wins_in_a_row=0;
            *winning_bets_sum=0;
       }
       *winner=-1; //reset winner after end of round
       *sumpc=0;
       *sumplayer=0;
}

// Main Function
// Function Length: 10 Rows
int main()
{
    Enter_Seed();
    int Frequency[max_card-min_card+1] = {0};
    int curr_amount=start, curr_bet=0, pc_sum=0, plyr_sum=0, card1, card2;
    int winner=-1, wins_in_a_row=0, winning_bets_sum=0;
    while(curr_amount>0) // as long as player still has money
    {
        Game_Simulation(&card1,&card2,&pc_sum,&plyr_sum,Frequency,&winner,
                        &wins_in_a_row,&winning_bets_sum,&curr_bet,
                        &curr_amount); // call game
    }
    printf("Game Over!\n");
    return 0;
}
