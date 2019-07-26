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

#include "ariadne.hpp"


using namespace Ariadne;

inline CompositeHybridAutomaton create_heating_system()
{
    RealConstant a("a",345);
    RealConstant b("b",8);
    RealConstant c("c",1);
    RealConstant d("d",20);
    RealConstant e("e",0.25_decimal);
    
    
 
    // creo le variabili
    RealVariable temp("temp"); //temperatura
    RealVariable t("t"); //tempo
    

    // Create the room object
    HybridAutomaton hr("hr");

    // Declare a trivial discrete location.
    DiscreteLocation heating;

    /* dot(temp) = 154*(t^0.25)+20 formula per t<21s
    dot(temp) = a*(log(b*(t-d)+t))+d formula per t>=21s */

    /* formula per il raffreddamento
    vedi quaderno
    */ 
    
    // modello del sistema che incrementa la sua temperatura
    //hr.new_mode(heating, {dot(temp) = 154*(pow(t,0.25))+20});
    hr.new_mode(heating,{dot(temp) = a*(log(b*(t-d)+t))+d});

    // Create the clock subsystem
    HybridAutomaton clock;
    clock.new_mode( {dot(t)=1.0_decimal} );
    

    CompositeHybridAutomaton heating_system({clock,hr});
    std::cout << "heating_system=" << heating_system << "\n" << "\n";

    return heating_system;

}
