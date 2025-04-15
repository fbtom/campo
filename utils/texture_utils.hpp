///
/// @file texture_utils.hpp
/// @author fbtom
/// @brief Utilities for texture conversion
/// @date 2025-04-15
///
/// @copyright Copyright (c) 2025
///

#pragma once

// External headers
#include "opencv2/opencv.hpp"
#include <GLFW/glfw3.h>

namespace utils
{

    /**
     * @brief Converts an OpenCV matrix to an OpenGL texture
     * @param mat The OpenCV matrix to convert
     * @return The OpenGL texture ID
     */
    GLuint cvMatToTexture(cv::Mat &mat)
    {
        GLuint textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return textureId;
    }

} // namespace utils