#ifndef TURTLE_H
#define TURTLE_H

#include <vector>
using namespace std;

struct Location
{
    double xCoord = 0;
    double yCoord = 0;
};

class Turtle
{

private:
    Location m_Location;
    double m_orientation;
    vector<Location> m_locationMemory;

public:
    Turtle();

    void Reset();
    void moveForward(double steps);
    void turn(double orientation);

};


#endif // TURTLE_H
