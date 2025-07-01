///
/// @file command_history.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "application/image/filter/add_filter_command.hpp"
#include "application/image/filter/filter_command.hpp"
#include "application/image/filter/remove_filter_command.hpp"
#include "application/image/history/command.hpp"
#include <deque>
#include <memory>
#include <stack>
#include <utility>
#include <vector>

namespace image {
namespace history {
struct CommandPair {
  std::unique_ptr<Command> do_command;
  std::unique_ptr<Command> undo_command;
};

class CommandHistory {
public:
  CommandHistory() = default;

  void executeCommand(std::unique_ptr<Command> do_command,
                      std::unique_ptr<Command> undo_command) {
    if (do_command && undo_command) {
      do_command->Execute();

      CommandPair pair{};
      pair.do_command = std::move(do_command);
      pair.undo_command = std::move(undo_command);

      undo_stack_.push(std::move(pair));

      while (!redo_stack_.empty()) {
        redo_stack_.pop();
      }
    }
  }

  void Undo() {
    if (canUndo()) {
      auto pair = std::move(undo_stack_.top());
      undo_stack_.pop();
      pair.undo_command->Execute();
      redo_stack_.push(std::move(pair));
    }
  }

  void Redo() {
    if (canRedo()) {
      auto pair = std::move(redo_stack_.top());
      redo_stack_.pop();

      auto *remove_cmd =
          static_cast<filter::RemoveFilterCommand *>(pair.undo_command.get());

      if (remove_cmd && remove_cmd->HasRemovedFilter()) {
        auto receiver = remove_cmd->GetReceiver();

        auto add_command = std::make_unique<filter::AddFilterCommand>(
            receiver, remove_cmd->GetRemovedFilter());
        add_command->Execute();

        auto new_remove_command =
            std::make_unique<filter::RemoveFilterCommand>(receiver);

        CommandPair new_pair{};
        new_pair.do_command = std::move(add_command);
        new_pair.undo_command = std::move(new_remove_command);

        undo_stack_.push(std::move(new_pair));
      } else {
        pair.do_command->Execute();
        undo_stack_.push(std::move(pair));
      }
    }
  }

  bool canUndo() const { return !undo_stack_.empty(); }

  bool canRedo() const { return !redo_stack_.empty(); }

private:
  std::stack<CommandPair> undo_stack_;
  std::stack<CommandPair> redo_stack_;
};

} // namespace history
} // namespace image
