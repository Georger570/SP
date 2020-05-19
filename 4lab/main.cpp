#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

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

int main()
{
    pid_t pid[2];
    int AF = 60;
    int BF = 60;
    int IO[2][2];
    if (pipe(IO[0])==-1)
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(IO[1])==-1) 
    { 
        fprintf(stderr, "Pipe Failed" );
        return 1; 
    }
    //Creating Processes
    pid[0] = fork();
    switch(pid[0])
    {
        case -1:
            fprintf(stderr, "Fork Failed" );
            exit(1);
        case 0: 
            execl("team", std::to_string(IO[0][0]).c_str(), std::to_string(IO[1][1]).c_str(), "A", std::to_string(AF).c_str(), NULL);
            break;
        default:
             pid[1] = fork();
             switch(pid[1])
             {
                 case -1:
                     fprintf(stderr, "Fork Failed" );
                     exit(1);
                 case 0:
                     execl("team", std::to_string(IO[1][0]).c_str(), std::to_string(IO[0][1]).c_str(), "B", std::to_string(BF).c_str(), NULL);
                     break;
                 default:
                     wait(NULL);
             }
 
             break;
    }
    char c_AF[100], c_BF[100];
    read(IO[1][0], c_AF, 100);
    read(IO[0][0], c_BF, 100);
    AF = to_int(c_AF);
    BF = to_int(c_BF);
    std::cout << "\nRemaining A forces: "<<AF
        <<"\nRemaining B forces: "<<BF<<"\n";
    if (AF > BF)
        std::cout << "\n\t\tTHE WINNER IS TEAM A!\n";
    else if (BF > AF)
        std::cout << "\n\t\tTHE WINNER IS TEAM B!\n";
    else
        std::cout << "\n\t\tDRAW!\n";
    exit(0);
}
