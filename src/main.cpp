#include "game/session.h"
#include "player/player_controller/cli_controller.h"


int main() {
    // initialize players
    std::vector<Player> players = {
        Player(new CliController, 0),
        Player(new CliController, 1),
        Player(new CliController, 2),
    };

    // start playing
    Session session = Session(players, 200, 20, 10);
    session.start();
}
