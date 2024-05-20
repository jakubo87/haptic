#ifndef RULES_HH
#define RULES_HH 

//RULES.hh
// a generated file to be read from the frontend and translated to functions readable by c++ 
// the output will be a vector of all  the places changes (i.e. all the fuzzy categories)

#include <vector>
#include <span>
//include "fuzzy.hh"
#include "concepts.hh"


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



#endif