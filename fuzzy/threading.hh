#ifndef THREADING_HH
#define THREADING_HH

#include <vector>
#include <span>
#include <thread>

//more permanent distribution - for now attept with async/futures
struct equal_distrib_tag{} edt;
struct no_assign_tag{} nat;
//struct other_distribution_tag{};

#ifndef NTH
#define NTH 4
#endif

//std::thread::hardware_concurrency() 

const int nThreads = NTH;


template <typename T>
decltype(auto)
distrib_trans(int nThreads, T& transitions, equal_distrib_tag){
  std::vector<std::span<typename T::value_type>> domains;
  domains.reserve(nThreads);

  //remember: int/int = floor(int/int)
  const size_t len = transitions.size() / nThreads; 
  const int left = transitions.size() % nThreads; //rest
  std::cout << len << std::endl;
  size_t st = 0;
  size_t ed = 0;
  for (int i=0; i < nThreads; ++i){
    ed = std::min(st + len + (i<left), transitions.size()); //<- this leads to more of the same transitions fired
    domains.emplace_back(transitions.begin()+st, transitions.begin()+ed);
    st = ed;
  }
  return domains;
}

template <typename T>
decltype(auto)
distrib_trans(int nThreads, T& transitions, no_assign_tag){
  std::vector<std::span<typename T::value_type>> domains;
  domains.reserve(nThreads);

  for (int i=0; i < nThreads; ++i){
    domains.emplace_back(transitions.begin(), transitions.end());
  }
  return domains;
}


#endif