/***************************************************************************
 *            twowatertanks.cpp
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
#include "hot_room2.hpp"
using namespace Ariadne;

using std::cout; using std::endl;using std::flush;

Int main(Int argc, const char* argv[])
{
    Nat evolver_verbosity=get_verbosity(argc,argv);

    // Dichiaro le variabili condivise
    RealVariable temp("temp"); //temperatura
    RealVariable t("t"); //tempo

    //automi creati
    StringVariable heating("heating"); //hot_room
    
    //stati iniziali
    StringConstant startHeating("startHeating");
    StringConstant end("end"); //unico stato hot_room per riscaldamento
    
    
    //creo il sistema locazione + tempo
    CompositeHybridAutomaton heating_system=create_heating_system();
    cout << heating_system << endl;

    // Creato un GeneralHybridEvolver object
    GeneralHybridEvolver evolver(heating_system);
    evolver.verbosity = evolver_verbosity;

    // Set the evolution parameters
    evolver.configuration().set_maximum_enclosure_radius(3.05);
    evolver.configuration().set_maximum_step_size(0.002);

    // Declare the type to be used for the system evolution
    typedef GeneralHybridEvolver::OrbitType OrbitType;
    
    TaylorSeriesIntegrator integrator(MaximumError(1e-2), Order(3) );
    evolver.set_integrator(integrator);

    
    std::cout << "Computing evolution... " << std::flush;
    Dyadic Cinit_max(1,10u);

    //HybridSet initial_set(hr|heating_s,{temp==3482, t==20});
    double r=1.0/1024; double Ti=20;
    Real Tinitmin(Ti+r); Real Tinitmax(Ti+3*r); Real Cinitmin(0+r); 
    HybridSet initial_set(heating|startHeating,{Tinitmin<=temp<=Tinitmax, 0<=t<=Cinit_max});
    // ariadne calcola in secondi mentre la formula in minuti quindi se in
    // 3 min raggiungo 600 gradi 
    HybridTime evolution_time(4.6,10);
    OrbitType orbit = evolver.orbit(initial_set,evolution_time,Semantics::UPPER);
    std::cout << "done." << std::endl;

    std::cout << "Plotting trajectory... "<<std::flush;
    // plot fase riscaldamento stanza
    //Axes2d time_temp_axes(0<=TimeVariable()<=evolution_time.continuous_time(),3482<=temp<=10000);
    Axes2d time_temp_axes(0<=TimeVariable()<=evolution_time.continuous_time(),20<=temp<=1000);
    plot("FireHotRoom",time_temp_axes, Colour(0.0,0.5,1.0), orbit);



    std::cout << "done." << std::endl;
}
