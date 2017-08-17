find_package(KGraphViewerPart 2.3.90 CONFIG QUIET)
set_package_properties(KGraphViewerPart PROPERTIES
    DESCRIPTION "KPart to view callgraph dot files"
    PURPOSE "Required for dot preview"
    URL "https://www.kde.org/applications/graphics/kgraphviewer/"
    TYPE RECOMMENDED
)

if(KGraphViewerPart_FOUND)
    add_subdirectory(dot)
endif()

add_subdirectory(svg)


# pass -DUSE_QTWEBKIT=TRUE to enforce usage of webkit
if (NOT USE_QTWEBKIT)
    find_package(Qt5WebEngineWidgets CONFIG QUIET)
    set_package_properties(Qt5WebEngineWidgets PROPERTIES
        PURPOSE "Required for markdown preview, fallback is Qt5WebKitWidgets"
        URL "https://qt.io/"
        TYPE RECOMMENDED
    )
    if(NOT Qt5WebEngineWidgets_FOUND)
        set(USE_QTWEBKIT TRUE)
    endif()
endif()

if(USE_QTWEBKIT)
    find_package(Qt5WebKitWidgets CONFIG QUIET)
    set_package_properties(Qt5WebKitWidgets PROPERTIES
        PURPOSE "Required for markdown preview, if Qt5WebEngineWidgets is not available"
        URL "https://qt.io/"
        TYPE RECOMMENDED)
    find_package(Qt5WebChannel CONFIG QUIET)
endif()

if(Qt5WebEngineWidgets_FOUND OR Qt5WebKitWidgets_FOUND AND Qt5WebChannel_FOUND)
    add_subdirectory(markdown)
endif()

add_subdirectory(qtui)
