#include "game/session.h"
#include "player/player_controller/cli_controller.h"


int main() {
    // initialize players
    std::vector<CliController> controllers = { CliController(), CliController(), CliController() };
    std::vector<Player> players = {
        Player(&controllers[0], 0),
        Player(&controllers[1], 1),
        Player(&controllers[2], 2),
    };

    // start playing
    Session session = Session(players, 200, 20, 10);
    session.start();
}
