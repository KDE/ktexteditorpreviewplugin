cmake_minimum_required(VERSION 3.0)

project(ktexteditorpreview)

set(REQUIRED_ECM_VERSION "5.23.0")
find_package(ECM ${REQUIRED_ECM_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

set(REQUIRED_KF5_VERSION "5.23.0")
find_package(KF5 ${REQUIRED_KF5_VERSION} REQUIRED COMPONENTS
    I18n
    TextEditor
)

add_subdirectory(src)
