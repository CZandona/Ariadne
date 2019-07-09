set(EXAMPLES_FIRE
    fire_all
    
)

foreach(example ${EXAMPLES_FIRE})
    add_executable(${example} ${example}.cpp)
    target_link_libraries(${example} ariadne)
endforeach()
add_dependencies(examples ${EXAMPLES_FIRE})
