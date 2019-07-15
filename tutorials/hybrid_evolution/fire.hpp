/***************************************************************************
 *            tank.hpp
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
#include <iostream>

using namespace Ariadne;

inline HybridAutomaton getFire()
{
    // Declare the system constants
    RealConstant tempmin("tempmin",20_decimal);
    RealConstant tempmax("tempmax",600_decimal);
    RealConstant logaritmo10 ("logaritmo10", 10_decimal);
    

    // Declase the variables for the dynamics
    TimeVariable time;
    RealVariable temp("temp");
    
    
    // Create the tank automaton; in this case we use the HybridAutomaton class since
    // it is more permissive than AtomicHybridAutomaton; in particular it uses
    // a DiscreteLocation instead of an AtomicDiscreteLocation; a DiscreteLocation allows
    // an empty label, which is cleaner from a logging perspective when there is only one
    // location in an automaton.
    HybridAutomaton fire("fire");

    // Declare a trivial discrete location.
    DiscreteLocation increasing;

    // The water level is always given by the same dynamic.
    // The inflow is controlled by the valve aperture, the outflow depends on the
    // pressure, which is proportional to the water height.
    fire.new_mode(increasing,{dot(temp)=(345*8)/((8*(time-20)+1)*log(logaritmo10)), dot(time)=1_decimal});
    //fire.new_mode(increasing,{dot(time)=1.0_decimal});

    return fire;
}
