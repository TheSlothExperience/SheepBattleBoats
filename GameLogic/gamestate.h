#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState
{
private:
    GameState();

public:
     static int points;
    static void addPoints(int pointsTobeAdded);
//    static int getPointCount(){return points;}
};


#endif // GAMESTATE_H
