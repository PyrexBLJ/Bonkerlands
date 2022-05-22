#pragma once

enum CGUIAnimStates
{
	gui_noanim = 0,
	gui_switching_tabs_1,
	gui_switching_tabs_2,
	gui_scroll_bar,
	gui_animpending = 69420
};

enum anim_type
{
	anim_default,
	anim_int,
	anim_vec2,
	anim_vec3,
	anim_max
};

class animation_data
{
public:
	uint32_t animation_hash;

	clock_t start_time;
	clock_t move_time;
	clock_t end_time;

	anim_type type;

	void(*callback)();
	float(*interpolate)(float val);
	union
	{
		int* data;
		float* data_f;
		vec3_t* vec3;
		vec2_t* vec2;
	};
	union
	{
		int start_data;
		float start_data_f;
	};
	
	union
	{
		int move;
		float move_f;
		vec2_t move_vec2;
		vec3_t move_vec3;
	};

	int duration;

	int* state;
	int state_type;
	int state_end_type;

	void process_animation();
	static void add_animation(const char* name, void(*callback)(), float(*interpolate)(float), int* Data, int* Move, clock_t Duration, anim_type type = anim_default);
};

class animations
{
public:
	~animations()
	{
		for (auto& anim : animation_stack)
			if (anim)
				_sys_free(anim);

		animation_stack.clear();
	}

	void add_animation(const char* name, void(*callback)(), float(*interpolate)(float), float* data, float move, clock_t duration);
	void add_animation(const char* name, void(*callback)(), float(*interpolate)(float), int* data, int move, clock_t duration);
	void add_animation(const char* name, void(*callback)(), float(*interpolate)(float), vec2_t* data, vec2_t move, clock_t duration);
	void add_animation(const char* name, void(*callback)(), float(*interpolate)(float), vec3_t* data, vec3_t move, clock_t duration);
	void run_frame();

	animation_data* find_anim(unsigned int hash);

	animation_data* find_anim(const char* hash_name);

	std::vector<animation_data*> animation_stack;
};

extern animations anim;
