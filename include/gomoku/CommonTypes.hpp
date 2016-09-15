/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GO_COMMON_TYPES_HPP
#define GO_COMMON_TYPES_HPP
#include "Place.hpp"

namespace go {
    
    struct Board {
    	std::vector<std::vector<Place>> board;
    	std::list<Place*> played_places;
    };
}

#endif /* GO_COMMON_TYPES_HPP */
