set(markdownpreview_SRCS
    markdownpreviewplugin.cpp
    markdownpreviewwidget.cpp
    markdownsourcedocument.cpp
)

add_library(markdownpreviewplugin MODULE ${markdownpreview_SRCS})

set_target_properties(markdownpreviewplugin PROPERTIES
    EXPORT_NAME KTextEditorMarkdownPreview
)

target_link_libraries(markdownpreviewplugin
    KTextEditorPreview
    KMarkdownView
)

install(TARGETS markdownpreviewplugin DESTINATION ${KDE_INSTALL_PLUGINDIR}/ktexteditorpreview)
