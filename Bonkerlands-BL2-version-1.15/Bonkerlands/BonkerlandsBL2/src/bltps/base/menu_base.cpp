#include "include.h"

extern "C" void __cxa_pure_virtual() { while (1); }

menu_tab* menu_base::get_current()
{
	return this->menu_stack.top();
}

menu_tab* menu_base::find_menu(const wchar_t* name)
{
	for (auto* menu : this->submenus)
	{
		if (wcscmp(menu->get_name(), name) == 0)
			return menu;
	}

	return nullptr;
}

menu_tab* menu_base::find_tab(const wchar_t* name)
{
	for (auto* menu : this->tabs) {
		if (wcscmp(menu->get_name(), name) == 0)
			return menu;
	}

	return nullptr;
}

menu_tab* menu_base::get_tab(int index)
{
	return this->tabs[index];
}

CellPadData* menu_base::get_buttons()
{
	return &this->cell_buttons;
}

std::vector<menu_tab*> menu_base::get_tabs()
{
	return this->tabs;
}

bool menu_base::get_open()
{
	return this->test_flags(open);
}

unsigned int menu_base::get_flags()
{
	return this->flags;
}

void menu_base::set_flags(unsigned int flags)
{
	this->flags |= flags;
}

void menu_base::remove_flags(unsigned int flags)
{
	this->flags &= ~flags;
}

bool menu_base::test_flags(unsigned int bits)
{
	return this->flags & (bits);
}

std::wstring menu_base::get_name()
{
	return this->name;
}

menu_base::menu_base(const wchar_t* name)
{
	wcscpy(this->name, name);
	this->menu_stack.empty();
}

menu_base::~menu_base()
{
	this->menu_stack.empty();

	for (int i = 0; i < this->submenus.size(); i++)
	{
		auto submenu = this->submenus.at(i);
		delete submenu;
	}

	for (int i = 0; i < this->tabs.size(); i++)
	{
		auto tab = this->tabs.at(i);
		delete tab;
	}

	this->submenus.clear();
	this->tabs.clear();
	memset(this, 0, sizeof(*this));
}