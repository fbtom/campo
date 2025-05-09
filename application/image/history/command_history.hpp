///
/// @file command_history.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "application/image/history/command.hpp"
#include <deque>
#include <memory>
#include <vector>

namespace image {
namespace history {

constexpr size_t kMaxHistorySize = 256U;

class CommandHistory {
public:
  CommandHistory() = default;

  void executeCommand(std::unique_ptr<Command> command) {
    if (command) {
      command->Execute();
      undo_stack_.push_front(std::move(command));
      redo_stack_.clear();

      if (undo_stack_.size() > kMaxHistorySize) {
        undo_stack_.pop_back();
      }
    }
  }

  void Undo() {
    if (!undo_stack_.empty()) {
      std::unique_ptr<Command> command = std::move(undo_stack_.front());
      undo_stack_.pop_front();
      command->Undo();
      redo_stack_.push_front(std::move(command));
    }
  }

  void redo() {
    if (!redo_stack_.empty()) {
      std::unique_ptr<Command> command = std::move(redo_stack_.front());
      redo_stack_.pop_front();
      command->Execute();
      undo_stack_.push_front(std::move(command));
    }
  }

  bool canUndo() const { return !undo_stack_.empty(); }

  bool canRedo() const { return !redo_stack_.empty(); }

private:
  std::deque<std::unique_ptr<Command>> undo_stack_;
  std::deque<std::unique_ptr<Command>> redo_stack_;
};

} // namespace history
} // namespace image
