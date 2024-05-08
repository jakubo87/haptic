#include <array>
#include <algorithm>
#include <iostream>
#include <functional>

template <int N>
class FuMe{
  using self_t = FuMe<N>;
  public:


  self_t operator+(const self_t& other) const{
    self_t res;
    std::ranges::transform(this->members, other.members, res.members.begin(), std::plus{} );
    return res;
  }

  //private:
  std::array<float,N> members;

};

template<int N>
FuMe<N>
make_fume(float f){
    FuMe<N> res;
    std::ranges::fill(res.members, f);
    return res;
}


int main(){
	const auto& A = make_fume<3>(0.5);
	const auto& B = make_fume<3>(0.5);
	const auto& C = A + B;
	for (const auto a : C.members)
		std::cout << a << " ";
	std::cout << std::endl;
	return 0;
}
