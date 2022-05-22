#include "include.h"

const wchar_t* menu_tab::get_name()
{
	return this->name;
}

menu_item* menu_tab::get_option(unsigned int index)
{
	return this->options.at(index);
}

unsigned int menu_tab::get_count()
{
	return this->options.size();
}

short menu_tab::get_cursor()
{
	return this->cursor;
}

short menu_tab::get_cursor_offset()
{
	return this->offset;
}

menu_tab* menu_tab::get_parent()
{
	return this->parent;
}

void menu_tab::set_cursor(short index)
{
	this->cursor = index;
}

void menu_tab::set_cursor_offset(short offset)
{
	this->offset = offset;
}

void menu_tab::set_parent(menu_tab* parent)
{
	this->parent = parent;
}

menu_tab* menu_tab::add_option(menu_item* option)
{
	this->options.push_back(option);
	return this;
}

menu_tab::menu_tab(const wchar_t* name)
{
	wcscpy(this->name, name);
	this->cursor = 0;
}

menu_tab::~menu_tab()
{
	for (int i = 0; i < this->options.size(); i++)
	{
		auto option = this->options.at(i);
		delete option;
	}

	this->options.clear();
	memset(this, 0, sizeof(*this));
}