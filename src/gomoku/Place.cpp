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
	if (_owner != 0 && piece != 0)
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
//    printf("Looking for a sequence of four...\n Tolerance is %d, Level is %d\n", tolerance, lvl);
    if (lvl < 1) {
//        printf("    End of sequence...\n");
        // std::cout << "          TESTE 1.1.2.1a\n";
        return 0;
    } else if (tolerance > 0 && _owner == 0) {
        // std::cout << "          TESTE 1.1.2.b\n";
//        printf("    We have tolerance... Place is empty\n");
        critical_locations.push_back(std::make_pair(_x,_y));
        tolerance--;
        return _neighborhood[direction]->wes_find_piar(direction, lvl-1, color, tolerance, critical_locations);
    } else if (color != _owner) {
//        printf("    We have no tolerance OR this place is owned by the enemy\n");
        return 0;
    } else {
//        printf("    Normal situation, continue looking\n");
        return 1+_neighborhood[direction]->wes_find_piar(direction, lvl-1, color, tolerance, critical_locations);
    }        
}

void Place::test_me() {
    std::cout << "Position = ("<< _x <<", " << _y<<"); Owner = "<< _owner <<"\n"   ;

}

int Place::wes_find_piar_base(int direction, int lvl, int color, int& tolerance) {
    // std::cout << "      TESTE 1.1.2\n";
    int in_a_row = 1;

    in_a_row += _neighborhood[direction]->wes_find_piar(direction, lvl-1, color, tolerance);
    // std::cout << "      TESTE 1.1.3\n";
    return in_a_row;
}

int Place::wes_find_piar(int direction, int lvl, int color, int& tolerance) {
//    printf("Looking for a sequence of four...\n Tolerance is %d, Level is %d\n", tolerance, lvl);
    if (lvl < 1) {
//        printf("    End of sequence...\n");
        // std::cout << "          TESTE 1.1.2.1a\n";
        return 0;
    } else if (tolerance > 0 && _owner == 0) {
        // std::cout << "          TESTE 1.1.2.b\n";
//        printf("    We have tolerance... Place is empty\n");
        tolerance--;
        return _neighborhood[direction]->wes_find_piar(direction, lvl-1, color, tolerance);
    } else if (color != _owner) {
//        printf("    We have no tolerance OR this place is owned by the enemy\n");
        return 0;
    } else {
//        printf("    Normal situation, continue looking\n");
        return 1+_neighborhood[direction]->wes_find_piar(direction, lvl-1, color, tolerance);
    }        
}

int Place::wep_find_piar_base(int direction, int lvl, int color, int& tolerance) {
    // std::cout << "      TESTE 1.1.2\n";
    int in_a_row = 1;

    in_a_row += _neighborhood[direction]->wep_find_piar(direction, lvl-1, color, tolerance);
    // std::cout << "      TESTE 1.1.3\n";
    return in_a_row;
}

int Place::wep_find_piar(int direction, int lvl, int color, int& tolerance) {
//    printf("Looking for a sequence of four...\n Tolerance is %d, Level is %d\n", tolerance, lvl);
    if (lvl < 1 || _owner == 0) {
//        printf("    End of sequence...\n");
        // std::cout << "          TESTE 1.1.2.1a\n";
        return 0;
    } else if (color != _owner) {
//        printf("    We have no tolerance OR this place is owned by the enemy\n");
        tolerance--;
        return 0;
    } else {
//        printf("    Normal situation, continue looking\n");
        return 1+_neighborhood[direction]->wep_find_piar(direction, lvl-1, color, tolerance);
    }        
}

Place* Place::get_neighbor(int direction){
    return _neighborhood[direction];
}

