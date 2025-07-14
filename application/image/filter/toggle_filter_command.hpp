///
/// @file toggle_filter_command.hpp
/// @author fbtom
/// @brief
/// @date 2025-07-01
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "application/image/history/command.hpp"
#include "application/image/image_process/image_processor_manager.hpp"
#include <memory>

namespace image {
namespace filter {

class ToggleFilterCommand : public history::Command {
public:
  ToggleFilterCommand(image::process::ImageProcessorManager *manager,
                      size_t index)
      : manager_(manager), index_(index) {}

  void Execute() override {
    if (manager_) {
      manager_->ToggleFilter(index_);
    }
  }

  size_t GetIndex() const { return index_; }

  image::process::ImageProcessorManager *GetManager() const { return manager_; }

private:
  image::process::ImageProcessorManager *manager_;
  size_t index_;
};

} // namespace filter
} // namespace image
