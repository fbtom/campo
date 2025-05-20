///
/// @file filter_command.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "application/image/decorator/filter_decorators.hpp"
#include "application/image/filter/filter_command_receiver.hpp"
#include "application/image/history/command.hpp"
#include "application/image/image_process/image_processor.hpp"
#include <memory>
#include <utility>

namespace image {
namespace filter {

class FilterCommand : public history::Command {
public:
  FilterCommand(std::shared_ptr<FilterCommandReceiver> receiver,
                std::unique_ptr<decorator::FilterDecorator> filter);

  void Execute() override;

  std::shared_ptr<FilterCommandReceiver> GetReceiver() const {
    return receiver_;
  }

private:
  std::shared_ptr<FilterCommandReceiver> receiver_;
  std::unique_ptr<decorator::FilterDecorator> filter_to_apply_;
};

} // namespace filter
} // namespace image
