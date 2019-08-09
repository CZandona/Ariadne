/***************************************************************************
 *            controller1-urgent.hpp
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

inline AtomicHybridAutomaton getController1()
{

    // Declare the shared system variables
    RealVariable aperture_V_IN_T1("aperture_V_IN_T1");
    RealVariable aperture_V_OUT_T1("aperture_V_OUT_T1");
    RealVariable inutile("inutile");
    RealVariable height_T1("height_T1");

    // Declare the events we use
    DiscreteEvent A("A");
    DiscreteEvent B("B");
    DiscreteEvent C("C");
    DiscreteEvent v_out_t1_can_open("v_out_t1_can_open");
    DiscreteEvent I("I");

    AtomicHybridAutomaton controller1("controller1");

    // Declare the values the controller1 can variable can have
    AtomicDiscreteLocation v_in_t1_opened("v_in_t1_opened");
    AtomicDiscreteLocation v_in_t1_closing("v_in_t1_closing");
    AtomicDiscreteLocation v_in_t1_closed("v_in_t1_closed");
    AtomicDiscreteLocation tmp_1("tmp_1");
    AtomicDiscreteLocation v_in_t1_opening("v_in_t1_opening");

    // Since aperture is a known constant when the controller1 is open or closed,
    // specify aperture by an algebraic equation.
    controller1.new_mode(v_in_t1_opened,{
            let(aperture_V_IN_T1)=1});
    controller1.new_mode(v_in_t1_closing,{
            dot(aperture_V_IN_T1)=-1/T});
    controller1.new_mode(v_in_t1_closed,{
            let(aperture_V_IN_T1)=0});
    controller1.new_mode(v_in_t1_opening,{
            dot(aperture_V_IN_T1)=1/T});
    

    controller1.new_transition(v_in_t1_opened,v_out_t1_can_open,v_in_t1_closing,{
            next(aperture_V_IN_T1)=aperture_V_IN_T1},
            false,EventKind::URGENT);
    controller1.new_transition(v_in_t1_closed,v_out_t1_can_open,v_in_t1_opening,{
            next(aperture_V_IN_T1)=aperture_V_IN_T1},
            false,EventKind::URGENT);
    controller1.new_transition(v_in_t1_opening,v_out_t1_can_open,v_in_t1_opened,
            false,EventKind::URGENT); 

    controller1.new_transition(v_in_t1_opened,A,v_in_t1_closing,{
            next(aperture_V_IN_T1)=aperture_V_IN_T1},
            height_T1>=hmax-delta,EventKind::URGENT);
    controller1.new_transition(v_in_t1_closing,v_out_t1_can_open,v_in_t1_closed,
            aperture_V_IN_T1<=0+delta,EventKind::URGENT);
    controller1.new_transition(v_in_t1_closed,I,v_in_t1_opening,{
            next(aperture_V_IN_T1)=aperture_V_IN_T1});
    controller1.new_transition(v_in_t1_opening,C,v_in_t1_opened,
            aperture_V_IN_T1>=1-delta,EventKind::URGENT);
  
    return controller1;
}
