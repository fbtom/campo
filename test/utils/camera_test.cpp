///
/// @file camera_test.cpp
/// @author fbtom
/// @brief
/// @date 2025-04-07
///
/// @copyright Copyright (c) 2025
///

#include "utils/camera.hpp"
#include <gtest/gtest.h>

namespace utils
{

    TEST(CameraTest, GivenMultipleCameraIDsAndSavedIDExpectIDToBeEqualToSavedID)
    {
        std::vector<int> camera_ids{0, 1, 2, 3, 4, 5};
        std::optional<int> saved_id{2};
        const int expected_id{2};

        EXPECT_EQ(getValidCameraID(camera_ids, saved_id), expected_id);
    }

    TEST(CameraTest, GivenMultipleCameraIDsAndNoSavedIDExpectIDToBeEqualToFirstCameraID)
    {
        std::vector<int> camera_ids{0, 1, 2, 3, 4, 5};
        std::optional<int> saved_id{std::nullopt};
        const int expected_id{0};

        EXPECT_EQ(getValidCameraID(camera_ids, saved_id), expected_id);
    }

    TEST(CameraTest, GivenNoCameraIDsAndSavedIDExpectIDToBeEqualToNoAvailableCameraValue)
    {
        std::vector<int> camera_ids{};
        std::optional<int> saved_id{2};
        const int expected_id{-1};

        EXPECT_EQ(getValidCameraID(camera_ids, saved_id), expected_id);
    }

    TEST(CameraTest, GivenNoCameraIDsAndNoSavedIDExpectIDToBeEqualToNoAvailableCameraValue)
    {
        std::vector<int> camera_ids{};
        std::optional<int> saved_id{std::nullopt};
        const int expected_id{-1};

        EXPECT_EQ(getValidCameraID(camera_ids, saved_id), expected_id);
    }
}