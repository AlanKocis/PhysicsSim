#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>

namespace Hobbes
{
	enum EventType
	{
		NULL_EVENT, GUI_EVENT, NUM_EVENT_TYPES
	};

	struct Event
	{
		EventType type;
		//data
	};

	class EventQueue
		#define MAX_PENDING 20
	{
	public:
		static EventQueue& Instance();
		void PushBack(const Event &event);
		void PopFront();
//
		Event GrabFront();
		int IsFull();
		int IsEmpty();
	private:
		Event bytes[MAX_PENDING]{};
		int head;
		int tail;
		EventQueue();
	};
}
#endif


