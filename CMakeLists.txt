cmake_minimum_required(VERSION 3.10)

project(RobotechTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

function(install_package package_name)
    find_package(${package_name} QUIET)
    if(NOT ${package_name}_FOUND)
        message(STATUS "${package_name} not found. Attempting to install...")
        execute_process(
                COMMAND sudo apt-get install -y ${ARGN}
                RESULT_VARIABLE result
        )
        if(result)
            message(FATAL_ERROR "Failed to install ${package_name}.")
        endif()
    endif()
endfunction()

install_package(Boost libboost-all-dev)
find_package(Boost 1.65 REQUIRED COMPONENTS system)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found after installation.")
endif()

install_package(PkgConfig pkg-config libmodbus-dev)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMODBUS REQUIRED libmodbus)
if(NOT LIBMODBUS_FOUND)
    message(FATAL_ERROR "libmodbus not found after installation.")
endif()

find_package(Threads REQUIRED)

add_subdirectory(Server)
add_subdirectory(Client)
