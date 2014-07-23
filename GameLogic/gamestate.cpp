#include "gamestate.h"


int GameState::points = 0;

void GameState::addPoints(int pointsTobeAdded){
    GameState::points=pointsTobeAdded;
}


