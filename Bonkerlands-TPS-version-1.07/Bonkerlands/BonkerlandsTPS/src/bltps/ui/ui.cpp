#include "include.h"
#include <set>

namespace bltps {
	variables* vars;
	ui* _menu;

	std::string file_name;
	detour* appLoadFileFromString_d = nullptr;
	int appLoadFileFromString(FString* output, wchar_t* buffer, int file_manager, int arg4)
	{
		if (wcsstr(buffer, L"Bonk") != nullptr)
		{
			static char* s_buffer = nullptr;
			
			int file_size = libpsutil::filesystem::get_file_size(BONKERLANDS_DIR + file_name);
			sys_memory_info* container_info;
			sys_memory_get_user_memory_size(container_info);
			if (container_info->available_user_memory < file_size * 3) {
				toast::show(L"Bonkerlands", L"File Too Large", 3500, nullptr);
				appLoadFileFromString_d->restore();
				return false;
			}
			sys_memory_allocate(20 * 1024 * 1024, SYS_MEMORY_PAGE_SIZE_1M, (sys_addr_t*)&s_buffer);
			libpsutil::filesystem::read_file(BONKERLANDS_DIR + file_name, s_buffer, file_size);

			if (s_buffer[0] == '\0') {
				toast::show(L"Bonkerlands", L"Failed To Read File", 3500, nullptr);
				appLoadFileFromString_d->restore();
				return false;
			}

			appBufferToString(output, s_buffer, file_size);

			memset(s_buffer, 0, file_size);
			sys_memory_free((sys_addr_t)s_buffer);

			toast::show(L"Bonkerlands", L"File Run", 3500, nullptr);
			appLoadFileFromString_d->restore();
			return true;
		}
	}

	void runPatch() {
		auto* ptr = (wchar_t*)_menu->get_current()->get_option(_menu->get_current()->get_cursor())->get_name();

		char* buf = new char[100];
		mbstate_t state;
		_wcsrtombs(buf, &ptr, wcslen(ptr), &state);
		file_name = buf;
		appLoadFileFromString_d = new detour(appLoadFileFromString_a, appLoadFileFromString);//0x005E59C
		render_t::controller->ConsoleCommand(L"exec Bonk", 0);
		delete buf;
	}

	bool InGame() {
		if (bltps::render_t::controller == nullptr)
			return false;

		return bltps::vars->ingame;
	}


