#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
//#include <spinlock.h>

//Team simulation
void* team(void* arg);

//Game attack function
int Attack(const int AttackPower, int* DefenderCount);

//Game function to reinforce forces *BADUM-TS*
int Regenerate(int* OwnCount,const int Reinforcements);

struct Targ {
    int TF;
    char Ttag;
};

//-----------GAME OPTIONS--------------
const int RFs_coeff = 700;
const int pause_time=1;
const int rounds = 5;

//--------------INIT-------------------
int AF = 60;
int BF = 60;
pthread_spinlock_t lock;
pthread_barrier_t barrier;
int main()
{

    pthread_spin_init(&lock, 0);
    pthread_barrier_init(&barrier, NULL, 2);
    pthread_t tid[2];
    for (int i = 0; i<2; i++) {
        int status = pthread_create( &tid[ i ], NULL, team, (void *)i );
        if (status !=0) perror( "pthread_create" ), exit( EXIT_FAILURE );
    }
    for (int i=0; i < 2; i++)
        pthread_join( tid[ i ], NULL );
    std::cout<<"\nRemaining A forces: "<<AF
        <<"\nRemaining B forces: "<<BF<<"\n";
    if (AF > BF)
        std::cout << "\n\t\tTHE WINNER IS TEAM A!\n";
    else if (BF > AF)
        std::cout << "\n\t\tTHE WINNER IS TEAM B!\n";
    else
        std::cout << "\n\t\tDRAW!\n";
    pthread_barrier_destroy(&barrier);
    pthread_spin_destroy(&lock);
    return ( EXIT_SUCCESS );
}

void* team(void * arg)
{ 
    
    //-----------INIT----------------------
    char team;
    int* OF;
    int* EF;
    int* i = (int*)arg;
    
    if (i == 0)
    {
        team = 'A';
        OF = &AF;
        EF = &BF;
    }
    else
    {
        team = 'B';
        OF = &BF;
        EF = &AF;
    }
    //------------GAME---------------------
    for( int i = 0; i<rounds; i++ )
    {
        //Good looking output
        if (team == 'B')
            sleep(1);        
        if (team == 'A')
            std::cout << "\n\t\tRound " << i+1<<"\n\n";
        std::cout << "\tTeam's "<<team<<" power: "<<*OF<<"\n\n";  
        if ( team == 'A')
        srand(time(NULL));
        pthread_barrier_wait(&barrier);
        sleep(1);
        //Regeneration phase
        int RFs = *OF;
        if (*OF < 15)
            RFs=10;
        RFs = rand() % (RFs_coeff/RFs);
        Regenerate(OF, RFs);
        pthread_spin_lock(&lock);
        std::cout << "\nTeam " << team << " forces got " << RFs << " reinforcments and now on " << *OF << " manpower!" << std::endl;
        pthread_spin_unlock(&lock);
        pthread_barrier_wait(&barrier);
        sleep(1);
        //Attack phase
        int AP = rand() % *OF;
        pthread_spin_lock(&lock);
        std::cout << "\nTeam " << team << " forces going to attack with " << AP << " power!" << std::endl;
        pthread_spin_unlock(&lock);
        Attack(AP, EF);
        pthread_spin_lock(&lock);
        std::cout <<"Defending forces suffered losses and now on " << *EF << " manpower!" << std::endl;
        pthread_spin_unlock(&lock);
        //Sending rcurrent enemy forces quantity
        //Forces can't go below zero
        if ((*OF) < 0)
            (*OF) = 0;
        pthread_barrier_wait(&barrier);
        sleep(2);
    }
    return NULL;
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
