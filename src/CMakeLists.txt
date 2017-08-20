add_definitions(-DTRANSLATION_DOMAIN=\"ktexteditorpreview\")

set(ktexteditorpreviewplugin_SRCS
    ktexteditorpreviewplugin.cpp
    ktexteditorpreviewview.cpp
    previewwidget.cpp
    kpartview.cpp
)

add_library(ktexteditorpreviewplugin MODULE ${ktexteditorpreviewplugin_SRCS})

set_target_properties(ktexteditorpreviewplugin PROPERTIES
    EXPORT_NAME KTextEditorPreview
)

target_link_libraries(ktexteditorpreviewplugin
    KF5::TextEditor
    KF5::I18n
)

install(TARGETS ktexteditorpreviewplugin DESTINATION ${KDE_INSTALL_PLUGINDIR}/ktexteditor)
