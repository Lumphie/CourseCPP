#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <vector>
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

const int nTotal    =   13;         // total number of filaments
const int eTotal    =   nTotal * 2; // total number of events
const double tEnd   =   10.0;
const double tStep  =   1.0;

const double d      =   8.0e-9;
const double h      =   d / nTotal;
const double force  =   0.0;
const double temp   =   289;
const double kB     =   1.38e-23;
const double kOn    =   200.0;
const double kOff   =   50.0;


void addSubunit(int filament, double &lengthTube, std::vector<double> &lengthFilaments);
void removeSubunit(int filament, double &lengthTube, std::vector<double> &lengthFilaments);
void updateDX(std::vector<double> &dX, const std::vector<double> &lengthFilaments, double lengthTube);
double determineDT(rnd::discrete_distribution &rates);

int main()
{
    std::ofstream output("data.csv");
    rnd::set_seed();

    // initialise
    std::vector<double> lengthFilaments(nTotal, d);
    std::vector<double> dX(nTotal, 0.0);
    double lengthTube = d;

    for (int i = 0; i < nTotal; ++i)
    {
        lengthFilaments[i] -= i*h;
    }
    updateDX(dX, lengthFilaments, lengthTube);

    // for loop

    for (double t = 0.0, tsav = 0.0; t < tEnd;)
    {
        rnd::discrete_distribution rates(eTotal);
        for (int rate = 0; rate < eTotal; ++rate)
        {
            if (rate < nTotal)
                rates[rate] = kOn * exp (-((force * dX[rate])/(kB*temp)));
            else
                rates[rate] = kOff;

        }

        // determine waiting time and draw event
        const double dt = determineDT(rates);
        const Events event = static_cast<Events>(rates.sample());

        // update population state
        t += dt;

        if (event < nTotal)
            addSubunit(event, lengthTube, lengthFilaments);

        else
            removeSubunit((event - nTotal), lengthTube, lengthFilaments);

        updateDX(dX, lengthFilaments, lengthTube);

        //output << t << ' ' << lengthTube * 1e6 << ' '<< ((lengthTube - d) / t ) * 1e6 <<'\n';


        if (t > tsav)
        {
            std::cout << t << ' ' << lengthTube * 1e6 << ' '
                      << ((lengthTube - d) / t ) * 1e6 <<'\n';

            tsav += tStep;

        }
    }

    return 0;
}

void addSubunit(int filament, double &lengthTube, std::vector<double> &lengthFilaments)
{
    lengthFilaments[filament] += d;
    if (lengthFilaments[filament] > lengthTube)
            lengthTube = lengthFilaments[filament];
}

void removeSubunit(int filament, double &lengthTube, std::vector<double> &lengthFilaments)
{
    lengthFilaments[filament] -= d;

    if (lengthFilaments[filament] == lengthTube)
        lengthTube = *max_element(lengthFilaments.begin(), lengthFilaments.end());

    if (lengthFilaments[filament] < 0.0)
        lengthFilaments[filament] = d - filament*h;
}


void updateDX(std::vector<double> &dX, const std::vector<double> &lengthFilaments, double lengthTube)
{
    for (int i = 0; i < nTotal; ++i)
    {
        dX[i] = d - (lengthTube - lengthFilaments[i]);
        if (dX[i] < 0.0)
            dX[i] = 0.0;
    }
}

double determineDT(rnd::discrete_distribution &rates)
{
    double sum = 0.0;
    for(int i = 0; i < nTotal; ++i)
        sum += rates[i];

    verify(sum > 0.0);
    return rnd::exponential(sum);
}
