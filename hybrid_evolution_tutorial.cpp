/***************************************************************************
 *            hybrid_evolution_tutorial.cpp
 *
 *  Copyright  2008-17  Pieter Collins
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <ariadne.hpp>

namespace Ariadne {
namespace HeatingSystemTutorial {

using std::cout; using std::endl; using std::flush;
using namespace Ariadne;

template<class T> void write(const char* filename, const T& t) {
    std::ofstream ofs(filename); ofs << t; ofs.close();
}

typedef GeneralHybridEvolver HybridEvolverType;

// The automaton has two modes, both
// with two variables, room_temperature and time_of_day
// The dynamics is given by the differential equations
//   dot(T) = K(T_ext(t) - T(t)) + P delta(q(t) = On) \f$
// where
//   T_ext(t) = T_av - T_amp \cos(2 pi t)
// is the external temperature and
//   q(t) in {On,Off}
// is the discrete state of the heater.
//
// The parameters are the insulation coefficient K, the average
// external temperature T_av, the amplitude of the
// temperature fluctuations T_amp and the power of the heater P.
//
// The heater is controlled by a thermostat. The heater turns off whenever
// the temperature rises above a fixed threshold T_Off and
// turns on nondeterministically for a temperature \T between
// T^+_On and \f$T^-_On.
//
// The clock time tau is reset to zero whenever tau becomes
// equal to one.

// System variables
//   time-of-day t (d)
//   room temperature T (C)

// System paramters
//   Heating power P
//   Thermal coefficient K
//   Average external temperature Te
//   Amplitude of external temperature fluctuations Ta
//   Temperature at which the heater is turned off Toff
//   Temperature below which the heater may be turned on Tonact
//   Temperature below which the heater must be turned on Toninv



inline CompositeHybridAutomaton create_heating_system()
{
    RealConstant fatt("fatt",345_decimal);
    RealConstant fatt1("fatt",8_decimal);
    RealConstant fatt2("fatt2",1_decimal);
    RealConstant fatt3("fatt3",20_decimal);
    //RealConstant fatt4("fatt4",20_decimal);

 
    StringVariable heating("heating");
    StringConstant on("on");
    RealVariable temp("temp");
    RealVariable time("time");
    

    // Create the room object
    HybridAutomaton hr("hr");

    // Declare a trivial discrete location.
    //DiscreteLocation heating;

    // The water level is always given by the same dynamic
    // The inflow is controlled by the valve aperture, the outflow depends on the
    // pressure, which is proportional to the water height.
    hr.new_mode(heating|on,{dot(temp) = fatt*log(fatt1*time+fatt2)+fatt3});


    // Create the clock subsystem
    HybridAutomaton clock;
    clock.new_mode( {dot(time)=1.0_decimal} );

    CompositeHybridAutomaton heating_system({clock,hr});
    std::cout << "heating_system=" << heating_system << "\n" << "\n";

    return heating_system;
}
//! [create_heating_system]

//! [create_evolver]
inline HybridEvolverType create_evolver(const CompositeHybridAutomaton& heating_system)
{
    // Create a GeneralHybridEvolver object
    HybridEvolverType evolver(heating_system);

    // Set the evolution parameters
    evolver.configuration().set_maximum_enclosure_radius(0.25);
    evolver.configuration().set_maximum_step_size(0.5);
    evolver.verbosity=1;
    cout <<  evolver.configuration() << endl << endl;

    return evolver;
}
//! [create_evolver]

//! [simulate_evolution]
inline Void simulate_evolution(const CompositeHybridAutomaton& heating_system,const GeneralHybridEvolver& evolver)
{
    StringVariable clock("clock");
    StringVariable heating("heating");
    StringVariable hr("hr");
    RealVariable time("time");
    RealVariable temp("temp");
    TimeVariable t;

    // Create a simulator object.
    HybridSimulator simulator;
    simulator.set_step_size(0.03125);

    // Set an initial point for the simulation
    HybridRealPoint initial_point(hr|heating, {time=0.0_decimal,temp=20.0_decimal});
    cout << "initial_point=" << initial_point << endl;

    // Set the maximum simulation time
    HybridTime simulation_time(8.0_decimal,9);
    cout << "simulation_time=" << simulation_time << endl;

    // Compute a simulation trajectory
    cout << "Computing simulation trajectory... \n" << flush;
    Orbit<HybridApproximatePoint> trajectory = simulator.orbit(heating_system,initial_point,simulation_time);
    cout << "    done." << endl;

    // Write the simulation trajectory to standard output and plot.
    cout << "Writing simulation trajectory... " << flush;
    write("tutorial-trajectory.txt",trajectory);
    cout << "done." << endl;
    cout << "Plotting simulation trajectory... " << flush;
    plot("tutorial-trajectory.png",Axes2d(0.0<=t<=8.0,14.0<=temp<=23.0), Colour(0.0,0.5,1.0), trajectory);
    cout << "done." << endl << endl;
}
//! [simulate_evolution]


//! [compute_evolution]
inline Void compute_evolution(const CompositeHybridAutomaton& heating_system,const GeneralHybridEvolver& evolver)
{
    StringVariable clock("clock");
    StringVariable heating("heating");
    StringVariable hr("hr");
    RealVariable temp("temp");
    RealVariable time("time");
    TimeVariable t;

    // Set the initial set.
    Dyadic Tinit=20; Dyadic Cinit_max(1,10u);
    HybridSet initial_set({hr|heating}, {temp==Tinit,0<=t<=Cinit_max} );
    cout << "initial_set=" << initial_set << endl;

    // Compute the initial set as a validated enclosure.
    HybridEnclosure initial_enclosure = evolver.enclosure(initial_set);
    cout << "initial_enclosure="<<initial_enclosure << endl << endl;

    // Set the maximum evolution time
    HybridTime evolution_time(3.0_decimal,4);
    cout << "evolution_time=" << evolution_time << endl;

    // Compute a validated orbit.
    cout << "Computing orbit... \n" << flush;
    Orbit<HybridEnclosure> orbit = evolver.orbit(initial_set,evolution_time,Semantics::UPPER);
    cout << "    done." << endl;

    // Write the validated orbit to standard output and plot.
    cout << "Writing orbit... " << flush;
    write("tutorial-orbit.txt",orbit);
    cout << "done." << endl;
    cout << "Plotting orbit... " << flush;
    plot("tutorial-orbit.png",Axes2d(0.0<=time<=1.0,20.0<=temp<=600.0), Colour(0.0,0.5,1.0), orbit);
    plot("tutorial-orbit-time.png",Axes2d(0.0<=time<=3.0,20.0<=temp<=600.0), Colour(0.0,0.5,1.0), orbit);
    cout << "done." << endl << endl;


    // Compute reachable and evolved sets
    cout << "Computing reach and evolve sets... \n" << flush;
    ListSet<HybridEnclosure> reach,evolve;
    make_lpair(reach,evolve) = evolver.reach_evolve(initial_enclosure,evolution_time,Semantics::UPPER);
    cout << "    done." << endl;

    // Write the orbit to standard output and plot.
    cout << "Plotting reach and evolve sets... " << flush;
    plot("tutorial-reach_evolve.png",Axes2d(0.0<=time<=1.0,20.0<=temp<=600.0),
         Colour(0.0,0.5,1.0), reach, Colour(0.0,0.25,0.5), initial_enclosure, Colour(0.25,0.0,0.5), evolve);
    cout << "    done." << endl;
/*
    plot("tutorial-reach_evolve-off.png",Axes2d(0.0<=C<=1.0,14.0<=T<=23.0),
         Colour(0.0,0.5,1.0), reach[heating|off], Colour(0.25,0.0,0.5), evolve[heating|off]);
    plot("tutorial-reach_evolve-on.png",Axes2d(0.0<=C<=1.0,14.0<=T<=23.0),
         Colour(0.0,0.5,1.0), reach[heating|on], Colour(0.0,0.25,0.5), initial_enclosure, Colour(0.25,0.0,0.5), evolve[heating|on]);
*/
}
//! [compute_evolution]

