#include <iostream>
#include <string>

// school method for integer addition
std::string school(const std::string &I1, const std::string &I2, int B){
    int c = 0; // initializes the carry to 0
    size_t ml = std::max(I1.size(), I2.size()); // finds the max length of I1 and I2
    std::string result(ml, '0'); // initializes result string to store sum

    // iterates through digits of both integers and the carry
    for (std::size_t i = 0; i < ml; i++) {
        int d1 = 0;
        if (i < I1.size()) {
            d1 = I1[I1.size() - 1 - i] - '0'; // extracts the digit from I1 or use 0 if no more digits
        }
        int d2 = 0;
        if (i < I2.size()) {
            d2 = I2[I2.size() - 1 - i] - '0'; // extracts the digit from I2 or use 0 if no more digits
        }

        // calculates the sum of the digits and the carry
        int sum = d1 + d2 + c;
        c = sum / B; // calculates the new carry
        sum = sum % B; // calculates the remainder

        result[ml - 1 - i] = '0' + sum; // adds remainder to the front of result string
    }
    
    // if there is a carry,
    if (c > 0) {
        result.insert(result.begin(), '0' + c); // adds carry to the front of result string
    }
    return result; // returns final sum
}

// helper function: school method for integer subtraction
std::string sub(const std::string &I1, const std::string &I2, int B){
    int borrow = 0; // initializes borrow to 0
    std::string result(I1.size(), '0'); // initializes result string to store the difference

    // iterates through digits of both integers
   for (size_t i = 0; i < I1.size(); i++) {
        int d1 = I1[I1.size() - 1 - i] - '0'; // extracts the digit from I1 or use 0 if no more digits
        int d2 = 0;
        if (i < I2.size()) {
            d2 = I2[I2.size() - 1 - i] - '0'; // extracts the digit from I2 or use 0 if no more digits
        }

        // calculates the difference between the digits along with any borrow
        int diff = d1 - d2 - borrow;

        // adjusts borrow
        if (diff < 0){
            diff += B;
            borrow = 1; // borrow occurs if d1 < d2
        } else {
            borrow = 0; // no borrow if d1 >= d2
        }
        result[I1.size() - 1 - i] = diff + '0'; // adds difference to the front of result string
    }

    // removes leading zeros
    while (result.size() > 1 && result[0] == '0'){
        result.erase(0, 1);
    }

    return result; // returns the final difference
}

// karatsuba algorithm for integer multiplication
std::string karatsuba(const std::string &I1, const std::string &I2, int B){
    // if the value of I1 or I2 is 0, return 0 itself
    if (I1 == "0" || I2 == "0"){
        return "0";
    }

    // if integers are single digits, directly multiply
    if (I1.size() == 1 && I2.size() == 1){
        int product = (I1[0] - '0') * (I2[0] - '0');
        if (product < B){
            return std::string(1, product + '0');
        } else {
            return std::string(1, product / B + '0') + std::string(1, product % B + '0');
        }
    }

    size_t n = std::max(I1.size(), I2.size()); // find max length of both integers
    size_t h = n / 2; // divides length of the integers by 2

    // divides the integers into parts
    std::string p1, p2, p3, p4;
    if (I1.size() > h){
        p1 = I1.substr(0, I1.size() - h);
        p2 = I1.substr(I1.size() - h);
    } else {
        p1 = "0";
        p2 = I1;
    }

    if (I2.size() > h){
        p3 = I2.substr(0, I2.size() - h);
        p4 = I2.substr(I2.size() - h);
    } else {
        p3 = "0";
        p4 = I2;
    }

    // recursive multiplication
    std::string r1 = karatsuba(p2, p4, B);
    std::string r2 = karatsuba(p1, p3, B);
    
    // additions and subtractions for middle
    std::string sum1 = school(p2, p1, B);
    std::string sum2 = school(p4, p3, B);
    std::string r3 = karatsuba(sum1, sum2, B);
    std::string diff = sub(r3, r2, B);
    std::string r4 = sub(diff, r1, B);

    // construct final result
    for (size_t i = 0; i < 2 * h; i++) {
        r2 = r2 + '0';
    }
    for (size_t i = 0; i < h; i++) {
        r4 = r4 + '0';
    }
    std::string result = school(school(r2, r4, B), r1, B);

    // removes leading zeros
    while (result.size() > 1 && result[0] == '0'){
        result.erase(0, 1);
    }

    return result; // returns final result
}

// school method for integer division
std::string div(const std::string &I1, const std::string &I2, int B) {
    // checks if I2 is zero
    if (I2 == "0") {
        return "Error: I2 will not be 0"; // division by zero not allowed
    }

    // if dividend is less than divisor, quotient becomes 0
    if (I1.size() < I2.size() || (I1.size() == I2.size() && I1 < I2)) {
        return "0";
    }

    std::string q; // initializes the quotient
    std::string r = "0"; // initializes the remainder to 0

    // iterates through each digit of I1
    for (std::size_t i = 0; i < I1.size(); i++) {
        std::string rb = karatsuba(r, std::to_string(B), B); // calls karatsuba function to multiply remainder and base
        std::string cd = std::string(1, I1[i]); // finds the first digit of I1
        r = school(rb, cd, B); // calculates remainder

        int qd = 0;
        // calculates quotient digit
        while (r.size() > I2.size() || (r.size() == I2.size() && r >= I2)) {
            r = sub(r, I2, B);
            qd++;
        }
        q += std::to_string(qd); // adds quotient digit to quotient
    }

    // removes leading zeros
    while (q.size() > 1 && q[0] == '0') {
        q.erase(0, 1);
    }

    return q; // returns quotient
}

int main() {

    std::string I1, I2; // declares variables to store input integers
    int B; // declares variable to store base

    // input
    std::cin >> I1 >> I2 >> B;

    // function calls
    std::string result = school(I1, I2, B);
    std::string product = karatsuba(I1, I2, B);
    std::string ratio = div(I1, I2, B);

    // output
    std::cout << result << " " << product << " " << ratio << std::endl;

    return 0;
}