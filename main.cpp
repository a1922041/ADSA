/*
Your program takes one line as input. The input line contains three integers separated by spaces. Let the
three integers be ”I1 I2 B”. I1 and I2 are both nonnegative integers up to 100 digits long. B represents I1
and I2’s base (B is from 2 to 10).

Your program should output the sum of I1 and I2, using the school method, then the product of I1 and I2,
using the Karatsuba algorithm, and finally the ratio between I1 and I2 (rounded down). 
*/



/*school method:
adding the digits of the two numbers from right to left, carrying over any overflow to the next higher digit position.*/

/*karatsuba method:  divide-and-conquer*/

#include <iostream>   // for cin, cout
#include <vector>     // for vector
#include <string>     // for string
#include <algorithm>  // for reverse
using namespace std;


/*

convert sting number to vector. 

the input is in a string becayse it can be up to 100 digits

converting each number into an array of single digits is so we can work digit by digit.

*/


vector<int> toDigits(const string &s) {
    vector<int> digits;
    for (char c : s) digits.push_back(c - '0');
    return digits;
}








int main(){



}