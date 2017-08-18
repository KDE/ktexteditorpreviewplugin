add_definitions(-DTRANSLATION_DOMAIN=\"markdownpart\")

set(markdownpart_SRCS
    markdownpart.cpp
    markdownsourcedocument.cpp
    markdownbrowserextension.cpp
)

add_library(markdownpart  MODULE ${markdownpart_SRCS})


target_link_libraries(markdownpart
    KMarkdownView
    KF5::I18n
    KF5::Parts
)

install( TARGETS markdownpart  DESTINATION ${KDE_INSTALL_PLUGINDIR})
install(PROGRAMS markdownpart.desktop  DESTINATION ${KDE_INSTALL_APPDIR})

install(
  FILES markdownpartui.rc
  DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/markdownpart
)
