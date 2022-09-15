#include <iostream>

using namespace std;

/*
Multiples of 3 or 5

Problem 1
If we list all the natural numbers below 10 that are multiples of 3 or 5, we get 3, 5, 6 and 9. The sum of these multiples is 23.

Find the sum of all the multiples of 3 or 5 below 1000.
*/

int sum_multiply(int x, int n)
{
    int count = (n - 1) / x;
    int sum = (count * (count + 1) / 2) * x;
    return sum;
}

int main(int argc, char const *argv[])
{
    int n = 1000;
    cout << sum_multiply(3, n) + sum_multiply(5, n) - sum_multiply(15, n) << endl;
    return 0;
}