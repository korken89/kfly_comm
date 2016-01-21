############################################################################
#
# Copyright (C) 2015 Emil Fresk.
# All rights reserved.
#
# This file is part of the cppViconStream library.
#
# GNU Lesser General Public License Usage
# This file may be used under the terms of the GNU Lesser
# General Public License version 3.0 as published by the Free Software
# Foundation and appearing in the file LICENSE included in the
# packaging of this file.  Please review the following information to
# ensure the GNU Lesser General Public License version 3.0 requirements
# will be met: http://www.gnu.org/licenses/lgpl-3.0.html.
#
# If you have questions regarding the use of this file, please contact
# Emil Fresk at emil.fresk@gmail.com.
#
############################################################################

cmake_minimum_required(VERSION 2.8.3)

########################################
# Project name
########################################
project(kflytelemetry)
find_package(catkin QUIET)

########################################
# Enable C++11 and other C++ flags
########################################
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

########################################
# catkin requirements
########################################

if (catkin_FOUND)
    catkin_package(
        DEPENDS pthread
        INCLUDE_DIRS ${catkin_INCLUDE_DIRS} include lib
        LIBRARIES ${PROJECT_NAME}
    )
endif()

########################################
# Library linking and source
########################################
include_directories(${catkin_INCLUDE_DIRS}
                    include
                    lib)

add_library(${PROJECT_NAME}
            src/kfly_telemetry.cpp)


if (catkin_FOUND)

    add_dependencies(${PROJECT_NAME}
                     ${${PROJECT_NAME}_EXPORTED_TARGETS}
                     ${catkin_EXPORTED_TARGETS})

endif ()

target_link_libraries(${PROJECT_NAME}
                      ${catkin_LIBRARIES})

########################################
# Include the example in the build
########################################
# add_subdirectory(example)

