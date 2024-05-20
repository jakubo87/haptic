
    #ifndef TRANSITIONS_HH
    #define TRANSITIONS_HH 


    #include <vector>
    #include "rules.hh"
    
    const size_t N_TRANS = 7;

    template<typename P, typename M, typename D>
    decltype(auto)
    make_transitions(P& pl, M& mut, D&& data )
    {
      std::vector<std::function<std::vector<double>()>> transitions;
      transitions.reserve(N_TRANS);

      transitions.emplace_back([&]() -> std::vector<double> { //<------ return type must be value type not self deduced
            std::scoped_lock lck(mut[pl[1].pos], mut[pl[0].pos], mut[pl[3].pos]);
            return rule1(pl[1], pl[0], pl[3], std::forward<D>(data));
        });

        transitions.emplace_back([&]() -> std::vector<double> { //<------ return type must be value type not self deduced
            std::scoped_lock lck(mut[pl[2].pos], mut[pl[3].pos], mut[pl[0].pos]);
            return rule1(pl[2], pl[3], pl[0], std::forward<D>(data));
        });

        transitions.emplace_back([&]() -> std::vector<double> { //<------ return type must be value type not self deduced
            std::scoped_lock lck(mut[pl[1].pos], mut[pl[2].pos]);
            return rule2(pl[1], pl[2], std::forward<D>(data));
        });

        transitions.emplace_back([&]() -> std::vector<double> { //<------ return type must be value type not self deduced
            std::scoped_lock lck(mut[pl[2].pos], mut[pl[1].pos]);
            return rule2(pl[2], pl[1], std::forward<D>(data));
        });

        transitions.emplace_back([&]() -> std::vector<double> { //<------ return type must be value type not self deduced
            std::scoped_lock lck(mut[pl[1].pos], mut[pl[2].pos]);
            return rule2(pl[1], pl[2], std::forward<D>(data));
        });

        transitions.emplace_back([&]() -> std::vector<double> { //<------ return type must be value type not self deduced
            std::scoped_lock lck(mut[pl[0].pos], mut[pl[1].pos]);
            return rule2(pl[0], pl[1], std::forward<D>(data));
        });

        transitions.emplace_back([&]() -> std::vector<double> { //<------ return type must be value type not self deduced
            std::scoped_lock lck(mut[pl[2].pos], mut[pl[4].pos]);
            return rule2(pl[2], pl[4], std::forward<D>(data));
        });

        

      return transitions;
    }

    #endif
    