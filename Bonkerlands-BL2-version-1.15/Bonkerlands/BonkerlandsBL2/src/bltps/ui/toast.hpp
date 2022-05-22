class toast
{
public:
	static void start();
	static void stop();

	static void show(const std::wstring& title, const std::wstring& description, int length, void(*callback)() = nullptr);

	static void handler();
private:
	class ui_toast
	{
	public:
		std::wstring title;
		std::wstring desc;
		int length;
		int start;
		void(*callback)();
	};

	static void draw(ui_toast* toast);

	static std::deque<ui_toast> Queue;
};

extern toast popup;