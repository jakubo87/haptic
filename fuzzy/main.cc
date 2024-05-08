#include <iostream>
#include <vector>
#include <array>
//#include <mutex>
#include <algorithm>
#include <random>
#include <functional>
#include <execution>
#include <iomanip>
#include <thread>
#include <chrono>
#include <cmath>
#include <future>

//#include "fuzzy.hh"

#include "tags.hh"
#include "places.hh" //places.hh before rules.hh
#include "rules.hh"

//constexpr const size_t N_PLACES = places_size();
//constexpr const size_t DATASIZE = data_size();
//static std::vector<FPlace> places {N_PLACES};
//static std::vector<double> data {DATASIZE};
//create_Net(data, places);

const int REP = 20000;

//const int NThreads = 4;

//transistions list
//const size_t N_TRANS = 3;
//consteval static std::array<std::function<bool()>,N_TRANS> transitions;
// make_transistions(data, places);

//for book keeping
std::atomic<size_t> succT = 0;
std::atomic<size_t> unsuccT = 0;

inline
void showfuzzy(const auto& data, const auto& places){
//  for(const auto& p : places){
  for(int i = 0; i<places.size(); ++i){

    for(int j = places[i].start; j < places[i].start + places[i].length; ++j){
      std::cout << std::setw(9) << data[j];
    }
    std::cout << std::endl;
  }
}

inline
void fill_random(auto&& b_it, auto&& e_it){
 //Random
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0,N_TRANS-1);
  std::generate(b_it, e_it, [&](){return distrib(gen);});
}


//run for future, with resulting std string output after
// n steps
std::string execute(const std::vector<std::function<bool()>>& trans, size_t no_steps, size_t offset){
  std::string res = "";
  std::vector<size_t> rnds (no_steps);
  fill_random(rnds.begin(), rnds.end());
    
  for(size_t i = 0; i < no_steps; ++i){
    bool succ = std::invoke(trans[rnds[i]]);
    
    if (succ){
      //note change for export
      //...TODO
      res += "{s}";
      ++succT;
    }
    else{
      res += "{}";
      ++unsuccT;
    }
  }
  //std::cout << res << std::endl;
  return res;
}

int main (){

  std::vector<std::mutex> mutexes {N_PLACES};

  auto places       = create_Net(std::move(inputLengths));
  auto transitions  = make_transitions(places, mutexes, data);

  for (const auto & p : places){
    std::cout << p.start << std::endl;
  }

  //show
  showfuzzy(data, places);
  //distrib_data(); obsolete for now -> futures
 
  std::vector<std::string> result (REP*4);


  //std::invoke(execute, transitions, 5, 0);


  using namespace std::chrono_literals;
  //std::this_thread::sleep_for(2000ms);  

 
  //auto F2 = std::async(std::launch::async, execute, transitions, 5, 0);
  //auto F3 = std::async(std::launch::async, execute, transitions,5,0);
  //auto F4 = std::async(std::launch::async, execute, transitions,5,0);
  
  const int no_steps = 2000;

  auto th1 = [&,offset = 0]() mutable{
    auto F1 = std::async(std::launch::async, execute, transitions, no_steps, offset);
    offset += no_steps; //local
    result[0] += F1.get();
  };

  auto th2 = [&,offset = 0]() mutable{
    auto F2 = std::async(std::launch::async, execute, transitions, no_steps, offset);
    offset += no_steps;
    result[1] += F2.get();
  };

  auto th3 = [&,offset = 0]() mutable{
    auto F3 = std::async(std::launch::async, execute, transitions, no_steps, offset);
    offset += no_steps;
    result[2] += F3.get();
  };

  auto t0 = std::chrono::steady_clock::now();

  //run!
  // for (int i=0; i<NThreads; ++i){
  //   pool.emplace_back(std::thread(run, chunks[i].first, chunks[i].second));
  // } 
  // //join
  // for (int i=0; i<NThreads; ++i){
  //   pool[i].join(); 
  // } 
  for (int i=0; i<REP-1; i+=3){

    th1();
    th2();
    th3();
  //  std::cout << std::endl;
  //  showfuzzy(data, places);

    //result[i+2]=F3.get();
    //result[i+3]=F4.get();
  }

  // std::cout << "result:\n" << std::endl;
  
 
  auto t1 = std::chrono::steady_clock::now();
  std::chrono::duration<double> diff = t1-t0; 

  std::cout << "After execution:\n" << std::endl;
  showfuzzy(data, places);
  // for(auto s : result){
  //   std::cout << s << std::endl;
  // }

  std::cout << "execution time:" << diff << std::endl;
  std::cout << "existing Nodes: " << N_PLACES << std::endl;
  std::cout << "existing Transitions: " << N_TRANS << std::endl;
  std::cout << "Transitions fired: " << REP*no_steps*3 << std::endl;

  // std::cout << "starting tokens: " << Tkns << std::endl;
  // std::cout << "#scans through all possible transitions: " << REP << std::endl;
  // std::cout << "#executions of transitions: " << succT << "  " << succT*100/(REP*T) << "%" << std::endl;
  // std::cout << "#failed transitions: " << unsuccT << "  " << unsuccT*100/(REP*T) << "%" << std::endl;
  return 0;
}



//TODO upon change, give hint about whether a transition can fire (may be inaccurate due to changes)
//vector<std::atomic_flag> ready
//but only in graph, as transitions need to be identified first, and for that you need adjacency!
