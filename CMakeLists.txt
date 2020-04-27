cmake_minimum_required(VERSION 3.16)
project(pyker)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(pyker
        src/main.cpp

        src/cards/card.cpp
        src/cards/card.h
        src/cards/deck.cpp
        src/cards/deck.h
        src/cards/hand.cpp
        src/cards/hand.h

        src/game/game.cpp
        src/game/game.h
        src/game/session.cpp
        src/game/session.h
        src/game/public_state/log_item.h
        src/game/public_state/public_state.cpp
        src/game/public_state/public_state.h

        src/player/player.cpp
        src/player/player.h
        src/player/player_controller/baseline_bot.cpp
        src/player/player_controller/baseline_bot.h
        src/player/player_controller/cli_controller.cpp
        src/player/player_controller/cli_controller.h
        src/player/player_controller/player_controller.h
)
