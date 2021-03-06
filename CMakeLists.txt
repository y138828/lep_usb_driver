cmake_minimum_required(VERSION 2.8)
project(EndpointUSBDriver)

set(USBDRIVER_MAJOR_VERSION 3)
set(USBDRIVER_MINOR_VERSION 6)
set(USBDRIVER_PATCH_VERSION 0)
set(USBDRIVER_BUILD_NO 010)

if (NOT CPU_COUNT)
    set(CPU_COUNT 4)
endif()

if(DEVMODE)
    add_definitions(-D DEVMODE=1)
endif(DEVMODE)


set(CMAKE_VERBOSE_MAKEFILE ON)
set(BUILD_SHARED_LIBS ON)
set(BUILD_STATIC_LIBS ON)

set(CMAKE_CXX_FLAGS "-Wall -pthread -std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")


set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/export)
set(CMAKE_INSTALL_CONFIG_NAME ${CMAKE_BUILD_TYPE})
set(CPACK_PACKAGE_VERSION ${USBDRIVER_MAJOR_VERSION}.${USBDRIVER_MINOR_VERSION}.${USBDRIVER_PATCH_VERSION}-${USBDRIVER_BUILD_NO})
set(CPACK_PACKAGE_VERSION_MAJOR ${USBDRIVER_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${USBDRIVER_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${USBDRIVER_PATCH_VERSION})
set(CPACK_PACKAGE_ARCHITECTURE ${KERNEL_MACHINE})
set(CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}_${CPACK_PACKAGE_VERSION}_${KERNEL_NAME}-${CPACK_PACKAGE_ARCHITECTURE})
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_PACKAGING_INSTALL_PREFIX "")
set(CPACK_COMPONENTS_ALL "dev")
set(CPACK_GENERATOR TGZ)
include(CPack)


link_directories(${CMAKE_CURRENT_BINARY_DIR}/src/skyguard/EndpointUSBDriver/lib)

add_subdirectory(src/skyguard/EndpointUSBDriver/driver)
add_subdirectory(src/skyguard/EndpointUSBDriver/lib) 
add_subdirectory(src/skyguard/EndpointUSBDriver/utils)
