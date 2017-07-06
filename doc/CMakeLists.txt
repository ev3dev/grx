add_custom_target (doc ALL)

# NOTE: gtkdoc is handled in src/CMakeLists.txt

if (GRX_ENABLE_DOC)

add_subdirectory (man)
add_subdirectory (valadoc)
add_subdirectory (yelp)

endif ()