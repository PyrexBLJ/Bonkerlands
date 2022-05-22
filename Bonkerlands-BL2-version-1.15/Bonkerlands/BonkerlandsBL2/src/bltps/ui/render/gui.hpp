#pragma once

typedef wchar_t wchar;
namespace bltps {
	enum text_alignment 
	{
		text_left,
		text_center,
		text_right,
	};
	class render_t 
	{
	public:
		static UCanvas* canvas;
		static UWillowGameViewportClient* engine;
		static UEngine* UEngine;
		static AWillowPlayerController* controller;
		static AWillowPlayerStats* playerstats;
		static AWorldInfo* worldinfo;
		static FName consoleKey;
		static AWillowHUD* hud;

		void add_text(const wchar* text, vec2_t pos, int style = 0, text_alignment align = text_left, float size = .7f, unsigned char* clr = color(255, 255, 255, 255));
		void add_text(const wchar* text, vec2_t pos, int style, text_alignment align, float size_x, float size_y, unsigned char* clr);
		void add_text(const wchar* text, float x, float y, int style, text_alignment align, float size_x, float size_y, unsigned char* clr);
		void add_text(const wchar* text, rectDef_s rect, int style, text_alignment align, float size = .7f, unsigned char* clr = color(255, 255, 255, 255));
		void add_text(const wchar* text, rectDef_s rect, int style, text_alignment align, float size_x, float size_y, unsigned char* clr);
		void add_text(const wchar* text, rectDef_s rect, int style, text_alignment align, float size, unsigned char* clr, rectDef_s* text_rect);
		void add_rect(vec2_t pos, vec2_t size, int width, unsigned char* clr);
		void add_rect(float x, float y, float w, float h, int width, unsigned char* clr);
		void add_filled_rect(vec2_t pos, vec2_t size, unsigned char* clr);
		void add_filled_rect(float x, float y, float w, float h, unsigned char* clr);
		int get_width(const wchar* text, float scale);
		int get_height(const wchar* text, float scale);
	};

	class gui 
	{
	public:
		static menu_t* menu_data;
		static bool is_ready();
		static void draw_slider(float x, float y, int index, menu_item* option);
		static void draw_list_box(float x, float y, int index, menu_item* option);
		static void draw_list_option(float x, float y, int index, menu_item* option);
		static void draw_value(float x, float y, int index, rectDef_s rect, menu_item* option);
	};
	static render_t renderer;
	static render_t* render()
	{
		return &renderer;
	}
}