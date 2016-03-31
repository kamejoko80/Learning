#include <stdio.h>
#include <time.h>
#include <sys/time.h>

long get_time();
void f1_N(int n);
void f2_N2(int n);
void f3_N3(int n);
void f4_N2(int n);
void f5_N4(int n);
void f6_N3(int n);

int main(int argc, char* argv[])
{
    void (*pFunction[6])(int) = {f1_N, f2_N2, f3_N3, f4_N2, f5_N4, f6_N3};
    long time1;
    long time2;

    for (int i = 0; i < 6; i++)
    {
        time1 = get_time();
        (*pFunction[i])(130);
        time2 = get_time();
        printf("f%d : %ld\n", i + 1, time2 - time1);
    }
    return 0;
}

long get_time()
{
    clock_t start = clock();
    long usec = start * 1000000 / CLOCKS_PER_SEC;
    return usec;
}

long get_time_1()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_usec;
}

void f1_N(int n) 
{
    int sum = 0;
    for (int i = 0; i < n; i++)
        sum++;
}

void f2_N2(int n) 
{
    int sum = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            sum++;
}

void f3_N3(int n) 
{
    int sum = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n * n; j++)
            sum++;
}

void f4_N2(int n) 
{
    int sum = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < i; j++)
            sum++;
}

void f5_N4(int n) 
{
    int sum = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < i * i; j++)
            for (int k = 0; k < j; k++)
                sum++;
}

void f6_N3(int n) 
{
    int sum = 0;
    for (int i = 0; i < n; i++)
        for (int j = 1; j < i * i; j++)
            if (j % i == 0)
                for (int k = 0; k < j; k++)
                    sum++;
}
