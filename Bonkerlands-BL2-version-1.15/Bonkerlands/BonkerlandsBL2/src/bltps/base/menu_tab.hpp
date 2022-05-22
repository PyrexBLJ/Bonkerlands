#pragma once


class menu_tab
{
public:
	menu_tab(const wchar_t* name);
	~menu_tab();

	const wchar_t* get_name();
	menu_item* get_option(unsigned int index);
	unsigned int get_count();

	short get_cursor();
	short get_cursor_offset();
	menu_tab* get_parent();

	void set_cursor(short index);
	void set_cursor_offset(short offset);
	void set_parent(menu_tab* parent);

	menu_tab* add_option(menu_item* option);

	std::vector<menu_item*> options;

private:
	wchar_t name[32];
	
	short cursor;
	short offset;

	menu_tab* parent;
};