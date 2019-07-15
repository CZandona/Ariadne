#include <cstdarg>
#include "ariadne.hpp"
#include "clock.hpp"

using namespace Ariadne;

inline AtomicHybridAutomaton getController()
{

    // Costanti per
    RealConstant tempmin("tempmin",20_decimal);  
    RealConstant tempmax("tempmax",600_decimal);  
    

    RealVariable temp("temp");
    RealVariable time("time");

    DiscreteEvent e_can_increment("e_can_increment");
    DiscreteEvent e_alert_fire("e_alert_fire");
    DiscreteEvent e_must_break_fire("e_must_break_fire");
    
    AtomicHybridAutomaton controller("controller");

    // Declare the values the controller variable can have
    AtomicDiscreteLocation raising("raising");
    AtomicDiscreteLocation firing("firing");
    AtomicDiscreteLocation end_firing("end_firing");

    // INPUT
    controller.new_transition(raising, e_alert_fire, firing);
    controller.new_transition(firing, e_must_break_fire, end_firing);

  
    controller.new_transition(firing,e_must_break_fire, end_firing, true,  EventKind::URGENT);
    

    return controller;
}
