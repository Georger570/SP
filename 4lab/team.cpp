#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>


//Char* to Int conversion
int to_int(char* str);

//Timing to actually catch something
void Timing(char team, int time);

//Game attack function
int Attack(const int AttackPower, int* DefenderCount);

//Game function to reinforce forces *BADUM-TS*
int Regenerate(int* OwnCount,const int Reinforcements);


int main(int argc, char *argv[])
{ 
    //-----------GAME OPTIONS--------------
    int RFs_coeff = 900;
    int pause_time=1;
    int rounds = 5;
    //-----------INIT----------------------
    int OF, EF;
    char c_OF[100], c_EF[100];
    char* team;
    //Getting input/output PIPES
    int IO[2] = {0, 0};
    for (int i =  0; i<2; ++i)
    IO[i] = to_int(argv[i]);
    team = argv[2];
    //Getting initial Force
    OF = to_int(argv[3]);
    //------------GAME---------------------
    for( int i = 0; i<rounds; i++ )
    {
        //Good looking output
        if ( team[0] == 'B')
            sleep(1);
        if (team[0] == 'A')
            std::cout << "\n\t\tRound " << i+1<<"\n\n";
        std::cout << "\tTeam's "<<team<<" power: "<<OF<<"\n\n";  
        if ( team[0] == 'A')
            sleep(1);
        Timing(team[0], pause_time);
        srand(time(NULL));
        //Regeneration phase
        int RFs = OF;
        if (OF < 15)
            RFs=10;
        RFs = rand() % (700/RFs);
        Regenerate(&OF, RFs);
        std::cout << "\nTeam " << team << " forces got " << RFs << " reinforcments and now on " << OF << " manpower!" << std::endl;
        //Send current own forces quantity
        write(IO[1], std::to_string(OF).c_str(), 100);
        read(IO[0], c_EF, 100);
        EF = to_int(c_EF);
        //Attack phase
        int AP = rand() % OF;
        Timing(team[0], pause_time);
        std::cout << "\nTeam " << team << " forces going to attack with " << AP << " power!" << std::endl;
        Attack(AP, &EF);
        std::cout <<"Defending forces suffered losses and now on " << EF << " manpower!" << std::endl;
        //Sending rcurrent enemy forces quantity
        write(IO[1], std::to_string(EF).c_str(), 100);
        read(IO[0], c_OF, 100);
        //Forces can't go below zero
        OF = to_int(c_OF);
        if (OF < 0)
            OF = 0;
        sleep(2);
    }
    //Final output
    write(IO[1], c_OF, 100);
    exit(0);
}

int Attack(const int AttackPower, int* DefenderCount)
{
    (*DefenderCount) -= AttackPower;
    return 0;
}

int Regenerate(int* OwnCount,const int Reinforcements)
{
    (*OwnCount) += Reinforcements;
    return 0;
}

int to_int(char* str)
{
    int value =0;
    int i = 0;
    while (str[i] != '\0')
    {
        value = value*10 + str[i] - '0';
        ++i;
    }
    return value;
}

void Timing(char team, int time)
{
    sleep(time);
    if ( team == 'B')
        sleep(time);
}

