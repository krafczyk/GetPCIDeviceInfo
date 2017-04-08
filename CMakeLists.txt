cmake_minimum_required (VERSION 2.8.11)
project(GetPCIDeviceInfo)

find_package(PkgConfig)
pkg_search_module(libpci REQUIRED libpci)

include_directories(${libpci_INCLUDE_DIR})

add_executable(GetPCIDeviceInfo GetPCIDeviceInfo.c)

target_link_libraries(GetPCIDeviceInfo ${libpci_LDFLAGS})

install(TARGETS GetPCIDeviceInfo DESTINATION bin)
