/***************************************************************************
 *            hot_room.hpp
 *       Copyright  2017 Luca Geretti
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
    RealConstant a("a",945); //costante per end 
    RealConstant b("b",8); //costante per end 
    RealConstant c("c",1); //costante per end 
    RealConstant d("d",0.20_decimal); //timer per end 
    RealConstant g("g",400); // costante per coolon 
    RealConstant timef("timef",1.0940_decimal); // tempo in cui si raggiungono 600 incendio
    RealConstant l("l",330); // costante per coolgoon e cooloff
    RealConstant h("h",2.1_decimal); // costante per coolgoon
    RealConstant m("m",19.99_decimal); // costante per stopHeating temperatura minima
    RealConstant p("p",0.50_decimal); // tempo in cui rimanere in coolgoon
    RealConstant q("q",0.90_decimal); // tempo in cui rimanere in cooloff
    RealConstant r("r",10); // costante per end
    RealConstant u("u",2.0_decimal); // fattore correttivo per coolon e coolgoon
    RealConstant v("v",1.3_decimal); // fattore correttivo per cooloff
    RealConstant tempf("tempf",600); // temperatura massima d'incendio
    RealConstant ts("ts",0.0005_decimal); //costante per end
    
    StringVariable heating("heating");
    StringConstant start("start");
    StringConstant end("end");
    StringConstant coolgoon("coolgoon");
    StringConstant coolon("coolon");
    StringConstant cooloff("cooloff");
    StringConstant stopHeating("stopHeating");
    

    RealVariable temp("temp"); //temperatura
    RealVariable t("t"); //tempo
    
    HybridAutomaton hr("hr");

    DiscreteEvent more_hot("more_hot"); // evento da start a end
    DiscreteEvent too_hot("too_hot"); // evento da end a coolon
    DiscreteEvent less_hot("less_hot"); // evento da coolon a coolgoon
    DiscreteEvent less_less_hot("less_less_hot"); // evento da coolgoon a cooloff
    DiscreteEvent end_cool("end_cool"); // evento da cooloff a stopHeating

    
    
    hr.new_mode( heating|start, {dot(temp) = 950*pow(t,Nat(0.25))+200} );
    hr.new_mode( heating|end, {dot(temp) = a*((log(b*(d-ts)+c))/log(r))+ts} );
    hr.new_mode( heating|coolon, {dot(temp) = tempf+((-g)*(timef-((timef+0.01_decimal)*(-u))))});
    hr.new_mode( heating|coolgoon, {dot(temp) = tempf+((-l)*(h-(timef+p))*(timef-((timef+p)*(-u))))});
    hr.new_mode( heating|cooloff, {dot(temp) = tempf+((-l)*(timef-((timef+p+q))*(-v)))});
    hr.new_mode( heating|stopHeating, {dot(temp) = 0});

    
    
    hr.new_transition( heating|start, more_hot, heating|end, {next(temp)=temp}, t>=d, EventKind::URGENT );
    hr.new_transition( heating|end, too_hot, heating|coolon, {next(temp)=temp}, temp >=tempf, EventKind:: URGENT);
    hr.new_transition( heating|coolon, less_hot, heating|coolgoon, {next(temp)=temp}, t>=timef+p, EventKind::URGENT );
    hr.new_transition( heating|coolgoon, less_less_hot, heating|cooloff, {next(temp)=temp}, t>=timef+p+q, EventKind::URGENT );
    hr.new_transition( heating|cooloff, end_cool, heating|stopHeating, {next(temp)=temp}, temp<=m, EventKind::URGENT );
    
    
    return hr;


}
