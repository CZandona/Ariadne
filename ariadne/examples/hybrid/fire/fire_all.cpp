
#include <cstdarg>
#include "ariadne.hpp"
#include "fire.hpp"


using namespace Ariadne;
using std::cout; using std::endl;

Int main(Int argc, const char* argv[])
{
    // Acquire the verbosity value from the command line
    Nat evolver_verbosity=get_verbosity(argc,argv);

    // Declare the shared system variables required in the following
    RealVariable temperature("temperature");

    StringVariable fire("fire");
    StringConstant increasing("increasing");
    StringConstant stopping("stopping");

    // Get the automata
    HybridAutomaton fire_automaton = getFire();
    
    // Compose the automata
    CompositeHybridAutomaton fire_system({fire_automaton});

    // Print the system description on the command line
    cout << fire_system << endl;

    // Compute the system evolution

    // Create a GeneralHybridEvolver object and set its verbosity
    GeneralHybridEvolver evolver(fire_system);
    evolver.verbosity = evolver_verbosity;

    // Set the evolution parameters
    evolver.configuration().set_maximum_enclosure_radius(2.05); // The maximum size of an evolved set before early termination
    evolver.configuration().set_maximum_step_size(0.3); // The maximum value that can be used as a time step for integration

    // Declare the type to be used for the system evolution
    typedef GeneralHybridEvolver::OrbitType OrbitType;

    std::cout << "Computing evolution... " << std::flush;

    // Define the initial set, by supplying the location as a list of locations for each composed automata, and
    // the continuous set as a list of variable assignments for each variable controlled on that location
    // (the assignment can be either a singleton value using the == symbol or an interval using the <= symbols)
    HybridSet initial_set({fire|increasing},{20_decimal<=temperature<=600});
    // Define the evolution time: continuous time and maximum number of transitions
    HybridTime evolution_time(4.0,5);
    // Compute the orbit using upper semantics
    OrbitType orbit = evolver.orbit(initial_set,evolution_time,Semantics::UPPER);
    std::cout << "done." << std::endl;

    // Plot the trajectory using two different projections
    std::cout << "Plotting trajectory... "<<std::flush;
    Axes2d time_temperature_axes(0<=TimeVariable()<=300,20<=temperature<=600);
    plot("fire_time_temperature",time_temperature_axes, Colour(0.0,0.5,1.0), orbit);
    std::cout << "done." << std::endl;
}
