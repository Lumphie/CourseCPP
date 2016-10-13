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

void createInitialFilament(std::vector<double> &lengthFilaments, const double d);

void createRates(
        rnd::discrete_distribution &rates,
        const double kOn,
        const double force,
        const std::vector<double> dX,
        const double kB,
        const double temp,
        const double kOff
        );

void updateFilament(
        const int event,
        std::vector<double> &lengthFilaments,
        const double &d,
        const int &nTotal
        );

double getLengthTube(const std::vector<double> &lengthFilaments);
void removeSubunit(int filament, std::vector<double> &lengthFilaments, const double &d);
void updateDX(std::vector<double> &dX, const double d, const std::vector<double> &lengthFilaments);
double determineDT(rnd::discrete_distribution &rates);

void outputElongationRate(
        const double &t,
        const double &force,
        const double &d,
        const std::vector<double> &lengthFilaments,
        std::ofstream &myFile);

int main()
{
    rnd::set_seed();

    // initialise

    const int nTotal        =   13;         // total number of filaments
    const int eTotal        =   nTotal * 2; // total number of events
    const double tEnd       =   200.0;
    const double maxRuns    =   10.0;
    const double maxForce   =   1.1e-11;

    const double d          =   8.0e-9;
    const double temp       =   289.0;
    const double kB         =   1.38e-23;
    const double kOn        =   200.0;
    const double kOff       =   50.0;

    std::ofstream myFile("data.csv");

    for (double run = 0.0, force = 0.0; force < maxForce;)
    {
        std::vector<double> lengthFilaments(nTotal, d);
        std::vector<double> dX(nTotal, 0.0);

        createInitialFilament(lengthFilaments, d);
        updateDX(dX, d, lengthFilaments);
        double t = 0.0;
        for (; t < tEnd;)
        {
            rnd::discrete_distribution rates(eTotal);
            createRates(rates, kOn, force, dX, kB, temp, kOff);

            // determine waiting time and draw event
            const double dt = determineDT(rates);
            const Events event = static_cast<Events>(rates.sample());

            // update filaments state
            t += dt;
            updateFilament(event, lengthFilaments, d, nTotal);
            updateDX(dX, d, lengthFilaments);

            if (t >= tEnd)
            {
                outputElongationRate(t, force, d, lengthFilaments, myFile);
                ++run;
            }

            if (run >= maxRuns)
            {
                run = 0.0;
                force += 1.0e-12;
            }
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

void updateFilament(
        const int event,
        std::vector<double> &lengthFilaments,
        const double &d,
        const int &nTotal
        )
{
    if (event < nTotal)
        // Make filament 1 subunit longer.
        lengthFilaments[event] += d;
    else
        removeSubunit((event - nTotal), lengthFilaments, d);
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

void createInitialFilament(std::vector<double> &lengthFilaments, const double d)
{
    for (int i = 0; i < static_cast<int>(lengthFilaments.size()); ++i)
        lengthFilaments[i] -= i * (d / static_cast<int>(lengthFilaments.size()));
}

void createRates(
        rnd::discrete_distribution &rates,
        const double kOn,
        const double force,
        const std::vector<double> dX,
        const double kB,
        const double temp,
        const double kOff
        )
{
    for (int rate = 0; rate < static_cast<int>(rates.size()) ; ++rate)
    {
        if (rate < (static_cast<double>(rates.size()) / 2))
            rates[rate] = kOn * exp (-((force * dX[rate])/(kB*temp)));
        else
            rates[rate] = kOff;
    }
}

void outputElongationRate(
        const double &t,
        const double &force,
        const double &d,
        const std::vector<double> &lengthFilaments,
        std::ofstream &myFile)
{
    std::cout << force << ' ' << t << ' ' << ((getLengthTube(lengthFilaments) - d) / t ) * 1e6 <<'\n';

    myFile << force << ' ' << t << ' ' << ((getLengthTube(lengthFilaments) - d) / t ) * 1e6 <<'\n';
}
