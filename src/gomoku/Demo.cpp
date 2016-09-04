#include "gomoku/Demo.hpp"
#include <cstdio>

void Demo::onUpdateRenderer(Renderer& window) {
    graphics.update(*this, window);
} 

Demo::Transition Demo::onProcessInput(Input& input) {
           // std::cout << "[CALL] Demo::onProcessInput" << std::endl;
        auto move = players[currentPlayer].processInput(board, input);
        // Jogadas fora do tabuleiro são respondidas com -1, -1
        if (move != gm::Position{-1, -1}) {
            // Verifica se a posição está desocupada
            if (board[move.row+1][move.column+1].put_piece(currentPlayer+1)){ 
                played_places.push_back(&board[move.row+1][move.column+1]); //MAS QUE NOJO
                int winner = check_game_won(move.row+1, move.column+1);
                if(winner>-1){
                    std::cout << "!!!!!!!! PLAYER "<< winner << " WIN !!!!!!!!\n";
                    // return {Transition::Type::CLOSE, this};
                }
                currentPlayer = 1 - currentPlayer;
                critical_places = find_four(move.row+1, move.column+1);
            }
        }
        return {Transition::Type::SELF, this};
}

void Demo::generate_places() {
    for (unsigned i = 1; i < Traits<Type::GOMOKU>::BOARD_DIMENSION+1; ++i) {
        board[i].reserve(Traits<Type::GOMOKU>::BOARD_DIMENSION+2);
        board[i][0] = Place(-1, i, 0); // Wall O 
        
        for (unsigned j = 1; j < Traits<Type::GOMOKU>::BOARD_DIMENSION+1; ++j) {
            board[i][j] = Place(0, i, j); //playable places
        }
        board[i][Traits<Type::GOMOKU>::BOARD_DIMENSION+1] = Place(-1, i, Traits<Type::GOMOKU>::BOARD_DIMENSION+1); // Wall L
    }
    
    board[0].reserve(Traits<Type::GOMOKU>::BOARD_DIMENSION+2);
    board[Traits<Type::GOMOKU>::BOARD_DIMENSION+1].reserve(Traits<Type::GOMOKU>::BOARD_DIMENSION+2);
    
    for (unsigned i = 1; i < Traits<Type::GOMOKU>::BOARD_DIMENSION+1; ++i) {
        board[0][i] = Place(-1, 0, i); // Wall N
        board[Traits<Type::GOMOKU>::BOARD_DIMENSION+1][i] = Place(-1, Traits<Type::GOMOKU>::BOARD_DIMENSION+1, i); // Wall S
    }

    board[0][0] = Place(-1, 0, 0); // Wall NO
    board[0][Traits<Type::GOMOKU>::BOARD_DIMENSION+1] = Place(-1, 0, Traits<Type::GOMOKU>::BOARD_DIMENSION+1); // Wall NL
    board[Traits<Type::GOMOKU>::BOARD_DIMENSION+1][0] = Place(-1, Traits<Type::GOMOKU>::BOARD_DIMENSION+1, 0); // Wall SO
    board[Traits<Type::GOMOKU>::BOARD_DIMENSION+1][Traits<Type::GOMOKU>::BOARD_DIMENSION+1] = Place(-1, Traits<Type::GOMOKU>::BOARD_DIMENSION+1, Traits<Type::GOMOKU>::BOARD_DIMENSION+1); // Wall SL
}

void Demo::generate_neighbors() {
    for (unsigned i = 1; i < 16; ++i) {
        for (unsigned j = 1; j < 16; ++j) {
            board[i][j].generate_neighborhood({&board[i-1][j-1], &board[i-1][j], &board[i-1][j+1], 
                                       &board[i][j-1],                    &board[i][j+1],
                                       &board[i+1][j-1], &board[i+1][j], &board[i+1][j+1]
                                      });
        }
    }
}
    
int Demo::check_game_won(int row, int column) {
    
    Place last_place;
    last_place = board[row][column];
    
    //pieces_in_row: -1 is done because find_piar() count two times the piece in last_place;
    int pieces_in_row = last_place.find_piar(E, 5)+last_place.find_piar(W, 5)-1; // Horizontal
    if(pieces_in_row >= 5)
        return currentPlayer;
    
    pieces_in_row = last_place.find_piar(N, 5)+last_place.find_piar(S, 5)-1; // Vertical
    if(pieces_in_row >= 5)
        return currentPlayer;
    
    pieces_in_row = last_place.find_piar(NW, 5)+last_place.find_piar(SE, 5)-1; // Diagonal
    if(pieces_in_row >= 5)
        return currentPlayer;
    
    pieces_in_row = last_place.find_piar(NE, 5)+last_place.find_piar(SW, 5)-1; // Inverse Diagonal
    if(pieces_in_row >= 5)
        return currentPlayer;
    
    return -1;
}

