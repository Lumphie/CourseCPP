#ifndef TURTLE_H
#define TURTLE_H

#include <vector>
#include <fstream>

using namespace std;

struct Point                                    // Struct not only holds X,Y and alpha
{
    ///RJCB: added default constructor with proper initialization using an initializer list
    Point (
      const double x = 0.0,
      const double y = 0.0,
      const double any_orientation = 0.0,
      const double any_stepSize = 0.0
    )
      : xCoord{x},
        yCoord{y},
        orientation{any_orientation},
        stepSize{any_stepSize}
    {
    }
    //~RJCB
    double xCoord = 0;
    double yCoord = 0;
    double orientation = 0;
    double stepSize = 1;                        // but also the stepSize it has to take.
};


class Turtle
{

private:
    Point m_Point;
    vector<Point> m_PointGoLeftMemory;
    vector<Point> m_PointMemory;
    std::string m_Sequence;

public:
    Turtle();                                   // When creating a turtle use default constructor

    void Reset();
    void moveForward(double steps);
    void turn(double orientation);
    Point getLocation();
    std::string readSeqFromFile(ifstream &inputFile);
    void followSeq(std::string const &sequence);
    vector<Point> getPoints();



};


#endif // TURTLE_H
