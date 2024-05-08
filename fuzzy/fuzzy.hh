#ifndef FUZZY_HH
#define FUZZY_HH

//#include "settings.hh" maybe laterunified places with tags
#include <mutex>
#include <array>



class FPlace{
  private:
    std::mutex mutex;
  
  public:
    size_t start; //index at which the fuzzy set starts
    size_t length; // number of entries
  
    FPlace(size_t st,size_t le) : start(st), length{le} {
    }
    
    //attempted locking that fails after x attempts 
    // bool lock(int x = 20){
    //   bool succ = false;
    //   do{
    //      succ = mutex.try_lock();
    // }while(!succ && (x-- > 0));
    //   return succ;
    // }

    void lock(){
      mutex.lock();
    }

    bool try_lock(){
      return mutex.try_lock();
    }

    void unlock(){
      mutex.unlock();
    }

    auto begin() const{
      return data.begin()+start;
    }
    
    auto cbegin() const{
      return data.cbegin()+start;
    }
    
    auto end() const{
      return data.begin()+start+length;
    }
    
    auto cend() const{
      return data.cbegin()+start+length;
    }
};


#endif