#ifndef PLACES_HH
#define PLACES_HH

#include <vector>
#include "current_input.hh"

// #######   FUZZY PLACE CLASS   ########

class FPlace{
  private:
  
  public:
    size_t start; //index at which the fuzzy set starts
    size_t length; // number of entries
    size_t pos; // position of the mutex and also own position in places 
    const volatile size_t BUFFER; //for alignment // TO TEST

};



//likely no policy with constexpr
//constexpr size_t DATASIZE{ std::accumulate(input.begin(), input.end(), 0, [](size_t sum, auto a){return a.size() + sum;})}; 

constexpr size_t DATASIZE { data.size()};
constexpr size_t N_PLACES { inputLengths.size()};

//create data and net from string through binding
//first create data array
//then fill with data
//then add places array, lastly return both
constexpr
decltype(auto) create_Net(const auto& inlengths){
    std::vector<FPlace> res;
    res.reserve(inlengths.size()); 
    size_t Dindex = 0;
    size_t Pindex = 0;
    for (const auto& l : inlengths){
        res.emplace_back(Dindex,l, Pindex, 0);
        Dindex += l;
        ++Pindex;
    }
    return res;
}


#endif