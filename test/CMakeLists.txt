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

enable_testing()

add_executable(
    shader_test
    shader_test.cpp
)
target_link_libraries(
    shader_test
    gtest_main
    glance
)
target_include_directories(
    shader_test PUBLIC
    ${CMAKE_SOURCE_DIR}/submodules/googletest/googletest/include
    ${CMAKE_SOURCE_DIR}/include
)

include(GoogleTest)
gtest_discover_tests(shader_test)