	bool ui::unload_project;
	void ui::on_build() {
		vars->menu = { 100, 60, 420, 400 };
		vars->menu.max_options = 8;
		vars->menu.scrollable_amount = 9;
		vars->menu.clr_accent = color(91, 43, 173, 255);
		vars->menu.clr_bg = color(56, 56, 56, 255);
		vars->menu.clr_main = color(33, 33, 33, 255);
		vars->menu.clr_outline = color(255, 255, 255, 255);
		vars->menu.clr_text = color(255, 255, 255, 255);

		gui::menu_data = &vars->menu;

		color_white = color(255, 255, 255, 255);
		color_black = color(0, 0, 0, 170);
		color_green = color(0, 255, 0, 255);

		// Main tabs
		auto player_menu = this->find_tab(L"Player");
		auto patch_menu = this->find_tab(L"Patches");
		auto visual_menu = this->find_tab(L"Visuals");
		auto save_menu = this->find_tab(L"Save");

		player_menu->add_option(new menu_item(L"God Mode", []() {
			if (InGame()) {
				render_t::controller->bGodMode = vars->godmode;
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
				vars->godmode = false;
			}
			}, type_bool, &vars->godmode));
		player_menu->add_option(new menu_item(L"No-Clip", []() {
			if (InGame()) {
				if (vars->noclip) {
					vars->ourPawn = render_t::controller->Pawn;
					render_t::controller->ServerSpectate();
					render_t::controller->bCollideWorld = false;
					render_t::controller->SpectatorCameraSpeed = (float)vars->noclipSpeed;
				}
				else {
					vars->ourPawn->Location = render_t::controller->Location;
					render_t::controller->Possess(vars->ourPawn, true);
				}
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
				vars->noclip = false;
			}
			}, type_bool, &vars->noclip));
		player_menu->add_option(new menu_item(L"No-Clip Speed", &vars->noclipSpeed, 100, 1000, 10000, []() {
			render_t::controller->SpectatorCameraSpeed = (float)vars->noclipSpeed;
			toast::show(L"Bonkerlands", L" No-Clip Speed Set", 3500, nullptr);
			}, type_integer_none));
		player_menu->add_option(new menu_item(L"Save Location", []() {
			if (InGame()) {
				vars->savedPos = render_t::controller->Pawn->GetPawnViewLocation();
				toast::show(L"Bonkerlands", L"Saved Location", 3500, nullptr);
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
			}
			}, type_func));
		player_menu->add_option(new menu_item(L"Load Location", []() {
			if (InGame()) {
				if (!vars->savedPos.empty()) {
					render_t::controller->NoFailSetPawnLocation(render_t::controller->Pawn, vars->savedPos);
					toast::show(L"Bonkerlands", L"Loaded Location", 3500, nullptr);
				}
				else {
					toast::show(L"Bonkerlands", L"No Saved Location", 3500, nullptr);
				}
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
			}
			}, type_func, nullptr, L"Must Save Position First"));
		player_menu->add_option(new menu_item(L"Quit Without Saving", []() {
			if (InGame()) {
				render_t::controller->ConsoleCommand(L"disconnect", 0);
				vars->ingame = false;
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
			}
			}, type_func));
		player_menu->add_option(new menu_item(L"Freeze Time", []() {
			render_t::hud->WorldInfo->bPlayersOnly = vars->playersonly;
			}, type_bool, &vars->playersonly));
		player_menu->add_option(new menu_item(L"Instant Kill", []() {
			if (InGame()) {
				if(render_t::controller->Pawn->Weapon != nullptr){
					render_t::controller->Pawn->Weapon->InstantHitDamageBaseValue = 999999999999;
					toast::show(L"Bonkerlands", L"Damage Set", 3500, nullptr);
				}
				else {
					toast::show(L"Bonkerlands", L"no gun chief", 3500, nullptr);
				}
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
			}
			}, type_func, nullptr, L"Sets Current Held Weapon Damage"));
		player_menu->add_option(new menu_item(L"Player Speed", &vars->playerspeed, 10, 0, 5000, []() {
			if (InGame()) {
				render_t::controller->Pawn->GroundSpeedBaseValue = vars->playerspeed;
				render_t::controller->Pawn->GroundSpeed = vars->playerspeed;
				render_t::controller->Pawn->AirSpeedBaseValue = vars->playerspeed + 60;
				render_t::controller->Pawn->AirSpeed = vars->playerspeed + 60;
				toast::show(L"Bonkerlands", L"Speed Set", 3500, nullptr);
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
			}
			}, type_integer_none, L"Default is 440"));
		player_menu->add_option(new menu_item(L"Jump Height", &vars->jump_height, 10, 0, 5000, []() {
			if (InGame()) {
				render_t::controller->Pawn->JumpZ = vars->jump_height;
				toast::show(L"Bonkerlands", L"Jump Height Set", 3500, nullptr);
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
			}
			}, type_integer_none, L"Default is 630"));
		player_menu->add_option(new menu_item(L"Open Fast Travel Station", []() {
			if (InGame()) {
				_menu->remove_flags(open);
				render_t::controller->PlayGfxMovieDefinition(L"UI_FastTravelStation.FastTravelStation_ThirdPerson_Definition");
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
			}
			}, type_func, nullptr, L"Works Anywhere"));
		player_menu->add_option(new menu_item(L"Console Command", []() {
			key.start(osk_wide, [](osk_callback_data_t* data, void* arg) {
				render_t::controller->ConsoleCommand(data->message.wide, 0);
					  }, nullptr);
			}, type_func, nullptr, L"say, set, FOV, etc."));
		
		visual_menu->add_option(new menu_item(L"Toggle Camera Position", []() {
			render_t::controller->ConsoleCommand(vars->camerapos ? L"camera 3rd" : L"camera 1st", 0);
			}, type_bool, & vars->camerapos, L"Swaps From 1st Person to 3rd Person"));
		visual_menu->add_option(new menu_item(L"Toggle HUD", []() {
			render_t::controller->ConsoleCommand(L"togglehud", 0);
			}, type_func));
		visual_menu->add_option(new menu_item(L"FOV 90", []() {
			if (InGame()) {
				render_t::controller->ConsoleCommand(L"FOV 90", 0);
				toast::show(L"Bonkerlands", L"FOV Set", 3500, nullptr);
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
			}
			}, type_func));
		visual_menu->add_option(new menu_item(L"Show FPS", []() {
			render_t::controller->ConsoleCommand(L"stat fps", 0);
			}, type_func));
		visual_menu->add_option(new menu_item(L"FPS Boost", []() {
			if (vars->fpsboost) {
				render_t::controller->ConsoleCommand(L"USEPOSTPROCESS WillowEngineMaterials.RyanScenePostProcess", 0);
				render_t::UEngine->bSmoothFrameRate = false;
				render_t::UEngine->bSmoothFrameRateRuntimeToggle = false;
				toast::show(L"Bonkerlands", L"FPS Boost On", 3500, nullptr);
			}
			else {
				render_t::controller->ConsoleCommand(L"USEPOSTPROCESS WillowEngineMaterials.WillowScenePostProcess", 0);
				render_t::UEngine->bSmoothFrameRate = true;
				render_t::UEngine->bSmoothFrameRateRuntimeToggle = true;
				toast::show(L"Bonkerlands", L"FPS Boost Off", 3500, nullptr);
			}
			}, type_bool, & vars->fpsboost));
			


		save_menu->add_option(new menu_item(L"Player Level", &vars->playerlevel, 1, 1, 70, []() {
			if (InGame()) {
				//level
				render_t::hud->WPRI->ExpPointsNextLevelAt = vars->playerlevel;
				//new level max xp value
				render_t::hud->WPRI->ExpPointsNextLevelAtModifierStack.Max = render_t::controller->GetExpPointsRequiredForLevel(vars->playerlevel);
				render_t::hud->WPRI->LevelUpCount = render_t::controller->GetExpPointsRequiredForLevel(vars->playerlevel);
				//earn the xp
				render_t::controller->ExpEarn(render_t::controller->GetExpPointsRequiredForLevel(vars->playerlevel-1) - render_t::controller->GetExpPoints(), 0, 0);
				toast::show(L"Bonkerlands", L"Level Set", 3500, nullptr);
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
			}
			}, type_integer_none, L"Works for increasing level"));
		save_menu->add_option(new menu_item(L"Skill Points", &vars->skillpoints, 1, 0, 70, []() {
			if (InGame()) {
				render_t::hud->WPRI->bClassModIsBuffingTeamShields = vars->skillpoints; //idk why its bClassModIsBuffingTeamShields on tps the structure is all kinds of fucked
				toast::show(L"Bonkerlands", L"Skill Points Set", 3500, nullptr);
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
			}
			}, type_integer_none, L"Should Have 68 at lvl 70"));
		save_menu->add_option(new menu_item(L"Set Money", &vars->money, 1000, 0, 99999999, []() {
			if (InGame()) {
				render_t::hud->WPRI->Currency[1].CurrentAmount = vars->money;
				toast::show(L"Bonkerlands", L"Money Set", 3500, nullptr);
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
			}
			}, type_integer_none));
		save_menu->add_option(new menu_item(L"Set Moonstones", &vars->eridium, 1, 0, 500, []() {
			if (InGame()) {
				render_t::hud->WPRI->Currency[2].CurrentAmount = vars->eridium;
				toast::show(L"Bonkerlands", L"Moonstones Set", 3500, nullptr);
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
			}
			}, type_integer_none));
		save_menu->add_option(new menu_item(L"Respec Skill Points", []() {
			if (InGame()) {
				_menu->remove_flags(open);
				render_t::controller->VerifySkillRespec();
			}
			}, type_func, nullptr, L"Respec on the go"));
		save_menu->add_option(new menu_item(L"Add 5 Golden Keys", []() {
			if (InGame()) {
				render_t::controller->AddGoldenKeysFromSource(0, 5);
				toast::show(L"Bonkerlands", L"5 Keys Added", 3500, nullptr);
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
			}
			}, type_func));
		save_menu->add_option(new menu_item(L"Set High Badass Rank", []() {
			if (InGame()) {
				render_t::controller->SetBadassPoints(9999999);
				toast::show(L"Bonkerlands", L"High Badass Rank Set", 3500, nullptr);
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
			}
			}, type_func));
		/*save_menu->add_option(new menu_item(L"Reset Badass Rank", []() {
			if (InGame()) {
				render_t::controller->ResetBadass();
				toast::show(L"Bonkerlands", L"Badass Rank Reset", 3500, nullptr);
			}
			else {
				toast::show(L"Bonkerlands", L"Must Be In-Game", 3500, nullptr);
			}
			}, type_func));*/

		this->menu_stack.push(player_menu);
	}

