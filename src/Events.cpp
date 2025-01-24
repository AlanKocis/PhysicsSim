#include <h/Events.h>
using namespace Hobbes;

EventQueue &EventQueue::Instance()
{
	static EventQueue *instance = new EventQueue();
	return *instance;
}

EventQueue::EventQueue()
{
	this->head = 0;
	this->tail = 0;
}

void EventQueue::PushBack(const Event &event)
{
	bytes[head] = event;
	head = (head + 1) % MAX_PENDING;

	if (head == tail)
		tail = (tail + 1) % MAX_PENDING;
}
void Hobbes::EventQueue::PopFront()
{
	if (IsEmpty())
		return;
	tail = (tail + 1) % MAX_PENDING;

}

Event Hobbes::EventQueue::GrabFront()
{
	if (IsEmpty())
		return Event();
	return bytes[head];
}

int Hobbes::EventQueue::IsFull()
{
	return (head + 1) % MAX_PENDING == tail;
}

int Hobbes::EventQueue::IsEmpty()
{
	return (head == tail);
}

