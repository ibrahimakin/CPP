#include <iostream>

using namespace std;

/*
Largest palindrome product

Problem 4
A palindromic number reads the same both ways. The largest palindrome made from the product of two 2-digit numbers is 9009 = 91 × 99.

Find the largest palindrome made from the product of two 3-digit numbers.
*/

int opposite(int n)
{
    int m = 0;
    while (n > 0)
    {
        m = m * 10 + n % 10;
		n = n / 10;
	}
    return m;
}

int main(int argc, char const *argv[])
{
    for (int i = 999999; i > 10000; i--)
    {
        if (i == opposite(i))
        {
            int d = 999;
            while (i / d < 1000)
            {
                if (i % d == 0)
                {
                    cout << i << endl;
                    return 0;
                }
                d--;
            }
        }
    }
    return 0;
}