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

inline HybridAutomaton create_heating_system()
{
    RealConstant a("a",345); //costante per end heating
    RealConstant b("b",8); //costante per end heating
    RealConstant c("c",1); //costante per end heating
    RealConstant d("d",0.20_decimal); //costante 20s per end heating
    RealConstant e("e",0.25_decimal); // costante per start heating
    RealConstant f("f",154); //costante per start heating
    RealConstant g("g",625); // costante per coolon 
    RealConstant timef("timef",2.0630_decimal); // tempo in cui si raggiungono 600 incendio
    RealConstant i("i",0.30_decimal); // fattore correttivo cooling
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

    StringVariable heating("heating");
    StringConstant start("start");
    StringConstant end("end");
    StringConstant coolgoon("coolgoon");
    StringConstant coolon("coolon");
    StringConstant cooloff("cooloff");
    StringConstant startHeating("startHeating");
    StringConstant stopHeating("stopHeating");
    


    // creo le variabili
    RealVariable temp("temp"); //temperatura
    RealVariable t("t"); //tempo
    RealVariable aperture1("aperture1");
        

    // Create the room object
    HybridAutomaton hr("hr");

    //dichiarare gli eventi
    DiscreteEvent must_heating("must_heating");
    DiscreteEvent on("on");
    DiscreteEvent more_hot("more_hot");
    DiscreteEvent too_hot("too_hot");
    DiscreteEvent less_hot("less_hot");
    DiscreteEvent less_less_hot("less_less_hot");
    DiscreteEvent end_cool("end_cool");

    
    // modello del sistema che incrementa la sua temperatura
    //hr.new_mode( heating|startHeating, {dot(temp) = 0, dot(t)=1.0_decimal} );
    hr.new_mode( heating|start, {dot(temp) = 154*pow(t,Nat(0.25))+20, dot(t)=1.0_decimal} );
    hr.new_mode( heating|end, {dot(temp) = a*((log(b*(t-d)+c))/log(r))+d, dot(t)=1.0_decimal} );
    hr.new_mode( heating|coolon, {dot(temp) = (-g)*(aperture1)*(t-timef*(i))+tempf, dot(t)=1.0_decimal});
    hr.new_mode( heating|coolgoon, {dot(temp) = (-l)*(aperture1)*(h-timef)*(t-timef*(i))+tempf, dot(t)=1.0_decimal});
    hr.new_mode( heating|cooloff, {dot(temp) = (-l)*(aperture1)*(t-timef*(i))+tempf, dot(t)=1.0_decimal});
    hr.new_mode( heating|stopHeating, {dot(temp)=0, dot(t)=1.0_decimal});

    
    //hr.new_transition( heating|startHeating, on, heating|start, {next(temp)=temp, next(t)=t}, t>=ts, EventKind::URGENT );
    hr.new_transition( heating|start, more_hot, heating|end, {next(temp)=temp, next(t)=t}, t>=d, EventKind::URGENT );
    hr.new_transition( heating|end, too_hot, heating|coolon, {next(temp)=temp, next(t)=t}, temp >=tempf, EventKind:: URGENT);
    hr.new_transition( heating|coolon, less_hot, heating|coolgoon, {next(temp)=temp, next(t)=t}, t>=timef+p, EventKind::URGENT );
    hr.new_transition( heating|coolgoon, less_less_hot, heating|cooloff, {next(temp)=temp, next(t)=t}, t>=timef+p+q, EventKind::URGENT );
    hr.new_transition( heating|cooloff, end_cool, heating|stopHeating, {next(temp)=temp, next(t)=t}, temp<=m, EventKind::URGENT );
    
    // Create the clock subsystem
    /*HybridAutomaton clock;
    clock.new_mode( {dot(t)=1.0_decimal} );
    

    CompositeHybridAutomaton heating_system({clock,hr});
    std::cout << "heating_system=" << heating_system << "\n" << "\n";
    */
    return hr;

}
