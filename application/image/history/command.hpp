///
/// @file command.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "opencv2/opencv.hpp"

namespace image {
namespace history {

class Command {
public:
  virtual ~Command() = default;
  virtual void Execute() = 0;
  virtual void Undo() = 0;
};

} // namespace commands
} // namespace utils
