/***************************************************************************
 *            valve_automaton.hpp
 *
 *  Copyright  2017 Luca Geretti
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

using namespace Ariadne;

inline HybridAutomaton getValve()
{

    // Declare some constants. Note that system parameters should be given as variables.
    RealConstant K("K",2); // Gain of the proportional controller
    RealConstant tempmin("tempmin",20);
    RealConstant tempfire("tempfire",600);

    // Declare the shared system variables
    RealVariable aperture1("aperture1");
    RealVariable aperture2("aperture2");
    RealVariable temp("temp");
    RealVariable temp2("temp2");


    HybridAutomaton valve_automaton("valve_automaton");

    //StringVariable opened("opened");
    StringVariable valve("valve");
    StringConstant start("start");
    StringConstant firer2("firer2");
    StringConstant modulated("modulated");
    StringConstant end("end");
        

    //dichiarare gli eventi
    DiscreteEvent fire_room2("fire_room2");
    DiscreteEvent fire_room1("fire_room1");
    DiscreteEvent closing("closing");
    
    valve_automaton.new_mode(valve|start,{let(aperture1)=0, let(aperture2)=0});
    valve_automaton.new_mode(valve|firer2,{dot(aperture1)=0, dot(aperture2)=1});
    valve_automaton.new_mode(valve|end, {dot(aperture1)=0, dot(aperture2)=0});
    // Specify the differential equation for when the proportional control is in effect
    //valve_automaton.new_mode(valve|modulated,{dot(aperture1)=K*(temp-temp2), let(aperture2)= (1-aperture1)});
    valve_automaton.new_mode(valve|modulated,{dot(aperture1)=K*(temp-temp2), dot(aperture2)= (-K)*(temp-temp2)});

    // Define the transitions
    valve_automaton.new_transition(valve|start,fire_room2,valve|firer2,{next(aperture1)=aperture1,next(aperture2)=aperture2},temp2>=tempfire,EventKind::URGENT);
    valve_automaton.new_transition(valve|firer2,fire_room1,valve|modulated,{next(aperture1)=aperture1,next(aperture2)=aperture2},temp>=tempfire,EventKind::URGENT);
    valve_automaton.new_transition(valve|modulated,closing,valve|end,{next(aperture1)=aperture1,next(aperture2)=aperture2},temp<tempmin,EventKind::URGENT);


    return valve_automaton;
}
