///
/// @file filter_command.cpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#include "application/image/filter/filter_command.hpp"
#include "application/image/decorator/filter_decorators.hpp"
#include "application/image/filter/filter_command_receiver.hpp"

namespace image {
namespace filter {
FilterCommand::FilterCommand(std::shared_ptr<FilterCommandReceiver> receiver,
                             std::unique_ptr<decorator::FilterDecorator> filter)
    : receiver_(receiver), filter_to_apply_(std::move(filter)) {}

void FilterCommand::Execute() {
  if (receiver_ && filter_to_apply_) {
    receiver_->AddFilter(std::move(filter_to_apply_));
  }
}

} // namespace filter
} // namespace image