long long int Place::score(int level) {
    
    // Verificação dos arredores
    // int fiar = 0, four = 0, three = 0, h_three = 0, two = 0, one = 0, crap = 0;


    // for (int dir = 0; dir < 4; ++dir) {
    //     auto left = _neighborhood[dir];
    //     auto right = _neighborhood[7-dir];
    //     int counter = 1;
    //     while (left->_owner == _owner) {
    //         counter++;
    //         left = left->_neighborhood[dir];
    //     }
    //     while (right->_owner == _owner) {
    //         counter++;
    //         right = right->_neighborhood[7-dir];
    //     }
    //     if (counter >= 5) {
    //         fiar++;
    //     } else if (counter >= 4) {
    //         four++;
    //     } else {
    //         if (counter > 2) {
    //             if (left->_owner == 0 && right->_owner == 0) {
    //                 three++;
    //             } else if (left->_owner == 0 || right->_owner == 0) {
    //                 h_three++;
    //             } else {
    //                 crap++;
    //             }
    //         } else {
    //             if (left->_owner == 0 && right->_owner == 0) {
    //                 if (counter < 2) {
    //                     one++;
    //                 } else {
    //                     two++;
    //                 }
    //             } else {
    //                 crap++;
    //             }
    //         }
    //     }
    // }
    // return fiar*fiar*1000 + four*four*100 + three*20 + two*2 + one;



    // printf("begin (%d, %d)\n", _x, _y);
    int empty_space = 0; // 000 1
    
    int two_in_row = 0;
    int b_two_in_row = 0;

    int bh_three_in_row = 0; // X 111 0
    int mgh_three_in_row = 0; // X 0 111 0
    int gh_three_in_row = 0; // 0 11 0 1 0
    int three_in_row = 0; // 00 111 00 

    int four_in_row = 0; // 0 1111 0
    int h_four_in_row = 0; // 1 0 111 0
    int bh_four_in_row = 0; // 1 0 11 0 1 
    int two_h_four_in_row = 0; // 1 0 111 0 1 || 11 0 11 0 11
    int b_four_in_row = 0;  // X 1111 0

    int five_in_row = 0; // 11111 

    //verificação de posições vazias
    // for( int direction = 0; direction < 8; direction++ ) {
    //     Place* current_a = _neighborhood[direction];
    //     int i = 0;
    //     for( ; i < 2 && current_a->_owner == 0; ++i ) {//range limitado a 3 
    //         current_a = current_a->_neighborhood[direction];
    //     }

    //     if( current_a->_owner == 0 || current_a->_owner == _owner ) {//só conta se não estiver bloqueado
    //         empty_space += i; 
    //     }
    // }
    
    int enemy = (_owner%2)+1;


    
    //testando própria sequência
    for ( int direction = 0; direction < 4; direction++ ) {
        int piar = 1; // própria casa = 1; // Pieces in a Row
        Place* current_l = _neighborhood[direction];
        Place* current_r = _neighborhood[7-direction];
        // printf("A(%d, %d)\n", current_l->_x, current_l->_y);

        auto left_owned = current_l->_owner;
        auto right_owned = current_r->_owner;
        while( left_owned == _owner ) {
            current_l = current_l->_neighborhood[direction];
            piar++;
            left_owned = current_l->_owner;
        }
        // printf("end_A(%d, %d)\n", current_l->_x, current_l->_y);
        // printf("B(%d, %d)\n", current_r->_x, current_r->_y);
        while( right_owned == _owner ) {
            current_r = current_r->_neighborhood[7-direction];
            piar++;
            right_owned = current_r->_owner;
        }
        // printf("end_B(%d, %d)\n", current_r->_x, current_r->_y);

        if( piar >= 5 ) { // 1 1 1 1 1 
            five_in_row++;
        } else if ( piar == 4 ) { // 1 1 1 1 
            if( left_owned == 0 && right_owned == 0 ) {// 0 1 1 1 1 0
                four_in_row++;
            } else if ( left_owned == 0 || right_owned == 0 ) { // 0 1 1 1 1 X
                b_four_in_row++;
            }
        } else if (piar == 3) {
            if( left_owned == 0 && right_owned == 0 ) { // 0 1 1 1 0
                auto right_skip = current_l->_neighborhood[direction]->_owner; 
                auto left_skip = current_r->_neighborhood[7-direction]->_owner;
                if( right_skip == _owner && left_skip == _owner) {// 1 0 1 1 1 0 1 DEAD
                    two_h_four_in_row++;
                } else if( right_skip == _owner || left_skip == _owner) { // 1 0 1 1 1 hope
                    h_four_in_row++;
                } else if ( right_skip ==  0 && left_skip == 0 ) {// 0 0 1 1 1 0 0 
                    three_in_row++;
                } else if (right_skip ==  0 || left_skip == 0) {// X 0 1 1 1 0
                    mgh_three_in_row++;
                }
            } else if  ( (left_owned == 0 &&  right_owned != 0) || (left_owned != 0 &&  right_owned == 0) ) {// X 1 1 1 0
                if( left_owned == 0 ) {
                    if( current_l->_neighborhood[direction]->_owner == _owner ) { // X 1 1 1 0 1
                        b_four_in_row++;
                    } else {
                        bh_three_in_row++;
                    }
                } else {
                    if ( current_r->_neighborhood[7-direction]->_owner == _owner ) {
                        b_four_in_row++;
                    } else {
                        bh_three_in_row++;
                    }
                }
                //   bh_three_in_row++;
            }    
        } else if ( piar == 2) {
            if( left_owned == 0 && right_owned == 0 ) { // 0 1 1 0
                Place* skip_l = current_l->_neighborhood[direction]; 
                int count_l = 0;
                while( skip_l->_owner == _owner ) {
                    skip_l = skip_l->_neighborhood[direction];
                    count_l++;
                }

                Place* skip_r = current_r->_neighborhood[7-direction];
                int count_r = 0;
                while( skip_r->_owner == _owner ) {
                    skip_r = skip_r->_neighborhood[direction];
                    count_r++;
                }

                if( count_r > 1 && count_l > 1 ) { // 1 1 0 1 1 0 1 1 END
                    // two_h_four_in_row++;
                    h_four_in_row++;
                } else if ( count_r > 1 || count_l > 1 ) { // 1 1 0 1 1
                    h_four_in_row++;
                } else if ( count_r == 1 && count_l == 1 ) {// 1 0 11 0 1
                    bh_four_in_row++;
                } else if ( (count_r == 1 && skip_r->_owner == 0) || (count_l == 1 && skip_l->_owner == 0) ) {// 0 11 0 1 0 
                    gh_three_in_row++;
                } else if ( (count_r == 1 && skip_r->_owner != 0) || (count_l == 1 && skip_l->_owner != 0) ) {// 0 11 0 1 X
                    bh_three_in_row++;
                } else if ( count_r == 0 && count_l==0 ) {// 0 11 0
                    if (skip_r->_owner == 0 && skip_l->_owner == 0) { // 00 11 00
                        two_in_row++;
                    } else if ( (skip_r->_owner == 0 && skip_l->_owner != 0)  ||  (skip_r->_owner != 0 && skip_l->_owner == 0) ) { // X 0 11 00
                        b_two_in_row++;
                    }
                } 
            } else if ( left_owned == 0 || right_owned == 0 ) {
                int var = left_owned == 0 ? 0 : 1;
                int dir = abs(7*var-direction);

                auto skip = var == 0 ? current_l->_neighborhood[dir] : current_r->_neighborhood[dir]; 
                int count = 0;
                while( skip->_owner == _owner ) {
                    skip = skip->_neighborhood[dir];
                    count++;
                }

                if (count > 1) {//X 11 0 11
                    if( skip->_owner == 0) {//X 11 0 11 0
                        if (skip->_neighborhood[dir]->_owner == _owner){//X 11 0 11 0 1
                            two_h_four_in_row++;
                        } else {//X 11 0 11 0 ?
                            bh_four_in_row++;
                        }
                    }
                } else if ( count == 1) {// X 11 0 1 
                    if( skip->_owner == 0) {// X 11 0 1 0
                        if (skip->_neighborhood[dir]->_owner == _owner){ // X 11 0 1 0 1
                            bh_four_in_row++;
                        } else if ( skip->_neighborhood[dir]->_owner == 0) {// X 11 0 1 00
                            bh_three_in_row++;
                        }
                    }

                } else {
                    b_two_in_row++;
                }
            } else {
                b_two_in_row++;
            }
        } else { //? 1 ?
            if (left_owned == 0 && right_owned == 0) { // ? 0 1 0 ?
                
                // Processo de Busca pela esquerda
                Place* skip_l = current_l->_neighborhood[direction]; 
                int count_l = 0;
                while( skip_l->_owner == _owner ) {
                    skip_l = skip_l->_neighborhood[direction];
                    count_l++;
                }
                // Processo de Busca pela direita
                Place* skip_r = current_r->_neighborhood[7-direction];
                int count_r = 0;
                while( skip_r->_owner == _owner ) {
                    skip_r = skip_r->_neighborhood[direction];
                    count_r++;
                }

                if (count_r > 2 || count_l > 2) { // ? 0 1 0 1 1 1 ?
                    if (count_r > count_l) { // ? 0 1 0 1 1 1 ?
                        if (skip_l->_owner == _owner) {

                        } else {

                        }
                    } else if (count_l > count_r) { // ? 1 1 1 0 1 0 ?
                        if (skip_r->_owner == _owner) { // ? 1 1 1 0 1 0 1 ?

                        } else {

                        }
                    } else {  // 1 1 1 0 1 0 1 1 1
                        four_in_row++;
                    }
                } else if (0) {

                } else {

                }
            } else if (left_owned == 0 || right_owned == 0) {
                if (left_owned == 0) {

                } else {

                }
            } else {

            }
        }
    }
/*
    int e_two_in_row = 0;
    int e_b_two_in_row = 0;

    int e_bh_three_in_row = 0; // X 111 0
    int e_mgh_three_in_row = 0; // X 0 111 0
    int e_gh_three_in_row = 0; // 0 11 0 1 0
    int e_three_in_row = 0; // 00 111 00 

    int e_four_in_row = 0; // 0 1111 0
    int e_h_four_in_row = 0; // 1 0 111 0
    int e_bh_four_in_row = 0; // 1 0 11 0 1 
    int e_two_h_four_in_row = 0; // 1 0 111 0 1 || 11 0 11 0 11
    int e_b_four_in_row = 0;  // X 1111 0

    int e_five_in_row = 0; // 11111 

    for ( int direction = 0; direction < 4; direction++ ) {
        int r_o, l_o;
        int piar = 1; // própria casa = 1; // Pieces in a Row
        Place* current_l = _neighborhood[direction];
        Place* current_r = _neighborhood[7-direction];
        // printf("A(%d, %d)\n", current_l->_x, current_l->_y);

        auto left_owned = current_l->_owner;
        auto right_owned = current_r->_owner;
        while( left_owned == enemy ) {
            current_l = current_l->_neighborhood[direction];
            piar++;
            left_owned = current_l->_owner;
        }
        // printf("end_A(%d, %d)\n", current_l->_x, current_l->_y);
        // printf("B(%d, %d)\n", current_r->_x, current_r->_y);
        while( right_owned == enemy ) {
            current_r = current_r->_neighborhood[7-direction];
            piar++;
            right_owned = current_r->_owner;
        }
        // printf("end_B(%d, %d)\n", current_r->_x, current_r->_y);

        if( piar >= 5 ) { // 1 1 1 1 1 
            e_five_in_row++;
        } else if ( piar == 4 ) { // 1 1 1 1 
            if( left_owned == 0 && right_owned == 0 ) {// 0 1 1 1 1 0
                e_four_in_row++;
            } else if (left_owned == 0 || right_owned == 0) { // 0 1 1 1 1 X
                e_b_four_in_row++;
            }
        } else if (piar == 3) {
            if( left_owned == 0 && right_owned == 0 ) { // 0 1 1 1 0
                auto right_skip = current_l->_neighborhood[direction]->_owner; 
                auto left_skip = current_r->_neighborhood[7-direction]->_owner;
                if( right_skip == enemy && left_skip == enemy) {// 1 0 1 1 1 0 1 DEAD
                    e_two_h_four_in_row++;
                } else if( right_skip == enemy || left_skip == enemy) { // 1 0 1 1 1 hope
                    e_h_four_in_row++;
                } else if ( right_skip ==  0 && left_skip == 0 ) {// 0 0 1 1 1 0 0 
                    e_three_in_row++;
                } else if (right_skip ==  0 || left_skip == 0) {// X 0 1 1 1 0
                    e_mgh_three_in_row++;
                }
            } else if  ( (left_owned == 0 &&  right_owned != 0) || (left_owned != 0 &&  right_owned == 0) ) {// X 1 1 1 0
                if( (l_o = current_l->_neighborhood[direction]->_owner) == enemy && left_owned == 0 ) { // X 1 1 1 0 1
                    e_b_four_in_row++;
                } else if ((r_o = current_r->_neighborhood[7-direction]->_owner) == enemy) {
                    e_b_four_in_row++;
                } else if (r_o == 0 || l_o == 0){ // x 1 1 1 0 
                    e_bh_three_in_row++;
                }
            }    
        } else if ( piar == 2) {
            if( left_owned == 0 && right_owned == 0 ) { // 0 1 1 0
                Place* skip_l = current_l->_neighborhood[direction]; 
                int count_l = 0;
                while( skip_l->_owner == enemy ) {
                    skip_l = skip_l->_neighborhood[direction];
                    count_l++;
                }

                Place* skip_r = current_r->_neighborhood[7-direction];
                int count_r = 0;
                while( skip_r->_owner == enemy ) {
                    skip_r = skip_r->_neighborhood[direction];
                    count_r++;
                }

                if( count_r > 1 && count_l > 1 ) { // 1 1 0 1 1 0 1 1 END
                    e_two_h_four_in_row++;
                } else if ( count_r > 1 || count_l > 1 ) { // 1 1 0 1 1
                    e_h_four_in_row++;
                } else if ( count_r == 1 && count_l == 1 ) {// 1 0 11 0 1
                    e_bh_four_in_row++;
                } else if ( (count_r == 1 && skip_r->_owner == 0) || (count_l == 1 && skip_l->_owner == 0) ) {// 0 11 0 1 0 
                    e_gh_three_in_row++;
                } else if ( (count_r == 1 && skip_r->_owner != 0) || (count_l == 1 && skip_l->_owner != 0) ) {// 0 11 0 1 X
                    e_bh_three_in_row++;
                } else if ( count_r == 0 && count_l==0 ) {// 0 11 0
                    if (skip_r->_owner == 0 && skip_l->_owner == 0) { // 00 11 00
                        e_two_in_row++;
                    } else if ( (skip_r->_owner == 0 && skip_l->_owner != 0)  ||  (skip_r->_owner != 0 && skip_l->_owner == 0) ) { // X 0 11 00
                        e_b_two_in_row++;
                    }
                } 
            } else if ( left_owned == 0 || right_owned == 0 ) {
                int var = left_owned == 0 ? 0 : 1;
                int dir = abs(7*var-direction);

                auto skip = var == 0 ? current_l->_neighborhood[dir] : current_r->_neighborhood[dir]; 
                int count = 0;
                while( skip->_owner == enemy ) {
                    skip = skip->_neighborhood[dir];
                    count++;
                }

                if (count > 1) {//X 11 0 11
                    if( skip->_owner == 0) {//X 11 0 11 0
                        if (skip->_neighborhood[dir]->_owner == enemy){//X 11 0 11 0 1
                            e_h_four_in_row++;
                        } else {//X 11 0 11 0 ?
                            e_bh_four_in_row++;
                        }
                    }
                } else if ( count == 1) {// X 11 0 1 
                    if( skip->_owner == 0) {// X 11 0 1 0
                        if (skip->_neighborhood[dir]->_owner == enemy){ // X 11 0 1 0 1
                            e_bh_four_in_row++;
                        } else if ( skip->_neighborhood[dir]->_owner == 0) {// X 11 0 1 00
                            e_bh_three_in_row++;
                        }
                    }

                } else {
                    e_b_two_in_row++;
                }
            } else {
                e_b_two_in_row++;
            }
        } else { //? 1 ?
            // if (left_owned == 0 && right_owned == 0) { // ? 0 1 0 ?
                
            //     // Processo de Busca pela esquerda
            //     Place* skip_l = current_l->_neighborhood[direction]; 
            //     int count_l = 0;
            //     while( skip_l->_owner == _owner ) {
            //         skip_l = skip_l->_neighborhood[direction];
            //         count_l++;
            //     }
            //     // Processo de Busca pela direita
            //     Place* skip_r = current_r->_neighborhood[7-direction];
            //     int count_r = 0;
            //     while( skip_r->_owner == _owner ) {
            //         skip_r = skip_r->_neighborhood[direction];
            //         count_r++;
            //     }

            //     if (count_r > 2 || count_l > 2) { // ? 0 1 0 1 1 1 ?
            //         if (count_r > count_l) {

            //         } else if (count_l > count_r) {

            //         } else {  // 1 1 1 0 1 0 1 1 1
            //             four_in_row++;
            //         }
            //     }
            // } else if (left_owned == 0 || right_owned == 0) {
            //     if (left_owned == 0) {

            //     } else {

            //     }
            // } else {

            // }
        }
    }
    // for ( int direction = 0; direction < 4; direction++ ) {
    //     int piar = 1; // própria casa = 1;
    //     Place* current_l = _neighborhood[direction];
    //     // printf("A(%d, %d)\n", current_l->_x, current_l->_y);
    //     while( left_owned == enemy ) {
    //         current_l = current_l->_neighborhood[direction];
    //         piar++;
    //     }
    //     // printf("end_A(%d, %d)\n", current_l->_x, current_l->_y);

    //     Place* current_r = _neighborhood[7-direction];
    //     // printf("B(%d, %d)\n", current_r->_x, current_r->_y);
    //     while( right_owned == enemy ) {
    //         current_r = current_r->_neighborhood[7-direction];
    //         piar++;
    //     }
    //     if( piar == 5) {
    //         four_in_row++;
    //     } else if ( piar == 4) {
    //         three_in_row++;
    //     }
    // }


    //int empty_space = 0; // 000 1
    
    // int two_in_row = 0;
    // int b_two_in_row = 0;

    // int bh_three_in_row = 0; // X 111 0
    // int gh_three_in_row = 0; // X 0 111 0
    // int three_in_row = 0; // 00 111 00 

    // int four_in_row = 0; // 0 1111 0
    // int h_four_in_row = 0; // 1 0 111 0
    // int bh_four_in_row = 0; // 1 0 11 0 1 
    // int two_h_four_in_row = 0; // 1 0 111 0 1 || 11 0 11 0 11
    // int b_four_in_row = 0;  // X 1111 0

    // int five_in_row = 0; // 11111 
    //b_two_in_row*10 
    // printf("end\n");*/
    return empty_space*5 + two_in_row*(1e2+50)
            + bh_three_in_row*1e2 + gh_three_in_row*(1e3) + mgh_three_in_row*(1e3+2) + three_in_row*(1e3+100)
            + b_four_in_row*(1e4-50) + bh_four_in_row*(1e3-50) + h_four_in_row*(1e5) + four_in_row*(1e7) + two_h_four_in_row*(1e7-50)
            + five_in_row*(1e10)*(level+1)
            //+ e_five_in_row*(1e10)*(level+1)
            //+ e_four_in_row*(1e6)
            ;
}

int Place::check_game_won() {
    for (int direction = 0; direction < 4; direction++) {
        int count_a = 0;
        Place* current_a = _neighborhood[direction];
        // printf("A(%d, %d)\n", current_a->_x, current_a->_y);
        while (current_a->_owner == _owner) {
            current_a = current_a->_neighborhood[direction];
            count_a++;
        }
        // printf("end_A(%d, %d)\n", current_a->_x, current_a->_y);
        int count_b = 0;
        Place* current_b = _neighborhood[7-direction];
        // printf("B(%d, %d)\n", current_b->_x, current_b->_y);
        while (current_b->_owner == _owner) {
            current_b = current_b->_neighborhood[7-direction];
            count_b++;
        }
        // printf("end_B(%d, %d)\n", current_b->_x, current_b->_y);
        int total = count_a + count_b + 1;
        
        if (total == 5)
            return _owner;        
    }
    return 0;
}
   