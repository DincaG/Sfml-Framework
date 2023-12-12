#pragma once
#include <vector>
#include <map>
#include "Clock.h"

// Class to schedule tasks or events to be performed in sequence over a period of time
// Each task's function gets called multiple times with a ratio of its supposed percentage until completion
// Tasks can be chained together so they start after others finish
class Scheduler
{
public:

	// Constructs an empty scheduler
	// Pushes itself into global schedulers' list
	Scheduler();

	// Cleans up all tasks and subsequent tasks
	// Removes itself from global schedulers' list
	~Scheduler();

	// Creates a task to execute (taskFunction) which is performed within a (duration)
	// Returns an ID assigned to the newly created task
	// Task can optionally be chained to another task and run after it finishes by specifying its ID with (predecessorTaskId)
	// Otherwise, it runs as soon as created
	int addTask(Time duration, void (*taskFunction)(float, void*), int predecessorTaskId = 0);
	
	// End a specific task with (taskId)
	// Returns true if task successfully killed or false if there isn't any task for the given (taskId)
	// Specify if subsequent tasks or successors should also be deleted with (killSubsequentTasks) flag
	// If not deleted, they're inherited by the task's predecessor
	// They're deleted by default
	bool killTask(int taskId, bool killSubsequentTasks = true);

	// Pauses a specific task with (taskId)
	void pauseTask(int taskId);

	// Resume or start a specific task with (taskId)
	// If task is chained, it gets removed from its predecessor and added to currently running tasks
	void startTask(int taskId);

	// Check if a specific task is running
	// Returns false if there isn't any task with the given (taskId) or if the task is paused
	// Returns true if the task is running
	bool isTaskRunning(int taskId);

	// Set context user data required to perform the tasks
	// Gets passed to every task's function
	void setTaskContextData(void* taskContextData);
	
	// Calls each running task's taskFunction with a ratio/percentage of the elapsed time
	// First call will always pass 0.f for percentage completion to signal the start of the task
	// Last call will always pass 1.f for percentage completion to signal the end of the task (unless otherwise killed)
	// Finished tasks get removed and replaced with their subsequent tasks from currently running tasks
	void progress();

	// Resumes all running tasks
	void start();

	// Pauses all running tasks
	void pause();

	// Returns true if tasks are running or false if they're currently paused
	bool isRunning();

	// Returns true if there aren't currently any tasks to be executed and false otherwise
	bool isEmpty();

	// Calls progress() for each scheduler inside the global scheduler's list
	static void executeTasks();

private:
 
	// duration		- time interval for the task to be performed
	// taskFunction	- the task itself, receives a float between 0.f and 1.f indicating the percentage of elapsed time
	// clock		- task's internal clock
	// predecessor	- if chained, task runs after predecessor task finishes
	// successors	- list of tasks to run once finished
	struct Task
	{	
		Time duration;
		void(*taskFunction)(float, void*);
		Clock clock{ false };
		Task* predecessor{ nullptr };
		std::vector<Task*> successors;

		// deleting the task deletes its successors as well
		~Task()
		{
			for (Task* task : successors)
			{
				delete task;
			}
		}
	};

	// Vector of currently running tasks
	std::vector<Task*> tasks;

	// Bidirectional access between tasks and IDs
	std::map<int, Task*> idToTask;
	std::map<Task*, int> taskToId;

	// Scheduler's internal ID "generator"
	// Gets incremented with every created task
	int idCount;

	// State of the scheduler's tasks
	bool paused;

	// Context user data needed for created tasks
	void* taskContextData;

	// Global list of schedulers
	static std::vector<Scheduler*> schedulers;
};