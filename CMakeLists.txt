cmake_minimum_required(VERSION 3.14)

project(test_sentry_qt LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/sentry-native/install/include
)

add_executable(test_sentry_qt
  main.cpp
)

target_link_libraries(test_sentry_qt
    Qt${QT_VERSION_MAJOR}::Core
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/sentry-native/build/libsentry.so
)


include(GNUInstallDirs)
install(TARGETS test_sentry_qt
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
