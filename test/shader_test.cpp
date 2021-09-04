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

#include <gtest/gtest.h>

#include "shader.hpp"

namespace Glance
{

class ShaderTest : public ::testing::Test
{
    // empty for now
};

TEST_F( ShaderTest, ThrowsShaderExceptionOnInvalidPath )
{
    EXPECT_THROW( Glance::Shader( "invalid/vertex/shader/path",
                                  "invalid/fragment/shader/path" ),
                  Glance::ShaderException );
}

}   // namespace Glance

int main( int argc, char** argv )
{
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
