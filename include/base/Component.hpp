/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

namespace base {
    template<typename Object, typename Info, typename Return = void>
    class Component {
     public:
        using Agent = Object;
        using Element = Info;
        using Product = Return;

        Return update(Object&, Info&);
     private:
        virtual Return doUpdate(Agent&, Element&) = 0;
    };    
}

#include "Component.ipp"

#endif /* COMPONENT_HPP */