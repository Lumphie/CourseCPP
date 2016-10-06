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
    m_Point.xCoord = 0.0;
    m_Point.yCoord = 0.0;
    m_Point.orientation = 0.0;
    m_PointGoLeftMemory.clear();
    m_PointMemory.clear();
    m_Point.stepSize = 1.0;

}

void Turtle::moveForward(double steps = 1.0)
{
    m_Point.xCoord += steps * cos(TWOPI * (m_Point.orientation / 360));
    m_Point.yCoord += steps * sin(TWOPI * (m_Point.orientation / 360));

    m_Point.xCoord = round(m_Point.xCoord * 10000.0) / 10000.0;
    if (m_Point.xCoord == -0.0)
    {
        m_Point.xCoord = 0.0;
    }

    m_Point.yCoord = round(m_Point.yCoord * 10000.0) / 10000.0;
    if (m_Point.yCoord == -0.0)
    {
        m_Point.yCoord = 0.0;
    }


}

void Turtle::turn(double orientation)
{
    m_Point.orientation += orientation;
    while (m_Point.orientation >= 360) {m_Point.orientation -= 360;}
    while (m_Point.orientation <= -360) {m_Point.orientation += 360;}
}

Point Turtle::getLocation()
{
    return m_Point;
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
// 'L': Save Point to memory then turn left 40 degrees
// 'R': Return to last saved Point (after an L), return to that Point and turn right.
void Turtle::followSeq(std::string const &sequence)
{
    m_PointMemory.push_back(m_Point);

    for (const char c: sequence)
    {

        switch(c)
        {
        case 'A': ;                                 // Fallthrough intentional A == B
        case 'B':

            this->moveForward(m_Point.stepSize);
            std::cout << "Moved forward" << '\n';
            break;
        case 'L':
            m_Point.stepSize *= 0.8;
            m_PointGoLeftMemory.push_back(m_Point); // Save this Point to go back to it when 'R'
            this->turn(40);
            std::cout << "Saved Point to memory, turned Left" << '\n';
            break;
        case 'R':
            m_Point = m_PointGoLeftMemory.back();   // Go back to the last Point on vector
            m_PointGoLeftMemory.pop_back();         // Remove used Point from vector

            this->turn(-40);
            std::cout << "Went back to last saved Point, " <<
                         "Removed Point from memory, turned Right" << '\n';
            break;
        default:
            assert(!"Did not get a correct command input: 'Missing ABLR' ");
        }

        m_PointMemory.push_back(m_Point);

    }
}

vector<Point> Turtle::getPoints()
{
    return m_PointMemory;
}
