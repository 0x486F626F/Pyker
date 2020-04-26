#ifndef PYKER_CLI_CONTROLLER_H
#define PYKER_CLI_CONTROLLER_H


#include "player_controller.h"


/// Lets a human type in their betting actions.
class CliController : PlayerController {
public:
    int bet(std::vector<Card> hand, BetRange bet_range) override;
};


#endif //PYKER_CLI_CONTROLLER_H
