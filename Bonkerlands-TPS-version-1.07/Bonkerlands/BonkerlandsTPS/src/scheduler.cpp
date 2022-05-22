#include "include.h"

// define list

using namespace bltps;

static const uint64_t frame_interval = 0;
std::vector<scheduler::task> scheduler::main_tasks;
std::vector<scheduler::task> scheduler::render_tasks;
std::vector<scheduler::task> scheduler::game_tasks;
std::vector<scheduler::task> scheduler::client_tasks;
std::vector<scheduler::task> scheduler::paint_tasks;
std::vector<scheduler::task> scheduler::view_tasks;

std::vector<scheduler::task>* scheduler::get_tasks(thread thread) {
	if (thread == thread::main) return &main_tasks;
	else if (thread == thread::render) return &render_tasks;
	else if (thread == thread::game) return &game_tasks;
	else if (thread == thread::client) return &client_tasks;
	else if (thread == thread::paint) return &paint_tasks;
	else if (thread == thread::view) return &view_tasks;
}

void scheduler::schedule(void(*callback)(), uint64_t interval, scheduler::thread thread) {
	scheduler::task task;
	task.callback = callback;
	task.interval = interval;
	task.thread = thread;
	task.last_call_time = get_time_now();
	task.is_temp = false;

	get_tasks(thread)->push_back(task);
}

void scheduler::once(void(*callback)(), thread thread) {
	scheduler::task task;
	task.callback = callback;
	task.interval = 0;
	task.thread = thread;
	task.last_call_time = get_time_now();
	task.is_temp = true;

	get_tasks(thread)->push_back(task);
}

void scheduler::delay(void(*callback)(), uint64_t delay, thread thread) {
	scheduler::task task;
	task.callback = callback;
	task.interval = delay;
	task.thread = thread;
	task.last_call_time = get_time_now();
	task.is_temp = true;

	get_tasks(thread)->push_back(task);
}

void scheduler::execute(thread thread) {
	if (bltps::ui::unload_project) return;

	auto tasks = get_tasks(thread);
	for (int i = 0; i < tasks->size(); i++) {
		auto task = tasks->at(i);

		const auto diff = get_time_now() - task.last_call_time;
		if (diff < task.interval)
			continue;

		task.last_call_time = get_time_now();
		task.callback();

		if (task.is_temp) {
			tasks->erase(tasks->begin() + i);
			--i;
		}
	}
}

void scheduler::AWillowHUD_DrawAlwaysOnTopItems(AWillowHUD* aWillowHud)
{
	if (aWillowHud->Canvas)
	{
		render_t::hud = aWillowHud;
		execute(render);
	}
	AWillowHUD_DrawAlwaysOnTopItems_o(aWillowHud);
}

int __cdecl scheduler::UObject_ProcessEvent(UObject* obj, UFunction* f, void* params, void* a)
{
	if (f)
	{
		auto* str = f->GetFullName();
		if (strstr(str, "Function WillowGame.WillowGameViewportClient.PostRender"))
		{
			auto* viewport = (UWillowGameViewportClient*)obj;
			if (viewport)
			{
				auto* parameters = (UWillowGameViewportClient_eventPostRender_Parms*)params; //get the parameters
				if (parameters)
				{
					bltps::render_t::engine = viewport;
					bltps::render_t::canvas = parameters->Canvas;
					auto* controller = bltps::render_t::controller;

					if (controller && vars) 
					{
						if (controller->WorldInfo && controller->WorldInfo->PawnList && bltps::render_t::canvas)
						{
							auto* world_info = controller->WorldInfo;

							vars->pawn = controller->Pawn;
						}
					}
					bltps::render_t::consoleKey = (FName)viewport->ViewportConsole->ConsoleKey;
					//viewport->ViewportConsole->ConsoleKey.Index = 0x000011dd;
				}
			}
		}

		if (strstr(str, "Function Engine.Canvas.SetDrawColor"))
		{
			auto* canvas_data = (UCanvas_execSetDrawColor_Parms*)obj;

			printf("%i %i %i %i\n", canvas_data->R, canvas_data->G, canvas_data->B, canvas_data->A);
		}

		

		/*if (strstr(str, "Function WillowGame.ChallengeManager.OnStatIncrement")) {
			auto* hudmovie = (AWillowPlayerStats*)obj;
			if (hudmovie) {
				blps::render_t::playerstats = hudmovie;
			}
		}*/
		

		/*if (strstr(str, "Function WillowGame.DeveloperPerksDefinition.IsDeveloper")) {
			auto* badassrankparams = (UDeveloperPerksDefinition_execIsDeveloper_Parms*)obj;
			badassrankparams->ReturnValue = true;
		}*/
	}

	return ProcessEvent(obj, f, params, a);
}

