#include "MCMath.h"
#include "MCBits.h"
#include "MCLog.h"

void MCMath_bye()
{
	debug_log("%s\n", "MCMath bye bye");
}

int MCMath_addInteger2(int a, int b)
{
	return a + b;
}

void MCMath_sortInt(int* sorted, size_t count)
{
    int n;
    for (n=0; n<count-1; n++) {
        int temp;
        if (sorted[n+1] < sorted[n]) {
            //swap
            temp = sorted[n];
            sorted[n] = sorted[n+1];
            sorted[n+1] = temp;
        }
    }
}

void MCMath_sortLong(long* sorted, size_t count)
{
    int n;
    for (n=0; n<count-1; n++) {
        long temp;
        if (sorted[n+1] < sorted[n]) {
            //swap
            temp = sorted[n];
            sorted[n] = sorted[n+1];
            sorted[n+1] = temp;
        }
    }
}

void MCMath_sortSizet(size_t* sorted, size_t count)
{
    size_t trycount = count-1;
    while (trycount--) {
        int n;
        for (n=0; n<count-1; n++) {
            size_t temp;
            if (sorted[n+1] < sorted[n]) {
                //swap
                temp = sorted[n];
                sorted[n] = sorted[n+1];
                sorted[n+1] = temp;
            }
        }
    }
}

int MCMath_accumulateMaxi(int* result, int value)
{
    if (value > *result) {
        *result = value;
    }
    return *result;
}

int MCMath_accumulateMini(int* result, int value)
{
    if (value < *result) {
        *result = value;
    }
    return *result;
}

double MCMath_accumulateMaxd(double* result, double value)
{
    if (value > *result) {
        *result = value;
    }
    return *result;
}

double MCMath_accumulateMind(double* result, double value)
{
    if (value < *result) {
        *result = value;
    }
    return *result;
}

bool MCMath_isPrime(int a)
{
    int i;
    for(i=2;i<=sqrt(a);i++)
        if(a%i==0)
            return false;
    return true;
}

bool MCMath_isPowerOfTwo(unsigned a)
{
    return (MCBitsSetBitNum(a) == 1);
}

unsigned MCMath_chebyshevDiatance(MCVector2i A, MCVector2i B)
{
    return (unsigned)MAX(abs(B.x - A.x), abs(B.y - A.y));
}

unsigned MCMath_factorialOf(unsigned N)
{
    unsigned res = 1, i;
    for (i=N; i>0; i--)
        res *= i;
    return res;
}

unsigned MCMath_KpermutationsOfN(unsigned K, unsigned N)
{
    //P(n,k) = n! / (n-k)!
    unsigned res = 1, i;
    for (i=N; i>N-K; i--) {
        res *= i;
    }
    return res;
}

unsigned MCMath_KcombinationsOfN(unsigned K, unsigned N)
{
    //C(n,k) = P(n,k) / k! = n! / k! * (n-k)!
    return MCMath_KpermutationsOfN(K, N) / MCMath_factorialOf(K);
}


