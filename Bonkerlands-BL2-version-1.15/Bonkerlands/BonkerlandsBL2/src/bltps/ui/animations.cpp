#include "include.h"

#define clamp(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

animations anim;

static vec3_t lerp(vec3_t vec0, vec3_t vec1, float t)
{
	return (vec0 + ((vec1 - vec0) * t));
}

static vec2_t lerp(vec2_t vec0, vec2_t vec1, float t)
{
	return (vec0 + ((vec1 - vec0) * t));
}

static float lerp(float f1, float f2, float t)
{
	return (f1 + ((f2 - f1) * t));
}

void animation_data::process_animation()
{
	float scale = clamp((float)(get_time_now() - start_time) / (float)duration, 0.0f, 1.0f);
	float interp = interpolate(scale);

	if (type == anim_default)
		*data_f = start_data_f + (float)move_f * interp;
	else if (type == anim_int)
		*data = start_data + (float)move * interp;
	else if (type == anim_vec2)
		*vec2 = lerp(*vec2, move_vec2, interp);
	else if (type == anim_vec3)
		*vec3 = lerp(*vec3, move_vec3, interp);

	if (get_time_now() >= end_time) {

		for (int i = 0; i < anim.animation_stack.size(); i++) {
			if (this == anim.animation_stack[i])
			{
				auto callback = anim.animation_stack[i]->callback;
				if (callback)
					callback();

				anim.animation_stack.erase(anim.animation_stack.begin() + i);
			}
		}

		_sys_free(this);
	}
}

void animation_data::add_animation(const char* name, void(*callback)(), float(*interpolate)(float), int* Data, int* Move, clock_t Duration, anim_type type)
{
	for (int i = 0; i < anim.animation_stack.size(); i++)
	{
		if (anim.animation_stack[i]->animation_hash == str_hash(name))
			return;
	}

	animation_data* newAnim = (animation_data*)_sys_malloc(sizeof(animation_data));
	if (!newAnim)
		return;

	newAnim->animation_hash = str_hash(name);
	newAnim->interpolate = interpolate;
	newAnim->start_time = get_time_now();
	newAnim->duration = Duration;
	newAnim->end_time = newAnim->start_time + newAnim->duration;

	newAnim->type = type;

	if (newAnim->type == anim_default)
	{
		newAnim->data_f = (float*)Data;
		newAnim->move_f = *(float*)Move;
		newAnim->start_data_f = *(float*)newAnim->data_f;
	}
	else if (newAnim->type == anim_int)
	{
		newAnim->data = Data;
		newAnim->move = *(int*)Move;
		newAnim->start_data = *(int*)newAnim->data;
	}
	else if (newAnim->type == anim_vec2)
	{
		newAnim->vec2 = (vec2_t*)Data;
		newAnim->move_vec2 = *(vec2_t*)Move;
	}
	else if (newAnim->type == anim_vec3)
	{
		newAnim->vec3 = (vec3_t*)Data;
		newAnim->move_vec3 = *(vec3_t*)Move;
	}

	newAnim->callback = callback;

	anim.animation_stack.push_back(newAnim);
}

void animations::add_animation(const char* name, void(*callback)(), float(*interpolate)(float), float* data, float move, clock_t duration)
{
	if (interpolate == nullptr) return;

	animation_data::add_animation(name, callback, interpolate, (int*)data, (int*)&move, duration, anim_default);
}

void animations::add_animation(const char* name, void(*callback)(), float(*interpolate)(float), int* data, int move, clock_t duration)
{
	if (interpolate == nullptr) return;

	animation_data::add_animation(name, callback, interpolate, data, &move, duration, anim_int);
}

void animations::add_animation(const char* name, void(*callback)(), float(*interpolate)(float), vec2_t* data, vec2_t move, clock_t duration)
{
	if (interpolate == nullptr) return;

	animation_data::add_animation(name, callback, interpolate, (int*)data, (int*)&move, duration, anim_vec2);
}

void animations::add_animation(const char* name, void(*callback)(), float(*interpolate)(float), vec3_t* data, vec3_t move, clock_t duration)
{
	if (interpolate == nullptr) return;

	animation_data::add_animation(name, callback, interpolate, (int*)data, (int*)&move, duration, anim_vec3);
}

void animations::run_frame()
{
	for (animation_data* Anim : animation_stack)
		Anim->process_animation();
}

animation_data* animations::find_anim(unsigned int hash)
{
	for (auto& anim : anim.animation_stack)
		if (anim->animation_hash == hash)
			return anim;

	return nullptr;
}

animation_data* animations::find_anim(const char* hash_name)
{
	const auto& hash = str_hash(hash_name);
	for (auto& anim : anim.animation_stack)
		if (anim->animation_hash == hash)
			return anim;

	return nullptr;
}
