#include "stdafx.h"
#include <iostream>
#include <vector>
#include "turtle.h"
#include <fstream>
#include <cassert>
#include <string>

using namespace std;

void updateSeq(string &oldSeq, int numOfSequences);
void exportSeq(std::string seq, int seqNumber, ofstream &outputFile);

// When creating the graph in Excel make sure decimal point is set to '.' Or Excel won't recognize the output as numbers.

int main()
{
    const int numOfSequences = 7;
    std::string arterialSeq {"A"};
    updateSeq(arterialSeq, numOfSequences);                         // Step 1, create the sequence Si
    cout << numOfSequences << " sequences created.\n\n";
    ifstream myInputFileStream("sequence.csv");
    cout << "Created file arterialtree.csv\n\n";
    ofstream myOutputFileStream("arterialtree.csv");

    for (int seqNum = 0; seqNum < numOfSequences; ++seqNum)
    {                                                               // Step 2:
        Turtle turtle;                                              // Create a Turtle called turtle
        turtle.followSeq(turtle.readSeqFromFile(myInputFileStream));// Let it follow the sequence
        vector<Point> allPoints = turtle.getPoints();               // in sequence.csv and
                                                                    // put all the points in a vector.
        int index = 0;
        for (const Point point: allPoints)
        {
            if (seqNum == 6)
            {

                myOutputFileStream << index + 1                     // Write the coordinates from
                                   << " " << point.xCoord           // all the start and end points
                                   << " " << point.yCoord           // to file arterialtree.csv
                                   << '\n';

                if (index % 2)                                      // after 2 points leave a blank line
                    myOutputFileStream << '\n';
            }
            ++index;

        }
        turtle.Reset();
        assert(turtle.getLocation().xCoord == 0.0);
        assert(turtle.getLocation().yCoord == 0.0);
    }
    cout << "Succesfully exported all coordinates to arterialtree.csv\n";


    return 0;
}

// Update the sequence a number of times according to the production rule:
// Replace every 'A' with "BLARA".
void updateSeq(std::string &oldSeq, int numOfSequences)
{
    std::string tempSeq;                                            // temporary string just like assignment 1
    // Open a file with filename sequence.csv
    ofstream myOutputFileStream("sequence.csv");
    cout << "\nCreated file sequence.csv\n\n";

    for (int index = 0; index < numOfSequences; ++index)
    {   
        // Look through all chars, whenever there is an 'A', add "BLARA" to temporary sequence.
        for (int i = 0; i < static_cast<int>(oldSeq.size()); ++i)
        {
            if (oldSeq[i] == 'A')
                tempSeq += "BLARA";
            else
                tempSeq += oldSeq[i];

        }

        // Copy the temporary string to the old string for new use.
        oldSeq = tempSeq;
        tempSeq.clear();
        assert(oldSeq != "");
        assert(tempSeq == "");
        exportSeq(oldSeq, index, myOutputFileStream);

    }

}

//Exports sequence number, size and sequence to outputFile
void exportSeq(std::string seq, int seqNumber, ofstream &outputFile)
{
    assert(seq != "");
    outputFile << seqNumber + 1 << " " << static_cast<int>(seq.size()) << " ";
    for(int i = 0; i < static_cast<int>(seq.size()); ++i)
    {
        outputFile << seq[i];
    }

    outputFile << '\n';
}
