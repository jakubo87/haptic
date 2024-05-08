#ifndef CONCEPTS_HH
#define CONCEPTS_HH

//concepts can be concatenated, like:
// requires fuzzy2cat::value == std::is_floating_point


template<typename P>
concept fuzzy1cat = requires (P place){
    place.length == 1;
};  

template<typename P>
concept fuzzy2cat = requires (P place){
    place.length == 2;
};

template<typename P>
concept fuzzy3cat = requires (P place){
    place.length == 3;
};

template<typename P>
concept fuzzy4cat = requires (P place){
    place.length == 4;
};

template<typename P>
concept fuzzy5cat = requires (P place){
    place.length == 5;
};

template<typename P>
concept fuzzy6cat = requires (P place){
    place.length == 6;
};

#endif
