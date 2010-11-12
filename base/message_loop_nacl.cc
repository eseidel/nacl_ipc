#include "base/message_loop.h"

void MessageLoop::AddDestructionObserver(DestructionObserver*)
{
}

bool MessageLoop::NestableTasksAllowed() const
{
  return false;
}

void MessageLoop::PostDelayedTask(const tracked_objects::Location& from_here, Task* task, int64 delay_ms)
{
  
}

void MessageLoop::PostTask(const tracked_objects::Location& from_here, Task* task)
{
  
}

void MessageLoop::QuitNow()
{
  
}

void MessageLoop::RemoveDestructionObserver(DestructionObserver*)
{
  
}

void MessageLoop::Run()
{
  
}

void MessageLoop::SetNestableTasksAllowed(bool)
{
  
}

MessageLoop* MessageLoop::current()
{
  return 0;
}

MessageLoop::DestructionObserver::~DestructionObserver()
{
  
}
