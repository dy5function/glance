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

file(GLOB EXAMPLE_SHADERS shader/*.vs
                          shader/*.fs)
file(GLOB EXAMPLE_TEXTURES texture/*.jpg)

add_executable(
    texture_example
    texture_example.cpp
    ${EXAMPLE_SHADERS}
    ${EXAMPLE_TEXTURES}
)
target_link_libraries(
    texture_example PUBLIC
    glance
)
target_include_directories(
    texture_example PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

add_custom_command(TARGET texture_example POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shader $<TARGET_FILE_DIR:texture_example>
                   DEPENDS ${PROJECT_SHADERS})
add_custom_command(TARGET texture_example POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/texture $<TARGET_FILE_DIR:texture_example>
                   DEPENDS ${PROJECT_TEXTURES})
