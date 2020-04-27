#ifndef PYKER_CLI_CONTROLLER_H
#define PYKER_CLI_CONTROLLER_H


#include "player_controller.h"


/// Lets a human type in their betting actions.
class CliController : public PlayerController {
public:
    int bet(t_card hand, const PublicState& public_state, BetRange bet_range) override;
};


#endif //PYKER_CLI_CONTROLLER_H