	void ui::on_draw() {
		if (render_t::canvas == nullptr)
			return;

		render_t::canvas->Font = UObject::FindObject<UFont>(Font_UI_Fonts_font_willowbody_18pt);

		if (render_t::canvas->Font == nullptr)
			return;

		if (this->get_open())
		{
			const auto menu_x = vars->menu.x;
			const auto menu_y = vars->menu.y;
			const auto menu_w = vars->menu.w;
			const auto menu_h = vars->menu.h;

			const auto& tab_bar_height = 60.f;
			const auto& title_bar_height = 80.0f;
			const auto& vertical_scroll_width = 20.f;

			const auto& adjusted_height = (title_bar_height - tab_bar_height);

			auto* cur = this->get_current();
			const auto& cur_cursor = cur->get_cursor();

			if (this->get_current()->get_count() == 0)
				return;

			auto* cur_option = cur->get_option(cur_cursor);

			rectDef_s tab_rect = { menu_x, menu_y + tab_bar_height, menu_w - vertical_scroll_width, tab_bar_height };
			rectDef_s vertical_scroll_rect = { menu_x + (menu_w - vertical_scroll_width), menu_y + (title_bar_height + tab_bar_height), vertical_scroll_width, menu_h - adjusted_height };

			const auto m_vars = gui::menu_data;

			auto* draw = render();

			int hashed_label = str_hash("bonkers");

			// version
			draw->add_text(L"1.0.3", rectDef_s(5, 10, draw->get_width(L"1.0.3", 0.3f), 10), 4, text_alignment::text_left, 0.6, 0.6 * 1.5, color_white);

			draw->add_text(L"Brought to you by Pyrex & Faultz", rectDef_s(5, (render()->canvas->SizeY - 46) * 1.5f, render()->get_width(L"Brought to you by Pyrex & Faultz", .7f), 20), 1, text_alignment::text_left, 0.7f);
			
			//how to use
			draw->add_text(L"Select: Square  Back: Circle  Scroll: Dpad", rectDef_s(5, (render()->canvas->SizeY - 75) * 1.5f, render()->get_width(L"Select: Square  Back: Circle  Scroll: Dpad", .7f), 20), 1, text_alignment::text_left, 0.7f);
			
			// title panel
			draw->add_filled_rect(menu_x, menu_y, menu_w, title_bar_height, m_vars->clr_accent);

			/// title text
			draw->add_text(L"Bonkerlands", rectDef_s(menu_x, menu_y, menu_w, title_bar_height), 4, text_alignment::text_center, 1.2f, 1.2f * 1.5f, color_white);

			/// tab panel
			draw->add_filled_rect(menu_x, menu_y + (title_bar_height), menu_w, tab_bar_height, vars->menu.clr_main);

			/// option panel
			draw->add_filled_rect(menu_x, menu_y + (title_bar_height + tab_bar_height), menu_w - (vertical_scroll_width), (menu_h - adjusted_height), m_vars->clr_bg);

			auto draw_text_spaced = [](std::vector<menu_tab*> Text, int NumberOfText, int Underlined, float W, float x, float y, float scale, color Color, color UnderLineColor)
			{
				float WidthTotal = 0.0f;
				float fTextWidth[10] = { 0.0f };
				float fTextHeight = 0.0f;

				const auto& scaled_height = (scale * 1.5f);

				for (int i = 0; i < NumberOfText; i++)
				{
					WidthTotal += fTextWidth[i] = render()->get_width(Text[i]->get_name(), scale);
				}


				float DeadSpaceDelta = (float)((W - WidthTotal) / (NumberOfText + 1));

				float PreviousWidth = DeadSpaceDelta;
				for (int i = 0; i < NumberOfText; i++)
				{
					vec2_t str_size;
					render_t::canvas->StrLen(FString(Text[i]->get_name()), &str_size.x, &str_size.y);

					fTextHeight = (str_size.y);

					color col = Color;

					if(Underlined == i && !_menu->test_flags(interactable))
						col = UnderLineColor;

					if (Underlined == i)
						render()->add_filled_rect(x + PreviousWidth - 2, y + (fTextHeight + 10) - 75, fTextWidth[i] + 4, 3, UnderLineColor);

					render()->add_text(Text[i]->get_name(), vec2_t(x + PreviousWidth, y), 1, text_left, scale, scaled_height, col);
					PreviousWidth += fTextWidth[i] + DeadSpaceDelta;
				}
			};

			for (int i = 0; i < this->get_tabs().size(); i++)
			{
				draw_text_spaced(this->get_tabs(), this->get_tabs().size(), this->tab_scroll, menu_w, tab_rect.x, tab_rect.y + (tab_rect.h) + 35, 0.59f, color(255, 255, 255, 255), m_vars->clr_accent);
			}

			float scrollable_area = vars->menu.max_options * 47;
			float viewable_ratio = scrollable_area / (float)((cur->get_count() * 45.f));
			float thumb_height = scrollable_area * viewable_ratio;
			float scroll_jump = (scrollable_area - thumb_height) / (float)(cur->get_count() - 1);

			color scroll_area_color = color(55, 55, 55, 255);
			if(cur->get_count() <= vars->menu.max_options)
			{
				thumb_height = scrollable_area;
				scroll_jump = 0;
				scroll_area_color.a = 255;
			}

			static float scroll_inc;
			float selected_vert_bar = ((cur->get_cursor()) * scroll_jump);
			float scroll_increment = fabsf(scroll_inc - selected_vert_bar) / 2;

			if(scroll_inc < selected_vert_bar && scroll_increment)
				scroll_inc += scroll_increment;
			else if(scroll_inc > selected_vert_bar && scroll_increment != 0)
				scroll_inc -= scroll_increment;

			// vertical scrollbar panel
			draw->add_filled_rect(menu_x + (menu_w - vertical_scroll_width), menu_y + (title_bar_height + tab_bar_height), vertical_scroll_width, menu_h - adjusted_height, color(33, 33, 33, 255));

			draw->add_filled_rect(menu_x + (menu_w - vertical_scroll_width), menu_y + (title_bar_height + tab_bar_height), vertical_scroll_width, menu_h - adjusted_height, color(24, 23, 24, 255));

			draw->add_filled_rect(vertical_scroll_rect.x + 5, (vertical_scroll_rect.y + 5) + scroll_inc, vertical_scroll_rect.w - 10, thumb_height - 10, scroll_area_color);

			// tooltip rect
			static rectDef_s tooltip_rect;

			// tooltip panel
			draw->add_filled_rect(menu_x, menu_y + (menu_h + tab_bar_height + tab_bar_height), menu_w, tooltip_rect.h + 50, color(33, 33, 33, 255));

			// tooltip text
			draw->add_text(cur_option->get_tooltip(), rectDef_s(menu_x + 10.f, (menu_y + (menu_h + (tab_bar_height *1.5) /*+ 50.f*/)), menu_w, tooltip_rect.h + 50.f), 1, text_alignment::text_left, 0.6f, color_white);

			static rectDef_s option_panel_rect = { menu_x, menu_y + (title_bar_height + tab_bar_height), menu_w - (vertical_scroll_width), (menu_h - adjusted_height) };

			int on_screen_option_count = cur->get_cursor() >= vars->menu.max_options ? ((vars->menu.max_options - 1) * 45.f) : (cur->get_cursor() * 45.f);
			static float horz_scroll_inc;
			float hselected_vert_bar = on_screen_option_count;
			float hscroll_increment = fabsf(horz_scroll_inc - hselected_vert_bar) / 2;

			if(horz_scroll_inc < hselected_vert_bar && hscroll_increment)
				horz_scroll_inc += hscroll_increment / 2;
			else if(horz_scroll_inc > hselected_vert_bar && hscroll_increment != 0)
				horz_scroll_inc -= hscroll_increment / 2;

			// horz scrollbar panel
			draw->add_filled_rect(menu_x, option_panel_rect.y + horz_scroll_inc, (menu_w - vertical_scroll_width), 45, color(15, 15, 15, 255));

			// bottom menu line
			draw->add_filled_rect(menu_x, option_panel_rect.y + option_panel_rect.h + 5, 1, 1, m_vars->clr_accent);

			const auto& option_x_offset = 10.f;
			const auto& option_y_offset = 10.f;
			for (int i = 0; i < cur->get_count(); i++)
			{
				if (i - cur->get_cursor_offset() >= 0 && i < cur->get_cursor_offset() + vars->menu.max_options) {
					auto* option = cur->get_option(i);

					const auto& option_x = option_panel_rect.x + option_x_offset;
					const auto& option_y = option_panel_rect.y + ((i - cur->get_cursor_offset()) * 45.f);

					const auto& option_text_x = option_x;
					const auto& option_text_y = (option_y + 98.f);

					draw->add_text(option->get_name(), rectDef_s(
						menu_x + 5, option_panel_rect.y + ((i - cur->get_cursor_offset()) * 45.f),
						(menu_w - vertical_scroll_width - 5), 45), 1, text_left, 0.59f, 0.59f * 1.5f, color_white);

					constexpr auto box_alignment = (45.f / 2.f) - (25.f / 2.f);

					switch (option->type)
					{
					case menu_item_type::type_bool:
						draw->add_filled_rect(option_x + (menu_w - vertical_scroll_width) - 40.f,
							static_cast<float>(option_y) + box_alignment, 20.f, 25.f, vars->menu.clr_main);
						draw->add_rect(option_x + (menu_w - vertical_scroll_width) - 40.f,
							static_cast<float>(option_y) + box_alignment, 20.f, 25.f, 1, color(255, 255, 255, 255));
						if (option->get_bool())
							draw->add_filled_rect(option_x + (menu_w - vertical_scroll_width) - 40.f + 1,
								static_cast<float>(option_y) + box_alignment + 1, 18.f, 23.f, color(255, 255, 255, 255));
						break;
					case menu_item_type::type_float_none:
					case menu_item_type::type_integer_none:
					{
						gui::draw_value(option_x + (menu_w - vertical_scroll_width), option_y, i, rectDef_s(
							menu_x + 5, option_panel_rect.y + (((i-1) - cur->get_cursor_offset()) * 45.f),
							(menu_w - vertical_scroll_width - 5), 45), option);
					}
					break;
					case menu_item_type::type_float:
					case menu_item_type::type_integer:
						gui::draw_slider(option_x + (menu_w - vertical_scroll_width), option_y, i, option);
						break;
					case menu_item_type::type_list:
					case menu_item_type::type_list_bool:
						gui::draw_list_option(option_x + (menu_w - vertical_scroll_width), option_y, i, option);
					}
				}

				if (this->test_flags(drop_down_open) && (cur_option->type == type_list_bool || cur_option->type == type_list))
				{
					gui::draw_list_box(option_panel_rect.x + option_x_offset + (menu_w - vertical_scroll_width), option_panel_rect.y + ((cur_cursor * 45.f)), cur_option->get_items()->selected_item, cur_option);
				}
			}
		}
	}

