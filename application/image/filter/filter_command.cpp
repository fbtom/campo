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
#include "application/image/image_process/image_processor_manager.hpp"

namespace image {
namespace filter {

void FilterCommand::Execute() {
  if (manager_ && filter_to_apply_) {
    std::unique_ptr<ImageProcessor> previous_head =
        manager_->ReleaseProcessor();
    filter_to_apply_->SetImage(std::move(previous_head));

    applied_filter_raw_ptr_ = filter_to_apply_.get();
    manager_->SetProcessor(std::move(filter_to_apply_));
  }
}

void FilterCommand::Undo() {
  if (manager_ && applied_filter_raw_ptr_) {
    std::unique_ptr<ImageProcessor> removed_filter_unique_ptr =
        manager_->UndoLast();

    if (removed_filter_unique_ptr &&
        removed_filter_unique_ptr.get() == applied_filter_raw_ptr_) {

      filter_to_apply_ = std::unique_ptr<decorator::FilterDecorator>(
          static_cast<decorator::FilterDecorator *>(
              removed_filter_unique_ptr.release()));
      applied_filter_raw_ptr_ = nullptr;
    } else {

      if (removed_filter_unique_ptr) {
        std::unique_ptr<ImageProcessor> current_head =
            manager_->ReleaseProcessor();
        static_cast<decorator::FilterDecorator *>(
            removed_filter_unique_ptr.get())
            ->SetImage(std::move(current_head));
        manager_->SetProcessor(std::move(removed_filter_unique_ptr));
      }
    }
  }
}

} // namespace filter
} // namespace image
