#include <iostream>
#include <vector>
#include "turtle.h"
#include <fstream>
#include <cassert>

using namespace std;

void updateSeq(string &oldSeq, int numOfSequences);
void printSeq(std::string seq, int seqNumber, ofstream &outputFile);


int main()
{

    const int numOfSequences = 7;
    std::string arterialSeq {"A"};

    updateSeq(arterialSeq, numOfSequences);

    ifstream myInputFileStream("sequence.csv");

    ofstream myOutputFileStream("arterialtree.csv");

    for (int seqNum = 0; seqNum < numOfSequences; ++seqNum)
    {
        cout << "Sequence " << seqNum << ".\n";
        Turtle turtle;
        turtle.followSeq(turtle.readSeqFromFile(myInputFileStream));
        //vector<State> finalPoints = turtle.getPoints();
        //cout << finalPoints. << '\n';
        vector<State> allPoints = turtle.getPoints();

        int index = 0;
        for (const State state: allPoints)
        {
            cout << index + 1 << " X: " << state.xCoord << " Y: " << state.yCoord << '\n';
            if (index % 2)
                cout << '\n';

            if (seqNum == 6)
            {

                myOutputFileStream << index + 1 << " " << state.xCoord << " " << state.yCoord << '\n';
                if (index % 2)
                    myOutputFileStream << '\n';
            }
            ++index;

        }
        turtle.Reset();
    }


    return 0;
}

// update the sequence a number of times according to the production rule:
// Replace every 'A' with "BLARA".
void updateSeq(std::string &oldSeq, int numOfSequences)
{
    std::string tempSeq;
    // Open a file with filename sequence.csv
    ofstream myOutputFileStream("sequence.csv");

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

        // Copy the temporary vector to the old vector for new use.
        oldSeq = tempSeq;
        tempSeq.clear();
        assert(oldSeq != "");
        assert(tempSeq == "");
        printSeq(oldSeq, index, myOutputFileStream);

    }

}

//Prints a sequence, its sequence number and its size.
//Exports sequence number, size and sequence to outputFile
void printSeq(std::string seq, int seqNumber, ofstream &outputFile)
{
    assert(seq != "");
    outputFile << seqNumber + 1 << " " << static_cast<int>(seq.size()) << " ";
    for(int i = 0; i < static_cast<int>(seq.size()); ++i)
    {
        cout << seq[i];
        outputFile << seq[i];
    }

    cout << "\ni: " << seqNumber + 1 << " size: " << static_cast<int>(seq.size()) << "\n\n";
    outputFile << '\n';
}
