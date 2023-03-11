#include <stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include <cuda.h>


#define Display 10000
#define clear() printf("\033[H\033[J")      //simple function to clear the screen
#define gotoxy(lin,col) printf("\033[%d;%dH", lin, col)         //simple function to go to coordinates on the screen


void play();
void help();
void options();
void author();
void end_all();
void game();
int gains(int CASE[], int bet);
__global__ void gains_kernel(int *CASE, int *GAINS);



void options()  //Display the options to the user
{
    int choice;
    clear();
    printf("\n\n\t\t~~Welcome to Slot Machine~~\r\n\n");
    printf("\n\t\t Here are your options\r\n\n");
    printf("\n\t\t\t1.Play Game\r\n");
    printf("\n\t\t\t2.Help\r\n");
    printf("\n\t\t\t3.Authors\r\n");
    printf("\n\t\t\t4.Exit\r\n");
    printf("\n\tPlease enter the number for the option   ");
    scanf("%d",&choice);
    printf("\r");

    if(choice==1)     //play
        game();

    else if(choice==2)    //help
        help();

    else if(choice==3)    //Authors
        author();

    else    //exit
        end_all();

}

int Bet()       //Initialise the bet
{
    char bet;
    gotoxy(31,35);
    printf("If you want to leave the game press '4'.  ");
    gotoxy(30,35);
    printf("Wonderfull, you choose to play. How much do you to bet ? 1, 2 or 3   ");
    scanf("%c", &bet);
    while(bet != '1' && bet != '2' && bet != '3' && bet != '4')
    {
        gotoxy(30,104);
        printf(" ");
        gotoxy(30,104);
        scanf("%c", &bet);
    }
    gotoxy(32,40);
    if(bet == '1')
    {
        gotoxy(27, 98);
        printf("%c", bet);
        return 1;
    }
    else if(bet == '2')
    {
        gotoxy(27, 98);
        printf("%c", bet);
        return 2;
    }
    else if(bet == '3')
    {
        gotoxy(27, 98);
        printf("%c", bet);
        return 3;
    }
    else
    {
        options();
    }
    return 1;
}

int Credits(int credits, int bet)       //Calculate the credits
{
    gotoxy(27,69);
    printf("%d",credits);
    gotoxy(27,69);
    credits = credits - bet;
    printf("%d", credits);
    return credits;
}

//////////////////////////////////////////////////////////

__global__ void gains_kernel(int *CASE, int *GAINS) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < 4) {
        if (CASE[i] == 0)
            GAINS[i] = 0;

        if (CASE[i] == 2)
            GAINS[i] = 10;

        if (CASE[i] == 3)
            GAINS[i] = 15;

        if (CASE[i] == 4)
            GAINS[i] = 20;

        if (CASE[i] == 6)
            GAINS[i] = 25;

        if (CASE[i] == 9)
            GAINS[i] = 30;
    }
}

int gains(int CASE[], int bet)      //Calculate the winnings
{
    int GAINS[3];
    int *d_CASE, *d_GAINS;

    for (int i = 0; i < 4; i++)
        GAINS[i] = 0;

    cudaMalloc((void **)&d_CASE, sizeof(int) * 4);
    cudaMalloc((void **)&d_GAINS, sizeof(int) * 4);

    cudaMemcpy(d_CASE, CASE, sizeof(int) * 4, cudaMemcpyHostToDevice);

    int block_size = 4;
    int num_blocks = 1;
    if (block_size < 4)
        num_blocks = ceil(4 / block_size);

    gains_kernel<<<num_blocks, block_size>>>(d_CASE, d_GAINS);

    cudaMemcpy(GAINS, d_GAINS, sizeof(int) * 4, cudaMemcpyDeviceToHost);

    cudaFree(d_CASE);
    cudaFree(d_GAINS);

    int a=GAINS[0], b=GAINS[1], c=GAINS[2];
    int gains = a+b+c;

    if (gains == 0)
        gains = (-1)*200;

    else if ((GAINS[1] == 2) & (GAINS[2] == 2) & (GAINS[3] == 2))
        gains = 200;

    else if ((GAINS[1] == 3) & (GAINS[2] == 3) & (GAINS[3] == 3))
        gains = 300;

    else if ((GAINS[1] == 4) & (GAINS[2] == 4) & (GAINS[3] == 4))
        gains = 400;

    else if ((GAINS[1] == 6) & (GAINS[2] == 6) & (GAINS[3] == 6))
        gains = (-1)*200;
    
    else if ((GAINS[1] == 9) & (GAINS[2] == 9) & (GAINS[3] == 9))
        gains = 1000;
    
    else if (gains < 30)
            gains = (-1)*100;
        
    gains = gains * bet;
    gotoxy(27,39);
    printf("%d",gains);

    return gains;

}


