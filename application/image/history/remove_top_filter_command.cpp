#include "application/image/history/remove_top_filter_command.hpp"
#include "application/image/decorator/filter_decorators.hpp"
#include "application/image/image_process/image_processor_manager.hpp"

namespace image {
namespace history {

RemoveTopFilterCommand::RemoveTopFilterCommand(
    process::ImageProcessorManager *manager)
    : manager_(manager), removed_filter_decorator_(nullptr) {}

void RemoveTopFilterCommand::Execute() {
  if (manager_) {
    removed_filter_decorator_ = manager_->UndoLast();
  }
}

void RemoveTopFilterCommand::Undo() {
  if (manager_ && removed_filter_decorator_) {
    decorator::FilterDecorator *decorator =
        dynamic_cast<decorator::FilterDecorator *>(
            removed_filter_decorator_.get());
    if (decorator) {
      std::unique_ptr<ImageProcessor> image_ptr = manager_->ReleaseProcessor();
      decorator->SetImage(std::move(image_ptr));
      manager_->SetProcessor(std::move(removed_filter_decorator_));
    } else {
      if (removed_filter_decorator_) {
        manager_->SetProcessor(std::move(removed_filter_decorator_));
      }
    }
  }
}

} // namespace history
} // namespace image
