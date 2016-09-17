#include <iostream>
#include "mbe/Engine.hpp"
#include "multimedia/Gomoku.hpp"
#include "Traits.hpp"
#include "IA.hpp"
#include "Player.hpp"


#ifndef GOMOKU_DEMO_HPP
#define GOMOKU_DEMO_HPP
/** Aqui é mostrado como utilizar o tipo Component, mais especificamente,
 * aqueles definidos em multimedia/Gomoku.hpp. Com esses recursos, é possível
 * criar um jogo de Gomoku com suporte a IA.
 *
 * Um Component é uma abstração sobre as funcionalidades de uma dada classe.
 * Seu uso tem como objetivo desacoplar essas funcionalidades da classe principal.
**/

#define NW 0
#define N  1
#define NE 2
#define W  3
#define E  4
#define SW 5
#define S  6
#define SE 7

// Board é uma representação simples de um tabuleiro de Gomoku.
// Um vetor de vetores de unsigned int, onde 0 = player preto, 1 = player branco,
// e 2 = desocupado.
// Forward declaration necessária
class Demo;


class DemoGraphics : public gm::Graphics<Demo> {
 private:
    // Método que realiza o update propriamente dito
    void doUpdate(Demo&, gm::Renderer&) override;
};


// Nossa classe Demo, com pequenas mudanças. MyGame não era necessário, portanto foi
// removido, e agora a herança de Demo é direta de mbe::Game::State
class Demo : public mbe::Game::State {
 public:
    // O construtor recebe dois players que precisam ser movidos
    // devido ao uso de std::unique_ptr na classe DemoPlayers
    // O default inicializa os dois DemoPlayers com o default de DemoPlayer
    // (i.e., players humanos)
    Demo(Player&& p1 = Player(new IA()),
         Player&& p2 = Player(new IA())) 
    : players{std::move(p1), std::move(p2)} {
        _board.board.reserve(Traits<Type::GOMOKU>::BOARD_DIMENSION+2);
        generate_places(); 
        generate_neighbors();
    }
    
    // Um getter para simplificar as coisas, mas que poderia ser EVITADO
    std::list<Place*>& getPlayedPlaces() { return _board.played_places; }
    const std::vector<std::pair<int, int> > & getCriticalPlaces() const { return critical_places; }

    go::Board & get_board() { return _board; }
    

 private:
    // O componente gráfico
    DemoGraphics graphics;
    // O array de jogadores
    std::array<Player, 2> players;
    // Variável para guardar o jogador atual
    short currentPlayer = 0;
    // O tabuleiro
    go::Board _board;
    
    std::vector<std::pair<int, int> > critical_places;

    // Agora nosso onUpdateRenderer chama o update de graphics
    void onUpdateRenderer(Renderer& window) override;

    // Aqui, mandamos o input para player e recebemos uma gm::Position de
    // resposta. Ela representa a jogada do player.
    Transition onProcessInput(Input& input) override;
    
    
    
    void generate_places();

    void generate_neighbors();
    
    int check_game_won(int row, int column);

    std::vector<std::pair<int, int> > find_four(int row, int column);
    

};
#endif