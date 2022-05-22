#pragma once

class scheduler {
public:
	enum thread {
		main,
		render,
		game,
		client,
		paint,
		view
	};

	static void start();
	static void stop();

	static void schedule(void(*callback)(), uint64_t interval = 0, thread thread = main);
	static void once(void(*callback)(), thread thread = main);
	static void delay(void(*callback)(), uint64_t delay, thread thread = main);

private:
	struct task {
		scheduler::thread thread;
		void(*callback)();
		uint64_t interval;
		uint64_t last_call_time;
		bool is_temp;
	};

	static std::vector<task> main_tasks;
	static std::vector<task> render_tasks;
	static std::vector<task> game_tasks;
	static std::vector<task> client_tasks;
	static std::vector<task> paint_tasks;
	static std::vector<task> view_tasks;

	static std::vector<task>* get_tasks(thread thread);

	static void execute(thread thread);

	static void AWillowHUD_DrawAlwaysOnTopItems(AWillowHUD* aWillowHud);
	static int __cdecl UObject_ProcessEvent(UObject* obj, UFunction* f, void* params, void* a);
};
