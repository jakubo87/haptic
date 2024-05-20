#ifndef RULES_HH
#define RULES_HH 

//RULES.hh
#include <vector>
#include <span>
//include "fuzzy.hh"
#include "concepts.hh"


// inline
// bool get_locks(auto& to_lock, int attempts = 20){
//   //std::cout << "getting locks" << std::endl;
//   auto maxlcnt = to_lock.size();
//   std::vector<bool> locked {maxlcnt}; //list of locks soon to be had
  
//   //cannot be vectorised as lock() is vec-unsafe/ but may still be reordered or parallelised
//   std::ranges::transform(std::execution::par, to_lock, locked,[&](auto a){
//     int att = attempts;
//     do{
//       bool suc = places[a].lock();
//     }
//     while (!suc && --att); //if fails after
//     return suc;
//   });

//   int n_locks = std::ranges::accumulate(locked,0);

//   if (n_locks == maxlcnt){
//     return true;
//   }
//   else{//unable to acquire all locks
//     for(auto& l : locks){
//         places[f].unlock();
//     }
//     //std::cout << "failed to get all locks" << std::endl;
//     return false;
//   }
// }

// inline
// void unlock(auto& to_unlock){
//   for (auto& fp : to_unlock){
//     fp.unlock();
//   }
// }

//TODO Create a template which will then serve as a blueprint for a python generated .hh file 

//#######    RULES    ########
//fuzzy n category set concept checks for soundness

//RULE 1: input1 2 dim, input2 3 dim, output C 3 dim
template<fuzzy2cat F2, fuzzy3cat F3> 
decltype(auto)
rule1(const F2& In1, const F3& In2, const F3& Out, auto& data ){
  // generic binding to variables:
  auto in1 = std::span(data.begin() + In1.start, In1.length);
  auto in2 = std::span(data.begin() + In2.start, In2.length);
  auto out = std::span(data.begin() + Out.start, Out.length);

  // std::cout << in1[0] << std::endl;  
  // std::cout << in1[1] << std::endl;  
  // std::cout << in2[2] << std::endl;  
  


  //define functions:
  // out[0] += 0.1*in1[0];
  // out[1] += 0.1*in2[0] - 0.1*in2[1]; //how arbitrary can these functions become?
  // out[2] += 0.1*in1[1] - 0.1*in2[1];

  //only permutations - basically swap back and forth
  out[0] = in2[2];
  out[1] = in2[1]; //how arbitrary can these functions become?
  out[2] = in2[0];

  // //access to out of bounds elements needs to be checked!
  //concepts do not yet work as expected
  
  //output only the change... since fuzzy rules only change the output 
  //later it may be necessary to extend this to all touched places
  //may be necessary to point to some permanent exchange data structure (vec<vec>>) for each thread
  #ifdef __cpp_lib_containers_ranges
    std::vector<double> v (std::from_range, out);  //needs to go out by value
  #else
    std::vector<double> v (out.begin(), out.end());
  #endif
  return v;
}

//RULE 2: input A 2 dim, output B 2 dim
template<fuzzy2cat F2>
decltype(auto)
rule2(const F2& In, const F2& Out, auto& data){
    
  auto in = std::span(data.begin() + In.start, + In.length);
  auto out = std::span(data.begin() + Out.start, + Out.length);


  // out[0] += 0.1*in[0];
  // out[1] += 0.1*in[0] - 0.1*in[1];

  //only permutations
  out[0] = in[1];
  out[1] = in[0];

  std::vector<double> v (out.begin(), out.end());
  return v;
 }


//##### CREATE TRANSITIONS #######

const size_t N_TRANS = 6;

template<typename P, typename M, typename D>
decltype(auto)
make_transitions(P& pl, M& mut, D& data )
{//casting to lambda
  std::vector<std::function<std::vector<double>()>> transitions;
  transitions.reserve(N_TRANS);

  // 1 0 3 
  transitions.emplace_back([&]() -> std::vector<double> { //<------ return type must be value type not self deduced
    std::scoped_lock lck(mut[pl[1].pos], mut[pl[0].pos], mut[pl[3].pos]);
    //works nicely: with 2 threads and 3 transitions it did not deadlock
    //will deadlock, if same lock is called twice
    return rule1(pl[1], pl[0], pl[3], data);
  });

  // 2 3 0
  transitions.emplace_back([&]() -> std::vector<double> {
    std::scoped_lock lck(mut[pl[2].pos], mut[pl[3].pos], mut[pl[0].pos]);
    //works nicely: with 2 threads and 3 transitions it did not deadlock
    //will deadlock, if same lock is called twice
    return rule1(pl[2], pl[3], pl[0], data);
  });

  // 1 2
  transitions.emplace_back([&]() -> std::vector<double> {
    std::scoped_lock lck(mut[pl[1].pos], mut[pl[2].pos]); 
    return rule2(pl[1], pl[2], data);
  });
  
  // 2 1
  transitions.emplace_back([&]() -> std::vector<double> {
    std::scoped_lock lck(mut[pl[2].pos], mut[pl[1].pos]); 
    return rule2(pl[2], pl[1], data);
  });
  
  // 0 1 - should not be possible due to concept constraints!!!
  transitions.emplace_back([&]() -> std::vector<double> {
    std::scoped_lock lck(mut[pl[0].pos], mut[pl[1].pos]); 
    return rule2(pl[0], pl[1], data);
  });
  
  // 2 3 - should not be possible due to concept constraints!!!
  transitions.emplace_back([&]() -> std::vector<double> { 
    std::scoped_lock lck (mut[pl[2].pos], mut[pl[3].pos]); 
    return rule2(pl[2], pl[3], data);
  });

  return transitions;
}



#endif