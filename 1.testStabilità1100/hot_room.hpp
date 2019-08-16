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
    RealConstant a("a",345); //costante per end heating
    RealConstant b("b",8); //costante per end heating
    RealConstant c("c",1); //costante per end heating
    RealConstant d("d",0.20_decimal); //costante 20s per end heating
    RealConstant h("h",5.0_decimal); 
    RealConstant r("r",10);
    RealConstant tempf("tempf",1100);
    RealConstant ts("ts",0.0005_decimal); 

    StringVariable heating("heating");
    StringConstant start("start");
    StringConstant end("end");
    StringConstant startHeating("startHeating");
    StringConstant stopHeating("stopHeating");
    


    // creo le variabili
    RealVariable temp("temp"); //temperatura
    RealVariable t("t"); //tempo
        

    // Create the room object
    HybridAutomaton hr("hr");

    //dichiarare gli eventi
    DiscreteEvent on("on");
    DiscreteEvent more_hot("more_hot");
    DiscreteEvent end_cool("end_cool");

    
    // modello del sistema che incrementa la sua temperatura
    hr.new_mode( heating|startHeating, {dot(temp) = 0} );
    hr.new_mode( heating|start, {dot(temp) = 154*pow(t,Nat(0.25))+20} );
    hr.new_mode( heating|end, {dot(temp) = a*((log(b*(t-d)+c))/log(r))+d} );
    hr.new_mode( heating|stopHeating, {dot(temp)=0});

    
    hr.new_transition( heating|startHeating, on, heating|start, {next(temp)=temp}, t>=ts, EventKind::URGENT );
    hr.new_transition( heating|start, more_hot, heating|end, {next(temp)=temp}, t>=d, EventKind::URGENT );
    hr.new_transition( heating|end, end_cool, heating|stopHeating, {next(temp)=temp}, t>h, EventKind::URGENT );
    
    // Create the clock subsystem
    HybridAutomaton clock;
    clock.new_mode( {dot(t)=1.0_decimal} );
    

    CompositeHybridAutomaton heating_system({clock,hr});
    std::cout << "heating_system=" << heating_system << "\n" << "\n";

    return heating_system;

}