	void ui::on_run() {

		if (this->get_open()) {
			static auto tick = 0;
			auto* current_menu = this->get_current();
			if (!current_menu) return;

			auto current_scroll = current_menu->get_cursor();

			if (this->get_current()->get_count() == 0)
				return;

			auto switch_tab = (anim.find_anim(str_hash("close_menu")) || anim.find_anim(str_hash("open_menu")));
			if (switch_tab)
				return;

			this->current_option = current_menu->get_option(current_scroll);
			if (!current_option)
				return;

			if (is_down(btn_dpad_up)) {
				if (this->test_flags(drop_down_open)) {
					auto size = current_option->items.strings.size();
					current_option->items.selected_item = current_option->items.selected_item - 1;
					if (current_option->items.selected_item < 0) {
						current_option->items.selected_item = size - 1;
						current_option->items.selected_item_offset = size - vars->menu.scrollable_amount + 1;
						if (current_option->items.selected_item_offset < 0)
							current_option->items.selected_item_offset = 0;
					}
					if (current_option->items.selected_item_offset > 0)
						current_option->items.selected_item_offset--;
				}
				else if (this->test_flags(interactable)) {
					short position = current_menu->get_cursor() - 1;
					if (position < 0) {
						position = current_menu->get_count() - 1;
						current_menu->set_cursor_offset(current_menu->get_count() - vars->menu.max_options + 1);
						if (current_menu->get_cursor_offset() < 0)
							current_menu->set_cursor_offset(0);
					}
					if (current_menu->get_cursor_offset() > 0) {
						current_menu->set_cursor_offset(current_menu->get_cursor_offset() - 1);
					}
					current_menu->set_cursor(position);

				}
				libpsutil::sleep(150);
			}
			else if (is_down(btn_dpad_down)) {
				if (this->test_flags(drop_down_open)) {
					auto size = current_option->items.strings.size();
					current_option->items.selected_item = current_option->items.selected_item + 1;
					if (size > 6)
						if (current_option->items.selected_item >= vars->menu.scrollable_amount)
							current_option->items.selected_item_offset++;

					if (current_option->items.selected_item > size - 1) {
						current_option->items.selected_item = 0;
						if (size > vars->menu.scrollable_amount)
							current_option->items.selected_item_offset = 0;
					}

				}
				else if (this->test_flags(interactable)) {
					short position = current_menu->get_cursor() + 1;
					if (current_menu->get_count() > vars->menu.max_options)
						if (position >= vars->menu.max_options)
							current_menu->set_cursor_offset(current_menu->get_cursor_offset() + 1);
					if (position > current_menu->get_count() - 1) {
						position = 0;
						if (current_menu->get_count() > vars->menu.max_options) {
							current_menu->set_cursor_offset(0);
						}
					}
					current_menu->set_cursor(position);
				}

				libpsutil::sleep(150);
			}
			else if (is_down(btn_square, CELL_PAD_BTN_OFFSET_DIGITAL2))
			{
				if (current_option) {

					if (!this->test_flags(interactable))
					{
						this->set_flags(interactable);

						libpsutil::sleep(150);

						return;
					}

					if (current_option->type == type_bool && current_option->value_ptr) {
						*(bool*)current_option->value_ptr = current_option->get_bool() ^ true;
					}

					if (current_option->get_callback() && current_option->type != type_menu && current_option->type != type_list) {
						scheduler::once(current_option->get_callback(), scheduler::thread::render);
					}

					if (current_option->type == type_list) {
						if (this->test_flags(drop_down_open)) {
							current_option->items.current_str = current_option->items.strings[(int)current_option->items.selected_item];
							*(int*)current_option->value_ptr = (int)current_option->items.selected_item;

							current_option->items.open = false;

							this->remove_flags(drop_down_open);
						}
						else
						{
							current_option->items.size = this->calc_sizes(current_option->items.strings, .49f);
							current_option->items.open = true;

							this->set_flags(drop_down_open);
						}
					}

					if (current_option->type == type_list_bool)
					{
						if (!current_option->items.open)
						{
							current_option->items.size = this->calc_sizes(current_option->items.strings, .49f);
							current_option->items.open = true;

							this->set_flags(drop_down_open);
						}
						else if (current_option->items.open)
						{
							current_option->items.toggles[current_option->items.selected_item] = current_option->items.toggles[current_option->items.selected_item] ^ true;
						}
					}
					if (current_option->type == type_menu) {
						auto* menu = this->find_menu(current_option->get_name());
						if (menu) {
							if (menu->get_count() == 0) {
								printf("Menu is empty\n");
							}
							else {
								auto stack = this->menu_stack.top();

								this->menu_stack.push(menu);
								this->menu_stack.top()->set_parent(stack);
							}
						}
					}
				}
				libpsutil::sleep(150);
			}
			else if (is_down(btn_triangle, CELL_PAD_BTN_OFFSET_DIGITAL2))
			{
			if (current_option) {
				if (!this->test_flags(interactable))
				{
					this->set_flags(interactable);

					libpsutil::sleep(150);

					return;
				}

				if (current_option->get_callback() && current_option->type == type_integer_none) {
					scheduler::once([]() {
						key.start(osk_ansi, [](osk_callback_data_t* data, void* arg) {
							auto* item = _menu->get_current()->get_option(_menu->get_current()->get_cursor());
							std::string val = data->message.ansi;
							val.erase(remove_if(val.begin(), val.end(), [](char c) { return !isdigit(c); }), val.end());
							int ret = atoi2(val.data());
							if (ret < item->limits.mins.i_value) {
								ret = item->limits.mins.i_value;
							}
							else if (ret > item->limits.maxs.i_value) {
								ret = item->limits.maxs.i_value;
							}
							item->set_int(ret);
							}, nullptr);
						}, scheduler::thread::render);
				}
			}
			libpsutil::sleep(150);
			}
			else if (is_down(btn_circle, CELL_PAD_BTN_OFFSET_DIGITAL2)) {
				if (this->test_flags(drop_down_open))
				{
					current_option->get_items()->open = false;
					remove_flags(drop_down_open);
				}
				else if (test_flags(interactable))
				{
					if (this->menu_stack.size() > 1) {
						this->menu_stack.pop();
					}
					else
					{
						remove_flags(interactable);
					}
					//render_t::controller->PlayUIAkEvent(UObject::FindObject<UAkEvent>("AkEvent Ake_UI.UI_Mission.Ak_Play_UI_Mission_Reward"));
				}
				else
				{
					remove_flags(open);
				}
				libpsutil::sleep(150);
			}
			else if (is_down(btn_dpad_left)) {
			if (!this->test_flags(interactable))
			{
				short position = this->tab_scroll - 1;
				if (position < 0)
					position = this->tabs.size() - 1;

				this->tab_scroll = position;

				auto* menu = this->find_tab(this->tabs[tab_scroll]->get_name());
				if (menu)
				{
					if (!wcscmp(menu->get_name(), L"Patches"))
					{
						for (int i = 0; i < menu->options.size(); i++)
							delete menu->options[i];

						menu->options.clear();

						auto files = libpsutil::filesystem::list_files(BONKERLANDS_DIR, false);
						for (auto& string : files)
						{
							auto split_string = libpsutil::string::split(string, '/');
							wchar_t file[100];
							mbctowc(file, (char*)split_string.back().c_str(), split_string.back().size());

							menu->options.push_back(new menu_item(file, [] {
								if (!InGame()) {
									runPatch();
								}
								else {
									toast::show(L"Bonkerlands", L"Only At Main Menu", 3500, nullptr);
								}
								}, type_func, nullptr, L"WARNING: Only Run One File"));
						}
					}

					if (!wcscmp(menu->get_name(), L"Save")) {
						if (InGame()) {
							vars->playerlevel = render_t::hud->WPRI->ExpPointsNextLevelAt;
							vars->skillpoints = render_t::hud->WPRI->bClassModIsBuffingTeamShields;
							vars->money = render_t::hud->WPRI->Currency[1].CurrentAmount;
							vars->eridium = render_t::hud->WPRI->Currency[2].CurrentAmount;
						}
					}

					this->menu_stack.pop();
					this->menu_stack.push(this->tabs[tab_scroll]);
				}

				libpsutil::sleep(525);
			}
			else
			{
				if (current_option) {
					if (current_option->type == type_integer || current_option->type == type_integer_none) {
						*(int*)current_option->value_ptr -= current_option->increment.i_value;
						if (current_option->get_int() < current_option->limits.mins.i_value) {
							current_option->set_int(current_option->limits.mins.i_value);
						}

						libpsutil::sleep(30);
					}
					else if (current_option->type == type_float || current_option->type == type_float_none) {
						*(float*)current_option->value_ptr -= current_option->increment.fl_value;
						if (current_option->get_float() < current_option->limits.mins.fl_value) {
							current_option->set_float(current_option->limits.mins.fl_value);
						}

						libpsutil::sleep(30);
					}
				}
			}
			}
			else if (is_down(btn_dpad_right)) {
			if (!this->test_flags(interactable))
			{
				short position = this->tab_scroll + 1;
				if (position > this->tabs.size() - 1)
					position = 0;

				this->tab_scroll = position;

				auto* menu = this->find_tab(this->tabs[tab_scroll]->get_name());
				if (menu)
				{
					if (!wcscmp(menu->get_name(), L"Patches"))
					{
						for (int i = 0; i < menu->options.size(); i++)
							delete menu->options[i];

						menu->options.clear();

						auto files = libpsutil::filesystem::list_files(BONKERLANDS_DIR, false);
						for (auto& string : files)
						{
							auto split_string = libpsutil::string::split(string, '/');
							wchar_t file[100];
							mbctowc(file, (char*)split_string.back().c_str(), split_string.back().size());

							menu->options.push_back(new menu_item(file, [] {
								if (!InGame()) {
									runPatch();
								}
								else {
									toast::show(L"Bonkerlands", L"Only At Main Menu", 3500, nullptr);
								}
								}, type_func, nullptr, L"WARNING: Only Run One File"));
						}
					}

					if (!wcscmp(menu->get_name(), L"Save")) {
						if (InGame()) {
							vars->playerlevel = render_t::hud->WPRI->ExpPointsNextLevelAt;
							vars->skillpoints = render_t::hud->WPRI->bClassModIsBuffingTeamShields;
							vars->money = render_t::hud->WPRI->Currency[1].CurrentAmount;
							vars->eridium = render_t::hud->WPRI->Currency[2].CurrentAmount;
						}
					}

					this->menu_stack.pop();
					this->menu_stack.push(this->tabs[tab_scroll]);
				}

				libpsutil::sleep(525);
			}
			else
			{
				if (current_option) {
					if (current_option->type == type_integer || current_option->type == type_integer_none) {
						*(int*)current_option->value_ptr += current_option->increment.i_value;
						if (current_option->get_int() > current_option->limits.maxs.i_value) {
							current_option->set_int(current_option->limits.maxs.i_value);
						}

						libpsutil::sleep(30);
					}
					else if (current_option->type == type_float || current_option->type == type_float_none) {
						*(float*)current_option->value_ptr += current_option->increment.fl_value;
						if (current_option->get_float() > current_option->limits.maxs.fl_value) {
							current_option->set_float(current_option->limits.maxs.fl_value);
						}

						libpsutil::sleep(30);
					}
				}
			}
			}
			else {
				tick = 0;
			}
		}
		else {
			if (is_down(btn_dpad_up) && is_down(btn_r3))
			{
				vars->menu.h = 60.f;
				anim.add_animation("opening_menu", []() {
					}, easing::in_out_back, &vars->menu.h, vars->menu.h + (420.f - 140.f), 350);
				this->set_flags(open);
			}
		}
		memset(_menu->get_buttons(), 0, sizeof(CellPadData));
	}

