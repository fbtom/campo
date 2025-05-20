///
/// @file add_filter_command.hpp
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

class AddFilterCommand : public history::Command {
public:
  AddFilterCommand(std::shared_ptr<FilterCommandReceiver> receiver,
                   std::unique_ptr<decorator::FilterDecorator> filter)
      : receiver_(receiver), filter_to_add_(std::move(filter)) {}

  void Execute() override {
    if (receiver_ && filter_to_add_) {
      receiver_->AddFilter(std::move(filter_to_add_));
    }
  }
  
  std::shared_ptr<FilterCommandReceiver> GetReceiver() const {
    return receiver_;
  }

private:
  std::shared_ptr<FilterCommandReceiver> receiver_;
  std::unique_ptr<decorator::FilterDecorator> filter_to_add_;
};

} // namespace filter
} // namespace image
