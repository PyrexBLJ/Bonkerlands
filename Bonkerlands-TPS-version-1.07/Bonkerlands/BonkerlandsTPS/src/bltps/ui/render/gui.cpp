#include "include.h"

namespace bltps {
	menu_t* gui::menu_data;
	UCanvas* render_t::canvas;
	UEngine* render_t::UEngine;

	UWillowGameViewportClient* render_t::engine;
	AWillowPlayerStats* render_t::playerstats;
	AWillowPlayerController* render_t::controller;
	AWorldInfo* render_t::worldinfo;
	FName render_t::consoleKey;
	AWillowHUD* render_t::hud;

	void CL_DrawText(const wchar* text, int maxChars, float x, float y, float rotation, float xScale, float yScale, unsigned char* color, int style) {
		if (render_t::canvas == nullptr)
			return;

		if (!color)
			color = color::color(255, 255, 255, 255);

		FFontRenderInfo render_info = render()->canvas->CreateFontRenderInfo(0, 1, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, }, { 1.0f, 1.0f });

		render_t::canvas->SetPos(x, y / 1.5, 0);
		render_t::canvas->SetDrawColor(color[0], color[1], color[2], color[3]);
		render_t::canvas->DrawText(FString(text), false, xScale, yScale, &render_info);
	}
	void CL_DrawTextCenter(const wchar* text, int maxChars, float x, float y, float rotation, float xScale, float yScale, unsigned char* color, int style) {
		if (render_t::canvas == nullptr)
			return;

		vec2_t size;
		render_t::canvas->StrLen(FString(text), &size.x, &size.y);

		auto adjusted_x = x - (size.x / 2 * xScale);

		if (!color)
			color = color::color(255, 255, 255, 255);

		FFontRenderInfo render_info = render()->canvas->CreateFontRenderInfo(0, 1, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, }, { 1.0f, 1.0f });

		render_t::canvas->SetPos(adjusted_x, y / 1.5, 0);
		render_t::canvas->SetDrawColor(color[0], color[1], color[2], color[3]);
		render_t::canvas->DrawText(FString(text), 1, xScale, yScale, &render_info);
	}
	void CL_DrawTextRight(const wchar* text, int maxChars, float x, float y, float rotation, float xScale, float yScale, unsigned char* color, int style) {
		if (render_t::canvas == nullptr)
			return;

		vec2_t size;
		render_t::canvas->StrLen(FString(text), &size.x, &size.y);

		auto adjusted_x = x - (size.x * xScale);

		if (!color)
			color = color::color(255, 255, 255, 255);

		FFontRenderInfo render_info = render()->canvas->CreateFontRenderInfo(0, 1, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, }, { 1.0f, 1.0f });

		render_t::canvas->SetPos(adjusted_x, y / 1.5, 0);
		render_t::canvas->SetDrawColor(color[0], color[1], color[2], color[3]);
		render_t::canvas->DrawText(FString(text), 1, xScale, yScale, &render_info);
	}

	void render_t::add_text(const wchar* text, vec2_t pos, int style, text_alignment align, float size, unsigned char* clr) {
		if (!gui::is_ready())
			return;

		float font_height = size;

		if (align == text_alignment::text_left)
			CL_DrawText(text, wcslen(text), pos.x, pos.y / 1.5, 0.f, size, font_height, clr, style);
		else if (align == text_alignment::text_center)
			CL_DrawTextCenter(text, wcslen(text), pos.x, pos.y / 1.5, 0.f, size, font_height, clr, style);
		else
			CL_DrawTextRight(text, wcslen(text), pos.x, pos.y / 1.5, 0.f, size, font_height, clr, style);
	}

	void render_t::add_text(const wchar* text, vec2_t pos, int style, text_alignment align, float size_x, float size_y, unsigned char* clr) {
		if (!gui::is_ready())
			return;

		if (align == text_alignment::text_left)
			CL_DrawText(text, wcslen(text), pos.x, pos.y / 1.5, 0.f, size_x, size_y, clr, style);
		else if (align == text_alignment::text_center)
			CL_DrawTextCenter(text, wcslen(text), pos.x, pos.y / 1.5, 0.f, size_x, size_y, clr, style);
		else
			CL_DrawTextRight(text, wcslen(text), pos.x, pos.y / 1.5, 0.f, size_x, size_y, clr, style);
	}

	void render_t::add_text(const wchar* text, float x, float y, int style, text_alignment align, float size_x, float size_y, unsigned char* clr) {
		if (!gui::is_ready())
			return;

		vec2_t pos = vec2_t(x, y);

		if (align == text_alignment::text_left)
			CL_DrawText(text, wcslen(text), pos.x, pos.y / 1.5, 0.f, size_x, size_y, clr, style);
		else if (align == text_alignment::text_center)
			CL_DrawTextCenter(text, wcslen(text), pos.x, pos.y / 1.5, 0.f, size_x, size_y, clr, style);
		else
			CL_DrawTextRight(text, wcslen(text), pos.x, pos.y / 1.5, 0.f, size_x, size_y, clr, style);
	}

	//void render_t::add_text(const char* text, rect_def_t rect, int style, short flags, float size_x, float size_y, float* clr) {
	//	if (!gui::is_ready())
	//		return;
	//	float font_height = size_y;

	//	if (flags & text_alignment::align_left)
	//	{
	//		auto adjusted_y = (rect.y + (rect.h / 2) + ((game_font->pixelHeight * font_height) / 2));
	//		R_AddCmdDrawText(text, 0x7F, game_font, rect.x, adjusted_y, size_x, font_height, 0.0f, clr, style);
	//	}
	//	else if (flags & text_alignment::align_center)
	//	{
	//		auto adjusted_x = (rect.x + (rect.w / 2) - ((R_TextWidth(text, strlen(text), game_font) / 2) * size_x));
	//		auto adjusted_y = rect.y;
	//		if (flags & align_top)
	//			adjusted_y += rect.h;
	//		if (flags & align_middle)
	//			adjusted_y += ((rect.h / 2) + ((game_font->pixelHeight * font_height) / 2));
	//		else if (flags & align_bottom)
	//			adjusted_y += ((rect.h / 2) + ((game_font->pixelHeight * font_height)));

	//		R_AddCmdDrawText(text, 0x7F, game_font, adjusted_x, adjusted_y, size_x, font_height, 0.0f, clr, style);
	//	}
	//	else
	//	{
	//		auto adjusted_x = ((rect.x + rect.w) - (R_TextWidth(text, 0x7F, game_font) * size_x));
	//		auto adjusted_y = rect.y;
	//		if (flags & align_top)
	//			adjusted_y += rect.h;
	//		if (flags & align_middle)
	//			adjusted_y += ((rect.h / 2) + ((game_font->pixelHeight * font_height) / 2));
	//		else if (flags & align_bottom)
	//			adjusted_y += ((rect.h / 2) + ((game_font->pixelHeight * font_height)));

	//		R_AddCmdDrawText(text, 0x7F, game_font, adjusted_x, adjusted_y, size_x, font_height, 0.0f, clr, style);
	//	}
	//}

	void render_t::add_text(const wchar* text, rectDef_s rect, int style, text_alignment align, float size, unsigned char* clr) {
		if (!gui::is_ready())
			return;

		float font_height = size;

		vec2_t str_size;
		render_t::canvas->StrLen(FString(text), &str_size.x, &str_size.y);

		FFontRenderInfo render_info = canvas->CreateFontRenderInfo(0, 1, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, }, { 1.0f, 1.0f });

		//font_height *= 1.5f;
		if (align == text_alignment::text_left)
		{
			auto adjusted_y = (rect.y + (rect.h / 2) + ((str_size.y) / 2));

			render_t::canvas->SetPos(rect.x, adjusted_y / 1.5, 0);
			render_t::canvas->SetDrawColor(clr[0], clr[1], clr[2], clr[3]);
			render_t::canvas->DrawText(FString(text), 1, size, font_height, &render_info);
		}
		else if (align == text_alignment::text_center)
		{
			auto adjusted_x = (rect.x + (rect.w / 2) - ((str_size.x / 2) * size));
			auto adjusted_y = (rect.y + (rect.h / 2)) - (str_size.y / 2) /** render()->get_height(text, size)) / 2)*/;

			render_t::canvas->SetPos(adjusted_x, adjusted_y / 1.5, 0);
			render_t::canvas->SetDrawColor(clr[0], clr[1], clr[2], clr[3]);
			render_t::canvas->DrawText(FString(text), 1, size, font_height, &render_info);
		}
		else
		{
			auto adjusted_x = ((rect.x + rect.w) - (str_size.x * size));
			auto adjusted_y = (rect.y + rect.h);

			render_t::canvas->SetPos(adjusted_x, adjusted_y / 1.5, 0);
			render_t::canvas->SetDrawColor(clr[0], clr[1], clr[2], clr[3]);
			render_t::canvas->DrawText(FString(text), 1, size, font_height, &render_info);
		}

		//add_rect(rect.x, rect.y, rect.w, rect.h, 2, color(255, 0, 0, 150));
	}

	void render_t::add_text(const wchar* text, rectDef_s rect, int style, text_alignment align, float size_x, float size_y, unsigned char* clr) {
		if (!gui::is_ready())
			return;

		vec2_t str_size;
		render_t::canvas->StrLen(FString(text), &str_size.x, &str_size.y);

		FFontRenderInfo render_info = canvas->CreateFontRenderInfo(0, 1, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, }, {1.0f, 1.0f});

		str_size.y *= 1.5;
		//font_height *= 1.5f;
		if (align == text_alignment::text_left)
		{
			auto adjusted_y = (rect.y);

			render_t::canvas->SetPos(rect.x, adjusted_y / 1.5, 0);
			render_t::canvas->SetDrawColor(clr[0], clr[1], clr[2], clr[3]);
			render_t::canvas->DrawText(FString(text), 1, size_x, size_y, &render_info);
		}
		else if (align == text_alignment::text_center)
		{
			auto adjusted_x = (rect.x + (rect.w / 2) - ((str_size.x / 2) * size_x));
			auto adjusted_y = (rect.y);

			render_t::canvas->SetPos(adjusted_x, adjusted_y / 1.5, 0);
			render_t::canvas->SetDrawColor(clr[0], clr[1], clr[2], clr[3]);
			render_t::canvas->DrawText(FString(text), 1, size_x, size_y, &render_info);
		}
		else
		{
			auto adjusted_x = ((rect.x + rect.w) - (str_size.x * size_x));
			auto adjusted_y = (rect.y + rect.h);

			render_t::canvas->SetPos(adjusted_x, adjusted_y / 1.5, 0);
			render_t::canvas->SetDrawColor(clr[0], clr[1], clr[2], clr[3]);
			render_t::canvas->DrawText(FString(text), 1, size_x, size_y, &render_info);
		}

		//add_rect(rect.x, rect.y, rect.w, rect.h, 2, color(255, 0, 0, 150));
	}

	void render_t::add_text(const wchar* text, rectDef_s rect, int style, text_alignment align, float size, unsigned char* clr, rectDef_s* text_rect) {
		if (!gui::is_ready())
			return;

		int text_alignment;
		switch (align)
		{
		case text_alignment::text_right: text_alignment = 3; break;
		}
	}

	void render_t::add_rect(vec2_t pos, vec2_t size, int width, unsigned char* clr) {
		if (!gui::is_ready())
			return;

		draw_outlined_rect(pos.x, pos.y, size.x, size.y, clr, width);
	}

	void render_t::add_rect(float x, float y, float w, float h, int width, unsigned char* clr) {
		if (!gui::is_ready())
			return;

		draw_outlined_rect(x, y, w, h, clr, width);
	}

	void render_t::add_filled_rect(vec2_t pos, vec2_t size, unsigned char* clr) {
		if (!gui::is_ready())
			return;

		draw_rect(pos.x, pos.y, size.x, size.y, clr);
	}


	void render_t::add_filled_rect(float x, float y, float w, float h, unsigned char* clr) {
		if (!gui::is_ready())
			return;

		draw_rect(x, y, w, h, clr);
	}


	int render_t::get_width(const wchar* text, float scale) {
		vec2_t str_size;
		render_t::canvas->StrLen(FString(text), &str_size.x, &str_size.y);

		return str_size.x * scale;
	}

	int render_t::get_height(const wchar* text, float scale) {
		vec2_t str_size;
		render_t::canvas->StrLen(FString(text), &str_size.x, &str_size.y);

		return str_size.y * scale;
	}

	bool gui::is_ready() {
		return vars && menu_data;
	}

	void gui::draw_slider(float x, float y, int index, menu_item* option)
	{
		constexpr int box_half_calc = (45.f / 2.f) - (25.f / 2.f);
		auto box_rect = rectDef_s((x - 13.f - 40.f) - 67.f,
			y + box_half_calc, 100.f, 25.f);

		switch (option->type)
		{
		case menu_item_type::type_float:
		{
			auto slider_width_calc = (((float)option->get_float() - (float)option->limits.mins.fl_value) / ((float)option->limits.maxs.fl_value - (float)option->limits.mins.fl_value) * box_rect.w);

			/// full slider rect
			render()->add_filled_rect(box_rect.x, box_rect.y, box_rect.w, box_rect.h, vars->menu.clr_main);

			/// value rect
			render()->add_filled_rect(box_rect.x, box_rect.y, slider_width_calc, box_rect.h, vars->menu.clr_accent);

			render()->add_text(libpsutil::string::va("%.2f/%.2f", option->get_float(), option->limits.maxs.fl_value), box_rect, 1, text_center, .4f, .4f * 1.5f, color(255, 255, 255, 255));

			/// outline of full rect
			render()->add_rect(box_rect.x, box_rect.y, box_rect.w, box_rect.h, 1, color(255, 255, 255, 255));
		}
		break;
		case menu_item_type::type_integer:
		{
			auto slider_width_calc = (((float)option->get_int() - (float)option->limits.mins.i_value) / ((float)option->limits.maxs.i_value - (float)option->limits.mins.i_value) * box_rect.w);

			/// full slider rect
			render()->add_filled_rect(box_rect.x, box_rect.y, box_rect.w, box_rect.h, vars->menu.clr_main);

			/// value rect
			render()->add_filled_rect(box_rect.x, box_rect.y, slider_width_calc, box_rect.h, vars->menu.clr_accent);
			render()->add_text(libpsutil::string::va("%i/%i", option->get_int(), option->limits.maxs.i_value), box_rect, 1, text_center, .4f, .4f * 1.5f, color(255, 255, 255, 255));

			/// outline of full rect
			render()->add_rect(box_rect.x, box_rect.y, box_rect.w, box_rect.h, 1, color(255, 255, 255, 255));
		}
		break;
		}
	}

	void gui::draw_value(float x, float y, int index, rectDef_s rect, menu_item* option)
	{
		rect.w = rect.w - 10;
		switch (option->type)
		{
		case menu_item_type::type_float_none:
		{
			render()->add_text(libpsutil::string::va("<%.2f/%.2f>", option->get_float(), option->limits.maxs.fl_value), rect, 0, text_alignment::text_right, 0.7f);
		}
		break;
		case menu_item_type::type_integer_none:
		{
			render()->add_text(libpsutil::string::va("<%i/%i>", option->get_int(), option->limits.maxs.i_value), rect, 0, text_alignment::text_right, 0.7f);
		}
		break;
		}
	}

	void gui::draw_list_box(float x, float y, int index, menu_item* option)
	{
		const auto& width = option->get_items()->size + 35.f;

		constexpr int box_half_calc = (45.f / 2.f) - (25.f / 2.f);
		const auto& box_rect = rectDef_s((x - 13.f - 40.f) - width,
			y + box_half_calc, width + 33.f, option->items.strings.size());

		int option_count = vars->menu.scrollable_amount;
		if (option->get_items()->get_strings().size() < 9)
			option_count = option->get_items()->get_strings().size() + 1;

		// take size (being the size of the longest string in the vector) into account

		render()->add_filled_rect(box_rect.x, box_rect.y, box_rect.w, box_rect.h + ((option_count - 1) * 20.0f), vars->menu.clr_main);
		render()->add_filled_rect(box_rect.x, box_rect.y + ((option->get_items()->get_cursor() - option->get_items()->get_offset()) * 20.f), box_rect.w, 20.0f, color(90, 90, 90, 255));
		render()->add_rect(box_rect.x, box_rect.y, box_rect.w, box_rect.h + ((option_count - 1) * 20.0f), 1, color(255, 255, 255, 255));
		for (int i = 0; i < option->items.strings.size(); i++)
		{
			if (i - option->get_items()->get_offset() >= 0 && i < option->get_items()->get_offset() + vars->menu.scrollable_amount)
			{
				render()->add_text(option->get_items()->get_strings().at(i),
					rectDef_s(
						box_rect.x + 2,
						box_rect.y + ((i - option->get_items()->get_offset()) * 20.0f),
						box_rect.w - 2,
						20.0f), 0, option->type == menu_item_type::type_list_bool ? text_left : text_center, 0.4f, 0.4f, color(255, 255, 255, 255));

				if (option->type != type_list_bool)
					continue;

				render()->add_rect(box_rect.x + width + 15.f,
					box_rect.y + ((i - option->get_items()->get_offset()) * 20.0f) + 5, 8.f, 10.f, 1, color(255, 255, 255, 255));
				if (option->get_items()->get_toggled(i))
				{
					render()->add_filled_rect(box_rect.x + width + 15.f,
						box_rect.y + ((i - option->get_items()->get_offset()) * 20.0f) + 5, 8.f, 10.f, color(255, 255, 255, 255));
				}
			}
		}
	}

	void gui::draw_list_option(float x, float y, int index, menu_item* option)
	{
		constexpr int box_half_calc = (45.f / 2.f) - (25.f / 2.f);
		auto box_rect = rectDef_s((x - 13.f - 40.f) - render()->get_width(option->get_items()->get_string(), .7f),
			y + box_half_calc, render()->get_width(option->get_items()->get_string(), .7f) + 33.f, 25.f);

		switch (option->type)
		{
		case menu_item_type::type_list:
			render()->add_filled_rect(box_rect.x, box_rect.y, box_rect.w, box_rect.h, vars->menu.clr_main);
			render()->add_rect(box_rect.x, box_rect.y, box_rect.w, box_rect.h, 1, color(255, 255, 255, 255));
			render()->add_text(option->get_items()->get_string(), box_rect, 1, text_center, .6f, .6f, color(255, 255, 255, 255));
			break;
		case menu_item_type::type_list_bool:
		{
			wchar_t buf[500];
			for (int i = 0; i < option->get_items()->get_toggles().size(); i++)
			{
				if (option->get_items()->get_toggled(i) != true)
					continue;

				wcscat(buf, option->get_items()->strings[i]);
				if (i + 1 <= (option->get_items()->get_toggles().size() - 1))
					wcscat(buf, L" | ");
			}
			if (buf[0] == '\0')
				wcscpy(buf, L"none");

			box_rect.x = (x - 13.f - 40.f) - render()->get_width(buf, .7f);
			box_rect.w = render()->get_width(buf, .7f) + 33.f;

			render()->add_filled_rect(box_rect.x, box_rect.y, box_rect.w, box_rect.h, vars->menu.clr_main);
			render()->add_rect(box_rect.x, box_rect.y, box_rect.w, box_rect.h, 1, color(255, 255, 255, 255));

			render()->add_text(buf, box_rect, 1, text_center, .6f, .6f, color(255, 255, 255, 255));
		}
		break;
		}
	}
}
