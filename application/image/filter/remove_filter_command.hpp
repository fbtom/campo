///
/// @file remove_filter_command.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-16
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "application/image/decorator/filter_decorators.hpp"
#include "application/image/history/command.hpp"
#include "application/image/image_process/image_processor.hpp"
#include "application/image/filter/filter_command_receiver.hpp"
#include <memory>
#include <utility>

namespace image {
namespace filter {

class RemoveFilterCommand : public history::Command {
public:
  RemoveFilterCommand(std::shared_ptr<FilterCommandReceiver> receiver)
      : receiver_(receiver), removed_filter_(nullptr) {}

  void Execute() override {
    if (receiver_) {
      removed_filter_ = receiver_->RemoveLastFilter();
    }
  }

  std::unique_ptr<decorator::FilterDecorator> GetRemovedFilter() {
    return std::move(removed_filter_);
  }
  
  bool HasRemovedFilter() const {
    return removed_filter_ != nullptr;
  }
  
  std::shared_ptr<FilterCommandReceiver> GetReceiver() const {
    return receiver_;
  }

private:
  std::shared_ptr<FilterCommandReceiver> receiver_;
  std::unique_ptr<decorator::FilterDecorator> removed_filter_;
};

} // namespace filter
} // namespace image
