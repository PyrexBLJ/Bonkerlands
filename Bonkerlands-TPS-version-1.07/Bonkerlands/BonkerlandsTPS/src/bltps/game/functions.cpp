#include "include.h"

#include "src/bltps/ui/render/gui.hpp"

void draw_rect(float x, float y, float w, float h, unsigned char* background_color)
{
	static UTexture2D* tex;
	if (!tex)
		tex = UObject::FindObject<UTexture2D>("Texture2D EngineResources.WhiteSquareTexture");

	bltps::render()->canvas->SetPos(x, y / 1.5, 0);
	bltps::render()->canvas->SetDrawColor(background_color[0], background_color[1], background_color[2], background_color[3]);
	bltps::render()->canvas->DrawRect(w, h / 1.5, tex);
}
void draw_outlined_rect(float x, float y, float width, float height, unsigned char* color, int thickness)
{
	bltps::render()->canvas->SetPos(x, y / 1.5, 0);
	bltps::render()->canvas->SetDrawColor(color[0], color[1], color[2], color[3]);
	bltps::render()->canvas->DrawBox(width, height / 1.5);
}
int atoi2(const char* s) {
	int i, sum = 0;
	for (i = 0; isdigit(*s); s++) {
		sum = (sum * 10) + (*s - '0');
		i++;
	}
	return sum;
}
