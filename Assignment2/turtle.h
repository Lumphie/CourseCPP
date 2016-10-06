#ifndef TURTLE_H
#define TURTLE_H

#include <vector>
#include <fstream>

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
    vector<State> m_StateGoLeftMemory;
    vector<State> m_PointMemory;
    std::string m_Sequence;


public:
    Turtle();

    void Reset();
    void moveForward(double steps);
    void turn(double orientation);
    State getLocation();
    std::string readSeqFromFile(ifstream &inputFile);
    void followSeq(std::string const &sequence);



};


#endif // TURTLE_H
