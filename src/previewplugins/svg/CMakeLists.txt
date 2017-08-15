set(svgpreviewplugin_SRCS
    svgpreviewplugin.cpp
    svgpreviewwidget.cpp
)

add_library(svgpreviewplugin MODULE ${svgpreviewplugin_SRCS})

set_target_properties(svgpreviewplugin PROPERTIES
    EXPORT_NAME KTextEditorSvgPreview
)

target_link_libraries(svgpreviewplugin
    KTextEditorPreview
    Qt5::Svg
)

install(TARGETS svgpreviewplugin DESTINATION ${KDE_INSTALL_PLUGINDIR}/ktexteditorpreview)