detour* UObject_CallFunction_d = nullptr;
int  __cdecl UObject_CallFunction(UObject* obj, void* frame, void* a3, UFunction* func)
{
	auto callerName = obj->GetFullName();
	auto functionName = func->GetFullName();

	if (strstr(functionName, "Function Engine.PlayerController.IsLocalController"))
	{
		if (obj)
			render_t::controller = (AWillowPlayerController*)obj;
	}

	if (strstr(functionName, "Function Engine.PlayerController.ConsoleCommand")) {
		if (obj) {
			auto* console = (APlayerController_execConsoleCommand_Parms*)obj;
			printf("%s\n", console->ReturnValue);
		}
	}
	if (strstr(functionName, "Function Engine.Engine.GetEngine")) {
		auto* engine_data = (UEngine_execGetEngine_Parms*)obj;
		if (engine_data) {
			bltps::render_t::UEngine = (UEngine*)engine_data->ReturnValue;
		}
	}

	if (strstr(functionName, "Function Engine.Engine.GetCurrentWorldInfo")) {
		if (obj) {
			auto* world = (UEngine_execGetCurrentWorldInfo_Parms*)obj;
			bltps::render_t::worldinfo = (AWorldInfo*)world->ReturnValue;
			//printf("got worldinfo\n");
		}
	}

	if (strstr(functionName, "Function WillowGame.WillowPlayerController.StartNewPlaySession")) {
		if (obj) {
			vars->ingame = true;
			//printf("StartNewPlaySession set true\n");
		}
	}
	if (strstr(functionName, "Function WillowGame.WillowPlayerController.PostBeginPlay")) {
		if (obj) {
			vars->ingame = true;
			//printf("PostBeginPlay set true\n");
		}
	}
	if (strstr(functionName, "Function Engine.PlayerController.NotifyDisconnect")) {
		if (obj) {
			vars->ingame = false;
			//printf("NotifyDisconnect set false\n");
		}
	}
	if (strstr(functionName, "Function WillowGame.PauseGFxMovie.CompleteQuitToMenu")) {
		if (obj) {
			vars->ingame = false;
			//printf("CompleteQuitToMenu set false\n");
		}
	}

	//if (functionName.find("") != std::string::npos)
	//{
	//	printf("%s\n0x%X\n\n", func->GetFullName(), func->Func);
	//}

	return UObject_CallFunction_d->invoke<int>(obj, frame, a3, func);
}

void scheduler::start() {
	*(uintptr_t*)AWillowHUD_DrawAlwaysOnTopItems_VTABLE = *(uintptr_t*)AWillowHUD_DrawAlwaysOnTopItems;
	*(uintptr_t*)UObject_ProcessEvent_VTABLE = *(uintptr_t*)UObject_ProcessEvent;

	UObject_CallFunction_d = new detour(UObject_CallFunction_a, UObject_CallFunction);
	//appLoadFileFromString_d = new detour(0x005E59C, appLoadFileFromString);//0x005E59C
}

void scheduler::stop() {
	*(uintptr_t*)AWillowHUD_DrawAlwaysOnTopItems_VTABLE = AWillowHUD_DrawAlwaysOnTopItems_a;
	*(uintptr_t*)UObject_ProcessEvent_VTABLE = UObject_ProcessEvent_a;

	delete UObject_CallFunction_d;
	//delete appLoadFileFromString_d;
}