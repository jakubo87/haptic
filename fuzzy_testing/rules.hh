#ifndef RULES_HH
#define RULES_HH 

//RULES.hh
#include <vector>
#include <numeric>
#include <thread>


//#######    RULES    ########

//maybe mdspan some day
template<int m, int n>
using matrix = std::array<std::array<double,n>,m>;



template<size_t m, size_t n>
constexpr
void MVmult(const matrix<m,n>& M, const auto& in, auto& out)
requires (std::ranges::size(in) == n && std::ranges::size(out) == m)
{
	for (size_t i = 0; i<m ; ++i){
        out[i] = std::inner_product(M[i].cbegin(), M[i].cend(), in.cbegin(), 0.0); //<- choose this 0.0 carefully	 just 0 is an int!
    }
}


decltype(auto)
rule0(const auto& in, auto&& out)
{
  const matrix<3,3> mat {{ {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0} }};

  MVmult(mat,in, out);


  #ifdef __cpp_lib_containers_ranges
    std::vector<double> v (std::from_range, out);  //needs to go out by value
  #else
    std::vector<double> v (out.begin(), out.end());
  #endif
  return v;
}


#endif
