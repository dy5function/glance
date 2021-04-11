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

#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glance.hpp"

/**
 * @brief   Process user input
 * @note    Process user inputs during the render loop. Supported user inputs
 *          are:
 *          ESC     Exit the program
 *          W       Toggle wireframe mode
 * @todo #2 Refactor code into modular handling with simple way for adding new
 *          inputs and actions.
 * @param   aWindow [in/out] Window for which to process input
 */
void processInput(GLFWwindow* aWindow);

int main()
{
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR,
                    GLANCE_GLFW_CONTEXT_VERSION_MAJOR );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR,
                    GLANCE_GLFW_CONTEXT_VERSION_MINOR );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    constexpr int width = 800;
    constexpr int height = 600;
    GLFWwindow* window = glfwCreateWindow( width, height, "Glance",
                                           nullptr, nullptr );

    if ( !window )
    {
        std::cerr << "ERROR: Failed to create window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent( window );

    if ( !gladLoadGL() )
    {
        std::cerr << "ERROR: Failed to create OpenGL context." << std::endl;
        glfwTerminate();
        return -1;
    }
    std::cerr << "INFO: Opengl " << glGetString( GL_VERSION ) << std::endl;

    glViewport( 0, 0, width, height );
    glfwSetFramebufferSizeCallback
        (
        window,
        [] ( GLFWwindow*, int aWidth, int aHeight )
        {
            glViewport( 0, 0, aWidth, aHeight );
        });

    /**
     * @todo #3 This currently only works when the program is invoked from
     *          within it'sown directory. Some logic should be added to find
     *          the shaders independent of the current location in the
     *          filesystem.
     */
    Glance::Shader shader = Glance::Shader( "shader.vs", "shader.fs" );

    // Triangle
    float vertices[] = {
        -0.5f, -0.5f, .0f,      // low left
         0.5f, -0.5f, .0f,      // low right
         0.0f,  0.5f, .0f       // top
    };
    GLuint indices[] = { 0, 1, 2 };

    GLuint vertexBufferObject;
    glGenBuffers( 1, &vertexBufferObject );

    GLuint elementBufferObject;
    glGenBuffers( 1, &elementBufferObject );

    GLuint vertexArrayObject;
    glGenVertexArrays( 1, &vertexArrayObject );

    glBindVertexArray( vertexArrayObject );
    glBindBuffer( GL_ARRAY_BUFFER, vertexBufferObject );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices,
                  GL_STATIC_DRAW );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, elementBufferObject );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices,
                  GL_STATIC_DRAW );
    glVertexAttribPointer
        (
        /* index        = */ 0,
        /* size         = */ 3,
        /* type         = */ GL_FLOAT,
        /* normalized   = */ GL_FALSE,
        /* stride       = */ 0,
        /* pointer      = */ (const void*)0
        );
    glEnableVertexAttribArray( 0 );

    // Color of the triangle is updated over time
    float time, red, green, blue;

    while ( !glfwWindowShouldClose( window ) )
    {
        processInput( window );

        // Clear background
        glClearColor( .2f, .3f, .3f, 1.f );
        glClear( GL_COLOR_BUFFER_BIT );

        // Update triangle color
        time    = glfwGetTime();
        red     = ( .5f * std::sin( time ) ) + .5f;
        green   = ( .5f * std::sin( time + .667f * M_PI ) ) + .5f;
        blue    = ( .5f * std::sin( time - .667f * M_PI ) ) + .5f;

        shader.Use();
        glBindVertexArray( vertexArrayObject );
        shader.SetFloatUniform( "aColor", red, green, blue, 1.f );

        glDrawElements
            (
            /* mode     = */ GL_TRIANGLES,
            /* count    = */ 3,
            /* type     = */ GL_UNSIGNED_INT,
            /* indices  = */ (const void*)0
            );

        glfwSwapBuffers( window );

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* aWindow)
{
    // State viariables
    static bool wireframe = false;
    static bool keyPressedEsc = false;
    static bool keyPressedW = false;

    // Close window
    if (GLFW_PRESS == glfwGetKey(aWindow, GLFW_KEY_ESCAPE))
    {
        if (!keyPressedEsc)
        {
            glfwSetWindowShouldClose(aWindow, true);
            keyPressedEsc = true;
        }
    }
    else
    {
        keyPressedEsc = false;
    }
    // Toggle wirefram mode on/off
    if (GLFW_PRESS == glfwGetKey(aWindow, GLFW_KEY_W))
    {
        if (!keyPressedW)
        {
            wireframe = !wireframe;
            glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
            keyPressedW = true;
        }
    }
    else
    {
        keyPressedW = false;
    }
}
