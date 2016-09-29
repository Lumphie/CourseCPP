#include <iostream>
#include <fstream>

using namespace std;

bool updateCell(bool leftState, bool ownState, bool rightState)
{
   /* Original function commented out. So you can see how I did it originally for the assignment.
    *
    * // if the left neighbor == 1 the next state will always be 0
    * // except when the cell and right neighbor == 0 then the next state will be 1
    *   if (leftState)
    *{
    *   if (!ownState && !rightState)
    *       return 1;
    *   else
    *       return 0;
    *}
    *   // if the left neighbor == 0 the next state will always be 1
    *   // except when the cell and right neighbor == 0 then the next state will be 0
    *   else
    *    {
    *   if (!ownState && !rightState)
    *       return 0;
    *   else
    *       return 1;
    *   }
    */


    // Three dimensional array with [leftNeighbor][self][rightNeihbor].
    // Array is initialized in [0][0][0], [0][0][1], [0][1][0], etc. order.
   bool table[2][2][2] { { {0, 1}, {1, 1} },
                         { {1, 0}, {0, 0} } };

   return table[leftState][ownState][rightState];

}

int main()
{
    // Create .csv file named data.
    std::ofstream myOutputFileStream("data.csv");

    // Initialize the number of cells.
    int numOfCells = 100;

    // Create 2 arrays, one with the current state of all the cells and 1 temporary to copy the next state from.
    // First cell gets value 1 the rest gets value 0.
    bool currentStateAutomaton[numOfCells] {1};
    bool nextStateAutomaton[numOfCells];

    // Loop a number of times through the process.
    for (int index = 0; index < ((3*numOfCells)/2); ++index)
    {
        // Loop through all cells.
        for (int index = 0; index < numOfCells; ++index)
        {
        // Left neighbor of cell 0 is cell the last cell.
            if (index == 0)
            {
                // Put next state in the temporary array.
                nextStateAutomaton[0] = updateCell(currentStateAutomaton[numOfCells - 1], currentStateAutomaton[0], currentStateAutomaton[1]);
            }
            // Right neighbor of the last cell is cell 0.
            else if (index == numOfCells -1)
            {
                // Put next state in the temporary array.
                nextStateAutomaton[numOfCells - 1] = updateCell(currentStateAutomaton[numOfCells - 2], currentStateAutomaton[numOfCells - 1], currentStateAutomaton[0]);
            }
            else
            {
                // Put next state in a temporary array.
                nextStateAutomaton[index] = updateCell(currentStateAutomaton[index - 1], currentStateAutomaton[index], currentStateAutomaton[index + 1]);
            }
        }


        // Use temporary array to update state of all cells at the same time.
        for (int index = 0; index < numOfCells; ++index)
            currentStateAutomaton[index] = nextStateAutomaton[index];


        // Print current array and output it to a .csv file.
        if (index > numOfCells / 2)
        {
            for (int index = 0; index < numOfCells; ++index)
            {
                cout << currentStateAutomaton[index];

                myOutputFileStream << currentStateAutomaton[index] << ' ';
            }

            myOutputFileStream << '\n';

        }
    }

    return 0;
}
