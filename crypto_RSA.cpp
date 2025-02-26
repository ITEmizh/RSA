#include <iostream>
#include <random>

//binary exponentiation
unsigned long long bin_pow(unsigned long long base, unsigned long long d, unsigned long long n) { 
    if (d == 1) { 
        return base; 
    }
    if (d % 2 == 0) {
        long long t = bin_pow(base, d / 2, n);
        return t * t % n;
    }
    else {
        return bin_pow(base, d - 1, n) * base % n;
    }
}

bool Miller_Rabin_test(unsigned long long p){
    std::random_device rd;
    std::uniform_int_distribution<unsigned long long> testing_number(2, p - 2);
    unsigned long long s, d, a;
    unsigned long long x, y; 
    s = 0;
    d = p - 1;

    while(d % 2 == 0){
        s++;
        d = d/2;
    }

    for(int i = 0; i <= 64; i++){
        a = testing_number(rd);

        x = bin_pow(a, d, p);
        for(int i = 0; i < s; i++){ 
            y = bin_pow(x, 2, p);

            if(y == 1 && x != 1 && x != p - 1){ return false; }
            x = y;
        }

        if(y != 1){ return false; }
    }

    return true;
}

unsigned long long random_prime(unsigned long long begin = 0, unsigned long long end = 1000){
    std::random_device rd;
    std::uniform_int_distribution<unsigned long long> probably_prime(begin, end);
    unsigned long long p;

    while(true){
        p = probably_prime(rd);
        if(Miller_Rabin_test(p)){ return p; }       
    }
}

int main(){
    std::cout << random_prime(9000, 10000) << '\n';   
    if(Miller_Rabin_test(1000*1000)){std::cout << "prime"; }
}