#pragma once

struct menu_t {
	float x, y, w, h;

	int max_options;
	int scrollable_amount;

	color clr_accent;
	color clr_bg;
	color clr_text;
	color clr_outline;
	color clr_main;
};

struct variables {
	menu_t menu;
	//APawn* pawn;

	bool godmode;
	bool noclip;
	bool camerapos;
	FVector savedPos;
	
	bool playersonly = false;
	int weaponlevel;
	int playerspeed = 440;
	int noclipSpeed;
	int jump_height;
	int playerlevel;
	int skillpoints;
	int money;
	int eridium;
	int seraph;
	int Torgue;
	int fov = 65;
	bool ingame;
	bool fpsboost;
	bool freecam;
	APawn* ourPawn;
};

struct rectDef_s
{
	float x, y, w, h;

	rectDef_s() = default;
	rectDef_s(float X, float Y, float W, float H)
	{
		this->x = X;
		this->y = Y;
		this->w = W;
		this->h = H;
	}
};

static color color_white;
static color color_black;
static color color_green; 

void draw_rect(float x, float y, float w, float h, unsigned char* background_color);
void draw_outlined_rect(float x, float y, float width, float height, unsigned char* color, int thickness);
int atoi2(const char* s);