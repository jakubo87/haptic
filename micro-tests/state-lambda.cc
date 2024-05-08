#include <iostream>
#include <utility>
#include <random>



int main (){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis (1,20);

    auto fib = [i=0,j=1] () mutable {
    	i = std::exchange(j,i+j);
	return i;
    };
    auto I= dis(gen);

    for (int i = 0; i < I; ++i){
    	fib();
    }
    std::cout << fib() << std::endl;
    //std::cout << i << std::endl;
    return 0;
}
