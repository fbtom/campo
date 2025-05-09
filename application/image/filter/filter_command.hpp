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
#include "application/image/history/command.hpp"
#include "application/image/image_process/image_processor.hpp"
#include "application/image/image_process/image_processor_manager.hpp"
#include <memory>
#include <utility>

namespace image {
namespace filter {

class FilterCommand : public history::Command {
public:
  FilterCommand(image::process::ImageProcessorManager *manager,
                std::unique_ptr<decorator::FilterDecorator> filter)
      : manager_(manager), filter_to_apply_(std::move(filter)),
        applied_filter_raw_ptr_(nullptr) {}

  void Execute() override;
  void Undo() override;

private:
  image::process::ImageProcessorManager *manager_;
  std::unique_ptr<decorator::FilterDecorator> filter_to_apply_;
  ImageProcessor *applied_filter_raw_ptr_;
};

} // namespace filter
} // namespace image
