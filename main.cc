#include <iostream>
#include <vector>
#include <mutex>
#include <algorithm>
#include <random>
#include <functional>
//#include <execution>
#include <iomanip>
#include <thread>
#include <chrono>
#include <cmath>

const size_t N = 10000;
const size_t T = 50000;
const size_t Tkns = 50000;
const int REP = 2000;
const int NThreads = 4;

static std::atomic<size_t> succT = 0;
static std::atomic<size_t> unsuccT = 0;
//using 


//TODO redistribute heuristics like k means it
//check adjacent nodes and exert pressure: a thread gets a category (colour)
//nodes get a voting system: if a transition is between predominantly "red" threads the transition becomes red and the least red switches to the next most accessed colour.
//
//This however messes up 2 things.



//data structures
static std::vector<size_t> places (N); //counts in these places rather
static std::vector<std::mutex> M(N); //need 2(...) locks for each operation
static std::vector<std::function<bool()>> transitions;
static std::vector<std::pair<size_t, size_t>> chunks{NThreads, std::make_pair(0,0)};

constexpr void
distrib_data(){
  int len = ceil(transitions.size() / NThreads);
  for (int i=0; i < NThreads; ++i){
    size_t st = i*len;
    size_t ed = std::min(st+len, transitions.size());
    chunks[i] = std::make_pair(st,ed);
  }
}


void show(){
  for(auto p : places){
    std::cout << std::setw(3) << p;
  }
  std::cout << std::endl;
}

void run(unsigned int st, unsigned int ed){
  std::cout << std::this_thread::get_id() << " started"  << std::endl;
  //execute
  //not ideal as it does not allow tx to fire infinitely without others firing several time in between(if they can) -> too fair
  for (int i=0; i<REP; ++i){
    std::for_each(/*std::execution::unseq,*/transitions.begin()+st, transitions.begin()+ed, [&](const auto& f)
        {
          f();
        });
  }
  std::cout << std::this_thread::get_id() << " finished"  << std::endl;
}



int main (){
  
  //p_locks.resize(N); doesnt work with mutexes
  
  //distribute tokens across places
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0,N-1);
  for (size_t i = 0; i< Tkns; ++i){
    places[distrib(gen)]++;
  }

  //transitions.resize(T);
  //make transitions
  for (size_t i=0; i<T; ++i){
    auto f = distrib(gen);
    auto s = distrib(gen);
    while (f==s){
      s = distrib(gen);
    }
    size_t df = 1;
    size_t ds = 1;
    //std::cout << f << " " << s << std::endl;
    transitions.emplace_back([f,s,df,ds](){ //capture by value!!!

        //std::cout << "getting locks" << std::endl;
        int tbl = 2; //to be locked
        int count = 40;
        int index = 0;
        //rot lock
        std::vector<size_t> relmtx;
        relmtx.push_back(f);
        relmtx.push_back(s);
        
        while(tbl>0 and count-- > 0){
          if (relmtx[index]<-1){ //max of size_t
            if (M[relmtx[index]].try_lock()){
              relmtx[index]=-1;
              --tbl;
            }
          }
          ++index;
          if (index==relmtx.size()) index=0;
        }        
        if (!tbl){ //if all locked tbl==0 ie success
          //std::cout << "got locks" << std::endl;       
//          std::cout << "got lock" << std::endl;
//          std::cout << "P1:" << f << " tkn:" << places[f] << " df:" << df <<  std::endl;
          if (places[f] < df){// transition fails
            M[f].unlock(), M[s].unlock();
            ++succT;
            return false;
          }
          else{ //success
            places[f]-=df;
            places[s]+=ds;
            //std::cout << "sucessful transition" << std::endl;
            M[f].unlock(), M[s].unlock();
            ++succT;
            return true;
          }
        }
        else{
          M[f].unlock(), M[s].unlock();
          //std::cout << "failed to get all locks" << std::endl;
          ++unsuccT;
          return false;
        }
    });
  }  

  //show
  show();
  distrib_data();
 
  for (auto a : chunks)
    std::cout <<a.first << " " << a.second << std::endl;

  std::vector<std::thread> pool;


  auto t0 = std::chrono::steady_clock::now();

  //run!
  for (int i=0; i<NThreads; ++i){
    pool.emplace_back(std::thread(run, chunks[i].first, chunks[i].second));
  } 
  //join
  for (int i=0; i<NThreads; ++i){
    pool[i].join(); 
  } 

  auto t1 = std::chrono::steady_clock::now();
  std::chrono::duration<double> diff = t1-t0; 

  std::cout << "After execution:\n" << std::endl;
  show();

  std::cout << "execution time:" << diff << std::endl;
  std::cout << "existing Nodes: " << N << std::endl;
  std::cout << "existing Transitions: " << T << std::endl;
  std::cout << "starting tokens: " << Tkns << std::endl;
  std::cout << "#scans through all possible transitions: " << REP << std::endl;
  std::cout << "#executions of transitions: " << succT << "  " << succT*100/(REP*T) << "%" << std::endl;
  std::cout << "#failed transitions: " << unsuccT << "  " << unsuccT*100/(REP*T) << "%" << std::endl;
  return 0;
}



//TODO upon change, give hint about whether a transition can fire (may be inaccurate due to changes)
//vector<std::atomic_flag> ready
//but only in graph, as transitions need to be identified first, and for that you need adjacency!
