#ifndef PHX_EVENT_EVENT_HPP
#define PHX_EVENT_EVENT_HPP

#include <type_traits>
#include <variant>

namespace event {

    namespace helpers {

	template <class T>
	struct false_case : std::false_type {};

	template <class... Ts>
	struct event_cases : Ts... {
	    using Ts::operator()...;
	};

	template <class... Ts>
	event_cases(Ts...)->event_cases<Ts...>;

    }; // namespace helpers

    struct EventWantsQuit {};

    struct EventIntent {
	int x, y;
    };

    struct EventLostIntent : EventIntent {};

    struct EventDraggingBox {
	int x0, x1, y0, y1;
    };

    struct EventFinishedBox : EventDraggingBox {};

    using Event =
      std::variant<EventWantsQuit, EventIntent, EventLostIntent,
                   EventDraggingBox, EventFinishedBox>;

}; // namespace event

#endif // PHX_EVENT_EVENT_HPP
