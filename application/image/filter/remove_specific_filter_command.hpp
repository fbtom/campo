///
/// @file remove_specific_filter_command.hpp
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

class RemoveSpecificFilterCommand : public history::Command {
public:
  RemoveSpecificFilterCommand(image::process::ImageProcessorManager *manager,
                              size_t index)
      : manager_(manager), index_(index), removed_filter_(nullptr) {}

  void Execute() override {
    if (manager_) {
      removed_filter_ = manager_->RemoveFilter(index_);
    }
  }

  std::unique_ptr<decorator::FilterDecorator> GetRemovedFilter() {
    return std::move(removed_filter_);
  }

  bool HasRemovedFilter() const { return removed_filter_ != nullptr; }

  size_t GetIndex() const { return index_; }

  image::process::ImageProcessorManager *GetManager() const { return manager_; }

private:
  image::process::ImageProcessorManager *manager_;
  size_t index_;
  std::unique_ptr<decorator::FilterDecorator> removed_filter_;
};

} // namespace filter
} // namespace image
