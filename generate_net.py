#generate_net.py
#from iteration_utilities import deepflatten



### PLACES
def init_data(places):
    res = [item for sublist in places for item in sublist]
    #list(deepflatten(places))
    return res

def set_lengths(places):
    res = []
    for a in places:
        res.append(len(a))
    return res 

def generate_places(places):
    data = init_data(places)
    lengths = set_lengths(places)
    llstr = str(lengths)[1:-1] #crop first and last bracket
    plcstr = str(data)[1:-1]

    content = """//this file will be automatically generated by python before compiling and executing 
#include <array>

std::array<double, {ldat} > data {{ {dat} }};

std::array<size_t, {nplc} > inputLengths {{ {lplc} }};
""".format( ldat = len(data), dat = plcstr, nplc = len(lengths),  lplc = llstr)

    f = open("./fuzzy/current_input.hh", "w") # fixed file name / overwrite
    f.write(content)
    f.close()

### RULES TODO?

# rules = {"rule1": {}}

# def generate_rules(rules):

#     for R in rules:
#         for p in 

#     content = """//RULES.hh
#     #ifndef RULES_HH
#     #define RULES_HH 

#     #include <vector>
#     #include <span>

#     decltype(auto)
#     rule1(const auto& In1, const auto& In2, const auto& Out, auto&& data ){
#     // generic binding to variables:
#     auto in1 = std::span(data.begin() + In1.start, In1.length);
#     auto in2 = std::span(data.begin() + In2.start, In2.length);
#     auto out = std::span(data.begin() + Out.start, Out.length);

#     //only permutations - basically swap back and forth
#     out[0] = in2[2];
#     out[1] = in2[1]; //how arbitrary can these functions become?
#     out[2] = in2[0];

#     #ifdef __cpp_lib_containers_ranges
#         std::vector<double> v (std::from_range, out);  //needs to go out by value
#     #else
#         std::vector<double> v (out.begin(), out.end());
#     #endif
#     return v;
#     }

#     #endif
#     """.format()

#     f = open("./transitions.hh", "w") # fixed file name / overwrite
#     f.write(content)
#     f.close()





##### TRANSITIONS #######

def generate_transitions(transitions):
    TR = []
    # for each transition
    for T in transitions:
        R = T[0]
    
        #for x in range (1,len(X)):
        mut = ["mut[pl[{}].pos]".format(x) for x in  T[1:]] 
        pla = ["pl[{}]".format(x) for x in  T[1:]] 

        mut = str(mut)[1:-1].replace('\'','')
        pla = str(pla)[1:-1].replace('\'','')

        trans =  """transitions.emplace_back([&]() -> std::vector<double> {{ //<------ return type must be value type not self deduced
            std::scoped_lock lck({mut});
            return {rule}({pla}, std::forward<D>(data));
        }});

        """.format(mut = mut, rule = R, pla =  pla)

        TR.append(trans)


    Trstr = "".join(TR)     

    # file
    content = """
    #ifndef TRANSITIONS_HH
    #define TRANSITIONS_HH 


    #include <vector>
    #include "rules.hh"
    
    const size_t N_TRANS = {nTrans};

    template<typename P, typename M, typename D>
    decltype(auto)
    make_transitions(P& pl, M& mut, D&& data )
    {{
      std::vector<std::function<std::vector<double>()>> transitions;
      transitions.reserve(N_TRANS);

      {trans}

      return transitions;
    }}

    #endif
    """.format(nTrans = len(transitions), trans = Trstr)
    #print(content)

    f = open("./fuzzy/transitions.hh", "w") # fixed file name / overwrite
    f.write(content)
    f.close()

#generate_transitions(transitions)