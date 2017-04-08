cmake_minimum_required (VERSION 2.8.11)
project(GetPCIDeviceInfo)

find_package(PkgConfig)
pkg_search_module(libpci REQUIRED libpci)
pkg_search_module(ArgParse REQUIRED ArgParse)

add_definitions(-std=c++11 -Wall -Werror -Wextra)

include_directories(${libpci_INCLUDE_DIR} ${ArgParse_INCLUDE_DIR})

add_executable(GetPCIDeviceInfo GetPCIDeviceInfo.cpp)

target_link_libraries(GetPCIDeviceInfo ${libpci_LDFLAGS})
target_link_libraries(GetPCIDeviceInfo ${ArgParse_LDFLAGS})

install(TARGETS GetPCIDeviceInfo DESTINATION bin)
