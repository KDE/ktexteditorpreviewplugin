set(markdownpreview_SRCS
    markdownpreviewplugin.cpp
    markdownpreviewwidget.cpp
    markdownpreviewpage.cpp
    documentproxy.cpp
)

qt5_add_resources(markdownpreview_SRCS
    data/data.qrc
)

add_library(markdownpreviewplugin MODULE ${markdownpreview_SRCS})

set_target_properties(markdownpreviewplugin PROPERTIES
    EXPORT_NAME KTextEditorMarkdownPreview
)

target_link_libraries(markdownpreviewplugin
    KTextEditorPreview
    Qt5::WebEngineWidgets
)

install(TARGETS markdownpreviewplugin DESTINATION ${KDE_INSTALL_PLUGINDIR}/ktexteditorpreview)