void AffNum (int Case, int number)      //Display the numbers
{
    int retour = 10 + (Case*38);
    if (number == 0)
    {
        gotoxy(5, retour);
        printf("****************************");
        for(int i=6; i<22; i++)
        {
            gotoxy(i, retour);
            printf("*                          *");
        }
        gotoxy(22, retour);
        printf("****************************");
    }

    else if (number == 2)
    {
        gotoxy(5, retour);
        printf("****************************");
        for(int i=6; i<14; i++)
        {
            gotoxy(i, retour);
            printf("                           *");
        }
        gotoxy(14, retour);
        printf("****************************");
        for(int i =15; i<22;i++)
        {
            gotoxy(i, retour);
            printf("*                           ");
        }
        gotoxy(22, retour);
        printf("****************************");
    }

    else if (number == 3)
    {
        gotoxy(5, retour);
        printf("****************************");
        for(int i=6; i<14; i++)
        {
            gotoxy(i, retour);
            printf("                           *");
        }
        gotoxy(14, retour);
        printf("****************************");
        for(int i=15; i<22; i++)
        {
            gotoxy(i, retour);
            printf("                           *");
        }
        gotoxy(22, retour);
        printf("****************************");
    }

    else if (number == 4)
    {
        for(int i=5; i<14; i++)
        {
            gotoxy(i, retour);
            printf("*                          *");
        }
        gotoxy(14, retour);
        printf("****************************");
        for(int i=15; i<23; i++)
        {
            gotoxy(i, retour);
            printf("                           *");
        }
    }

    else if (number == 6)
    {
        for(int i =5; i<14;i++)
        {
            gotoxy(i, retour);
            printf("*                           ");
        }
        gotoxy(14, retour);
        printf("****************************");
        for(int i=15; i<22; i++)
        {
            gotoxy(i, retour);
            printf("*                          *");
        }
        gotoxy(22, retour);
        printf("****************************");
    }

    else if (number == 9)
    {
        gotoxy(5, retour);
        printf("****************************");
        for(int i=6; i<14; i++)
        {
            gotoxy(i, retour);
            printf("*                          *");
        }
        gotoxy(14, retour);
        printf("****************************");
        for(int i=15; i<23; i++)
        {
            gotoxy(i, retour);
            printf("                           *");
        }
    }


}

void Affichage(int NUM[], int credits)    //Display the machine
{
    FILE *display;
    char str[Display];
    char* filename = "machine.txt";

    display = fopen(filename,"r");


    if (display == NULL)
    {
        printf("Could not open file %s\n", filename);
        exit(-1);
    }

    while (fgets(str, Display, display) != NULL)
    {
        printf("%s", str);
    }


    while (credits>0)
    {

        srand(time(NULL));
        //defined the number
        int nbr1 = NUM[rand() %6];
        int nbr2 = NUM[rand() %6];
        int nbr3 = NUM[rand() %6];
        
        gotoxy(27,69);
        printf("%d", credits);

        int bet = Bet();
        credits = Credits(credits, bet);

        //Display the number
        AffNum(0, nbr1);
        AffNum(1, nbr2);
        AffNum(2, nbr3);

        int CASE[3]={nbr1, nbr2, nbr3};
        int gains_tot = gains(CASE, bet);
        credits = credits + gains_tot;
        gotoxy(27,69);
        printf("%d", credits);
        printf("\n");
    }
          
}

void game()     //Initialise the game
{
    clear();
    int credits=200;
    int NUM[6]={0,2,3,4,6,9};
    Affichage(NUM, credits);

}

void help()     //explains the rules of the slot game which i don't really know myself so it would be fun
{
    clear();
    char m;
    printf("\n\n\t\t\tHere are the rules.\n\n");
    printf("\t\tYou start with 200 credits.\n\t\tThree numbers will be randomly choosen. \n\t\tDepending on the combinaison you will gain or lose a certain amount of points.\n\n");
    printf("\t\tTo return to the menu press 'r'. ");
    scanf("%c", &m);
    if(m == 'r' || m == 'R')
        options();

    else
        help();
    
}

void author()       //gives some background on who created it
{
    clear();
    char m;
    printf("\n\t\t\tAuthors are Chloé Maillot and Christine Kostine\r\n");
    printf("\t\t\tStarted in April, 6th 2020\r\n");
    printf("\t\t\tDone as project of first year\r\n");
    printf("\t\t\tStudents of ESIEA-Engineering school and research center \r\n");
    printf("\n\t\tTo return to the menu press 'r'. ");
    scanf("%c", &m);
    if(m == 'r' || m == 'R')
        options();
    
    else
        author();

}

void end_all()      //the good bye message and terminating function
{
    clear();
    printf("Thank you for playing with our slot machine ! ʕ•̀ω•́ʔ✧ \r\n\n");
    exit(0);
}


int main() 
{
    options();
    return 0;
}