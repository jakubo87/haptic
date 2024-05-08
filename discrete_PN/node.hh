#include <mutex>

class pnode{
  public:

  pnode(size_t cnt) : count(cnt){}

  bool dec(size_t r){
    if (r > count)
      return false;
    else
  }

  private:
  std::

  std::atomic<size_t> count;
};

