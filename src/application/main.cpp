#include "ApplicationUtils.h"
#include <project.h>
#include <iostream>
#include <mutex>
#include <thread>

class Box {
public:
	explicit Box(int num) : num_things{ num } {}

	int getNumThings() {
		std::lock_guard<std::mutex> lock(m);
		return num_things;
	}

	void setNumThings(int num) {
		std::lock_guard<std::mutex> lock(m);
		num_things = num;
	}

	void transfer(Box& other, int num) {
		std::lock(m, other.m);
		std::lock_guard<std::mutex> lock1(m, std::adopt_lock);
		std::lock_guard<std::mutex> lock2(other.m, std::adopt_lock);
		// critical section: transfer num things from this box to other box
		num_things -= num;
		other.num_things += num;
	}


	int num_things;
	std::mutex m;
};
/*template <typename T>
class ThreadSafeVariable {
public:
	ThreadSafeVariable(const T& initialValue) : value(initialValue) {}

	T get() {
		
		return value;
	}

	void set(const T& newValue) {
		//std::lock_guard<std::mutex> lock(mutex);
		value = newValue;
	}

	T operator=(const T& newValue) {
		set(newValue);
		return newValue;
	}

	T operator()() {
		return get();
	}

	void execute(std::function<void(ThreadSafeVariable<T>&)> function) {
		std::unique_lock lock1{ mutex, std::defer_lock };
		std::lock_guard<std::mutex> lock(mutex);


		function(*this);
	}
private:
	T value;
	std::mutex mutex;
};*/
/*	ThreadGroup group;
	group.addThread([] {
		// task 1
		std::cout << "threadGroup 1" << std::endl;
	});
	group.addThread([] {
		// task 2
		std::cout << "threadGroup 2" << std::endl;
	});
	group.addThread([] {
		// task 3
		std::cout << "threadGroup 3" << std::endl;
	});
	group.stopAll();
	group.waitForAll();*/

using namespace Atlas;

void transfer(Box& from, Box& to, int num)
{
	// don't actually take the locks yet
	//std::unique_lock lock1{ from.m, std::defer_lock };
	//std::unique_lock lock2{ to.m, std::defer_lock };

	// lock both unique_locks without deadlock
	//std::lock(lock1, lock2);

	//from.num_things -= num;
	//to.num_things += num;



	// “from.m” and “to.m” mutexes unlocked in unique_lock dtors
}

int main(int argc, char* argv[]) {
	std::shared_ptr<PROJECT_TYPENAME> project = std::make_shared<PROJECT_TYPENAME>();

	Atlas::Application::FrameManager& manager = Atlas::Application::FrameManager::Instance();
	manager.userProject = project;

	manager.launchThreads();

	//std::shared_ptr<ThreadSafeVariable< int>> counter = std::make_shared<ThreadSafeVariable< int>>(0);
/*	ThreadSafeVariable< int> counter = ThreadSafeVariable< int>(0);

	// Create 10 threads that increment the counter
	std::thread threads[10];
	for (int i = 0; i < 10; i++) {
		threads[i] = std::thread([&counter]() {
			for (int j = 0; j < 100; j++) {

				counter.execute([&counter](ThreadSafeVariable< int>& var) {
					
					int value = var.get();
					value++;
					std::cout << "Thread: " << std::this_thread::get_id() << ": " << value << std::endl;

					counter.set(value);
					std::this_thread::yield();
				});
			}
		});
	}

	// Wait for all threads to finish
	for (int i = 0; i < 10; i++) {
		threads[i].join();
	}

	// Print the final value of the counter
	std::cout << "Final counter value: " << counter.get() << std::endl;

	while (true) {

	}

	return 0;*/


/*	project.preInit();
	Log("Project pre-initialized.");

	Log("Finishing Initialization...");

	InitializeConfigRegistry();
	ConfigFileRegistry& configRegistry = GetConfigFileRegistry();
	
	Renderer& renderer = project.getRenderer();
	WindowDecorations& decorations = project.getWindowDecorations();
	
	Log("Setting up scripting API...");
	ScriptingAPI scriptingAPI;

	InitScripting(scriptingAPI, project);

	Log("Done setting up scripting API.");

	Log("Initializing window...");
	InitWindow(decorations);
	
	Log("Done initializing window.");

	Log("Finishing project's initialization...");
	project.init(argc, argv);
	project.postInit();

	Log("Done finishing project's initialization.");

	Log("Launching threads.");

	

	SetTargetFPS(decorations.targetFPS);

	manager.launchThreads(project);
	
	Log("Initialization is now finished, starting main loop.");
	while (!WindowShouldClose()) {
		HandleFrame(project);
	}
	
	Log("Shutting down...");

	Log("Cleaning up...");
	project.cleanup(0);

	manager.killThreads();

	Log("Done cleaning up.");

	CloseWindow();

	Log("Done shutting down. Goodbye!");*/



	return 0;
}
