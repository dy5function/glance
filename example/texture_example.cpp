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
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
void processInput(GLFWwindow *aWindow);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,
                   GLANCE_GLFW_CONTEXT_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,
                   GLANCE_GLFW_CONTEXT_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    constexpr int windowWidth = 800;
    constexpr int windowHeight = 600;
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Glance",
                                          nullptr, nullptr);

    if (!window)
    {
        std::cerr << "ERROR: Failed to create window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cerr << "ERROR: Failed to create OpenGL context." << std::endl;
        glfwTerminate();
        return -1;
    }
    std::cerr << "INFO: Opengl " << glGetString(GL_VERSION) << std::endl;

    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetFramebufferSizeCallback(window,
                                   [](GLFWwindow *, int aWidth, int aHeight)
                                   {
                                       glViewport(0, 0, aWidth, aHeight);
                                   });

    /**
     * @todo #3 This currently only works when the program is invoked from
     *          within it's own directory. Some logic should be added to find
     *          the shaders independent of the current location in the
     *          filesystem.
     */
    Glance::Shader shader = Glance::Shader("texture_example_shader.vs",
                                           "texture_example_shader.fs");

    // Texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int textureWidth, textureHeight, textureChannels;
    stbi_uc *textureData = stbi_load("container.jpg", &textureWidth,
                                     &textureHeight, &textureChannels,
                                     /* magic number? */ 0);
    if (textureData)
    {
        glTexImage2D(/* target         = */ GL_TEXTURE_2D,
                     /* level          = */ 0,
                     /* internalFormat = */ GL_RGB,
                     /* width          = */ textureWidth,
                     /* height         = */ textureHeight,
                     /* border         = */ 0,
                     /* format         = */ GL_RGB,
                     /* type           = */ GL_UNSIGNED_BYTE,
                     /* data           = */ textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "ERROR: Could not load texture." << std::endl;
    }
    stbi_image_free(textureData);

    // Geometry
    // Each vertex consists of 8 floats that encode the following properties:
    // x-pos, y-pos, z-pos, r-color, g-color, b-color, x-texture-pos, y-texture-pos
    float vertices[] = {
        /* top-right    */ 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        /* bottom right */ 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        /* bottom left  */ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        /* top left     */ -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0};

    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);

    GLuint elementBufferObject;
    glGenBuffers(1, &elementBufferObject);

    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);

    glBindVertexArray(vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(/* index         = */ 0,
                          /* size          = */ 3,
                          /* type          = */ GL_FLOAT,
                          /* normalized    = */ GL_FALSE,
                          /* stride        = */ 8 * sizeof(float),
                          /* offset        = */ (const void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(/* index         = */ 1,
                          /* size          = */ 3,
                          /* type          = */ GL_FLOAT,
                          /* normalized    = */ GL_FALSE,
                          /* stride        = */ 8 * sizeof(float),
                          /* offset        = */ (const void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(/* index         = */ 2,
                          /* size          = */ 2,
                          /* type          = */ GL_FLOAT,
                          /* normalized    = */ GL_FALSE,
                          /* stride        = */ 8 * sizeof(float),
                          /* offset        = */ (const void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Clear background
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vertexArrayObject);

        glDrawElements(
            /* mode     = */ GL_TRIANGLES,
            /* count    = */ 6,
            /* type     = */ GL_UNSIGNED_INT,
            /* indices  = */ (const void *)0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *aWindow)
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
