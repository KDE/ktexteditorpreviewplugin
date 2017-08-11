add_definitions(-DTRANSLATION_DOMAIN=\"ktexteditorpreview\")

set(ktexteditorpreview_SRCS
    ktexteditorpreviewplugin.cpp
    ktexteditorpreviewview.cpp
)

add_library(ktexteditorpreview MODULE ${ktexteditorpreview_SRCS})

target_link_libraries(ktexteditorpreview
    KF5::TextEditor
    KF5::I18n
)

install(TARGETS ktexteditorpreview DESTINATION ${KDE_INSTALL_PLUGINDIR}/ktexteditor)
