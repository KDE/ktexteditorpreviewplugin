add_definitions(-DTRANSLATION_DOMAIN=\"ktexteditorpreview\")

ecm_setup_version(
  0.1.0
  VARIABLE_PREFIX KTEXTEDITORPREVIEW
  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KTextEditorPreviewConfigVersion.cmake"
)

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
    VERSION ${KTEXTEDITORPREVIEW_VERSION}
    SOVERSION ${KTEXTEDITORPREVIEW_SOVERSION}
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

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KTextEditorPreview")
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KTextEditorPreviewConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KTextEditorPreviewConfig.cmake"
    INSTALL_DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/KTextEditorPreviewConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/KTextEditorPreviewConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(
    EXPORT KTextEditorPreviewTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KTextEditorPreviewTargets.cmake
)
