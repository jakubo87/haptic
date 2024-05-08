//more permanent distribution - for now attept with async/futures
template <typename T>
void
distrib_data(const T& ){
  int len = ceil(transitions.size() / NThreads);
  for (int i=0; i < NThreads; ++i){
    size_t st = i*len;
    size_t ed = std::min(st+len, transitions.size());
    chunks[i] = std::make_pair(st,ed);
  }
}