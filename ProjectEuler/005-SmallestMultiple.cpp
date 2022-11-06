#include <iostream>

using namespace std;

/*
Smallest multiple

Problem 5
2520 is the smallest number that can be divided by each of the numbers from 1 to 10 without any remainder.

What is the smallest positive number that is evenly divisible by all of the numbers from 1 to 20?
*/

// greatest common divisor
unsigned long long gcd(unsigned long long a, unsigned long long b)
{
    while (a != 0)
    {
        unsigned long long c = a;
        a = b % a;
        b = c;
    }
    return b;
}

// least common multiple
unsigned long long lcm(unsigned long long a, unsigned long long b)
{
    // parenthesis avoid overflow
    return a * (b / gcd(a, b));
}

int main(int argc, char const *argv[])
{
    unsigned int x = 20;
    // find least common multiple of all numbers
    unsigned long long result = 1;
    for (unsigned int i = 2; i <= x; i++)
    {
        result = lcm(result, i);
    }
    cout << result << endl;
    return 0;
}