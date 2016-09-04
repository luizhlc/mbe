/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

#ifndef TRAITS_HPP
#define TRAITS_HPP

#include <SFML/Graphics.hpp>

// Add Traits Types as necessary
enum class Type {
    GM, GOMOKU
};

template<Type T>
struct Traits { };

template<>
struct Traits<Type::GOMOKU> {
    constexpr static unsigned BOARD_DIMENSION = 15;
};

template<>
struct Traits<Type::GM> {
    static constexpr float SQUARE_SIZE = 48;
    constexpr static float BORDER_WIDTH = SQUARE_SIZE;
    constexpr static unsigned STONE_RADIUS = 18;
    constexpr static unsigned STONE_BORDER_WIDTH = 2; 
    const static sf::Color WHITE_COLOR;
    const static sf::Color BLACK_COLOR;
    const static sf::Color RED_COLOR;
    const static sf::Color WHITE_OUTLINE_COLOR;
    const static sf::Color BLACK_OUTLINE_COLOR;
};

using GomokuTraits = Traits<Type::GOMOKU>;
using GMTraits = Traits<Type::GM>;

#endif /* TRAITS_HPP */
