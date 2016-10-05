#ifndef TURTLE_H
#define TURTLE_H

#include <vector>
using namespace std;

struct State
{
    double xCoord = 0;
    double yCoord = 0;
    double orientation = 0;
};

class Turtle
{

private:
    State m_State;
    vector<State> m_StateMemory;

public:
    Turtle();

    void Reset();
    void moveForward(double steps);
    void turn(double orientation);
    State getLocation();
    void addStateToMem(State currentState);
    void removeStateFromMem();


};


#endif // TURTLE_H
