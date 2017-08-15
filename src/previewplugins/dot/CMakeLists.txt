include_directories(
    ${KGraphViewerPart_INCLUDE_DIRS}
)

set(dotpreviewplugin_SRCS
    dotpreviewplugin.cpp
    dotpreviewwidget.cpp
)

add_library(dotpreviewplugin MODULE ${dotpreviewplugin_SRCS})

set_target_properties(dotpreviewplugin PROPERTIES
    EXPORT_NAME KTextEditorDotPreview
)

target_link_libraries(dotpreviewplugin
    KTextEditorPreview
)

install(TARGETS dotpreviewplugin DESTINATION ${KDE_INSTALL_PLUGINDIR}/ktexteditorpreview)
