#include <iostream>
#include <math.h>

using namespace std;

/*
Largest prime factor

Problem 3
The prime factors of 13195 are 5, 7, 13 and 29.

What is the largest prime factor of the number 600851475143 ?
*/

int main(int argc, char const *argv[])
{
    long int n = 600851475143;
    // Print the number of 2s that divide n
    while (n % 2 == 0)
    {
        n = n / 2;
    }

    // n must be odd at this point. So we can skip
    // one element (Note i = i + 2)
    for (int i = 3; i <= sqrt(n); i = i + 2)
    {
        // While i divides n, print i and divide n
        while (n % i == 0)
        {
            n = n / i;
        }
    }

    cout << n << endl;
    return 0;
}