///
/// @file filter_command_receiver.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-17
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "application/image/decorator/filter_decorators.hpp"
#include "application/image/image_process/image_processor_manager.hpp"
#include <memory>

namespace image {
namespace filter {

class FilterCommandReceiver {
public:
  FilterCommandReceiver(image::process::ImageProcessorManager *manager)
      : manager_(manager) {}

  void AddFilter(std::unique_ptr<decorator::FilterDecorator> filter) {
    if (manager_ && filter) {
      manager_->AddFilter(std::move(filter));
    }
  }

  std::unique_ptr<decorator::FilterDecorator> RemoveLastFilter() {
    if (!manager_) {
      return nullptr;
    }
    return manager_->Undo();
  }

  image::process::ImageProcessorManager *GetManager() const { return manager_; }

private:
  image::process::ImageProcessorManager *manager_;
};

} // namespace filter
} // namespace image
