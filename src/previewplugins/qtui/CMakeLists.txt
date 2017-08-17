set(qtuipreviewplugin_SRCS
    qtuipreviewplugin.cpp
    qtuipreviewwidget.cpp
)

add_library(qtuipreviewplugin MODULE ${qtuipreviewplugin_SRCS})

set_target_properties(qtuipreviewplugin PROPERTIES
    EXPORT_NAME KTextEditorQtUiPreview
)

target_link_libraries(qtuipreviewplugin
    KTextEditorPreview
)

install(TARGETS qtuipreviewplugin DESTINATION ${KDE_INSTALL_PLUGINDIR}/ktexteditorpreview)
