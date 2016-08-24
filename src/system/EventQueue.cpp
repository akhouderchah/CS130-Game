#include "EventQueue.h"
#include "InputConstants.h"

EventQueue::EventQueue(EOverflowBehavior behavior) :
	m_ReadIndex(1), m_WriteIndex(1)
{
	m_Events[0].event = EGE_END;
	(void)behavior;
}

EventQueue::~EventQueue()
{
}

Event& EventQueue::Consume()
{
	// if done reading, index = 0, else index = m_ReadIndex
	bool read = (m_ReadIndex-m_WriteIndex != 0);
	size_t index = -m_ReadIndex*(!read) + m_ReadIndex;

	// if we read, increment m_ReadIndex
	m_ReadIndex += read;
	// Make sure m_ReadIndex is between 1 and N+1
	m_ReadIndex -= (m_ReadIndex/(N+1)) * N;

	// Because write index will be > N+1 if we push N elements
	m_WriteIndex -= (m_WriteIndex/(N+1)) * N;

	return m_Events[index];
}

bool EventQueue::PushEvent(const Event& event)
{
	// @TODO - Overflow behavior goes here
	if(m_WriteIndex > (N+1))
	{
		return false;
	}

	m_Events[m_WriteIndex++] = event;
	if(m_WriteIndex > N){ m_WriteIndex -= N; }

	if(m_WriteIndex == m_ReadIndex){ m_WriteIndex += N+1; }
	return true;

	// @TODO FIX THIS CODE!!!
	/*
	size_t nextIndex = m_ReadIndex + 1;
	nextIndex -= (nextIndex/(N+1)) * N;

	// Push element here
	m_Events[m_WriteIndex] = event;

	bool last = nextIndex == m_ReadIndex;

	// "Elevate" the write index if we're pushing to the last element in the queue
	m_WriteIndex = last*(m_WriteIndex+N+1) + (1-last)*(nextIndex);

	return true;
	*/
}

