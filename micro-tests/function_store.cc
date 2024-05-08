#include <iostream>
#include <functional>




int main (){

std::vector<std::function<void()>> fv;

auto a_1 = [](auto a){
    std::cout << a << std::endl;
    };
auto a_2 = [](auto a, auto b){
    std::cout << a + b << std::endl;
    };

//casting to std::function<void()>
fv.push_back([&](){a_1(1);});
fv.push_back([&](){a_1(2);});
fv.push_back([&](){a_2(1,2);});
fv.push_back([&](){a_2(2,2);});

for (auto a : fv){
    std::invoke(a);
}
    return 0;
}