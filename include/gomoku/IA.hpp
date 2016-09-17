#ifndef GOMOKU_IA_HPP
#define GOMOKU_IA_HPP

#include "CommonTypes.hpp"
#include "base/Player.hpp"
#include <limits>
#include <vector>
#include <set>

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
    int ia_number = -1;
    int game_lvl = 4;
    gm::Position doUpdate(go::Board& board, gm::GameInput& input) override {
                if(ia_number == -1) {
                    auto last_played = board.played_places;
                    bool empty = last_played.empty(); 
                    if(empty)
                        ia_number = 0;
                    else
                        ia_number = 1; 
                }

                Place *place = minimax_base(game_lvl, ia_number, board);
                // printf("============COMECANDO MINIMAX================");
                return {place->get_x()-1, place->get_y()-1};
    }
    
    
    Place* minimax_base(int level, int player, go::Board& board){
        Place* best_place;
        double alpha = -1*std::numeric_limits<double>::max();
        double beta = std::numeric_limits<double>::max();
        double best_score = -1*std::numeric_limits<double>::max();
       
        
        std::set<Place*> playable_places = generate_positions(board, ia_number);
        	// printf("MAX %d - Começo da iteração\n", level);
        // printf("Consideradas a partir da raíz... \n");
        for (auto current_place : playable_places){ //para cada lugar possível
            current_place->put_piece(ia_number+1); //simula a jogada para Player 2  _ ATENCAO: 2 pq IA esta fixado como Player 2
            board.played_places.push_back(current_place);
            double child_score = current_place->score(level, ia_number);//calculado antecipadamente, para computar score do Path
            // printf("\tMAX %d - Esse lugar (%d, %d) vale:%d \n", level, current_place->get_x()-1, current_place->get_y()-1, child_score);
            double final_score = child_score;
            if( !current_place->check_game_won() ) {
               final_score = minimax(level - 1, 1 - player, final_score, board, alpha, beta);
            }
            // printf("\tMAX - A jogada (%d, %d) vale:%lld \n", current_place->get_x(), current_place->get_y() ,  final_score);
            current_place->put_piece(0); //limpa a jogada após o retorno da recursão;
            board.played_places.pop_back();
            if (final_score > alpha){
                alpha = final_score;
                // printf("\tMAX %d - Substitui ALPHA\n", level);
            }
            if (final_score >= best_score) {
            	// printf("\tMAX %d - Essa jogada é melhor\n", level);
                best_place = current_place;
                // best_place->test_me();
                best_score = final_score;
            }
            if (alpha >= beta) {
                break; 
            }
            // printf(", %lld", best_score);
        }
        printf("%d:\t(%d, %d)\tscore: %f\n", ia_number, best_place->get_x()-1, best_place->get_y()-1, best_score);

        return best_place;
    }
    
    //pensar em como usar o vetor de posições críticas pra tetar acelerar a busca.
    double minimax(int level, int player, double path_score, go::Board& board, double alpha, double beta){
        if (!level) return path_score;
        
        double best_score = 0;

        std::set<Place*> playable_places = generate_positions(board, ia_number);
        if(player == ia_number) { //MAX - mudar lógica para verificar se Player é CPU
            // printf("(in max step)\n");
            best_score = -1*std::numeric_limits<double>::max();
            for (auto current_place : playable_places){
            	// printf("MAX %lld - Começo da iteração MAX\n", level);
                board.played_places.push_back(current_place);
                current_place->put_piece(player + 1); //simula a jogada para Player 2  _ ATENCAO: 2 pq IA esta fixado como Player 2
                double child_score = current_place->score(level, ia_number); //calculado antecipadamente, para computar score do Path
                // printf("\tMAX %lld - Esse lugar (%lld, %lld) vale:%lld \n", level, current_place->get_x()-1, current_place->get_y()-1, child_score);
                double final_score = path_score + child_score;
                if (!current_place->check_game_won()) {
                    final_score = minimax(level - 1, 1 - player, final_score, board, alpha, beta);
                }
                // printf("\tMAX %lld - A jogada vale:%lld \n", level, final_score);
                current_place->put_piece(0); //limpa a jogada após o retorno da recursão;
                board.played_places.pop_back();
                if (final_score > alpha)
                    alpha = final_score;
                if (final_score > best_score) {
                    best_score = final_score;
                }
                if (alpha >= beta){
                    break;
                }
            }
            // printf("\t\t\tMax found score: %lld\n", best_score);
        } else {//MIN
            best_score = std::numeric_limits<double>::max();
            for (auto current_place : playable_places){
            	// printf("MIN %lld - Começo da iteração MIN\n", level);
                board.played_places.push_back(current_place);
                current_place->put_piece(player + 1); //simula a jogada para Player 1 _ ATENCAO: 1 pq Humano esta fixado como Player 1
                double child_score = current_place->score(level, ia_number);//calculado antecipadamente, para computar score do Path
                // printf("\tMIN %lld - Esse lugar (%lld, %lld) vale:%lld \n", level, current_place->get_x()-1, current_place->get_y()-1, child_score);
                double final_score = path_score - child_score;
                if( !current_place->check_game_won() )
                    final_score = minimax(level - 1, 1 - player, final_score, board, alpha, beta);
                // printf("\tMIN %lld - A jogada vale:%lld \n", level, final_score);
                current_place->put_piece(0); //limpa a jogada após o retorno da recursão;
                board.played_places.pop_back();
                if (final_score < beta)
                    beta = final_score;
                if (final_score < best_score)
                    best_score = final_score;
                if (alpha >= beta){
                    break;
                }
            }
            // printf("\t\t\tMin found score: %lld\n", best_score);
           
        }  
        // printf("END %lld\n", level);
        return best_score;
    }
    
    std::set<Place*> generate_positions(go::Board& b, int function){
        auto last_played = b.played_places;
        if(last_played.empty()){
            return {&b.board[8][8]};
        }
        std::set<Place*> playable_places; 
        if(0){
            std::list<Place*> p_places; 
            //alternativa com 2 últimas jogadas
            auto back = last_played.rbegin();
            p_places.push_back((Place*) *back);
            back++;
            if (back != last_played.rend()) 
                p_places.push_back((Place*) *back);
    
            //alternativa com todas       
    
    
            // //Lista jogadas já feitas
            // std::vector<Place*> played_places;
    
            // //faz jogadas até 2 de distancia
            for(auto played : p_places){
            	for(int i = 0; i < 8; ++i){
            		Place* first = played->get_neighbor(i);
            		if(first->get_owner()==-1)//se first for wall, não tente prosseguir
            			continue;
            		Place* second = first->get_neighbor(i);
            		if(first->get_owner() == 0)
            			playable_places.insert(first);
            		if(second->get_owner() == 0)
            			playable_places.insert(second);
            	}
            }
        } else {
            for(auto played : last_played){
                for(int i = 0; i < 8; ++i){
                    Place* first = played->get_neighbor(i);
                    if(first->get_owner()==-1)//se first for wall, não tente prosseguir
                        continue;
                    Place* second = first->get_neighbor(i);
                    if(first->get_owner() == 0)
                        playable_places.insert(first);
                    if(second->get_owner() == 0)
                        playable_places.insert(second);
                }
            }
        }
        return playable_places;
    }


    // std::set<Place*> evaluated_positions;

    // double score(go::Board& b){
    //     double score = 0;
    //     evaluated_positions = {};
    //     for (auto piece : b.played_places) {
    //         if (!evaluated_positions.count(piece)) {
    //             if(piece->get_owner() == 2) {
    //                 score += piece->score();
    //             } else {
    //                 score -= piece->score()+1;
    //             }
    //         }
    //     }
    //     return score;
    // }


    
};

#endif