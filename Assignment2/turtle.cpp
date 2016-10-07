#include "turtle.h"
#include <vector>
#include <cmath>
#include <cassert>
#include <iostream>
#include <fstream>

using namespace std;

double TWOPI = 2 * 3.14159265358979;

Turtle::Turtle() {Reset();}                                 // Default turtle is automatically reset

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
    if (m_Point.xCoord == -0.0)                                         // Rounding and removing
        m_Point.xCoord = 0.0;                                           // the negative sign of zeroes


    m_Point.yCoord = round(m_Point.yCoord * 10000.0) / 10000.0;
    if (m_Point.yCoord == -0.0)
        m_Point.yCoord = 0.0;


}

void Turtle::turn(double orientation)
{
    m_Point.orientation += orientation;
    while (m_Point.orientation >= 360) {m_Point.orientation -= 360;}    // prevent orientation from
    while (m_Point.orientation <= -360) {m_Point.orientation += 360;}   // becoming too big or too small
}                                                                       // not needed since sin and cos
                                                                        // should work the same above 360

Point Turtle::getLocation() {return m_Point;}

std::string Turtle::readSeqFromFile(ifstream &inputFile)                // puts sequence into memory
{   assert(inputFile.peek() != ifstream::traits_type::eof());           // Checks if inputFile is not empty
    int seqNum{0};                                                      // (found this snippet of code on http://stackoverflow.com/questions/2390912/checking-for-an-empty-file-in-c)
    int length{0};                                                      // inputFile should be:
    std::string sequence;                                               // S lenght seqString
    inputFile >> seqNum >> length >> sequence;                          // removes S and length

    return sequence;
}

//Get a string of 'A's 'B's 'L's and 'R's and follow it's command.
// 'A' or 'B': move forward
// 'L': Save Point to memory then turn left 40 degrees
// 'R': Return to last saved Point (after an L), return to that Point and turn right.
void Turtle::followSeq(std::string const &sequence)
{
    m_PointMemory.push_back(m_Point);               // Save first point (0,0) to memory.

    for (const char c: sequence)
    {
        switch(c)
        {
        case 'A': ;                                 // Fallthrough intentional A == B
        case 'B':
            this->moveForward(m_Point.stepSize);
            break;

        case 'L':
            m_Point.stepSize *= 0.8;                // Decrease stepSize <-- turtle's branching
            m_PointGoLeftMemory.push_back(m_Point); // Save this Point to go back to it when 'R'
            this->turn(40);
            break;

        case 'R':
            m_Point = m_PointGoLeftMemory.back();   // Go back to the last Point on vector
            m_PointGoLeftMemory.pop_back();         // Remove used Point from vector
            this->turn(-40);
            break;

        default:
            assert(!"Did not get a correct command input: 'Missing ABLR' ");
        }

        m_PointMemory.push_back(m_Point);           // Save point in memory
    }
}

vector<Point> Turtle::getPoints() {return m_PointMemory;}
