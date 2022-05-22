#include "include.h"

menu_item_items* menu_item::get_items()
{
    return &this->items;
}

const wchar_t* menu_item::get_tooltip()
{
    return this->tooltip;
}

const wchar_t* menu_item::get_name() {
    return this->name;
}

bool menu_item::get_bool() {
    return *(bool*)this->value_ptr;
}

int menu_item::get_int() {
    return *(int*)this->value_ptr;
}

float menu_item::get_float() {
    return *(float*)this->value_ptr;
}

void(*menu_item::get_callback())() {
    return this->callback;
}

void menu_item::set_bool(bool value)
{
    *(bool*)this->value_ptr = value;
}

void menu_item::set_int(int value)
{
    *(int*)this->value_ptr = value;
}

void menu_item::set_float(float value)
{
    *(float*)this->value_ptr = value;
}

menu_item::menu_item(const wchar_t* name, void* ptr) {
    memset(this, 0, sizeof(menu_item));
    wcscpy(this->name, name);
    wcscpy(this->tooltip, tooltip);
    this->value_ptr = ptr;
    this->type = type_menu;
}

menu_item::menu_item(const wchar_t* name, void(*callback)(), menu_item_type type, void* value_ptr, const wchar_t* tooltip) {
    memset(this, 0, sizeof(menu_item));
    wcscpy(this->name, name);
    wcscpy(this->tooltip, tooltip);
    this->callback = callback;
    this->value_ptr = value_ptr;
    this->type = type;
}

menu_item::menu_item(const wchar_t* name, void* i_value, int i_increment, int i_min, int i_max, void(*callback)(), menu_item_type type, const wchar_t* tooltip) {
    memset(this, 0, sizeof(menu_item));

    wcscpy(this->name, name);
    wcscpy(this->tooltip, tooltip);
    this->value_ptr = i_value;
    this->callback = callback;
    this->increment.i_value = i_increment;
    this->limits.mins.i_value = i_min;
    this->limits.maxs.i_value = i_max;
    this->type = type;
}

menu_item::menu_item(const wchar_t* name, void* fl_value, float fl_increment, float fl_min, float fl_max, void(*callback)(), menu_item_type type, const wchar_t* tooltip) {
    memset(this, 0, sizeof(menu_item));
    wcscpy(this->name, name);
    wcscpy(this->tooltip, tooltip);
    this->value_ptr = fl_value;
    this->callback = callback;
    this->increment.fl_value = fl_increment;
    memset(&this->items, 0, sizeof(menu_item_items));
    this->limits.mins.fl_value = fl_min;
    this->limits.maxs.fl_value = fl_max;
    this->type = type;
}

menu_item::menu_item(const wchar_t* name, void* i_value, std::vector<const wchar_t*> strings, menu_item_type type, const wchar_t* tooltip) {
    memset(this, 0, sizeof(menu_item));
    wcscpy(this->name, name);
    wcscpy(this->tooltip, tooltip);
    this->value_ptr = i_value;
    this->items.current_str = strings[0];

    if (type == menu_item_type::type_list_bool) {
        for (auto& str : strings)
            this->items.toggles.push_back(false);
    }
    this->items.strings = strings;
    this->callback = nullptr;
    this->type = type;
}