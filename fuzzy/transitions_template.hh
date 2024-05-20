#ifndef TRANSITIONS_HH
#define TRANSITIONS_HH 

//RULES.hh
#include <vector>
#include "rules.hh"


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