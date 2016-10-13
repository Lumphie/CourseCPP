#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <algorithm>
#include "random.h"
#include "utils.h"

enum Events {
    f1Add, f2Add, f3Add, f4Add, f5Add, f6Add,
    f7Add, f8Add, f9Add, f10Add, f11Add, f12Add,
    f13Add, f1Remove, f2Remove, f3Remove, f4Remove,
    f5Remove, f6Remove, f7Remove, f8Remove, f9Remove,
    f10Remove, f11Remove, f12Remove, f13Remove
};


double getLengthTube(const std::vector<double> &lengthFilaments);
void removeSubunit(int filament, std::vector<double> &lengthFilaments, const double &d);
void updateDX(std::vector<double> &dX, const double d, const std::vector<double> &lengthFilaments);
double determineDT(rnd::discrete_distribution &rates);

int main()
{
    rnd::set_seed();

    // initialise

    const int nTotal    =   13;         // total number of filaments
    const int eTotal    =   nTotal * 2; // total number of events
    const double tEnd   =   1000.0;
    const double tStep  =   100.0;

    const double d      =   8.0e-9;
    const double h      =   d / nTotal;
    const double force  =   0.0;
    const double temp   =   289.0;
    const double kB     =   1.38e-23;
    const double kOn    =   200.0;
    const double kOff   =   50.0;


    std::vector<double> lengthFilaments(nTotal, d);
    std::vector<double> dX(nTotal, 0.0);

    for (int i = 0; i < nTotal; ++i)
        lengthFilaments[i] -= i*h;

    updateDX(dX, d, lengthFilaments);

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

        // update filaments state
        t += dt;

        if (event < nTotal)
            // Make filament 1 subunit longer.
            lengthFilaments[event] += d;

        else
            removeSubunit((event - nTotal), lengthFilaments, d);

        updateDX(dX, d, lengthFilaments);

        if (t > tsav)
        {
            std::cout << t << ' ' << getLengthTube(lengthFilaments) * 1e6 << ' '
                      << ((getLengthTube(lengthFilaments) - d) / t ) * 1e6 <<'\n';
            tsav += tStep;
        }
    }
    return 0;
}

void removeSubunit(const int filament, std::vector<double> &lengthFilaments, const double &d)
{
    assert(filament >= 0 && filament < static_cast<int>(lengthFilaments.size()));
    lengthFilaments[filament] -= d;

    double h {d / static_cast<int>(lengthFilaments.size())};
    if (lengthFilaments[filament] < 0.0)
        lengthFilaments[filament] = d - filament * h;
}


void updateDX(std::vector<double> &dX, const double d, const std::vector<double> &lengthFilaments)
{
    for (int i = 0; i < static_cast<int>(lengthFilaments.size()); ++i)
    {
        dX[i] = d - (getLengthTube(lengthFilaments) - lengthFilaments[i]);
        if (dX[i] < 0.0)
            dX[i] = 0.0;
    }
}

double determineDT(rnd::discrete_distribution &rates)
{
    double sum = 0.0;
    for(int i = 0; i < static_cast<int>(rates.size()); ++i)
        sum += rates[i];

    verify(sum > 0.0);
    return rnd::exponential(sum);
}

double getLengthTube(const std::vector<double> &lengthFilaments)
{
    assert(static_cast<int>(lengthFilaments.size()) > 0);
    return *std::max_element(lengthFilaments.begin(), lengthFilaments.end());


}
