# Create the build directory and initalize it

execute_process (
    COMMAND ${CMAKE_COMMAND} -E make_directory build
)

if (APPLE)
    set(appleOptions "-DGRX_ENABLE_DOC=No")
endif ()

execute_process (
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${appleOptions} ..
    WORKING_DIRECTORY build
)
