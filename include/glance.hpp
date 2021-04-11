/**
 * Copyright 2021 Christoph Groß
 *
 * This file is part of Glance.
 *
 * Glance is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Glance is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Glance.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef GLANCE
#define GLANCE

#include "shader.hpp"

/**
 * @brief   Major version of GLFW to use with Glance
 * @details The major version that will be used for GLFW contexts. The minimum
 *          reuqired version to use with Glance is version 3 in order to use
 *          the OpenGL core profile.
 */
#define GLANCE_GLFW_CONTEXT_VERSION_MAJOR 4
/**
 * @brief   Minor version of GLFW to use with Glance
 * @details The minor version that will be used for GLFW contexts. The minor
 *          can be tweaked if necessary to support newer OpenGL features.
 */
#define GLANCE_GLFW_CONTEXT_VERSION_MINOR 0

namespace Glance
{

// empty for now

}   // namespace Glance

#endif  // GLITTER
