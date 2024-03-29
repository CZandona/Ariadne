/***************************************************************************
 *            fire.cpp
 *
 *  Copyright  2017	Luca Geretti
 *
 ****************************************************************************/
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "ariadne.hpp"
#include "hot_room.hpp"
#include "clock.hpp"
#include "hot_room2.hpp"
#include "valve-proportional-urgent.hpp"

using namespace Ariadne;

using std::cout; using std::endl;using std::flush;

Int main(Int argc, const char* argv[])
{
    Nat evolver_verbosity=get_verbosity(argc,argv);

    // Dichiaro le variabili condivise
    RealVariable temp("temp"); //temperatura
    RealVariable temp2("temp2"); //temperatura
    RealVariable aperture1("aperture1"); //apertura valvola
    RealVariable t("t"); //tempo
   

    //automi creati
    StringVariable heating("heating"); //hot_room
    StringVariable heating2("heating2"); //hot_room2
    StringVariable valve("valve"); // valvola
    StringVariable time("time"); //clock
  
    
    //stati iniziali
    StringConstant start("start"); //rooms
    StringConstant tmp("tmp"); //clock
    StringConstant closed("closed"); //valvola
    
    
    //creo il sistema locazione + tempo
    HybridAutomaton hotroom_system=create_heating_system();
    HybridAutomaton clock_system=getClock();
    HybridAutomaton hotroom2_system=create_heating_system2();
    AtomicHybridAutomaton valve_system = getValve();
    CompositeHybridAutomaton heating_system({clock_system,hotroom_system, hotroom2_system, valve_system});
    cout << heating_system << endl;

    // Creato un GeneralHybridEvolver object
    GeneralHybridEvolver evolver(heating_system);
    evolver.verbosity = evolver_verbosity;

    // Set the evolution parameters
    evolver.configuration().set_maximum_enclosure_radius(0.01);
    evolver.configuration().set_maximum_step_size(0.001);

    // Declare the type to be used for the system evolution
    typedef GeneralHybridEvolver::OrbitType OrbitType;
    
    TaylorSeriesIntegrator integrator(MaximumError(1e-1), Order(3) );
    evolver.set_integrator(integrator);

    
    std::cout << "Computing evolution... " << std::flush;
    Dyadic Cinit_max(1,10u);

    double r=1.0/1024; double Ti=20;
    Real Tinitmin(Ti+r); Real Tinitmax(Ti+3*r); Real Cinitmin(0+r); 
    HybridSet initial_set({heating|start, time|tmp, valve|closed, heating2|start},{temp==20, t==0,temp2==20});
    HybridTime evolution_time(2.7199,100);
    OrbitType orbit = evolver.orbit(initial_set,evolution_time,Semantics::UPPER);
    std::cout << "done." << std::endl;

    std::cout << "Plotting trajectory... "<<std::flush;
    Axes2d time_temp_axes(0<=TimeVariable()<=evolution_time.continuous_time()+1,19<=temp<=1000);
    plot("FireHotRoomTTemp",time_temp_axes, Colour(0.0,0.5,1.0), orbit);
    std::cout << "Fine plot 1" << "\n";
    Axes2d time_temp2_axes(0<=TimeVariable()<=evolution_time.continuous_time()+1,19<=temp2<=1000);
    plot("FireHotRoomTTemp2",time_temp2_axes, Colour(0.0,0.5,1.0), orbit);
    std::cout << "Fine plot 2" << "\n";
    Axes2d time_valve_axes(0<=TimeVariable()<=evolution_time.continuous_time()+1,-0.1_decimal<=aperture1<=1.3_decimal);
    plot("FireHotRoomTAperture1",time_valve_axes, Colour(0.0,0.5,1.0), orbit);
    std::cout << "Fine plot 3" << "\n";



    std::cout << "done." << std::endl;
}
