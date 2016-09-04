#include "CommonTypes.hpp"
#include "base/Player.hpp"

#ifndef GOMOKU_IA_HPP
#define GOMOKU_IA_HPP
// Skynet é nossa IA, ela herda de um componente definido em multimedia/Gomoku.hpp,
// o PlayerHandler. O parâmetro de template dessa classe define o tipo de objeto que
// recebe além do gm::GameInput. Uma observação importante é que PlayerHandler não
// é abstrato, você pode fazer um 'new PlayerHandler<Board>' e isto criará um handler
// para jogador humano.
//                                      ISTO
//                                        v
class IA : public gm::PlayerHandler<go::Board> {
 private:
// O método doUpdate é chamado sempre que chamado handler.update()
//                     DEFINE ISTO
//                          v
    gm::Position doUpdate(go::Board& board, gm::GameInput& input) override {
       // std::cout << "[CALL] Skynet::doUpdate" << std::endl;
        // Percorre o tabuleiro em busca de uma casa vazia, assim que a encontra
        // joga.
        //for (auto row : board) {
        for (unsigned i = 1; i < Traits<Type::GOMOKU>::BOARD_DIMENSION+1; ++i) {
            for (unsigned j = 1; j < Traits<Type::GOMOKU>::BOARD_DIMENSION+1; ++j) {
                Place place = board[i][j];
                if (place.get_owner() == 0) {
                    return {place.get_x()-1, place.get_y()-1};
                }
            }
        }
        return {-1, -1};
    }
};

#endif