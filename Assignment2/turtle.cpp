#include "turtle.h"
#include <vector>
#include <cmath>
#include <cassert>

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
    m_StateMemory.clear();

}

void Turtle::moveForward(double steps = 1.0)
{
    m_State.xCoord += steps * cos(TWOPI * (m_State.orientation / 360));
    m_State.yCoord += steps * sin(TWOPI * (m_State.orientation / 360));

    m_State.xCoord = round(m_State.xCoord * 100.0) / 100.0;
    if (m_State.xCoord == -0)
        m_State.xCoord = 0;
    m_State.yCoord = round(m_State.yCoord * 100.0) / 100.0;
    if (m_State.yCoord == -0)
        m_State.yCoord = 0;

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

void Turtle::addStateToMem(State currentState)
{
    m_StateMemory.push_back(currentState);
}

void Turtle::removeStateFromMem()
{
    m_StateMemory.pop_back();
}

