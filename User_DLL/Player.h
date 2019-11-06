#pragma once
#ifndef USER_Player_H
#define USER_Player_H
#include <string>

class Player final {
    public:
        Player();
        ~Player();
        
        static int          stack_in_bb(std::string const& bet_or_balance_symbol, std::string const& position, int chair_number);
        static int          hero_eff_stack(const std::vector<int>& stacks_vector, int allin_situation);
        static void         log_eff_stack(int stack_size);
        static int          chair_bit_number(std::string const& position);
        static int          hero_position();
        static std::string  hero_combo();

    private:
        static int          eff_stack_for_first_to_act(const std::vector<int>& stacks_vector, int player_position);
        static int          eff_stack_for_one_allin(const std::vector<int>& stacks_vector, int player_position, int one_allin_position);
        static int          eff_stack_for_two_allins(const std::vector<int>& stacks_vector, int player_position, int two_allin_positions);
        static int          eff_stack_for_three_allins(const std::vector<int>& stacks_vector, int player_position, int three_allin_positions);

};

Player _player;

#include "Player.inl"

#endif
