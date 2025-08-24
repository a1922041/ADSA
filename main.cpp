/*
Your program takes one line as input. The input line contains three integers separated by spaces. Let the
three integers be ”I1 I2 B”. I1 and I2 are both nonnegative integers up to 100 digits long. B represents I1
and I2’s base (B is from 2 to 10).

Your program should output the sum of I1 and I2, using the school method, then the product of I1 and I2,
using the Karatsuba algorithm, and finally the ratio between I1 and I2 (rounded down). 



school method: addition
adding the digits of the two numbers from right to left, carrying over any overflow to the next higher digit position.

karatsuba method: multiplication
 divide-and-conquer*/


#include <iostream>   // for cin cout
#include <vector>     // for vector
#include <string>     // for string
#include <algorithm>  // for reverse
using namespace std;


/*

convert sting number to vector. 

the input is in a string becayse it can be up to 100 digits

making each number into an array to work digit by digit.

*/


vector<int> toDigits(const string &s) {
    vector<int> digits; //hold digits in int 
    for (char c : s) //each character in string
        digits.push_back(c - '0'); // convert char to into 
    return digits;
}



/*
school method for addition:
add digits right to left 
carry over when sum is equal or larger than base


convert vector of digits back into string
remove unneeded leading zeros 

*/

string fromDigits(const vector<int> &digits ) {
    string s; //holds the result
    for (int d : digits) //goes through each digit
    s.push_back('0' + d); //converts int to char 

        //remove leading zeros 
    int pos = 0;
    while (pos + 1 < (int)s.size() && s[pos] == '0') //all positions in array
        pos++;
        return s.substr(pos); //returns the string with no extra zeros


}

vector<int> addSchool(vector<int> a, vector<int> b, int base) {

//add it right to left. need to reverse the arrays
reverse(a.begin(), a.end()); //number a
reverse(b.begin(), b.end()); //number b

int n = max(a.size(), b.size()); // max length of two numbers
vector<int> result; //store result / sum 
int carry = 0; // initualise carry

//loop through each digit
for (int i = 0; i < n; i++) {

    int d1 = (i < (int)a.size() ? a[i] : 0); //digit from a or 0 if none - incase one number is longer than the other
    int d2 = (i < (int)b.size() ? b[i] : 0); //digit from b or 0 if none 
    int sum = d1 + d2 + carry; //addition with carry

    result.push_back(sum % base); //the remainder 
    carry = sum / base; //carry to next column

}

if (carry) result.push_back(carry); //add leftover carry 


reverse(result.begin(), result.end()); //reverse to normal order 
return result;

}


/* Kiratsuba multiplication:

split a and b into two halves each

a|b and c|d

        ac
        bd
        (a+b)(c+d)

    
three multiplications / middle part
    (a+b)(c+d) - ac - bd
combine:
        result = ac * base^(2*m) + mid * base^m + bd



*/

//digit by digit subtraction for a - b when a >= b

vector<int> subDigits (vector<int> a, vector<int> b, int base) {

reverse(a.begin(), a.end()); //start with right to left
reverse(b.begin(), b.end()); //start with right to left
vector<int> result;
int borrow = 0;

for (int i = 0; i < (int)a.size(); i++) {

    int d1 = a[i] - borrow; //digit from a 
    int d2 = (i < (int)b.size() ? b[i] : 0);// from b or zero


    if (d1 < d2){ //need to borrow 
        d1 += base; //borrow from next
        borrow = 1; 

    }else {
        borrow = 0;
    }

    result.push_back(d1 - d2); //store difference

}

//remove leading zeros
    while (result.size() > 1 && result.back() == 0)
        result.pop_back();

    reverse(result.begin(), result.end()); // reverse back to normal order
    return result;
}



//shift digits left 
vector<int> shiftLeft(const vector<int> &a, int k) {
    vector<int> res = a;              // copy the vector
    for (int i = 0; i < k; i++)       // append k zeros at the end
        res.push_back(0);
    return res;
}




vector<int> karatsuba(vector<int> x, vector<int> y, int base) {
    // Remove leading zeros
    while (x.size() > 1 && x[0] == 0) x.erase(x.begin());
    while (y.size() > 1 && y[0] == 0) y.erase(y.begin());

    int n = max(x.size(), y.size()); // maximum length

    // Base case
    if (n == 1) {
        int val = x[0] * y[0];       //multiply single digits
        if (val >= base)             //split into 2 digits if needed
            return {val / base, val % base};
        else
            return {val};            //otherwise just one digit
    }

    //make both numbers the same length
    while ((int)x.size() < n) x.insert(x.begin(), 0);
    while ((int)y.size() < n) y.insert(y.begin(), 0);

    //if length is odd, add one more zero
    if (n % 2) {
        x.insert(x.begin(), 0);
        y.insert(y.begin(), 0);
        n++;
    }

    int m = n / 2; // midpoint

    //split numbers into halves
    vector<int> a(x.begin(), x.begin() + m);
    vector<int> b(x.begin() + m, x.end());
    vector<int> c(y.begin(), y.begin() + m);
    vector<int> d(y.begin() + m, y.end());

    // recursive multiplications
    vector<int> ac = karatsuba(a, c, base);  // a*c
    vector<int> bd = karatsuba(b, d, base);  // b*d
    vector<int> ab = addSchool(a, b, base);  // (a+b)
    vector<int> cd = addSchool(c, d, base);  // (c+d)
    vector<int> abcd = karatsuba(ab, cd, base); // (a+b)(c+d)

    // Middle part: (a+b)(c+d) - ac - bd
    vector<int> mid = subDigits(subDigits(abcd, ac, base), bd, base);

    // combine result: ac*base^(2m) + mid*base^m + bd
    vector<int> res = addSchool(
                        addSchool(shiftLeft(ac, 2*m), shiftLeft(mid, m), base),
                        bd,
                        base);

    return res;
}

/*

DIVISION

always return 0.
*/
string division() {
    return "0";
}



int main() {
    string I1, I2;  // numbers as strings
    int B;          // base
    cin >> I1 >> I2 >> B; // read input

    vector<int> x = toDigits(I1); // convert I1 to vector
    vector<int> y = toDigits(I2); // convert I2 to vector

    // 1. Addition
    vector<int> sumDigits = addSchool(x, y, B);

    // 2. Multiplication
    vector<int> prodDigits = karatsuba(x, y, B);

    // 3. Division (undergrad always 0)
    string divStr = division();

    // Print results, separated by spaces
    cout << fromDigits(sumDigits) << " "
         << fromDigits(prodDigits) << " "
         << divStr << "\n";

    return 0; 
}