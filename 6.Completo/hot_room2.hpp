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
    RealConstant a("a",875); //costante per end heating
    RealConstant b("b",8); //costante per end heating
    RealConstant c("c",1); //costante per end heating
    RealConstant d("d",0.20_decimal); //costante 20s per end heating
    RealConstant e("e",1.3146_decimal); // costante per coolon al posto del tempo t
    RealConstant f("f",1.6046_decimal); //costante per coolgoon
    RealConstant g("g",520); // costante per coolon 
    RealConstant timef("timef",1.3046_decimal); // tempo in cui si raggiungono 600 incendio
    RealConstant l("l",470); // costante per coolgoon e cooloff
    RealConstant h("h",2.7_decimal); // costante per coolgoon
    RealConstant m("m",20); // costante per stopHeating
    RealConstant p("p",0.50_decimal); // costante per coolgoon
    RealConstant q("q",0.90_decimal); // costante per cooloff
    RealConstant r("r",10);
    //RealConstant u("u",1.30_decimal);
    //RealConstant u("u",2.00_decimal);
    RealConstant u("u",1.30_decimal);
    RealConstant v("v",2.5_decimal); // tempo di cooloff
    RealConstant tempf("tempf",600);
    RealConstant ts("ts",0.0005_decimal); 
    RealConstant timecooloff("timecooloff",2.1046_decimal); //tempo per cooloff al posto di t
    

    StringVariable heating2("heating2");
    StringConstant start("start");
    StringConstant end("end");
    StringConstant coolgoon("coolgoon");
    StringConstant coolon("coolon");
    StringConstant cooloff("cooloff");
    StringConstant startHeating("startHeating");
    StringConstant stopHeating("stopHeating");
    


    // creo le variabili
    RealVariable temp2("temp2"); 
    RealVariable t("t"); 
    RealVariable aperture1("aperture1");
        

    // Create the room object
    HybridAutomaton hr2("hr2");

    //dichiarare gli eventi
    DiscreteEvent must_heating2("must_heating2");
    DiscreteEvent on2("on2");
    DiscreteEvent more_hot2("more_hot2");
    DiscreteEvent too_hot2("too_hot2");
    DiscreteEvent less_hot2("less_hot2");
    DiscreteEvent less_less_hot2("less_less_hot2");
    DiscreteEvent end_cool2("end_cool2");

    
    // modello del sistema che incrementa la sua temperatura
    hr2.new_mode( heating2|startHeating, {dot(temp2) = 0} );
    hr2.new_mode( heating2|start, {dot(temp2) = 700*pow(t,Nat(0.25))+200} );
    hr2.new_mode( heating2|end, {dot(temp2) = a*((log(b*(d-ts)+c))/log(r))+ts} );
    hr2.new_mode( heating2|coolon, {dot(temp2) = tempf+((-g)*(1-aperture1)*(timef-(timef+0.01_decimal)*(-u/1-aperture1)))});
    hr2.new_mode( heating2|coolgoon, {dot(temp2) = tempf+((-l)*(1-aperture1)*(h-(timef+p)))*(timef-(timef+p)*(-u/1-aperture1))});
    hr2.new_mode( heating2|cooloff, {dot(temp2) = tempf+((-l)*(1-aperture1)*(timef-(timef+p+q)*(-v/1-aperture1)))});
    hr2.new_mode( heating2|stopHeating, {dot(temp2)= 0});

    
    hr2.new_transition( heating2|startHeating, on2, heating2|start, {next(temp2)=temp2}, t>=ts, EventKind::URGENT );
    hr2.new_transition( heating2|start, more_hot2, heating2|end, {next(temp2)=temp2}, t>=d, EventKind::URGENT );
    hr2.new_transition( heating2|end, too_hot2, heating2|coolon, {next(temp2)=temp2}, temp2 >=tempf, EventKind:: URGENT);
    hr2.new_transition( heating2|coolon, less_hot2, heating2|coolgoon, {next(temp2)=temp2}, t>=timef+p, EventKind::URGENT );
    hr2.new_transition( heating2|coolgoon, less_less_hot2, heating2|cooloff, {next(temp2)=temp2}, t>=timef+p+q, EventKind::URGENT );
    hr2.new_transition( heating2|cooloff, end_cool2, heating2|stopHeating, {next(temp2)=temp2}, temp2<=m, EventKind::URGENT );
    
    
    return hr2;

}