std::vector<std::pair<int, int> > Demo::find_four(int row, int column) {
    Place last_place;
    last_place = board[row][column];

    printf("\nCheck E to W\n");
    std::vector<std::pair<int, int> > critical_locations, temp_crit;
    int tolerance = 1;
    int pieces_in_row = last_place.wes_find_piar(E, 5, tolerance, temp_crit)-1;
    pieces_in_row += last_place.wes_find_piar(W, 5 - pieces_in_row, tolerance, temp_crit);
    
    if(pieces_in_row > 3) {
        critical_locations.insert(critical_locations.begin(), temp_crit.begin(), temp_crit.end());
     std::cout << "(" << critical_locations.front().first << ", "<< critical_locations.front().second << "\n";
    }
    temp_crit.clear();

    printf("\nCheck W to E\n");
    tolerance = 1;
    pieces_in_row = last_place.wes_find_piar(W, 5, tolerance, temp_crit)-1;
    pieces_in_row += last_place.wes_find_piar(E, 5 - pieces_in_row, tolerance, temp_crit);
    if(pieces_in_row > 3) {
        critical_locations.insert(critical_locations.begin(), temp_crit.begin(), temp_crit.end());
     std::cout << "(" << critical_locations.front().first << ", "<< critical_locations.front().second << "\n";
    }
    temp_crit.clear();

    printf("\nCheck S to N\n");
    tolerance = 1;
    pieces_in_row = last_place.wes_find_piar(S, 5, tolerance, temp_crit)-1;
    pieces_in_row += last_place.wes_find_piar(N, 5 - pieces_in_row, tolerance, temp_crit);
    if(pieces_in_row > 3) {
        critical_locations.insert(critical_locations.begin(), temp_crit.begin(), temp_crit.end());
     std::cout << "(" << critical_locations.front().first << ", "<< critical_locations.front().second << "\n";
    }
    temp_crit.clear();

    printf("\nCheck N to S\n");
    tolerance = 1;
    pieces_in_row = last_place.wes_find_piar(N, 5, tolerance, temp_crit)-1;
    pieces_in_row += last_place.wes_find_piar(S, 5 - pieces_in_row, tolerance, temp_crit);
    if(pieces_in_row > 3) {
        critical_locations.insert(critical_locations.begin(), temp_crit.begin(), temp_crit.end());
     std::cout << "(" << critical_locations.front().first << ", "<< critical_locations.front().second << "\n";
    }
    temp_crit.clear();

    printf("\nCheck NE to SW\n");
    tolerance = 1;
    pieces_in_row = last_place.wes_find_piar(NE, 5, tolerance, temp_crit)-1;
    pieces_in_row += last_place.wes_find_piar(SW, 5 - pieces_in_row, tolerance, temp_crit);
    if(pieces_in_row > 3) {
        critical_locations.insert(critical_locations.begin(), temp_crit.begin(), temp_crit.end());
     std::cout << "(" << critical_locations.front().first << ", "<< critical_locations.front().second << "\n";
    }
    temp_crit.clear();

    printf("\nCheck SW to NE\n");
    tolerance = 1;
    pieces_in_row = last_place.wes_find_piar(SW, 5, tolerance, temp_crit)-1;
    pieces_in_row += last_place.wes_find_piar(NE, 5 - pieces_in_row, tolerance, temp_crit);
    if(pieces_in_row > 3) {
        critical_locations.insert(critical_locations.begin(), temp_crit.begin(), temp_crit.end());
     std::cout << "(" << critical_locations.front().first << ", "<< critical_locations.front().second << "\n";
    }
    temp_crit.clear();

    printf("\nCheck NW to SE\n");
    tolerance = 1;
    pieces_in_row = last_place.wes_find_piar(NW, 5, tolerance, temp_crit)-1;
    pieces_in_row += last_place.wes_find_piar(SE, 5 - pieces_in_row, tolerance, temp_crit);
    if(pieces_in_row > 3) {
        critical_locations.insert(critical_locations.begin(), temp_crit.begin(), temp_crit.end());
     std::cout << "(" << critical_locations.front().first << ", "<< critical_locations.front().second << "\n";
    }
    temp_crit.clear();

    printf("\nCheck SE to NW\n");
    tolerance = 1;
    pieces_in_row = last_place.wes_find_piar(SE, 5, tolerance, temp_crit)-1;
    pieces_in_row += last_place.wes_find_piar(NW, 5 - pieces_in_row, tolerance, temp_crit);
    if(pieces_in_row > 3) {
        critical_locations.insert(critical_locations.begin(), temp_crit.begin(), temp_crit.end());
         std::cout << "(" << critical_locations.front().first << ", "<< critical_locations.front().second << "\n";
    }

    return critical_locations;

}

void DemoGraphics::doUpdate(Demo& demo, gm::Renderer& renderer) {
    // Desenhamos o tabuleiro
    drawBoard(renderer);
    // Pegamos o tabuleiro
    auto stones = demo.getPlayedPlaces();

    for (auto stone : stones) {
        int owner = stone->get_owner();
        if (owner > 0) {
             sf::Color color = owner == 1 ? GMTraits::BLACK_COLOR : GMTraits::WHITE_COLOR;
                // gm::Stone é uma struct com uma gm::Position ({x, y}) e uma sf::Color
                drawStone({{stone->get_x()-1, stone->get_y()-1}, color}, renderer);
        }
    }

    // std::cout << "TESTE 1\n";
    auto critical = demo.getCriticalPlaces();
    // std::cout << "TESTE 2\n";
    for (auto place : critical) {
        sf::Color color = GMTraits::RED_COLOR;
        drawStone({{place.first-1, place.second-1}, color}, renderer);
    }
    // std::cout << "TESTE 3\n";


}
