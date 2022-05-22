#pragma once

enum menu_flags {
	open = (1 << 2),
	color_pick_open = (1 << 3),
	drop_down_open = (1 << 4),
	interactable = (1 << 8),
	slider_select = (1 << 9),
};

class menu_tab;	

class menu_base
{
public:
	menu_base() = default;
	menu_base(const wchar_t* name);
	~menu_base();

	virtual void on_build() = 0;
	virtual void on_draw() = 0;
	virtual void on_run() = 0;

	menu_tab* get_current();
	menu_tab* find_menu(const wchar_t* name);
	menu_tab* find_tab(const wchar_t* name);
	menu_tab* get_tab(int index);

	CellPadData* get_buttons();

	std::vector<menu_tab*> get_tabs();

	bool get_open();
	unsigned int get_flags();
	void set_flags(unsigned int flags);
	void remove_flags(unsigned int flags);
	bool test_flags(unsigned int flags);

	std::wstring get_name();

protected:
	std::stack<menu_tab*> menu_stack;
	std::vector<menu_tab*> submenus;
	std::vector<menu_tab*> tabs;
	unsigned int flags;
	CellPadData cell_buttons;
	wchar_t name[32];
};