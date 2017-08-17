add_definitions(-DTRANSLATION_DOMAIN=\"ktexteditorpreview\")

set(ktexteditorpreview_SRCS
    documentpreviewplugin.cpp
    documentpreviewwidget.cpp
)

add_library(KTextEditorPreview SHARED ${ktexteditorpreview_SRCS})
generate_export_header(KTextEditorPreview BASE_NAME ktexteditorpreview)

target_link_libraries(KTextEditorPreview
    LINK_PUBLIC
        KF5::TextEditor
    LINK_PRIVATE
        KF5::I18n
)

target_include_directories(KTextEditorPreview
        INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KTextEditorPreview>"
)
set_target_properties(KTextEditorPreview PROPERTIES
#     VERSION ${KTEXTEDITORPREVIEW_VERSION}
#     SOVERSION ${KTEXTEDITORPREVIEW_LIB_SOVERSION}
    EXPORT_NAME KTextEditorPreview
)

install(TARGETS KTextEditorPreview EXPORT KTextEditorPreviewTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

ecm_generate_headers(ktexteditorpreview_CC_PUBLIC_HDRS
    HEADER_NAMES
        DocumentPreviewWidget
        DocumentPreviewPlugin
    PREFIX KTextEditorPreview
    REQUIRED_HEADERS ktexteditorpreview_PUBLIC_HDRS
)

install(
    FILES
        ${ktexteditorpreview_PUBLIC_HDRS}
        ${CMAKE_CURRENT_BINARY_DIR}/ktexteditorpreview_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KTextEditorPreview/ktexteditorpreview
    COMPONENT Devel
)

install(
    FILES ${ktexteditorpreview_CC_PUBLIC_HDRS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KTextEditorPreview/KTextEditorPreview
    COMPONENT Devel
)
