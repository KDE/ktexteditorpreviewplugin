set(kmarkdownview_SRCS
    kmarkdownview.cpp
    kabstractmarkdownsourcedocument.cpp
)

if(NOT USE_QTWEBKIT)
list(APPEND kmarkdownview_SRCS
    kmarkdownviewpage.cpp
)
endif()

if(USE_QTWEBKIT)
qt5_add_resources(kmarkdownview_SRCS
    data/webkitdata.qrc
)
else()
qt5_add_resources(kmarkdownview_SRCS
    data/webenginedata.qrc
)
endif()

configure_file(kmarkdownview-config.h.in ${CMAKE_CURRENT_BINARY_DIR}/kmarkdownview-config.h)

add_library(KMarkdownView SHARED ${kmarkdownview_SRCS})
generate_export_header(KMarkdownView BASE_NAME kmarkdownview)

if(USE_QTWEBKIT)
    target_link_libraries(KMarkdownView Qt5::WebKitWidgets)
else()
    target_link_libraries(KMarkdownView Qt5::WebEngineWidgets)
endif()

install(TARGETS KMarkdownView ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
