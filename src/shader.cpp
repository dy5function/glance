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

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "shader.hpp"

namespace Glance
{

Shader::Shader
    (
    const std::string& aVertexPath,
    const std::string& aFragmentPath
    )
{
    std::string vertexSource;
    std::string fragmentSource;
    std::ifstream vertexSourceFile, fragmentSourceFile;

    vertexSourceFile.exceptions( std::ifstream::failbit |
                                 std::ifstream::badbit );
    fragmentSourceFile.exceptions( std::ifstream::failbit |
                                   std::ifstream::badbit );

    try
    {
        std::stringstream vertexSourceStream, fragmentSourceStream;

        vertexSourceFile.open( aVertexPath );
        vertexSourceStream << vertexSourceFile.rdbuf();
        vertexSourceFile.close();
        vertexSource = vertexSourceStream.str();

        fragmentSourceFile.open( aFragmentPath );
        fragmentSourceStream << fragmentSourceFile.rdbuf();
        fragmentSourceFile.close();
        fragmentSource = fragmentSourceStream.str();
    }
    catch( const std::ifstream::failure& e )
    {
        // When we cannot read the shader program, there is nothing we can
        // do but throw an exception.
        std::string errorMsg = "Exception while reading shader source: ";
        errorMsg += e.what();
        throw ShaderException( errorMsg );
    }

    const char* vertexSourcePtr = vertexSource.c_str();
    const char* fragmentSourcePtr = fragmentSource.c_str();

    GLuint vertexShaderId, fragmentShaderId;

    vertexShaderId = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShaderId, 1, &vertexSourcePtr, nullptr );
    glCompileShader( vertexShaderId );
    if ( !ShaderCompiled( vertexShaderId ) )
    {
        /// @todo #4 Errorhandling
    }
    fragmentShaderId = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShaderId, 1, &fragmentSourcePtr, nullptr );
    glCompileShader( fragmentShaderId );
    if ( !ShaderCompiled( fragmentShaderId ) )
    {
        /// @todo #4 Errorhandling
    }

    mProgramId = glCreateProgram();
    glAttachShader( mProgramId, vertexShaderId );
    glAttachShader( mProgramId, fragmentShaderId );
    glLinkProgram( mProgramId );
    if ( !ShaderLinked( mProgramId ) )
    {
        /// @todo #4 Errorhandling
    }

    glDeleteShader( vertexShaderId );
    glDeleteShader( fragmentShaderId );
}

GLint Shader::ShaderCompiled
    (
    GLuint aShaderId
    )
{
    GLint success, length, actualLength;
    char* log;

    glGetShaderiv( aShaderId, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderiv( aShaderId, GL_INFO_LOG_LENGTH, &length );
        if ( length )
        {
            log = new char[length];
            glGetShaderInfoLog( aShaderId, length, &actualLength, log );
            // glGetShaderInfoLog set actualLength to the number of characters
            // returned in log EXCLUDING the trailing null byte, while
            // glGetShaderiv sets the number of bytes INCLUDING the null byte.
            // See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetShader.xhtml
            if ( actualLength + 1 != length )
            {
                std::cerr << "ERROR: Unexpected compiler log length "
                          << actualLength << " (expected " << length << "). "
                          << "Compiler log might be scrambled." << std::endl;
            }
            std::cerr << "ERROR: Failed to compile shader " << aShaderId
                      << ". Compiler log:\n" << log << std::endl;

        }
    }
    return success;
}

GLint Shader::ShaderLinked
    (
    GLuint aProgramId
    )
{
    GLint success, length, actualLength;
    char* log;

    glGetProgramiv( aProgramId, GL_LINK_STATUS, &success );
    if ( !success )
    {
        glGetProgramiv( aProgramId, GL_INFO_LOG_LENGTH, &length );
        if ( length )
        {
            log = new char[length];
            glGetProgramInfoLog( aProgramId, length, &actualLength, log );
            // glGetShaderInfoLog set actualLength to the number of characters
            // returned in log EXCLUDING the trailing null byte, while
            // glGetShaderiv sets the number of bytes INCLUDING the null byte.
            // See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetShader.xhtml
            if ( actualLength + 1 != length )
            {
                std::cerr << "ERROR: Unexpected compiler log length "
                          << actualLength << " (expected " << length << "). "
                             "Compiler log might be scrambled." << std::endl;
            }
            std::cerr << "ERROR: Failed to link shader " << aProgramId
                      << ". Compiler log:\n" << log << std::endl;

        }
    }
    return success;
}

void Shader::Use()
{
    glUseProgram( mProgramId );
}

void Shader::SetBooleanUniform
    (
    const std::string&  aName,
    bool                aValue
    )
{
    GLint location = glGetUniformLocation( mProgramId, aName.c_str() );
    if ( -1 == location )
    {
        std::cerr << "ERROR: Could not find uniform " << aName << ". "
                     "This means that " << aName << " does not correspond "
                     "to an active uniform in this shader program or that the "
                     "specified name is reserved by OpenGL.";
        /// @todo #4 Error handling?
    }
    else
    {
        glUniform1i( location, static_cast< GLint >( aValue ) );
    }
}

void Shader::SetIntegerUniform
    (
    const std::string&  aName,
    int                 aValue
    )
{
    GLint location = glGetUniformLocation( mProgramId, aName.c_str() );
    if ( -1 == location )
    {
        std::cerr << "ERROR: Could not find uniform " << aName << ". "
                     "This means that " << aName << " does not correspond "
                     "to an active uniform in this shader program or that the "
                     "specified name is reserved by OpenGL.";
        /// @todo #4 Error handling?
    }
    else
    {
        glUniform1i( location, static_cast< GLint >( aValue ) );
    }
}

void Shader::SetFloatUniform
    (
    const std::string&  aName,
    float               aValue
    )
{
    GLint location = glGetUniformLocation( mProgramId, aName.c_str() );
    if ( -1 == location )
    {
        std::cerr << "ERROR: Could not find uniform " << aName << ". "
                     "This means that " << aName << " does not correspond "
                     "to an active uniform in this shader program or that the "
                     "specified name is reserved by OpenGL.";
        /// @todo #4 Error handling?
    }
    else
    {
        glUniform1f( location, static_cast< GLfloat >( aValue ) );
    }
}

void Shader::SetFloatUniform
    (
    const std::string&  aName,
    float               aValue0,
    float               aValue1,
    float               aValue2,
    float               aValue3
    )
{
    GLint location = glGetUniformLocation( mProgramId, aName.c_str() );
    if ( -1 == location )
    {
        std::cerr << "ERROR: Could not find uniform " << aName << ". "
                     "This means that " << aName << " does not correspond "
                     "to an active uniform in this shader program or that the "
                     "specified name is reserved by OpenGL." << std::endl;
        /// @todo #4 Error handling?
    }
    else
    {
        glUniform4f( location, static_cast< GLfloat >( aValue0 ),
                               static_cast< GLfloat >( aValue1 ),
                               static_cast< GLfloat >( aValue2 ),
                               static_cast< GLfloat >( aValue3 ) );
    }
}

ShaderException::ShaderException
    (
    std::string aErrorMsg
    ) noexcept
    : mErrorMsg( aErrorMsg )
{}

const char* ShaderException::what() const noexcept
{
    return mErrorMsg.c_str();
}

}   // namespace Glance
