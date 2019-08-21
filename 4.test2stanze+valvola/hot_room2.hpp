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

inline HybridAutomaton create_heating_system2()
{
    RealConstant a("a",735); //costante per end heating2
    RealConstant b("b",8); //costante per end heating2
    RealConstant c("c",1); //costante per end heating2
    RealConstant d("d",0.20_decimal); //costante 20s per end heating2
    RealConstant e("e",0.25_decimal); // costante per start heating2
    RealConstant f("f",154); //costante per start heating2
    RealConstant g("g",625); // costante per coolon 
    RealConstant timef("timef",1.3089_decimal); // tempo in cui si raggiungono 600 incendio
    RealConstant i("i",0.3_decimal); // fattore correttivo cooling
    RealConstant l("l",250); // costante per coolgoon e cooloff
    RealConstant h("h",3.5_decimal); // costante per coolgoon
    RealConstant m("m",20); // costante per stopHeating
    RealConstant n("n",1.5487_decimal); // costante per coolgoon
    RealConstant o("o",2.4487_decimal); // costante per cooloff
    RealConstant p("p",0.30_decimal); // costante per coolgoon
    RealConstant q("q",0.90_decimal); // costante per cooloff
    RealConstant r("r",10);
    RealConstant tempf("tempf",600);
    RealConstant ts("ts",0_decimal); 

    StringVariable heating2("heating2");
    StringConstant start("start");
    StringConstant end("end");
    StringConstant coolgoon("coolgoon");
    StringConstant coolon("coolon");
    StringConstant cooloff("cooloff");
    StringConstant startHeating("startHeating");
    StringConstant stopHeating("stopHeating");
    


    // creo le variabili
    RealVariable temp2("temp2"); //temperatura
    RealVariable t("t"); //tempo
    RealVariable aperture2("aperture2");

    // Create the room object
    HybridAutomaton hr2("hr2");

    //dichiarare gli eventi
    DiscreteEvent must_heating("must_heating");
    DiscreteEvent on("on");
    DiscreteEvent more_hot("more_hot");
    DiscreteEvent too_hot("too_hot");
    DiscreteEvent less_hot("less_hot");
    DiscreteEvent less_less_hot("less_less_hot");
    DiscreteEvent end_cool("end_cool");

    
    // modello del sistema che incrementa la sua temperatura
    hr2.new_mode( heating2|startHeating, {dot(temp2) = 0} );
    hr2.new_mode( heating2|start, {dot(temp2) = 140*pow(t,Nat(0.25))+20} );
    hr2.new_mode( heating2|end, {dot(temp2) = a*((log(b*(t-d)+c))/log(r))+d} );
    hr2.new_mode( heating2|coolon, {dot(temp2) = (-g)*(aperture2)*(t-timef*(i))+tempf});
    hr2.new_mode( heating2|coolgoon, {dot(temp2) = (-l)*(aperture2)*(h-timef)*(t-timef*(i))+tempf});
    hr2.new_mode( heating2|cooloff, {dot(temp2) = (-l)*(aperture2)*(t-timef*(i))+tempf});
    hr2.new_mode( heating2|stopHeating, {dot(temp2)=0});

    
    hr2.new_transition( heating2|startHeating, on, heating2|start, {next(temp2)=temp2 }, t>=ts, EventKind::URGENT );
    hr2.new_transition( heating2|start, more_hot, heating2|end, {next(temp2)=temp2}, t>=d, EventKind::URGENT );
    hr2.new_transition( heating2|end, too_hot, heating2|coolon, {next(temp2)=temp2}, temp2 >=tempf, EventKind:: URGENT);
    hr2.new_transition( heating2|coolon, less_hot, heating2|coolgoon, {next(temp2)=temp2}, t>=timef+p, EventKind::URGENT );
    hr2.new_transition( heating2|coolgoon, less_less_hot, heating2|cooloff, {next(temp2)=temp2}, t>=timef+p+q, EventKind::URGENT );
    hr2.new_transition( heating2|cooloff, end_cool, heating2|stopHeating, {next(temp2)=temp2}, temp2<=m, EventKind::URGENT );
    
    // Create the clock subsystem
    /*HybridAutomaton clock;
    clock.new_mode( {dot(t)=1.0_decimal} );
    

    CompositeHybridAutomaton heating_system({clock,hr2});
    std::cout << "heating_system=" << heating_system << "\n" << "\n";
    */
    return hr2;

}
