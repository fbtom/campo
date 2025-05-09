///
/// @file remove_top_filter_command.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "application/image/decorator/filter_decorators.hpp"
#include "application/image/history/command.hpp"
#include "application/image/image_process/image_processor_manager.hpp"
#include <memory>

namespace image {
namespace history {

class RemoveTopFilterCommand : public image::history::Command {
public:
  RemoveTopFilterCommand(process::ImageProcessorManager *manager);
  void Execute() override;
  void Undo() override;

private:
  process::ImageProcessorManager *manager_;
  std::unique_ptr<ImageProcessor> removed_filter_decorator_;
};

} // namespace history
} // namespace image
