add_custom_target (doc ALL)

if (GRX_ENABLE_DOC)

add_subdirectory (gtkdoc)
add_subdirectory (man)
add_subdirectory (valadoc)
add_subdirectory (yelp)

endif ()