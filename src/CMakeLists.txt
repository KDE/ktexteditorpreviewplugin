if(Qt5WebEngineWidgets_FOUND OR Qt5WebKitWidgets_FOUND)
    add_subdirectory(libkmarkdownview)
endif()

add_subdirectory(plugin)

add_subdirectory(kparts)
