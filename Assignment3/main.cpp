#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <algorithm>
#include <fstream>
#include "random.h"
#include "utils.h"

enum Events {
    f1Add, f2Add, f3Add, f4Add, f5Add, f6Add,
    f7Add, f8Add, f9Add, f10Add, f11Add, f12Add,
    f13Add, f1Remove, f2Remove, f3Remove, f4Remove,
    f5Remove, f6Remove, f7Remove, f8Remove, f9Remove,
    f10Remove, f11Remove, f12Remove, f13Remove
};

void createInitialFilaments(std::vector<double> &lengthFilaments, const double &h);

void createRates(
        rnd::discrete_distribution &rates,
        const double kOn,
        const double force,
        const std::vector<double> &dX,
        const double kB,
        const double temp,
        const double kOff,
        const double epsilon,
        const std::vector<double> &lengthFilaments,
        const double &d
        );

void updateFilament(
        const int event,
        std::vector<double> &lengthFilaments,
        const double &d,
        const int &nTotal,
        const double &h
        );

double getLengthTube(const std::vector<double> &lengthFilaments);

void removeSubunit(
        int filament,
        std::vector<double> &lengthFilaments,
        const double &d,
        const double &h
        );

void updateDX(std::vector<double> &dX, const double d, const std::vector<double> &lengthFilaments);
double determineDT(rnd::discrete_distribution &rates);

double calcDL(const double &leftLength, const double &ownLength, const double &rightLength, const double &d);

void outputElongationRate(
        const double &t,
        const double &h,
        const double &d,
        const std::vector<double> &lengthFilaments,
        std::ofstream &myFile);

int main()
{
    rnd::set_seed();

    // initialise

    const int nTotal        =   13;             // total number of filaments
    const double tEnd       =   200.0;          // minutes for every simulation
    const double maxRuns    =   5.0;

    const double d          =   8.0e-9;         // size of a subunit in meters
    const double maxForce   =   2.0e-11;
    const double h          =   d / nTotal;
    const double temp       =   289.0;          // temperature in kelvin
    const double kB         =   1.3806485e-23;  // Boltzmann constant
    const double kOn        =   200.0;          // Subunits / min added to a filament
    const double kOff       =   50.0;           // Subunits / min removed from a filament
    const double epsilon    =   1.0e-12;          // electrostatic attractive force in newton

    std::ofstream myFile("data.csv");

    // for loop for 5 runs for forces 0 - 1.1e-11 == 120 runs)
    for (double run = 0.0, force = 0.0; force < maxForce;)
    {

        std::cout << run << '\n';
        std::vector<double> lengthFilaments(nTotal, d);
        std::vector<double> dX(nTotal, 0.0);

        createInitialFilaments(lengthFilaments, h);
        updateDX(dX, d, lengthFilaments);

        //For loop for simulating tEnd minutes
        for (double t = 0.0; t < tEnd;)
        {
            // Create discrete_distributions
            rnd::discrete_distribution rates(nTotal * 2);
            createRates(rates, kOn, force, dX, kB, temp, kOff, epsilon, lengthFilaments, d);

            // determine waiting time and draw event
            const double dt = determineDT(rates);
            const Events event = static_cast<Events>(rates.sample());

            // add passed time
            t += dt;

            // update filaments state
            updateFilament(event, lengthFilaments, d, nTotal, h);
            updateDX(dX, d, lengthFilaments);

            // Output when a run is over
            if (t >= tEnd)
            {
                outputElongationRate(t, force, d, lengthFilaments, myFile);
                ++run;
            }

            // When a h has had maxruns start 10 new runs with a new force
            if (run >= maxRuns)
            {
                run = 0.0;

                if (force == 0.0)
                    force += 2.0e-12;
                else if (force == 2.0e-12)
                    force += 3.0e-12;
                else if (force > 4.9e-12 && force < 5.1e-12)
                    force += 5.0e-12;
                else if (force > 0.9e-11 && force < 1.1e-11)
                    force += 1e-11;
                else force *= 2;

            }
        }

    }
    return 0;
}

void removeSubunit(
        const int filament,
        std::vector<double> &lengthFilaments,
        const double &d,
        const double &h)
{
    assert(filament >= 0 && filament < static_cast<int>(lengthFilaments.size()));
    // Remove a unit's length from the filament
    lengthFilaments[filament] -= d;

    // if the filament becomes smaller than 0 reset the filament
    if (lengthFilaments[filament] < 0.0)
        lengthFilaments[filament] = d - filament * h;
}

