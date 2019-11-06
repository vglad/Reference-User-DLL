#pragma once
#ifndef USER_BOTLOGIC_H
#define USER_BOTLOGIC_H

class BotLogic final {
    public:
        BotLogic();
        ~BotLogic();

        static int      solution(int allin_situation);
        static int      fold();
        static int      allin();

    private:
        static bool     check_combo_for_allin(const std::vector<std::string>& gto_combo_allin_info, const std::vector<std::string>& exploit_combo_allin_info, int my_stack_size, int allin_situation, bool play_exploit);
        static double   combo_ev_correction(int my_stack_size, int allin_situation, bool play_exploit);

        static double   ev_correction_small(int my_stack_size);
        static double   ev_correction_big(int my_stack_size);

};

BotLogic _bot;

#include "BotLogic.inl"

#endif
