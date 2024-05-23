
    #ifndef TRANSITIONS_HH
    #define TRANSITIONS_HH 


    #include <vector>
    #include "rules.hh"
    
    

    decltype(auto)
    make_transitions() //M& mut, D&& data )
    {
      std::vector<std::function<std::vector<double>()>> transitions;

        transitions.emplace_back([&]() -> std::vector<double> { //<------ return type must be explicitly given and a value type (not self deduced / default) otherwise it would make reference to temporary or ill-defined
            return rule0(p1, p2);
        }); //cannot find type of array that isbeing pointed to .... however i could try with pairs that define length by values so later cast to correct type

        transitions.emplace_back([&]() -> std::vector<double> {
            return rule0(p2, p3);
        });

        transitions.emplace_back([&]() -> std::vector<double> {
            return rule0(p3, p4);
        });

        transitions.emplace_back([&]() -> std::vector<double> {
            return rule0(p4, p1);
        });

        transitions.emplace_back([&]() -> std::vector<double> {
            return rule0(p5, p6);
        });

        transitions.emplace_back([&]() -> std::vector<double> {
            return rule0(p6, p5);
        });

        // transitions.emplace_back([&]() -> std::vector<double> {
        //     return rule0(p7, p1);
        // });

        

      return transitions;
    }

    #endif
    