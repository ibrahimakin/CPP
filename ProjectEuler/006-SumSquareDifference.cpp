#include <iostream>
#include <cmath>

using namespace std;

/*
Sum square difference

Problem 6
The sum of the squares of the first ten natural numbers is, 1^2 + 2^2 + ... + 10^2 = 385

The square of the sum of the first ten natural numbers is,  (1 + 2 + ... + 10) ^ 2 = 55^2 = 3025

Hence the difference between the sum of the squares of the first ten natural numbers and the square of the sum is 3025 - 385 = 2640.

Find the difference between the sum of the squares of the first one hundred natural numbers and the square of the sum.
*/

// Sum of squares of n natural numbers formula: 1^2 + 2^2 + 3^2 + ... + n^2 = [n(n+1)(2n+1)] / 6
// Sum of n natural numbers formula:            1 + 2 + 3 + ... + n = n(n+1) / 2

int main(int argc, char const *argv[])
{
    int n = 100;
    int sum_of_squares = n * (n + 1) * (2 * n + 1) / 6;
    int square_of_sum = pow((n * (n + 1) / 2), 2);
    cout << square_of_sum - sum_of_squares << endl;
    return 0;
}