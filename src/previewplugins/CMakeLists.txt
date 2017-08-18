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


if(Qt5WebEngineWidgets_FOUND OR Qt5WebKitWidgets_FOUND)
    add_subdirectory(markdown)
endif()

add_subdirectory(qtui)
