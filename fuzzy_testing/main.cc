#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <functional>
#include <execution>
#include <iomanip>
#include <thread>
#include <chrono>
#include <tuple>

#include "current_input.hh"
#include "transitions.hh"
#include "threading.hh"



const int NO_STEPS = 10000;
const int BUFFER_LENGTH = 100;
const int RUNS = NO_STEPS/(nThreads*BUFFER_LENGTH);
//divisble ... for ease of use for now also implies No_steps is length is at least the length of buffer 
static_assert(NO_STEPS % (BUFFER_LENGTH * nThreads)  == 0);

//TODO subject to change when more places change with every transition
using result_type = typename std::tuple<decltype(std::this_thread::get_id()),
                              decltype(std::chrono::system_clock::now()),
                              std::vector<double>,
                              size_t>;

//for book keeping
//std::atomic<size_t> succT = 0;
//std::atomic<size_t> unsuccT = 0;

inline
void showfuzzy(){//const auto& places){
  // for(void* p : places){
  //   auto l = p->size();
  //   auto* arr = static_cast<std::array<double, l>(p);
  //   for(auto ele : *arr){
  //     std::cout << std::setw(9) << ele;
  //   }
  //   std::cout << std::endl;
  // }
    for(auto ele : p1){
      std::cout << std::setw(9) << ele;
    }
    std::cout << std::endl; 
}

inline
void fill_random(auto&& b_it, auto&& e_it, size_t length){
 //Random
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0,length-1); 
  std::generate(std::execution::unseq, b_it, e_it, [&]() {return distrib(gen);});
}


//run for future, with resulting std string output after
// n steps
void run(auto& domain, auto& result, size_t runs){
  //std::cout << std::this_thread::get_id() << std::endl;
 
  std::vector<size_t> rnds (BUFFER_LENGTH);
  const auto length = domain.size(); 
  //size_t out_index = 0 ; //relative to result.begin()
  //std::vector<double> delta;
  for(size_t r = 0; r < runs; ++r){
    fill_random(rnds.begin(), rnds.end(), length);  //init random numbers
    for(size_t i = 0; i < BUFFER_LENGTH; ++i){
      const size_t rnd=rnds[i]; 
      //emplace back seems to better benefit the lifetime considerations
      result.emplace_back(std::this_thread::get_id(),
                                std::chrono::system_clock::now(),
                                std::invoke(domain[rnd]),
                                rnd);
      //[out_index++] = tmp;
    }
  }
}


int main (){

  std::vector<std::mutex> mutexes {N_PLACES};

   auto transitions  = make_transitions();
  auto chunks       = distrib_trans(nThreads, transitions, equal_distrib_tag{}); 
  
  //result here
  std::vector<std::vector<result_type>> result (nThreads);
  for (int i=0; i<nThreads; ++i){
    result[i].reserve(NO_STEPS/nThreads);
  }
  
  std::vector<std::thread> pool;
  pool.reserve(nThreads);

  //show
  showfuzzy();
  //distrib_data(); obsolete for now -> futures
 
  
  auto t0 = std::chrono::steady_clock::now();

  //run!
  for (int i=0; i<nThreads; ++i){
    //th i needs to be captured otherwise it may go out of scope before the lambda starts running
    pool.emplace_back([&chunks, &result, i](){run(chunks[i],result[i],RUNS );});
  } 
  //join
  for (int i=0; i<nThreads; ++i){
    pool[i].join(); 
  }
 
  auto t1 = std::chrono::steady_clock::now();
  std::chrono::duration<double> diff = t1-t0; 

  std::cout << "After execution:\n" << std::endl;

  showfuzzy();

  //export
  std::ofstream outfile("out.txt");   // Create and open a text file

  // Write to the file - unsorted for now
  for (const auto & A : result){
    for (const auto & a : A){
      outfile << "{" << std::get<0>(a) << "," << std::get<1>(a);
      outfile << ",";
      
      auto s = std::span(std::get<2>(a)); 
      outfile << std::accumulate(s.begin()+1, s.end(), std::to_string(s[0]), [](const std::string& acc, auto b){
                    return acc + ", " + std::to_string(b);
                  });
      outfile << "," << std::get<3>(a) << "}" << std::endl;
    }
  }
  outfile.close();   // Close the file


  std::cout << "execution time:" << diff << std::endl;
  std::cout << "existing Nodes: " << N_PLACES << std::endl;
  std::cout << "existing Transitions: " << transitions.size() << std::endl;
  std::cout << "Transitions fired: " << NO_STEPS << std::endl;

  return 0;
}



//TODO upon change, give hint about whether a transition can fire (may be inaccurate due to changes)
//vector<std::atomic_flag> ready
//but only in graph, as transitions need to be identified first, and for that you need adjacency!
