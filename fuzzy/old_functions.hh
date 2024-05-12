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