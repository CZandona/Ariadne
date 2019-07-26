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
#include "hot_room.hpp"
using namespace Ariadne;

using std::cout; using std::endl;using std::flush;

Int main(Int argc, const char* argv[])
{
    Nat evolver_verbosity=get_verbosity(argc,argv);

    // Dichiaro le variabili condivise
    RealVariable temp("temp"); //temperatura
    RealVariable C("C"); //tempo

    //automi creati
    StringVariable hr("hr"); //hot_room
    
    //stati iniziali
    StringConstant heating("heating"); //unico stato hot_room per riscaldamento
    
    
    //creo il sistema locazione + tempo
    CompositeHybridAutomaton heating_system=create_heating_system();
    cout << heating_system << endl;

    // Creato un GeneralHybridEvolver object
    GeneralHybridEvolver evolver(heating_system);
    evolver.verbosity = evolver_verbosity;

    // Set the evolution parameters
    evolver.configuration().set_maximum_enclosure_radius(3.05);
    evolver.configuration().set_maximum_step_size(0.20);

    // Declare the type to be used for the system evolution
    typedef GeneralHybridEvolver::OrbitType OrbitType;

    std::cout << "Computing evolution... " << std::flush;
    Dyadic Cinit_max(1,10u);
    HybridSet initial_set(hr|heating,{temp==20, 0<=C<=Cinit_max});
    HybridTime evolution_time(2.0,25);
    OrbitType orbit = evolver.orbit(initial_set,evolution_time,Semantics::UPPER);
    std::cout << "done." << std::endl;

    std::cout << "Plotting trajectory... "<<std::flush;
    // plot fase riscaldamento stanza
    Axes2d time_temp_axes(0<=TimeVariable()<=evolution_time.continuous_time(),20<=temp<=600);
    plot("fire - hot room",time_temp_axes, Colour(0.0,0.5,1.0), orbit);



    std::cout << "done." << std::endl;
}
