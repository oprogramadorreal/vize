#ifndef LUCCA_COMMAND_HANDER_SIGNALS_HPP
#define LUCCA_COMMAND_HANDER_SIGNALS_HPP

#include "lucca/config.hpp"

#include <boost/signals2.hpp>

namespace lucca {

using CommandExecutedSignal = boost::signals2::signal<void()>;
using CommandExecutedSignalListener = CommandExecutedSignal::slot_function_type;

using CommandUndoneSignal = boost::signals2::signal<void()>;
using CommandUndoneSignalListener = CommandUndoneSignal::slot_function_type;

using CommandRedoneSignal = boost::signals2::signal<void()>;
using CommandRedoneSignalListener = CommandRedoneSignal::slot_function_type;

}

#endif // LUCCA_COMMAND_HANDER_SIGNALS_HPP