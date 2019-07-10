#include <cstdarg>
#include "ariadne.hpp"

using namespace Ariadne;

inline AtomicHybridAutomaton getFire()
{

    // Declare some constants. Note that system parameters should be given as variables.
    RealConstant T("T",4);

    // Declare some constants
    RealConstant tempmin("tempmin",20_decimal);
    RealConstant tempmax("tempmax",600_decimal);
    RealConstant logaritmo10 ("logaritmo10", 10_decimal);
    //RealConstant delta("delta",0.02_decimal);

    // Declare the shared system variables
    RealVariable temperature("temperature");

    // Declare the events we use
    DiscreteEvent e_can_stop("can_stop");

    // Create the controller automaton
    AtomicHybridAutomaton fire("fire");

    // Declare the locations for the controller
    AtomicDiscreteLocation increasing("increasing");
    AtomicDiscreteLocation stopping("stopping");

    // Instantiate modes for each location
    fire.new_mode(increasing,{dot(temperature)=(345*8)/((8*(T-20)+1)*log(logaritmo10))});
    fire.new_mode(stopping,{dot(temperature)=temperature});
    

    // Specify the invariants valid in each mode. Note that every invariant
    // must have an action label. This is used internally, for example, to
    // check non-blockingness of urgent actions.
    //fire.new_invariant(increasing,temperature<=tempmax,e_can_stop);
    //fire.new_invariant(stopping,temperature>=tempmax,e_can_stop);
    

    // Specify the transitions, starting from the source location, according to an event, to a target location;
    // Following those arguments you specify a guard and whether the event is permissive or urgent.
    fire.new_transition(increasing,e_can_stop,stopping,temperature>=tempmax,EventKind::URGENT);
    

    return fire;
}
