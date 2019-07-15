
#include <cstdarg>
#include "ariadne.hpp"
#include "fire.hpp"
//#include "controller.hpp"
#include "clock.hpp"


using namespace Ariadne;
using std::cout; using std::endl;

Int main(Int argc, const char* argv[])
{
    // Acquire the verbosity value from the command line
    Nat evolver_verbosity=get_verbosity(argc,argv);

    // Declare the shared system variables required in the following
    RealVariable t("time");
    RealVariable temp("temp");
    
    
    
    StringVariable fire("fire");
    StringVariable clk("clk");

    
    StringConstant increasing("increasing");
    StringConstant tmp("tmp");
    
    
    // Get the automata
    HybridAutomaton fire_automaton = getFire();
    //HybridAutomaton contr_automaton = getController();
    HybridAutomaton clk_automaton = getClock();
    
    // Compose the automata
    CompositeHybridAutomaton fire_system({fire_automaton,clk_automaton});

    // Print the system description on the command line
    cout << fire_system << endl;
    

    // Compute the system evolution

    // Create a GeneralHybridEvolver object and set its verbosity
    GeneralHybridEvolver evolver(fire_system);
    evolver.verbosity = evolver_verbosity;

    // Set the evolution parameters
    evolver.configuration().set_maximum_enclosure_radius(2.05); // The maximum size of an evolved set before early termination
    evolver.configuration().set_maximum_step_size(0.3); // The maximum value that can be used as a t step for integration

    // Declare the type to be used for the system evolution
    typedef GeneralHybridEvolver::OrbitType OrbitType;

    HybridSet initial_set( {fire|increasing, clk|tmp},{t==1, temp==20} );
    

    // Define the initial set, by supplying the location as a list of locations for each composed automata, and
    // the continuous set as a list of variable assignments for each variable controlled on that location
    // (the assignment can be either a singleton value using the == symbol or an interval using the <= symbols)
    
    
    // Define the evolution t: continuous t and maximum number of transitions
    HybridTime evolution_time(20.0,50);
    std::cout << "Computing evolution... " << std::flush;
    
    // Compute the orbit using upper semantics
    OrbitType orbit = evolver.orbit(initial_set,evolution_time,Semantics::UPPER);
    cout << "time: " << t << " temp: " << temp << "\n";
    std::cout << "done." << std::endl;

    // Plot the trajectory using two different projections
    std::cout << "Plotting trajectory... "<<std::flush;
    Axes2d time_T_axes(0<=TimeVariable()<=300,20<=temp<=600);
    plot("fire_time_T",time_T_axes, Colour(0.0,0.5,1.0), orbit);
    std::cout << "done." << std::endl;
}
