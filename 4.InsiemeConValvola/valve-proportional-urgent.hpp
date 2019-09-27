/***************************************************************************
 *            valve-proportional-urgent.hpp
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

#include <cstdarg>
#include "ariadne.hpp"

using namespace Ariadne;

inline AtomicHybridAutomaton getValve()
{
    // Declare some constants.
    RealConstant K("K",0.003_decimal); // Gain of the proportional controller
    //RealConstant Ref("Ref",7.0_decimal); // Reference height

    // Declare the shared system variables
    RealVariable aperture2("aperture2");
    RealVariable temp("temp");
    RealVariable temp2("temp2");

    // Declare the events we use
    DiscreteEvent modulating("modulating");
    DiscreteEvent finishing("finishing");
    DiscreteEvent opening("opening");
    

    // Create a valve automaton
    AtomicHybridAutomaton valve_automaton("valve");

    // Declare the locations for the valve automaton
    AtomicDiscreteLocation opened("opened");
    AtomicDiscreteLocation modulated("modulated");
    AtomicDiscreteLocation closed("closed");
    
    // Since aperture is a known constant when the valve is open or closed,
    // specify aperture by an algebraic equation
    valve_automaton.new_mode(opened,{let(aperture2)=1});
    //valve_automaton.new_mode(opened_m,{let(aperture2)=1});
    valve_automaton.new_mode(closed,{let(aperture2)=0});
    //valve_automaton.new_mode(closed_m,{let(aperture2)=0});
    // Specify the differential equation for when the proportional control is in effect
    valve_automaton.new_mode(modulated,{let(aperture2)=temp2/(temp+temp2)});

    // Define the transitions
    //valve_automaton.new_transition(modulated,finished_opening,closed_m,K*(temp-temp2)<=0,EventKind::URGENT);
    //valve_automaton.new_transition(modulated,finished_closing,opened_m,K*(temp-temp2)>=1,EventKind::URGENT);
    //valve_automaton.new_transition(opened_m,start_modulating,modulated,K*(temp-temp2)<=1,EventKind::URGENT);
    //valve_automaton.new_transition(closed_m,start_modulating,modulated,K*(temp-temp2)>=0,EventKind::URGENT);
    valve_automaton.new_transition(closed,opening,opened,temp2>=600,EventKind::URGENT);
    valve_automaton.new_transition(opened,modulating,modulated,temp>=600,EventKind::URGENT);
    valve_automaton.new_transition(modulated,finishing,closed,temp<=20,EventKind::URGENT);

    return valve_automaton;
}
