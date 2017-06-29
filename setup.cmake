# Create the build directory and initalize it

execute_process (
    COMMAND ${CMAKE_COMMAND} -E make_directory build
)

execute_process (
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ..
    WORKING_DIRECTORY build
)
