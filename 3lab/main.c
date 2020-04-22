#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int diagonal_sum(u_int8_t ** array, const u_int8_t array_size)
{
    int sum = 0;
    for (u_int8_t i=0; i<array_size; ++i)
    {
        if(i == (array_size - i - 1) )
            sum += array[i][i];
        else
            sum = sum + array[i][i] + array[i][array_size-i-1];
    
    }
    return sum;
}

int diagonal_sum_asm(u_int8_t ** array, const u_int8_t array_size)
{
    int sum = 0;
    __asm__
    (
     "xor %%rdx, %%rdx\n"
     "xor %%rbx, %%rbx\n"
     "xor %%r10, %%r10\n"
     "start_loop:\n"
        "mov (%%rsi), %%rdi\n"
        "mov %%rcx, %%rax\n"
        "dec %%rax\n"
        "add %%rbx, %%rdi\n"
        "movb (%%rdi), %%r10b\n"
        "add %%r10, %%rdx\n"
        "cmp %%rbx, %%rax\n"
     "je end_loop\n"
        "mov (%%rsi), %%rdi\n" 
        "add %%rax, %%rdi\n"
        "movb (%%rdi), %%r10b\n"
        "add %%r10, %%rdx\n"
     "end_loop:\n"
        "inc %%rbx\n"
        "add $8, %%rsi\n"
     "loop start_loop\n" 
     :"=d"(sum)
     :"S"(array),"c"(array_size)
     :"%rbx","%rax", "%r10", "%rdi"
    );
    return sum;
}


int main()
{
    u_int8_t array_size = 5;
    srand(time(NULL));
    u_int8_t ** array = (u_int8_t**) calloc(array_size, sizeof(u_int8_t*));
    for (u_int8_t i=0; i<array_size; ++i)
    {
        array[i] = (u_int8_t*) calloc(array_size, sizeof(u_int8_t));
        array[i][i] = rand() % 256;
        array[i][array_size-i-1] = rand() % 256;
    }
    for (u_int8_t i=0; i<array_size; ++i)
    {
        for (u_int8_t j=0; j<array_size; ++j)
        {
            printf("%d\t",array[i][j]);
        }
        printf("\n");
    }
    printf("%d\n", diagonal_sum(array, array_size));
    printf("%d\n", diagonal_sum_asm(array, array_size));
    return 0;
}
