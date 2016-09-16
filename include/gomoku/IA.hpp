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
    gm::Position doUpdate(go::Board& board, gm::GameInput& input) override {
                Place *place = minimax_base(4, 1, board);
                // printf("============COMECANDO MINIMAX================");
                return {place->get_x()-1, place->get_y()-1};
    }
    
    
    Place* minimax_base(int level, int player, go::Board& board){
        Place* best_place;
        long long int alpha = std::numeric_limits<long long int>::lowest();
        long long int beta = std::numeric_limits<long long int>::max();
        long long int best_score = std::numeric_limits<long long int>::lowest();
       
        
        std::set<Place*> playable_places = generate_positions(board);
        	// printf("MAX %d - Começo da iteração\n", level);
        printf("Consideradas a partir da raíz... \n");
        for (auto current_place : playable_places){ //para cada lugar possível
            
            // long long int path_score = 0;
            current_place->put_piece(2); //simula a jogada para Player 2  _ ATENCAO: 2 pq IA esta fixado como Player 2
            board.played_places.push_back(current_place);
            // printf("Begin lvl 0 Score\n");
            // current_place->test_me();
            long long int child_score = current_place->score(level);//calculado antecipadamente, para computar score do Path
            // current_place->test_me();
            // printf("End lvl 0 Scrore\n");
            // printf("\tMAX %d - Esse lugar (%d, %d) vale:%d \n", level, current_place->get_x()-1, current_place->get_y()-1, child_score);
            long long int final_score = child_score;
            // if( !current_place->check_game_won() ) {
            if( !current_place->check_game_won() ) {
               final_score = minimax(level - 1, 1 - player, final_score, board, alpha, beta);
            }
            // printf("End lvl 0 minimax %d\n", level);
            // printf("\n\nbest: %lld\n", best_score);
            // printf("\tMAX - A jogada (%d, %d) vale:%lld \n", current_place->get_x(), current_place->get_y() ,  final_score);
            // printf("best: %lld\n", best_score);
            current_place->put_piece(0); //limpa a jogada após o retorno da recursão;
            board.played_places.pop_back();

            // printf("Jogada encontrada\n");
            // current_place->test_me();
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
                // printf("== PODA ==\n");
                break; 
            }
            printf(", %lld", best_score);
            // break;
        }
        // best_place->test_me();
        // printf("Escolhi.\n");
        printf("\n\n\tESCOLHIDA (%d, %d)\n with the value of %lld\n\n",best_place->get_x()-1, best_place->get_y()-1, best_score);

        return best_place;
    }
    
    //pensar em como usar o vetor de posições críticas pra tetar acelerar a busca.
    long long int minimax(int level, int player, long long int path_score, go::Board& board, long long int alpha, long long int beta){
        //A verificação de level == 0 não faz sentido, pois calculamos os scores dos filhos nos pais para computar o path. Pode trocar uma chamada recursiva por if no pai 
        // printf("BEGIN %lld\n", level); //deve chamar o método para verificar 5 em linha

        if (!level) return path_score;
        
        long long int best_score = 0;
    // std::set<Place*> evaluated_positions;

    // long long int score(go::Board& b){
    //     long long int score = 0;
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
        std::set<Place*> playable_places = generate_positions(board);
        if(player == 1) { //MAX - mudar lógica para verificar se Player é CPU
            // printf("(in max step)\n");
            best_score = std::numeric_limits<long long int>::lowest();
            for (auto current_place : playable_places){
            	// printf("MAX %lld - Começo da iteração MAX\n", level);
                board.played_places.push_back(current_place);
                current_place->put_piece(2); //simula a jogada para Player 2  _ ATENCAO: 2 pq IA esta fixado como Player 2
                // printf("Begin\n");
                long long int child_score = current_place->score(level); //calculado antecipadamente, para computar score do Path
                // printf("End\n");
                // printf("\tMAX %lld - Esse lugar (%lld, %lld) vale:%lld \n", level, current_place->get_x()-1, current_place->get_y()-1, child_score);
                long long int final_score = path_score + child_score;
                // if( current_place->check_game_won() || level -1 == 0) {
                //     final_score = score(board);
                // }
                // else {
                if (!current_place->check_game_won()) {
                    final_score = minimax(level - 1, 1 - player, final_score, board, alpha, beta);
                }
                // printf("\tMAX %lld - A jogada vale:%lld \n", level, final_score);
                current_place->put_piece(0); //limpa a jogada após o retorno da recursão;
                board.played_places.pop_back();
                // if (final_score > alpha)
                //     alpha = final_score;
                if (final_score > best_score) {
                    best_score = final_score;
                }
                if (alpha >= beta)
                    break;
            }
            // printf("\t\t\tMax found score: %lld\n", best_score);
        } else {//MIN
            best_score = std::numeric_limits<long long int>::max();
            for (auto current_place : playable_places){
            	// printf("MIN %lld - Começo da iteração MIN\n", level);
                board.played_places.push_back(current_place);
                current_place->put_piece(1); //simula a jogada para Player 1 _ ATENCAO: 1 pq Humano esta fixado como Player 1
                // printf("Begin\n");
                long long int child_score = current_place->score(level);//calculado antecipadamente, para computar score do Path
                // printf("End\n");
                // printf("\tMIN %lld - Esse lugar (%lld, %lld) vale:%lld \n", level, current_place->get_x()-1, current_place->get_y()-1, child_score);
                long long int final_score = path_score - child_score;
                if( !current_place->check_game_won() )
                    final_score = minimax(level - 1, 1 - player, final_score, board, alpha, beta);
                // long long int final_score = 0;
                // if( current_place->check_game_won() || level -1 == 0) {
                    // final_score = score(board);
                // }
                // else {
                    // final_score = minimax(level - 1, 1 - player, 0, board, alpha, beta);
                // }
            // if( !current_place->check_game_won() ) {
                // printf("\tMIN %lld - A jogada vale:%lld \n", level, final_score);
                current_place->put_piece(0); //limpa a jogada após o retorno da recursão;
                board.played_places.pop_back();
                if (final_score < beta)
                    beta = final_score;
                if (final_score < best_score)
                    best_score = final_score;
                if (alpha >= beta)
                    break;
            }
            // printf("\t\t\tMin found score: %lld\n", best_score);
           
        }  
        // printf("END %lld\n", level);
        return best_score;
    }
    
    std::set<Place*> generate_positions(go::Board& b){
        auto last_played = b.played_places;
        if(last_played.empty()){
            return {&b.board[8][8]};
        }
        // std::<Place*> p_places; 
        std::set<Place*> playable_places; 
        //alternativa com 2 últimas jogadas
        // auto back = last_played.rbegin();
        // p_places.push_back((Place*) *back);
        // back++;
        // if (back != last_played.rend()) 
        //     p_places.push_back((Place*) *back);

        //alternativa com todas       


        // //Lista jogadas já feitas
        // std::vector<Place*> played_places;

        // //faz jogadas até 2 de distancia
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
        return playable_places;
    }


    // std::set<Place*> evaluated_positions;

    // long long int score(go::Board& b){
    //     long long int score = 0;
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