set(markdownpreview_SRCS
    markdownpreviewplugin.cpp
    markdownpreviewwidget.cpp
    documentproxy.cpp
)
if(NOT USE_QTWEBKIT)
list(APPEND markdownpreview_SRCS
    markdownpreviewpage.cpp
)
endif()

if(USE_QTWEBKIT)
qt5_add_resources(markdownpreview_SRCS
    data/webkitdata.qrc
)
else()
qt5_add_resources(markdownpreview_SRCS
    data/webenginedata.qrc
)
endif()

add_library(markdownpreviewplugin MODULE ${markdownpreview_SRCS})

set_target_properties(markdownpreviewplugin PROPERTIES
    EXPORT_NAME KTextEditorMarkdownPreview
)

target_link_libraries(markdownpreviewplugin
    KTextEditorPreview
)
if(USE_QTWEBKIT)
    target_link_libraries(markdownpreviewplugin Qt5::WebKitWidgets)
    target_compile_definitions(markdownpreviewplugin PRIVATE -DUSE_QTWEBKIT)
else()
    target_link_libraries(markdownpreviewplugin Qt5::WebEngineWidgets)
endif()

install(TARGETS markdownpreviewplugin DESTINATION ${KDE_INSTALL_PLUGINDIR}/ktexteditorpreview)
