#include "gomoku/Demo.hpp"
#include <cstdio>

void Demo::onUpdateRenderer(Renderer& window) {
    graphics.update(*this, window);
} 

Demo::Transition Demo::onProcessInput(Input& input) {
           // std::cout << "[CALL] Demo::onProcessInput" << std::endl;
        auto move = players[currentPlayer].processInput(_board, input);
        // Jogadas fora do tabuleiro são respondidas com -1, -1
        if (move != gm::Position{-1, -1}) {
            // Verifica se a posição está desocupada
            if (_board.board[move.row+1][move.column+1].put_piece(currentPlayer+1)){ 
                _board.played_places.push_back(&_board.board[move.row+1][move.column+1]); //MAS QUE NOJO
                int winner = check_game_won(move.row+1, move.column+1);
                if(winner>-1){
                    std::cout << "!!!!!!!! PLAYER "<< winner << " WIN !!!!!!!!\n";
                    return {Transition::Type::CLOSE, this};
                }
                currentPlayer = 1 - currentPlayer;
                critical_places = find_four(move.row+1, move.column+1);
            }
        }
        return {Transition::Type::SELF, this};
}

void Demo::generate_places() {
    for (unsigned i = 1; i < Traits<Type::GOMOKU>::BOARD_DIMENSION+1; ++i) {
        _board.board[i].reserve(Traits<Type::GOMOKU>::BOARD_DIMENSION+2);
        _board.board[i][0] = Place(-1, i, 0); // Wall O 
        
        for (unsigned j = 1; j < Traits<Type::GOMOKU>::BOARD_DIMENSION+1; ++j) {
            _board.board[i][j] = Place(0, i, j); //playable places
        }
        _board.board[i][Traits<Type::GOMOKU>::BOARD_DIMENSION+1] = Place(-1, i, Traits<Type::GOMOKU>::BOARD_DIMENSION+1); // Wall L
    }
    
    _board.board[0].reserve(Traits<Type::GOMOKU>::BOARD_DIMENSION+2);
    _board.board[Traits<Type::GOMOKU>::BOARD_DIMENSION+1].reserve(Traits<Type::GOMOKU>::BOARD_DIMENSION+2);
    
    for (unsigned i = 1; i < Traits<Type::GOMOKU>::BOARD_DIMENSION+1; ++i) {
        _board.board[0][i] = Place(-1, 0, i); // Wall N
        _board.board[Traits<Type::GOMOKU>::BOARD_DIMENSION+1][i] = Place(-1, Traits<Type::GOMOKU>::BOARD_DIMENSION+1, i); // Wall S
    }

    _board.board[0][0] = Place(-1, 0, 0); // Wall NO
    _board.board[0][Traits<Type::GOMOKU>::BOARD_DIMENSION+1] = Place(-1, 0, Traits<Type::GOMOKU>::BOARD_DIMENSION+1); // Wall NL
    _board.board[Traits<Type::GOMOKU>::BOARD_DIMENSION+1][0] = Place(-1, Traits<Type::GOMOKU>::BOARD_DIMENSION+1, 0); // Wall SO
    _board.board[Traits<Type::GOMOKU>::BOARD_DIMENSION+1][Traits<Type::GOMOKU>::BOARD_DIMENSION+1] = Place(-1, Traits<Type::GOMOKU>::BOARD_DIMENSION+1, Traits<Type::GOMOKU>::BOARD_DIMENSION+1); // Wall SL
}

void Demo::generate_neighbors() {
    for (unsigned i = 1; i < 16; ++i) {
        for (unsigned j = 1; j < 16; ++j) {
            _board.board[i][j].generate_neighborhood({&_board.board[i-1][j-1], &_board.board[i-1][j], &_board.board[i-1][j+1], 
                                       &_board.board[i][j-1],                    &_board.board[i][j+1],
                                       &_board.board[i+1][j-1], &_board.board[i+1][j], &_board.board[i+1][j+1]
                                      });
        }
    }
}
    
int Demo::check_game_won(int row, int column) {
    
    Place last_place;
    last_place = _board.board[row][column];
    
    //pieces_in_row: -1 is done because find_piar() count two times the piece in last_place;
    for(int i = 0; i <4; ++i){
        int pieces_in_row = last_place.find_piar(i, 5)+last_place.find_piar(7-i, 5)-1; // i e 7-i iteram sobre um sentido e seu oposto, respectivamente
        if(pieces_in_row >= 5)
            return currentPlayer;
    }    
    return -1;
}

std::vector<std::pair<int, int> > Demo::find_four(int row, int column) {
    Place last_place;
    last_place = _board.board[row][column];
    
    std::vector<std::pair<int, int> > critical_locations, temp_crit;
    for(int i = 0; i <4; ++i){// i e 7-i iteram sobre um sentido e seu oposto, respectivamente
        int tolerance = 1;
        //Busca em um sentido <-. ->
        int pieces_in_row = last_place.wes_find_piar(i, 5, tolerance, temp_crit)-1;
        pieces_in_row += last_place.wes_find_piar(7-i, 5 - pieces_in_row, tolerance, temp_crit);
    
        if(pieces_in_row > 3) {
            critical_locations.insert(critical_locations.begin(), temp_crit.begin(), temp_crit.end());
        }
        temp_crit.clear();
        
        //Busca em no sentido oposto ->, <-
        tolerance = 1;
        pieces_in_row = last_place.wes_find_piar(7-i, 5, tolerance, temp_crit)-1;
        pieces_in_row += last_place.wes_find_piar(i, 5 - pieces_in_row, tolerance, temp_crit);
        if(pieces_in_row > 3) {
            critical_locations.insert(critical_locations.begin(), temp_crit.begin(), temp_crit.end());
        }
        temp_crit.clear();
    }
    //A busca é feita nos dois sentidos pois podem existir peças identificadas e que não fazem parte da quadrupla

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
