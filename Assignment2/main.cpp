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

    const int numOfSequences = 6;
    std::string arterialSeq {"A"};

    updateSeq(arterialSeq, numOfSequences);

    ifstream myInputFileStream("sequence.csv");

    cout << "Sequence 1:\n";
    Turtle turtle1;
    turtle1.followSeq(turtle1.readSeqFromFile(myInputFileStream));

    cout << "\n\nSequence 2:\n";
    Turtle turtle2;
    turtle2.followSeq(turtle2.readSeqFromFile(myInputFileStream));

    cout << "\n\nSequence 3:\n";
    Turtle turtle3;
    turtle3.followSeq(turtle3.readSeqFromFile(myInputFileStream));

    cout << "\n\nSequence 4:\n";
    Turtle turtle4;
    turtle4.followSeq(turtle4.readSeqFromFile(myInputFileStream));

    cout << "\n\nSequence 5:\n";
    Turtle turtle5;
    turtle5.followSeq(turtle5.readSeqFromFile(myInputFileStream));

    cout << "\n\nSequence 6:\n";
    Turtle turtle6;
    turtle6.followSeq(turtle6.readSeqFromFile(myInputFileStream));



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
