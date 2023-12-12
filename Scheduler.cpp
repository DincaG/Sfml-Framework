#include "Scheduler.h"

Scheduler::Scheduler()
	: idCount{ 0 }
	, paused{ false }
	, taskContextData{ nullptr }
{
	// push scheduler into global schedulers' list
	schedulers.push_back(this);
}

Scheduler::~Scheduler()
{
	// delete all tasks
	for (Task* task : tasks)
	{
		delete task;
	}

	// remove scheduler from global schedulers' list
	for (size_t i{ 0 }; i < schedulers.size(); i++)
	{
		if (schedulers[i] == this)
		{
			schedulers.erase(schedulers.begin() + i);
			break;
		}
	}
}

int Scheduler::addTask(Time duration, void (*taskFunction)(float, void*), int predecessorTaskId)
{
	// create task and reference an ID for bidirectional access between the two
	Task* task{ new Task{ duration, taskFunction } };
	idCount++;
	idToTask[idCount] = task;
	taskToId[task] = idCount;

	// chain task to another existing one or push it directly into running tasks
	if (!predecessorTaskId)
	{
		// signal the beginning of the task and restart the clock
		task->taskFunction(0.f, taskContextData);
		task->clock.Restart();
		tasks.push_back(task);
	}
	else if (idToTask[predecessorTaskId])
	{
		idToTask[predecessorTaskId]->successors.push_back(task);
		task->predecessor = idToTask[predecessorTaskId];
	}

	// return newly created task's ID
	return idCount;
}

bool Scheduler::killTask(int taskId, bool killSubsequentTasks)
{
	// couldn't end task if there's no task for the given ID - return false
	Task* taskToKill{ idToTask[taskId] };
	if (!taskToKill) return false;

	// establish predecessor que
	std::vector<Task*>& predecessorQue{ tasks };
	if (taskToKill->predecessor)
	{
		predecessorQue = taskToKill->predecessor->successors;
	}

	// remove task from its predecessor que
	for (size_t i{ 0 }; i < predecessorQue.size(); i++)
	{
		if (predecessorQue[i] == taskToKill)
		{
			predecessorQue.erase(predecessorQue.begin() + i);
		}
	}

	// move successor tasks to its predecessor que
	if (!killSubsequentTasks)
	{
		for (size_t i{ 0 }; i < taskToKill->successors.size(); i++)
		{
			taskToKill->successors[i]->predecessor = taskToKill->predecessor;

			// if moved to running tasks
			if (predecessorQue == tasks)
			{
				// signal the beginning of the task and restart the clock
				taskToKill->successors[i]->taskFunction(0.f, taskContextData);
				taskToKill->successors[i]->clock.Restart();
			}

			predecessorQue.push_back(taskToKill->successors[i]);
		}
		taskToKill->successors.clear();
	}

	// remove references and delete task
	idToTask[taskToId[taskToKill]] = nullptr;
	taskToId[taskToKill] = 0;
	delete taskToKill;

	// task killed successfully - return true
	return true;
}

void Scheduler::pauseTask(int taskId)
{
	Task* task{ idToTask[taskId] };
	if (task) task->clock.Pause();
}

void Scheduler::startTask(int taskId)
{
	// make sure there is a task with the given ID
	Task* task{ idToTask[taskId] };
	if (!task) return;

	// if chained
	if (task->predecessor)
	{
		// remove itself from its predecessor
		for (size_t i{ 0 }; i < task->predecessor->successors.size(); i++)
		{
			if (task->predecessor->successors[i] == task)
			{
				task->predecessor->successors.erase(task->predecessor->successors.begin() + i);
			}
		}

		// add it to currently running tasks
		task->predecessor = nullptr;
		tasks.push_back(task);
	}

	// signal the beginning of the task if started for the first time
	if (task->clock.GetElapsedTime().asMicroseconds() == 0)
	{
		task->taskFunction(0.f, taskContextData);
	}
	
	// restart the clock
	task->clock.Restart();
}

bool Scheduler::isTaskRunning(int taskId)
{
	// return false if there isn't any task with the given ID
	Task* task{ idToTask[taskId] };
	if (!task) return false;

	// return task's internal clock state
	return !task->clock.IsPaused();
}

void Scheduler::setTaskContextData(void* taskContextData)
{
	this->taskContextData = taskContextData;
}

void Scheduler::progress()
{
	if (paused) return;
	for (size_t i{ 0 }; i < tasks.size(); i++)
	{
		// calculate the ratio between the elapsed time and the task's time interval
		float elapsedTime{ (float)tasks[i]->clock.GetElapsedTime().asMilliseconds() };
		float percentageComplete{ elapsedTime / tasks[i]->duration.asMilliseconds() };
		if (percentageComplete > 1.f) percentageComplete = 1.f;

		// then call the task function with the given ratio
		tasks[i]->taskFunction(percentageComplete, taskContextData); 

		// if task is finished
		if (percentageComplete == 1.f)
		{
			// move task's successors to running tasks
			for (size_t j{ 0 }; j < tasks[i]->successors.size(); j++)
			{
				tasks[i]->successors[j]->predecessor = nullptr;
				tasks[i]->successors[j]->taskFunction(0.f, taskContextData);
				tasks[i]->successors[j]->clock.Restart();
				tasks.push_back(tasks[i]->successors[j]);
			}
			tasks[i]->successors.clear();

			// remove references and delete task
			idToTask[taskToId[tasks[i]]] = nullptr;
			taskToId[tasks[i]] = 0;
			delete tasks[i];
			tasks.erase(tasks.begin() + i);
		}
	}
}

void Scheduler::start()
{
	for (Task* task : tasks)
	{
		task->clock.Restart();
	}
	paused = false;
}

void Scheduler::pause()
{
	for (Task* task : tasks)
	{
		task->clock.Pause();
	}
	paused = true;
}

bool Scheduler::isRunning()
{
	return !paused;
}

bool Scheduler::isEmpty()
{
	return tasks.empty();
}

void Scheduler::executeTasks()
{
	for (Scheduler* scheduler : schedulers)
	{
		scheduler->progress();
	}
}

std::vector<Scheduler*> Scheduler::schedulers;