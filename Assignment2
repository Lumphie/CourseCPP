#include <iostream>
#include <vector>

using namespace std;

void updateSeq(vector<char> &oldSeq, int numOfSequences);
void printSeq(vector<char> seq, int seqNumber);


int main()
{
    const int numOfSequences = 6;
    vector<char> arterialSeq {'A'};

    updateSeq(arterialSeq, numOfSequences);
    printSeq(arterialSeq, numOfSequences);

    return 0;
}

// update the sequence a number of times according to the production rule:
// Replace every 'A' with "BLARA".
void updateSeq(vector<char> &oldSeq, int numOfSequences)
{
    vector<char> tempSeq;

    for (int i = 0; i < numOfSequences; ++i)
    {
        // Look through all chars, whenever there is an 'A', add "BLARA" to temporary sequence.
        for (int i = 0; i < static_cast<int>(oldSeq.size()); ++i)
        {
            if (oldSeq[i] == 'A')
            {
                tempSeq.push_back('B');
                tempSeq.push_back('L');
                tempSeq.push_back('A');
                tempSeq.push_back('R');
                tempSeq.push_back('A');
            }
            else
                tempSeq.push_back(oldSeq[i]);
        }

        oldSeq = tempSeq;
        tempSeq.clear();
    }

}

//Prints a sequence, its sequence number and its size.
void printSeq(vector<char> seq, int seqNumber)
{
    for(int i = 0; i < static_cast<int>(seq.size()); ++i)
    {
        cout << seq[i];
    }

    cout << "\ni: " << seqNumber << " size: " << static_cast<int>(seq.size()) << "\n\n";
}
