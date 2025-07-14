///
/// @file add_filter_at_index_command.hpp
/// @author fbtom
/// @brief
/// @date 2025-07-01
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "application/image/decorator/filter_decorators.hpp"
#include "application/image/history/command.hpp"
#include "application/image/image_process/image_processor_manager.hpp"
#include <memory>
#include <utility>

namespace image {
namespace filter {

class AddFilterAtIndexCommand : public history::Command {
public:
  AddFilterAtIndexCommand(image::process::ImageProcessorManager *manager,
                          std::unique_ptr<decorator::FilterDecorator> filter,
                          size_t index)
      : manager_(manager), filter_to_add_(std::move(filter)), index_(index) {}

  void Execute() override {
    if (manager_ && filter_to_add_) {
      manager_->AddFilterAtIndex(std::move(filter_to_add_), index_);
    }
  }

  size_t GetIndex() const { return index_; }

  image::process::ImageProcessorManager *GetManager() const { return manager_; }

private:
  image::process::ImageProcessorManager *manager_;
  std::unique_ptr<decorator::FilterDecorator> filter_to_add_;
  size_t index_;
};

} // namespace filter
} // namespace image