inline Void compute_reachable_sets(const GeneralHybridEvolver& evolver)
{
/*
    // Create a ReachabilityAnalyser object
    HybridReachabilityAnalyser analyser(evolver);
    analyser.parameters().initial_grid_density=5;
    analyser.parameters().initial_grid_depth=6;
    analyser.parameters().maximum_grid_depth=6;


    // Define the initial set
    HybridImageSet initial_set;
    StringVariable heating("heating");
    DiscreteLocation heating_off(heating|"off");
    RealVariable T("T");
    RealVariable C("C");
    initial_set[heating_off]={0.0<=C<=0.015625/4, 16.0<=T<=16.0+0.0625/16};

    // Set the maximum evolution time
    HybridTime reach_time(1.5,4);


    // Compute lower-approximation to finite-time evolved set using lower-semantics.
    std::cout << "Computing lower evolve set... " << std::flush;
    HybridGridTreeSet lower_evolve_set = analyser.lower_evolve(heating_system,initial_set,reach_time);
    std::cout << "done." << std::endl;

    // Compute lower-approximation to finite-time reachable set using lower-semantics.
    std::cout << "Computing lower reach set... " << std::flush;
    HybridGridTreeSet lower_reach_set = analyser.lower_reach(heating_system,initial_set,reach_time);
    std::cout << "done." << std::endl;

    plot("tutorial-lower_reach_evolve.png",Axes2d(0.0,C,1.0, 14.0,T,21.0),
         Colour(0.0,0.5,1.0), lower_reach_set,
         Colour(0.0,0.25,0.5), initial_set,
         Colour(0.25,0.0,0.5), lower_evolve_set);

    // Compute over-approximation to finite-time evolved set using upper semantics.
    // Subdivision is used as necessary to keep the local errors reasonable.
    // The accumulated global error may be very large.
    std::cout << "Computing upper evolve set... " << std::flush;
    HybridGridTreeSet upper_evolve_set = analyser.upper_evolve(heating_system,initial_set,reach_time);
    std::cout << "done." << std::endl;

    // Compute over-approximation to finite-time reachable set using upper semantics.
    std::cout << "Computing upper reach set... " << std::flush;
    HybridGridTreeSet upper_reach_set = analyser.upper_reach(heating_system,initial_set,reach_time);
    std::cout << "done." << std::endl;

    plot("tutorial-upper_reach_evolve.png",Axes2d(0.0,C,1.0, 14.0,T,21.0),
         Colour(0.0,0.5,1.0), upper_reach_set,
         Colour(0.0,0.25,0.5), initial_set,
         Colour(0.25,0.0,0.5), upper_evolve_set);

    // Compute over-approximation to infinite-time chain-reachable set using upper semantics.
    std::cout << "Computing chain reach set... " << std::flush;
    HybridGridTreeSet chain_reach_set = analyser.chain_reach(heating_system,initial_set);
    std::cout << "done." << std::endl;
    plot("tutorial-chain_reach.png",Axes2d(0.0,C,1.0, 14.0,T,21.0), Colour(0.0,0.5,1.0), chain_reach_set);
*/
}


} // namespace HeatingSystemTutorial
} // namespace Ariadne

using namespace Ariadne::HeatingSystemTutorial;


//! [main]
Int main(Int argc, const char* argv[])
{
    Nat verbosity=1;
    if(argc>1) {
        if(std::strcmp(argv[1],"-v")==0) { if(argc>2) { verbosity=(Nat)std::atoi(argv[2]); } }
    }

    // Create the system
    CompositeHybridAutomaton heating_system=create_heating_system();
    cout << heating_system << "\n";

    // Create the analyser classes
    HybridEvolverType evolver=create_evolver(heating_system);
    evolver.verbosity = verbosity;
    cout << evolver << "\n";

    // Compute an approximate simulation of the system evolution
    simulate_evolution(heating_system,evolver);

    // Compute rigorous bounds on the system evolution
    compute_evolution(heating_system,evolver);
}
//! [main]