#include <iostream>
#include <random>
#include "BigInt.h"

typedef BigInt lngen;

//binary exponentiation
lngen bin_pow(lngen base, lngen d, lngen n = 1) { 
    if (d == 1) { 
        return base; 
    }
    if (d % 2 == 0) {
        lngen t = bin_pow(base, d / 2, n);
        return t * t % n;
    }
    else {
        return bin_pow(base, d - 1, n) * base % n;
    }
}

bool Miller_Rabin_test(lngen p){
    std::random_device rd;
    std::uniform_int_distribution<lngen> testing_number(2, p - 2);
    lngen s, d, a;
    lngen x, y; 
    s = 0;
    d = p - 1;

    while(d % 2 == 0){
        s++;
        d = d/2;
    }

    for(int i = 0; i <= 64; i++){
        a = testing_number(rd);

        x = bin_pow(a, d, p);
        for(lngen j = 0; j < s; j++){ 
            y = bin_pow(x, 2, p);

            if(y == 1 && x != 1 && x != p - 1){ return false; }
            x = y;
        }

        if(y != 1){ return false; }
    }

    return true;
}

lngen random_prime(lngen begin = 0, lngen end = 1000){
    std::random_device rd;
    std::uniform_int_distribution<lngen> probably_prime(begin, end);
    lngen p;

    while(true){
        p = probably_prime(rd);
        if(Miller_Rabin_test(p)){ return p; }       
    }
}

lngen exgcd(lngen a, lngen b, lngen &x, lngen&y) {
    
    if (a == 0){ 
        x = 0; 
        y = 1; 
        return b; 
    } 

    lngen x1, y1; 
    lngen gcd = exgcd(b%a, a, x1, y1); 
 
    x = y1 - (b/a) * x1; 
    y = x1; 
    return gcd; 
} 

int main(){
    lngen x = 123, p, q, e, d = 1, t = 1, n, phi, ciphertext;
    p = random_prime();
    q = random_prime();
    phi = (p - 1) * (q - 1);
    n = p * q;
    e = random_prime();

    std:: cout << "p = " << p << " q = " << q << " phi = " << phi << " n = " << n << " e = " << e << '\n';
    exgcd(e, phi, d, t);
    std::cout << "d = " << d << '\n';
    ciphertext = bin_pow(x, e, n);
    std::cout << "ciphertext is " << ciphertext << '\n';
    std::cout << bin_pow(ciphertext, d, n) << '\n';
    std::cout << bin_pow(100, 5, 9);
}