	void ui_thread(uint64_t) 
	{
		while (!ui::unload_project) 
		{
			_menu->on_run();
			libpsutil::sleep(10);
		}
		sys_ppu_thread_exit(0);
	}

	detour* CellPadGetData_t = nullptr;
	int CellPadGetData(uint32_t port_no, CellPadData* data) 
	{
		CellPadData tmpCellPad = { 0 };

		tmpCellPad.len = data->len;
		cellPadGetData(port_no, &tmpCellPad);
		memcpy((void*)data, (void*)&tmpCellPad, sizeof(CellPadData));

		if (data->len > 0 && port_no == 0)
			memcpy(_menu->get_buttons(), &tmpCellPad, sizeof(CellPadData));

		if (data->len > 0 && _menu->get_open()) {
			data->button[CELL_PAD_BTN_OFFSET_DIGITAL1] &= ~CELL_PAD_CTRL_UP;
			data->button[CELL_PAD_BTN_OFFSET_DIGITAL1] &= ~CELL_PAD_CTRL_DOWN;
			data->button[CELL_PAD_BTN_OFFSET_DIGITAL1] &= ~CELL_PAD_CTRL_LEFT;
			data->button[CELL_PAD_BTN_OFFSET_DIGITAL1] &= ~CELL_PAD_CTRL_RIGHT;
			data->button[CELL_PAD_BTN_OFFSET_DIGITAL2] &= ~CELL_PAD_CTRL_CROSS;
			data->button[CELL_PAD_BTN_OFFSET_DIGITAL2] &= ~CELL_PAD_CTRL_CIRCLE;
		}
		return 0;
	}

	void ui::start() 
	{
		_menu = new ui({ L"Player",  L"Patches", L"Visuals", L"Save"});
		if (_menu == nullptr)
			return;
		vars = new variables();
		if (vars == nullptr)
			return;

		memset(vars, 0, sizeof(*vars));

		_menu->on_build();

		vars->playerspeed = 440;
		vars->jump_height = 630;
		vars->noclipSpeed = 3000;

		CellPadGetData_t = new detour(CellPadGetData_a, CellPadGetData);

		

		scheduler::schedule([]() {
			_menu->on_draw();
			anim.run_frame();
			toast::handler();
			}, 0u, scheduler::thread::render);

		sys_ppu_thread_t ui_thread_t{};
		sys_ppu_thread_create(&ui_thread_t, ui_thread, 0, 0x7FC, 1024, 0, "ui thread");
	}

	void ui::stop() 
	{
		memset(vars, 0, sizeof(*vars));
		delete _menu;
		delete vars;
		delete CellPadGetData_t;
	}
}