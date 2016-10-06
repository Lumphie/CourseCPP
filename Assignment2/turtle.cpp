#include "turtle.h"
#include <vector>
#include <cmath>
#include <cassert>
#include <iostream>
#include <fstream>

using namespace std;

double TWOPI = 2 * 3.14159265358979;


Turtle::Turtle()

{
    Reset();
}

void Turtle::Reset()
{
    m_State.xCoord = 0;
    m_State.yCoord = 0;
    m_State.orientation = 0;
    m_StateGoLeftMemory.clear();

}

void Turtle::moveForward(double steps = 1.0)
{
    m_State.xCoord += steps * cos(TWOPI * (m_State.orientation / 360));
    m_State.yCoord += steps * sin(TWOPI * (m_State.orientation / 360));

    m_State.xCoord = round(m_State.xCoord * 100.0) / 100.0;
    if (m_State.xCoord == -0.0)
    {
        m_State.xCoord = 0.0;
    }

    m_State.yCoord = round(m_State.yCoord * 100.0) / 100.0;
    if (m_State.yCoord == -0.0)
    {
        m_State.yCoord = 0.0;
    }


}

void Turtle::turn(double orientation)
{
    m_State.orientation += orientation;
    while (m_State.orientation >= 360) {m_State.orientation -= 360;}
    while (m_State.orientation <= -360) {m_State.orientation += 360;}
}

State Turtle::getLocation()
{
    return m_State;
}

std::string Turtle::readSeqFromFile(ifstream &inputFile)
{

    int seqNum{0};
    int length{0};
    std::string sequence;
    inputFile >> seqNum >> length >> sequence;

    return sequence;
}

//Get a string of 'A's 'B's 'L's and 'R's and follow it's command.
// 'A' or 'B': move forward
// 'L': Save state to memory then turn left 40 degrees
// 'R': Return to last saved state (after an L), return to that state and turn right.
void Turtle::followSeq(std::string const &sequence)
{

    for (const char c: sequence)
    {
        m_PointMemory.push_back(m_State);

        switch(c)
        {
        case 'A': ;                                 // Fallthrough intentional A == B
        case 'B': this->moveForward(1);
            std::cout << "Moved forward" << '\n';
            break;
        case 'L':
            m_StateGoLeftMemory.push_back(m_State); // Save this state to go back to it when 'R'
            this->turn(40);
            std::cout << "Saved state to memory, turned Left" << '\n';
            break;
        case 'R':
            m_State = m_StateGoLeftMemory.back();   // Go back to the last state on vector
            m_StateGoLeftMemory.pop_back();         // Remove used state from vector
            this->turn(-40);
            std::cout << "Went back to last saved state," <<
                         "Removed state from memory , turned Right" << '\n';
            break;
        default:
            assert(!"Did not get a correct command input: 'Missing ABLR' ");
        }

    }
}

vector<State> Turtle::getPoints()
{
    return m_PointMemory;
}
