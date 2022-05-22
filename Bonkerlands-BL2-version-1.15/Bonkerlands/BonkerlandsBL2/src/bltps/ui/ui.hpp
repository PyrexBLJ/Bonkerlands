#pragma once

namespace bltps {
	enum btn_s : short {
		btn_x = CELL_PAD_CTRL_CROSS,
		btn_circle = CELL_PAD_CTRL_CIRCLE,
		btn_square = CELL_PAD_CTRL_SQUARE,
		btn_triangle = CELL_PAD_CTRL_TRIANGLE,
		btn_l1 = CELL_PAD_CTRL_L1,
		btn_r1 = CELL_PAD_CTRL_R1,
		btn_start = CELL_PAD_CTRL_START,
		btn_back = CELL_PAD_CTRL_SELECT,
		btn_l3 = CELL_PAD_CTRL_L3,
		btn_r3 = CELL_PAD_CTRL_R3,
		btn_l2 = CELL_PAD_CTRL_L2,
		btn_r2 = CELL_PAD_CTRL_R2,
		btn_dpad_up = CELL_PAD_CTRL_UP,
		btn_dpad_down = CELL_PAD_CTRL_DOWN,
		btn_dpad_left = CELL_PAD_CTRL_LEFT,
		btn_dpad_right = CELL_PAD_CTRL_RIGHT,
	};

	class ui : public menu_base {
	public:
		ui(std::vector<const wchar_t*> tabs)
		{
			for (int i = 0; i < tabs.size(); i++)
			{
				this->tabs.push(new menu_tab(tabs[i]));
			}
		};

		inline void on_build() override;
		inline void on_draw() override;
		inline void on_run() override;

		static void start();
		static void stop();

		inline bool is_down(int key, int digit = CELL_PAD_BTN_OFFSET_DIGITAL1) {
			return this->get_buttons()->button[digit] & (key);/* menu_ui->pad_bit[key] == true;*/
		}
		inline int calc_sizes(std::vector<const wchar_t*> values, float scale)
		{
			std::vector<int> sizes;
			for (auto& val : values)
				sizes.push_back(render()->get_width(val, scale));

			std::sort(sizes.begin(), sizes.end(), std::greater<int>());

			return sizes[0];
		}

		static bool unload_project;
	private:
		menu_item* current_option;
		char tab_scroll;
	};

	extern variables* vars;
	extern ui* _menu;
}