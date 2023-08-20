###############################################################################
# Copyright 2021 Christoph Groß
#
# This file is part of Glance.
#
# Glance is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Glance is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Glance.  If not, see <https://www.gnu.org/licenses/>.
###############################################################################

###############################################################################
# This program includes code from Glitter used under the following license:
#
# Copyright (c) 2015 Kevin Fung
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
###############################################################################

cmake_minimum_required(VERSION 3.9.0)
project(
        glance VERSION 0.1
        DESCRIPTION "OpenGL boilerplate project"
        LANGUAGES C CXX
)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TEST OFF)
add_subdirectory(submodules/glfw)
add_subdirectory(submodules/googletest)

include(CTest)
add_subdirectory(test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")

file(GLOB PROJECT_HEADERS include/*.hpp)
file(GLOB PROJECT_SOURCES src/*.cpp)
file(
        GLOB PROJECT_CONFIGS .gitignore
        .gitmodules
        CMakeLists.txt
        COPYING
        Doxyfile
        README.md
)
file(GLOB GLAD_SOURCES submodules/glad/src/glad.c)

add_definitions(-DGLFW_INCLUDE_NONE)
add_library(
        glance
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_CONFIGS}
        ${GLAD_SOURCES}
)
target_link_libraries(
        glance
        glfw
        ${GLFW_LIBRARIES}
        ${GLAD_LIBRARIES}
)
target_include_directories(
        glance PUBLIC
        include/
        submodules/glad/include/
        submodules/glfw/include/
        submodules/stb/
)

# set_target_properties(glance PROPERTIES
#                       RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/glance)

add_subdirectory(example)