void updateFilament(
        const int event,
        std::vector<double> &lengthFilaments,
        const double &d,
        const int &nTotal,
        const double &h
        )
{
    // if event 0 - 12 happens add a filament else remove 1
    if (event < nTotal)
        // Make filament 1 subunit longer.
        lengthFilaments[event] += d;
    else
        removeSubunit((event - nTotal), lengthFilaments, d, h);
}



void updateDX(std::vector<double> &dX, const double d, const std::vector<double> &lengthFilaments)
{
    assert(!dX.empty());
    assert(!lengthFilaments.empty());
    // Calculate all the dX for each filament
    for (int i = 0; i < static_cast<int>(lengthFilaments.size()); ++i)
    {
        dX[i] = d - (getLengthTube(lengthFilaments) - lengthFilaments[i]);
        if (dX[i] < 0.0)
            dX[i] = 0.0;
    }
}

double determineDT(rnd::discrete_distribution &rates)
{
    // Calculate the sum of all rates
    double sum = 0.0;
    for(int i = 0; i < static_cast<int>(rates.size()); ++i)
        sum += rates[i];

    assert(sum > 0.0);
    // pick a timestep between 0 and sum.
    return rnd::exponential(sum);
}

double getLengthTube(const std::vector<double> &lengthFilaments)
{
    // return biggest filament
    assert(!lengthFilaments.empty());
    return *std::max_element(lengthFilaments.begin(), lengthFilaments.end());
}

void createInitialFilaments(std::vector<double> &lengthFilaments, const double &h)
{
    assert(!lengthFilaments.empty());
    // Initial Filaments should all be shifted h down
    // relative to left neighbour
    for (int i = 0; i < static_cast<int>(lengthFilaments.size()); ++i)
        lengthFilaments[i] -= i * h;
}

double calcDL(const double &leftLength, const double &ownLength, const double &rightLength, const double &d)
{

    double dLeftNeighbour = leftLength - ownLength;
    if (dLeftNeighbour < 0.0)
        dLeftNeighbour = 0.0;
    if (dLeftNeighbour > d)
        dLeftNeighbour = d;

    double dRightNeighbour = rightLength - ownLength;
    if (dRightNeighbour < 0.0)
        dRightNeighbour = 0.0;
    if (dRightNeighbour > d)
        dRightNeighbour = d;

    return (d - dLeftNeighbour + d - dRightNeighbour);
}

// Calculates all the rates in the rates file.
void createRates(
        rnd::discrete_distribution &rates,
        const double kOn,
        const double force,
        const std::vector<double> &dX,
        const double kB,
        const double temp,
        const double kOff,
        const double epsilon,
        const std::vector<double> &lFils,
        const double &d
        )
{
    assert(!lFils.empty());
    assert(!dX.empty());


    for (int rate = 0; rate < static_cast<int>(rates.size()) ; ++rate)
    {
        int lNeighbour = 0, rNeighbour = 0;
        if (rate == 0)
            lNeighbour = static_cast<int>(rates.size()) - 1;
        else
            lNeighbour = rate - 1;
        if (rate == static_cast<int>(rates.size()) - 1)
            rNeighbour = 0;
        else
            rNeighbour = rate + 1;

        double dL = calcDL(lFils[lNeighbour], lFils[rate], lFils[rNeighbour], d);
        // First half of the rates get an adding rate
        if (rate < (static_cast<double>(rates.size()) / 2))
            rates[rate] = kOn
                    * exp (((epsilon * dL) / (2*kB*temp))-((force * dX[rate])/(kB*temp)));
        //Second half gets a removing rate
        else
            rates[rate] = kOff
                    * exp (-((epsilon * dL)/ (2*kB*temp)));
    }
}



// Output elongation rate to myFile (force t elongationRate)
void outputElongationRate(
        const double &t,
        const double &force,
        const double &d,
        const std::vector<double> &lengthFilaments,
        std::ofstream &myFile)
{
    assert(!lengthFilaments.empty());
    std::cout << force << ' ' << t << ' '
              << ((getLengthTube(lengthFilaments) - d) / t ) * 1e6 <<'\n';

    myFile << force << ' ' << t << ' ' << ((getLengthTube(lengthFilaments) - d) / t ) * 1e6 <<'\n';
}
