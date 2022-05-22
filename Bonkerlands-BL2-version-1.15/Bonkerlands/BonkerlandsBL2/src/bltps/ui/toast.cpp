#include "include.h"

using namespace bltps;

toast popup;

std::deque<toast::ui_toast> toast::Queue;
void toast::show(const std::wstring& title, const std::wstring& description, int length, void(*callback)())
{
	toast::Queue.clear();
	toast::Queue.push_back({ title, description, length, 0, callback });
}

void toast::draw(ui_toast* toast)
{
	if (!toast) return;

	int width = render()->canvas->SizeX;
	int height = render()->canvas->SizeY;
	int slideTime = 100;

	int duration = toast->length;
	int startTime = toast->start;

	int border = 1;
	int cornerSize = 15;
	int bHeight = 74;

	int imgDim = 10; //5

	float fontSize = 0.7f;
	float descSize = 0.9f;

	color wColor = { 255, 255, 255, 255 };
	color bgColor = { 0, 0, 0, 200 };
	color borderColor = (vars->menu.clr_accent);

	height /= 5;
	height *= 4;

	if (Sys_Milliseconds() < startTime || (startTime + duration) < Sys_Milliseconds()) return;

	// Fadein stuff
	else if (Sys_Milliseconds() - startTime < slideTime)
	{
		int diffH = render()->canvas->SizeY / 5;
		int diff = Sys_Milliseconds() - startTime;
		double scale = 1.0 - ((1.0 * diff) / (1.0 * slideTime));
		diffH = static_cast<int>(diffH * scale);
		height += diffH;
	}

	// Fadeout stuff
	else if (Sys_Milliseconds() - startTime > (duration - slideTime))
	{
		int diffH = render()->canvas->SizeY / 5;
		int diff = (startTime + duration) - Sys_Milliseconds();
		double scale = 1.0 - ((1.0 * diff) / (1.0 * slideTime));
		diffH = static_cast<int>(diffH * scale);
		height += diffH;
	}
	bHeight *= 1.5f;
	height += bHeight / 2 - cornerSize;

	// Calculate width data
	int iOffset = (bHeight /*- imgDim*/) / 2;
	int iOffsetLeft = iOffset /** 2*/;
	int titleSize = render()->get_width(toast->title.data(), fontSize);
	int descrSize = render()->get_width(toast->desc.data(), descSize);
	float bWidth = iOffsetLeft/* * 3 + imgDim */ + std::max(titleSize, descrSize);

	bWidth = (static_cast<int>(bWidth) + (static_cast<int>(bWidth) % 2)) * 1.0f;
	bHeight += (bHeight % 2);


	// Background
	//draw->add_filled_rect(menu_x, menu_y, menu_w, title_bar_height, m_vars->clr_accent);
	render()->add_filled_rect(static_cast<float>(width / 2 - bWidth / 2), static_cast<float>(height - bHeight / 2) * 1.5f, bWidth * 1.0f, bHeight * 1.0f, bgColor);

	float endTime = (startTime + duration) - Sys_Milliseconds();

	//float val = (bWidth * ((float)endTime / ((float)(startTime + duration))) * bWidth);

	// Debug
	//CL_DrawText(scrPlace, memory::va("%0.3f", val), 0x7F, game_font, 10, 10, 0, 1, 1, .6f, .6f, (float*)&color_white, 1);

	// Slider
	//CL_DrawStretchPic(scrPlace, static_cast<float>(width / 2 - bWidth / 2), static_cast<float>(height - bHeight / 2 + bHeight + 5.0f), val, 10, 0, 0, local_vars->accent_color, white);

	// Border
	render()->add_filled_rect(static_cast<float>(width / 2 - bWidth / 2 - border), static_cast<float>(height - bHeight / 2 - border) * 1.5f, border * 1.0f, bHeight + (border * 2.0f), borderColor); // Left
	render()->add_filled_rect(static_cast<float>(width / 2 - bWidth / 2 + bWidth), static_cast<float>(height - bHeight / 2 - border) * 1.5f, border * 1.0f, bHeight + (border * 2.0f), borderColor); // Right
	render()->add_filled_rect(static_cast<float>(width / 2 - bWidth / 2), static_cast<float>(height - bHeight / 2 - border) * 1.5f, bWidth * 1.0f, border * 1.0f, borderColor);                      // Top
	render()->add_filled_rect(static_cast<float>(width / 2 - bWidth / 2), static_cast<float>(height - bHeight / 2) * 1.5f + bHeight * 1.0f, bWidth * 1.0f, border * 1.0f, borderColor);                      // Bottom
	//static_cast<float>(height - bHeight / 2) * 1.5f + bHeight * 1.0f
	//static_cast<float>(height + bHeight /*/ 2*/) /*+ bHeight * 1.5f*/ original
	//draw->add_text(L"Bonkerlands", rectDef_s(menu_x, menu_y, menu_w, title_bar_height), 4, text_alignment::text_center, 1.2f, 1.2f * 1.5f, color_white);
	// Text
	float leftText = width / 2 - bWidth / 2 - cornerSize /*+ iOffsetLeft*/ * 2 + imgDim;
	float rightText = width / 2 + bWidth / 2 - cornerSize /*- iOffsetLeft*/;
	render()->add_text(toast->title.data(), rectDef_s(static_cast<float>(width / 2 - bWidth / 2), static_cast<float>(height - bHeight / 2) * 1.5f, bWidth, bHeight / 2), 0, text_center, fontSize, wColor);
	render()->add_text(toast->desc.data(), rectDef_s(static_cast<float>(width / 2 - bWidth / 2), static_cast<float>((height - bHeight / 2) * 1.5f) + (bHeight / 2), bWidth, bHeight / 2), 0, text_center, fontSize, wColor);

	//render()->add_text(toast->title.data(), static_cast<float>(leftText + (rightText - leftText) / 2 - titleSize / 2 + cornerSize), static_cast<float>(height - bHeight / 2 + cornerSize * 2 + 7) * 1.44f * 1.5f, 0, text_left, fontSize, fontSize, wColor); // Title
	//render()->add_text(toast->desc.data(), leftText + (rightText - leftText) / 2 - descrSize / 2 + cornerSize, static_cast<float>(height - bHeight / 2 + cornerSize * 2 + 33) * 1.44f * 1.5f, 0, text_left, descSize, descSize, wColor); // Description
}

void toast::handler()
{
	if (toast::Queue.empty()) return;

	toast::ui_toast* toast = &toast::Queue.front();

	// Set start time
	if (!toast->start)
	{
		toast->start = Sys_Milliseconds();
	}

	if ((toast->start + toast->length) < Sys_Milliseconds())
	{
		if (toast->callback) toast->callback();
		toast::Queue.pop_back();
	}
	else
	{
		toast::draw(toast);
	}
}


void toast::start() {
}

void toast::stop()
{
	toast::Queue = std::deque<toast::ui_toast>();
}