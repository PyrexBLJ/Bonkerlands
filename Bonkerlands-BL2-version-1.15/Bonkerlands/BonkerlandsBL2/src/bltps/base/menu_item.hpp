#pragma once


using namespace std;

enum menu_item_type {
    type_none,
    type_integer,
    type_float,
    type_bool,
    type_list,
    type_list_bool,
    type_menu,
    type_func,
    type_integer_none,
    type_float_none,
};

class menu_item_color {
public:
    const wchar_t* name;
    vec4_t* color;
    int index;
    bool rainbow;
    int speed;

    menu_item_color() = default;
    menu_item_color(const wchar_t* _name, vec4_t* _color, bool _rainbow, int _speed, int _index) {
        name = _name;
        color = _color;
        rainbow = _rainbow;
        speed = _speed;
        index = _index;
    }
};

union menu_item_limit {
    int i_value;
    float fl_value;
};

class menu_item_items {
public:
    bool open;
    const wchar_t* current_str;
    std::vector<const wchar_t*> strings;
    std::vector<bool> toggles;
    int size;
    int selected_item;
    short selected_item_offset;

    std::vector<const wchar_t*> get_strings() {
        return this->strings;
    };
    const wchar_t* get_string() { return this->current_str; };

    int get_count() {
        return this->strings.size();
    };

    int get_cursor() { return this->selected_item; };
    int get_offset() { return this->selected_item_offset; };
    std::vector<bool> get_toggles() { return this->toggles; };
    bool get_toggled(int index) { return this->toggles[index]; };
};

class menu_item_limits {
public:
    menu_item_limit mins;
    menu_item_limit maxs;
};

class menu_item {
public:
    wchar_t name[50];
    wchar_t tooltip[100];

    menu_item_type type;
    menu_item_items items;
    menu_item_limit increment;
    menu_item_limits limits;

    void(*callback)();
    void* value_ptr;

    menu_item_items* get_items();
    const wchar_t* get_tooltip();
    const wchar_t* get_name();
    bool get_bool();
    int get_int();
    float get_float();
    void(*get_callback())();

    void set_bool(bool value);
    void set_int(int value);
    void set_float(float value);

    menu_item(const wchar_t* name, void* ptr = nullptr);
    menu_item(const wchar_t* name, void(*callback)(), menu_item_type type = type_menu, void* value_ptr = nullptr, const wchar_t* tooltip = L"");
    menu_item(const wchar_t* name, void* i_value, int i_increment, int i_min, int i_max, void(*callback)() = nullptr, menu_item_type type = type_integer, const wchar_t* tooltip = L"");
    menu_item(const wchar_t* name, void* fl_value, float fl_increment, float fl_min, float fl_max, void(*callback)() = nullptr, menu_item_type type = type_float, const wchar_t* tooltip = L"");
    menu_item(const wchar_t* name, void* i_value, std::vector<const wchar_t*> strings, menu_item_type type = type_list, const wchar_t* tooltip = L"");

    ~menu_item() {
        memset(this, 0, sizeof(*this));

        items.strings.clear();
        items.toggles.clear();
    }
};