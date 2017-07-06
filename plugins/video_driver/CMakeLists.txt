if (GRX_PLUGIN_GTK3)
    add_subdirectory (gtk3)
endif ()

if (GRX_PLUGIN_LINUXFB)
    add_subdirectory (linuxfb)
endif ()

add_subdirectory (memory)
