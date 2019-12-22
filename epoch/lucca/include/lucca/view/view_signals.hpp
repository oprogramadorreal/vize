#ifndef LUCCA_VIEW_SIGNALS_HPP
#define LUCCA_VIEW_SIGNALS_HPP

#include "lucca/config.hpp"

#include <boost/signals2.hpp>

namespace lucca {

class View;

using SubViewActivatedSignal = boost::signals2::signal<void(View&)>;
using SubViewActivatedSignalListener = SubViewActivatedSignal::slot_function_type;

}

#endif // LUCCA_VIEW_SIGNALS_HPP