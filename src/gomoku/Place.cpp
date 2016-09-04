/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Casa.cpp
 * Author: luiz
 * 
 * Created on August 31, 2016, 1:43 PM
 */

#include "../include/gomoku/Place.hpp"
#include <iostream>
#include <cstdio>

Place::Place(int owner, int x, int y) : _owner(owner), _x(x), _y(y) {
}

Place::~Place() {
}

void Place::generate_neighborhood(std::vector<Place*> neighborhood){
	_neighborhood = neighborhood;
}

bool Place::put_piece(int piece) {
	if (_owner != 0 )
		return false;
	_owner = piece;
	return true;
}

int Place::find_piar(int direction, int lvl) {
    int in_a_row = 1;
    in_a_row += _neighborhood[direction]->find_piar(direction, lvl-1, _owner);
    return in_a_row;
} // pieces in a row

int Place::find_piar(int direction, int lvl, int color) {
    if (color != _owner || lvl < 1) {
        return 0;
    } else {
        return 1+_neighborhood[direction]->find_piar(direction, lvl-1, _owner);
    }
}

int Place::wes_find_piar(int direction, int lvl, int& tolerance, 
                        std::vector<std::pair<int, int> >& critical_locations) {
    // std::cout << "      TESTE 1.1.2\n";
    int in_a_row = 1;

    in_a_row += _neighborhood[direction]->wes_find_piar(direction, lvl-1, _owner, tolerance, critical_locations);
    // std::cout << "      TESTE 1.1.3\n";
    return in_a_row;
}

int Place::wes_find_piar(int direction, int lvl, int color, int& tolerance, 
                        std::vector<std::pair<int, int> >& critical_locations) {
    printf("Looking for a sequence of four...\n Tolerance is %d, Level is %d\n", tolerance, lvl);
    if (lvl < 1) {
        printf("    End of sequence...\n");
        // std::cout << "          TESTE 1.1.2.1a\n";
        return 0;
    } else if (tolerance > 0 && _owner == 0) {
        // std::cout << "          TESTE 1.1.2.b\n";
        printf("    We have tolerance... Place is empty\n");
        critical_locations.push_back(std::make_pair(_x,_y));
        tolerance--;
        return _neighborhood[direction]->wes_find_piar(direction, lvl-1, color, tolerance, critical_locations);
    } else if (color != _owner) {
        printf("    We have no tolerance OR this place is owned by the enemy\n");
        return 0;
    } else {
        printf("    Normal situation, continue looking\n");
        return 1+_neighborhood[direction]->wes_find_piar(direction, lvl-1, color, tolerance, critical_locations);
    }        
}

void Place::test_me() {
    std::cout << "Position = ("<< _x <<", " << _y<<"); Owner = "<< _owner <<"\n"   ;

}