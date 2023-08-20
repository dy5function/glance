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

#ifndef GLANCE_SHADER_HPP
#define GLANCE_SHADER_HPP

#include <string>
#include <exception>

#include <glad/glad.h>

namespace Glance
{

    class Shader
    {
    public:
        /**
         * @brief   Constructor
         * @details The constructor reads in the shader source file and compiles
         *          the shader program from it.
         * @throw   Throws ShaderException in case of unrecoverable error.
         * @param   aVertexPath [in] Path to the vertex shader source
         * @param   aFragmentPath [in] Path to the fragment shader source
         */
        Shader(
            const std::string &aVertexPath,
            const std::string &aFragmentPath);

        /**
         * @brief   Use this shader program
         * @details Use the compiled shader program. This function is typically
         *          called in the render loop.
         */
        void Use();

        /**
         * @brief   Set a boolean uniform
         * @details Set the boolean uniform identified by the specified name.
         * @note    This method performs a static_cast from bool to the OpenGL
         *          typedef GLint before setting the provided value for the
         *          uniform.
         * @param   aName [in] Name of the boolean uniform to set.
         * @param   aValue [in] Value to set for the boolean uniform.
         */
        void SetBooleanUniform(
            const std::string &aName,
            bool aValue);

        /**
         * @brief   Set an integer uniform
         * @details Set the integer uniform identified by the specified name.
         * @note    This method performs a static_cast from int to the OpenGL
         *          typedef GLint before setting the provided value for the
         *          uniform.
         * @param   aName [in] Name of the integer uniform to set.
         * @param   aValue [in] Value to set for the integer uniform.
         */
        void SetIntegerUniform(
            const std::string &aName,
            int aValue);

        /**
         * @brief   Set a floating-point uniform
         * @details Set the floating-point uniform identified by the specified
         *          name.
         * @note    This method performs a static_cast from float to the OpenGL
         *          typedef GLfloat before setting the provided value for the
         *          uniform.
         * @todo #1 Refactor overloads into more elegant solution?
         * @param   aName [in] Name of the floating-point uniform to set.
         * @param   aValue [in] Value to set for the floating-point uniform.
         */
        void SetFloatUniform(
            const std::string &aName,
            float aValue);
        void SetFloatUniform(
            const std::string &aName,
            float aValue0,
            float aValue1,
            float aValue2,
            float aValue3);

    private:
        // ID of the compiled shader program
        GLuint mProgramId;

        /**
         * @brief   Check compilation status for a shader
         * @details Get the compilation result for a shader and print any error
         *          messages to stderr.
         * @param   aShaderId [in] ID of the shader to get the compilation
         *          status for.
         * @return  true in case compilation was successful and
         *          false in case compilation resulted in an error.
         */
        GLint ShaderCompiled(
            GLuint aShaderId);

        /**
         * @brief   Check linker status for shader program
         * @details Get the linker result for a shader program and print any
         *          error messages to stderr.
         * @param   aProgramId [in] ID of the shader program to get the linker
         *          status for.
         * @return  true in case linker was successful and
         *          false in case compilation resulted in an error.
         */
        GLint ShaderLinked(
            GLuint aProgramId);
    };

    class ShaderException : public std::exception
    {
    public:
        /**
         * @brief   Constructor
         * @details Construct a ShaderException object with information on the
         *          error that occured.
         * @param   aErrorMsg [in] Descriptive error string explaining what went
         *          wrong
         */
        ShaderException(
            std::string aErrorMsg) noexcept;

        /**
         * @brief   Description of the exception
         */
        const char *what() const noexcept override;

    private:
        // A descriptive error message
        std::string mErrorMsg;
    };

} // namespace Glance

#endif // GLANCE_SHADER_HPP
