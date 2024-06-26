//this file will be automatically generated by python before compiling and executing 
#include <array>




std::array p1 { 0.3, 0.2, 0.1};
std::array p2 { 0.2, 0.2, 0.2};
std::array p3 { 0.3, 0.3, 0.1};
std::array p4 { 0.5, 0.4, 0.4};
std::array p5 { 0.5, 0.5, 0.5};
std::array p6 { 0.6, 0.6, 0.6};
std::array p7 { 0.1, 0.2}; // <- trap... doesn\t work with rule0 as it ony has 2 categories

//mapping objects to numbers -> index of array for random numbers
std::array<void*, 7> pl {&p1, &p2, &p3, &p4, &p5, &p6, &p7};

constexpr size_t N_PLACES = std::ranges::size(pl);

//std::any or union will take up too muhc space if there are few very large arrays