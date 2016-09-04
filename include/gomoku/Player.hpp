#include "CommonTypes.hpp"
#include "base/Player.hpp"
#ifndef GOMOKU_PLAYER_HPP
#define GOMOKU_PLAYER_HPP
// DemoPlayer é nosso Player, herdando de gm::Player, o qual requer dois templates:
// um objeto sobre o qual pode analisar sua jogada (Board) e a saída do processamento
// das entradas do jogo (gm::Position). Um jogador humano nunca precisa analisar o 
// objeto Board, enquanto um jogador artificial nunca precisa analisar as entradas. 
// Como não se sabe o que o handler é (IA ou humano), sempre são passados 
// os dois argumentos.
// 
//                                  ISTO(1)    ISTO(2) 
//                                     v         v
class Player : public gm::Player<go::Board, gm::Position> {
 public:
    // Esse using tem apenas a utilidade de diminuir a verbosidade das próximas
    // referências à gm::PlayerHandler<Board>
    using Handler = gm::PlayerHandler<go::Board>;

    // Construtor de Player recebe o handler, cujo default é ser humano
    Player(Handler * const handler = new Handler())
     : handler(handler) { }

 private:
    // É essencial que o PlayerHandler seja guardado como PONTEIRO, caso contrário,
    // sempre se comportará como humano, mesmo que seja inicializado com o Skynet.
    std::unique_ptr<Handler> handler;

// onProcessInput é chamado através de player.processInput(...) e é nele que é realizado
// o processamento das entradas do jogo. Nesse caso, como usamos um componente para fazer
// o serviço, basta chamar o update do mesmo.
// 
//  DEFINE ISTO(2)          DEFINE ISTO(1)
//        v                       v
    gm::Position onProcessInput(go::Board& board, gm::GameInput& input) {
//        std::cout << "[CALL] Player::onProcessInput" << std::endl;
        gm::Position position = handler->update(board, input);
        return position;
    }
};

#endif