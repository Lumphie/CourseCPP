#include "turtle.h"
#include <vector>
#include <cmath>

using namespace std;

double TWOPI = 6.28318530718;

Turtle::Turtle();

{
    Reset();
}

void Turtle::Reset()
{
    m_Location.xCoord = 0;
    m_Location.yCoord = 0;
    m_orientation = 0;
    m_locationMemory;

}

void Turtle::moveForward(double steps = 1)
{
    m_Location.xCoord += steps * cos(TWOPI * (m_orientation / 360));
    m_Location.yCoord += steps * sin(TWOPI * (m_orientation / 360));

}

void Turtle::turn(double orientation)
{
    m_orientation += orientation;
}
