#pragma once

#include "External/dx_renderer.h"

//forceinline::dx_renderer* renderer

struct FVector2D
{
	float X;                                                        // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	float Y;                                                        // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)

	inline FVector2D()
		: X(0), Y(0)
	{ }

	inline FVector2D(float x, float y)
		: X(x),
		Y(y)
	{ }


	FVector2D operator+(const FVector2D& v) {
		return FVector2D{ X + v.X, Y + v.Y };
	}
};



namespace Input
{
	static bool mouseDown[5];
	static bool mouseDownAlready[256];

	static bool keysDown[256];
	static bool keysDownAlready[256];

	static bool IsAnyMouseDown()
	{
		if (mouseDown[0]) return true;
		if (mouseDown[1]) return true;
		if (mouseDown[2]) return true;
		if (mouseDown[3]) return true;
		if (mouseDown[4]) return true;

		return false;
	}

	static bool IsMouseClicked(int button, int element_id, bool repeat)
	{



		if (GetAsyncKeyState_Spoofed(button))
		{
			if (!mouseDownAlready[element_id])
			{
				mouseDownAlready[element_id] = true;
				return true;
			}
			if (repeat)
				return true;
		}
		else
		{
			mouseDownAlready[element_id] = false;
		}
		return false;
	}
	static bool IsKeyPressed(int key, bool repeat)
	{
		if (keysDown[key])
		{
			if (!keysDownAlready[key])
			{
				keysDownAlready[key] = true;
				return true;
			}
			if (repeat)
				return true;
		}
		else
		{
			keysDownAlready[key] = false;
		}
		return false;
	}

	static void Handle()
	{
		if (GetAsyncKeyState_Spoofed(0x01) & 1)
			mouseDown[0] = true;
		else
			mouseDown[0] = false;
	}

	static FVector2D CursorPos()
	{
		POINT cursorPos;
		GetCursorPosA(&cursorPos);
		//LI_FN(GetCursorPos).get()(&cursorPos);
		return FVector2D{ (float)cursorPos.x, (float)cursorPos.y };
	}
	static bool MouseInZone(FVector2D pos, FVector2D size)
	{
		FVector2D cursor_pos = CursorPos();

		if (cursor_pos.X > pos.X && cursor_pos.Y > pos.Y)
			if (cursor_pos.X < pos.X + size.X && cursor_pos.Y < pos.Y + size.Y)
				return true;

		return false;
	}
}

static bool hover_element = false;
static FVector2D menu_pos = FVector2D{ 0, 0 };
static float offset_x = 0.0f;
static float offset_y = 0.0f;


static FVector2D first_element_pos = FVector2D{ 0, 0 };

static FVector2D last_element_pos = FVector2D{ 0, 0 };
static FVector2D last_element_size = FVector2D{ 0, 0 };

static int current_element = -1;
static FVector2D current_element_pos = FVector2D{ 0, 0 };
static FVector2D current_element_size = FVector2D{ 0, 0 };
static int elements_count = 0;

static bool sameLine = false;

static bool pushY = false;
static float pushYvalue = 0.0f;

static FVector2D dragPos;
using namespace forceinline;

namespace superduper
{
	class SuperDuperRenderer {

	private:
		//sec

		static wchar_t* s2wc(const char* c)
		{
			const size_t cSize = strlen(c) + 1;
			wchar_t* wc = new wchar_t[cSize];
			mbstowcs(wc, c, cSize);

			return wc;
		}
		FVector2D staticpos;
		dx_renderer* renderer;
	public:

		SuperDuperRenderer(dx_renderer* render) {
			renderer = render;
		}
		// u think i can include vector class? would it fuck entire srctry

		bool Window(const char* name, FVector2D pos, FVector2D size, bool show_menu) {
			elements_count = 0;

			if (!show_menu)
				return false;

			if (staticpos.X == 0 || staticpos.Y == 0)
				staticpos = pos;

			bool isHovered = Input::MouseInZone(FVector2D{ staticpos.X, staticpos.Y }, size);

			//Drop last element
			if (current_element != -1 && !GetAsyncKeyState_Spoofed(0x1))
			{
				current_element = -1;
			}

			//Drag
			if (hover_element && GetAsyncKeyState_Spoofed(0x1))
			{

			}
			else if ((isHovered || dragPos.X != 0) && !hover_element)
			{
				if (Input::IsMouseClicked(VK_LBUTTON, elements_count, true))
				{
					FVector2D cursorPos = Input::CursorPos();

					cursorPos.X -= size.X;
					cursorPos.Y -= size.Y;

					if (dragPos.X == 0)
					{
						dragPos.X = (cursorPos.X - staticpos.X);
						dragPos.Y = (cursorPos.Y - staticpos.Y);
					}
					staticpos.X = cursorPos.X - dragPos.X;
					staticpos.Y = cursorPos.Y - dragPos.Y;
				}
				else
				{
					dragPos = FVector2D{ 0, 0 };
				}
			}
			else
			{
				hover_element = false;
			}


			offset_x = 0; offset_y = 0.;
			menu_pos = FVector2D{ staticpos.X, staticpos.Y };
			first_element_pos = FVector2D{ 0, 0 };
			current_element_pos = FVector2D{ 0, 0 };
			current_element_size = FVector2D{ 0, 0 };

			//Bg
			this->renderer->draw_filled_rect(staticpos.X, staticpos.Y, size.X, size.Y, 0xff1e1e1e); //you didnt put color o h yeah what shoukd be the color?? just make it nice idc
			//drawFilledRect(FFVector2DD{ pos->X, pos.y }, size.X, size.Y, Colors::Window_Background);
			//drawFilledRect(FFVector2DD{ pos->X, pos.y }, 122, size.Y, FLinearColor{ 0.006f, 0.006f, 0.006f, 1.0f });//My tabs bg

			//Header
			this->renderer->draw_filled_rect(staticpos.X, staticpos.Y, size.X, 25.0f, 0xff1e1e1e);
			//drawFilledRect(FFVector2DD{ pos->X, pos.y }, size.X, 25.0f, Colors::Window_Header);

			offset_y += 25.0f;

			//Title
			//FFVector2DD titlePos = FFVector2DD{ pos->X + size.X / 2, pos.y + 25 / 2 };
			//TextCenter(name, titlePos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

			this->renderer->draw_text(s2wc(name), staticpos.X + size.X / 2, staticpos.Y + 6, 0xffffffff, true, false);
		}

		bool ButtonTab(const char* name, FVector2D size, bool active)
		{
			elements_count++;

			FVector2D padding = FVector2D{ 10, 10 };
			FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
			if (sameLine)
			{
				pos.X = last_element_pos.X + last_element_size.X + padding.X;
				pos.Y = last_element_pos.Y;
			}
			if (pushY)
			{
				pos.Y = pushYvalue;
				pushY = false;
				pushYvalue = 0.0f;
				offset_y = pos.Y - menu_pos.Y;
			}
			bool isHovered = Input::MouseInZone(FVector2D{ pos.X, pos.Y }, size);

			//Bg
			if (active)
			{
				this->renderer->draw_filled_rect(pos.X, pos.Y, size.X, size.Y, 0xff424242);
			}
			else if (isHovered)
			{
				this->renderer->draw_filled_rect(pos.X, pos.Y, size.X, size.Y, 0xff424242);
				hover_element = true;
			}
			else
			{
				this->renderer->draw_filled_rect(pos.X, pos.Y, size.X, size.Y, 0xff242424);
			}

			if (!sameLine)
				offset_y += size.Y + padding.Y;

			//Text
			FVector2D textPos = FVector2D{ pos.X + size.X / 2, pos.Y + 6 };
			this->renderer->draw_text(s2wc(name), textPos.X, textPos.Y, 0xffffffff, true, false);
			//TextCenter(name, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);


			sameLine = false;
			last_element_pos = pos;
			last_element_size = size;
			if (first_element_pos.X == 0.0f)
				first_element_pos = pos;

			if (isHovered && Input::IsMouseClicked(VK_LBUTTON, elements_count, false))
				return true;

			return false;
		}


		static void SameLine()
		{
			sameLine = true;
		}

		void Text(const char* text, bool center = false, bool outline = false)
		{
			elements_count++;

			float size = 25;
			FVector2D padding = FVector2D{ 10, 10 };
			FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
			if (sameLine)
			{
				pos.X = last_element_pos.X + last_element_size.X + padding.X;
				pos.Y = last_element_pos.Y;
			}
			if (pushY)
			{
				pos.Y = pushYvalue;
				pushY = false;
				pushYvalue = 0.0f;
				offset_y = pos.Y - menu_pos.Y;
			}

			if (!sameLine)
				offset_y += size + padding.Y;

			//Text
			FVector2D textPos = FVector2D{ pos.X + 5.0f, pos.Y + size / 2 };
			this->renderer->draw_text(s2wc(text), textPos.X, textPos.Y, 0xffffffff, center, outline);

			sameLine = false;
			last_element_pos = pos;
			//last_element_size = size;
			if (first_element_pos.X == 0.0f)
				first_element_pos = pos;
		}

		void Checkbox(char* name, bool* value)
		{
			elements_count++;

			float size = 14;
			FVector2D padding = FVector2D{ 10, 10 };
			FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
			if (sameLine)
			{
				pos.X = last_element_pos.X + last_element_size.X + padding.X;
				pos.Y = last_element_pos.Y;
			}
			if (pushY)
			{
				pos.Y = pushYvalue;
				pushY = false;
				pushYvalue = 0.0f;
				offset_y = pos.Y - menu_pos.Y;
			}
			bool isHovered = Input::MouseInZone(FVector2D{ pos.X, pos.Y }, FVector2D{ size, size });

			//Bg
			if (isHovered)
			{
				//this->renderer->draw_filled_rect( pos.X, pos.Y , size, size,0x90000000);
				this->renderer->draw_filled_rect(pos.X, pos.Y, size, size, 0xff2D2D30);
				hover_element = true;
			}
			else
			{
				this->renderer->draw_filled_rect(pos.X, pos.Y, size, size, 0xff2D2D30);
			}

			if (!sameLine)
				offset_y += size + padding.Y;

			if (*value)
			{
				this->renderer->draw_filled_rect(pos.X + 3, pos.Y + 3, size - 6, size - 6, 0xff000000);
				//drawFilledRect(FVector2D{ pos.X + 9, pos.Y + 9 }, size - 18, size - 18, Colors::Checkbox_Hovered);
			}



			//Text
			FVector2D textPos = FVector2D{ pos.X + size + 24.0f, pos.Y + 1 };
			//if (!TextOverlapedFromActiveElement(textPos))
			this->renderer->draw_text(s2wc(name), textPos.X, textPos.Y, 0xffffffff, false, false);
			//TextLeft(name, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);


			sameLine = false;
			last_element_pos = pos;
			//last_element_size = size;
			if (first_element_pos.X == 0.0f)
				first_element_pos = pos;

			if (isHovered && Input::IsMouseClicked(VK_LBUTTON, elements_count, false))
				*value = !*value;
		}

		
		void Separator()
		{
			elements_count++;

			float size = 370;
			FVector2D padding = FVector2D{ 10, 10 };
			FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
			if (sameLine)
			{
				pos.X = last_element_pos.X + last_element_size.X + padding.X;
				pos.Y = last_element_pos.Y;
			}
			if (pushY)
			{
				pos.Y = pushYvalue;
				pushY = false;
				pushYvalue = 0.0f;
				offset_y = pos.Y - menu_pos.Y;
			}

			this->renderer->draw_filled_rect(pos.X, pos.Y, size, 2, 0xffffffff);


			if (!sameLine)
				offset_y += 2 + padding.Y;


			sameLine = false;
			last_element_pos = pos;
			//last_element_size = size;
			if (first_element_pos.X == 0.0f)
				first_element_pos = pos;
		}

		void SliderInt(char* name, int* value, int min, int max)
		{
			elements_count++;

			FVector2D size = FVector2D{ 240, 50 };
			FVector2D slider_size = FVector2D{ 200, 10 };
			FVector2D padding = FVector2D{ 10, 12 };
			FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
			if (sameLine)
			{
				pos.X = last_element_pos.X + last_element_size.X + padding.X;
				pos.Y = last_element_pos.Y;
			}
			if (pushY)
			{
				pos.Y = pushYvalue;
				pushY = false;
				pushYvalue = 0.0f;
				offset_y = pos.Y - menu_pos.Y;
			}
			bool isHovered = Input::MouseInZone(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size);

			if (!sameLine)
				offset_y += size.Y + padding.Y - 20;

			//Bg
			if (isHovered || current_element == elements_count)
			{
				//Drag
				if (Input::IsMouseClicked(VK_LBUTTON, elements_count, true))
				{
					current_element = elements_count;

					FVector2D cursorPos = Input::CursorPos();
					*value = ((cursorPos.X - pos.X) * ((max - min) / slider_size.X)) + min;
					if (*value < min) *value = min;
					if (*value > max) *value = max;
				}

				//drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size.X, slider_size.Y, Colors::Slider_Hovered);
				//DrawFilledCircle(FVector2D{ pos.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Progress);
			//	DrawFilledCircle(FVector2D{ pos.X + slider_size.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Hovered);

				hover_element = true;
			}
			else
			{
				//drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size.X, slider_size.Y, Colors::Slider_Idle);
				//DrawFilledCircle(FVector2D{ pos.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Progress);
				//DrawFilledCircle(FVector2D{ pos.X + slider_size.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Idle);
			}

			//Text
			FVector2D textPos = FVector2D{ pos.X, pos.Y + 5 };
			this->renderer->draw_text(s2wc(name), textPos.X, textPos.Y, 0xffffffff, false);

			//Empty
			float oneP = slider_size.X / (max - min);

			this->renderer->draw_filled_rect(pos.X, pos.Y + slider_size.Y + padding.Y, slider_size.X, slider_size.Y, 0xff2D2D30);

			//Value
			this->renderer->draw_filled_rect(pos.X, pos.Y + slider_size.Y + padding.Y, oneP * (*value - min), slider_size.Y, 0xff000000);

			char buffer[32];
			sprintf_s(buffer, skCrypt("%i"), *value);
			FVector2D valuePos = FVector2D{ pos.X + slider_size.X, pos.Y + 5 };
			this->renderer->draw_text(s2wc(buffer), valuePos.X, valuePos.Y, 0xffffffff, true, false);

			sameLine = false;
			last_element_pos = pos;
			last_element_size = size;
			if (first_element_pos.X == 0.0f)
				first_element_pos = pos;
		}

		bool checkbox_enabled[256];
		void Combobox(const char* name, FVector2D size, int* value, const char* arg, ...)
		{
			elements_count++;

			FVector2D padding = FVector2D{ 10, 16 };
			FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
			if (sameLine)
			{
				pos.X = last_element_pos.X + last_element_size.X + padding.X;
				pos.Y = last_element_pos.Y;
			}
			if (pushY)
			{
				pos.Y = pushYvalue;
				pushY = false;
				pushYvalue = 0.0f;
				offset_y = pos.Y - menu_pos.Y;
			}
			bool isHovered = Input::MouseInZone(FVector2D{ pos.X, pos.Y }, size);

			//Bg
			if (isHovered || checkbox_enabled[elements_count])
			{
				this->renderer->draw_filled_rect(pos.X, pos.Y, size.X, size.Y, 0xff212124); //we gonna find good colors

				hover_element = true;
			}
			else
			{
				this->renderer->draw_filled_rect(pos.X, pos.Y, size.X, size.Y, 0xff2D2D30);
			}

			if (!sameLine)
				offset_y += size.Y + padding.Y;

			//Text
			FVector2D textPos = FVector2D{ pos.X + size.X + 5.0f, pos.Y + 6 };
			this->renderer->draw_text(s2wc(name), textPos.X, textPos.Y, 0xffffffff, false, false);

			//Elements
			bool isHovered2 = false;
			FVector2D element_pos = pos;
			int num = 0;

			if (checkbox_enabled[elements_count])
			{
				current_element_size.X = element_pos.X - 5.0f;
				current_element_size.Y = element_pos.Y - 5.0f;
			}
			va_list arguments;
			for (va_start(arguments, arg); arg != NULL; arg = va_arg(arguments, const char*))
			{
				//Selected Element
				if (num == *value)
				{
					FVector2D _textPos = FVector2D{ pos.X + size.X / 2, pos.Y + 6 };
					this->renderer->draw_text(s2wc((char*)arg), _textPos.X, _textPos.Y, 0xffffffff, false);
				}

				if (checkbox_enabled[elements_count])
				{
					element_pos.Y += 25.0f;

					isHovered2 = Input::MouseInZone(FVector2D{ element_pos.X, element_pos.Y }, FVector2D{ size.X, 25.0f });
					if (isHovered2)
					{
						hover_element = true;
						this->renderer->draw_filled_rect(element_pos.X, element_pos.Y, size.X, 25.0f, 0xff212124);

						//Click
						if (Input::IsMouseClicked(VK_LBUTTON, elements_count, false))
						{
							*value = num;
							checkbox_enabled[elements_count] = false;
						}
					}
					else
					{
						this->renderer->draw_filled_rect(element_pos.X, element_pos.Y, size.X, 25.0f, 0xff2D2D30);
					}

					this->renderer->draw_text(s2wc((char*)arg), element_pos.X + 5.0f, element_pos.Y + 8.0f, 0xffffffff, false);
				}
				num++;
			}
			va_end(arguments);
			if (checkbox_enabled[elements_count])
			{
				current_element_size.X = element_pos.X + 5.0f;
				current_element_size.Y = element_pos.Y + 5.0f;
			}

			sameLine = false;
			last_element_pos = pos;
			last_element_size = size;
			if (first_element_pos.X == 0.0f)
				first_element_pos = pos;

			if (isHovered && Input::IsMouseClicked(VK_LBUTTON, elements_count, false))
			{
				checkbox_enabled[elements_count] = !checkbox_enabled[elements_count];
			}
			if (!isHovered && !isHovered2 && Input::IsMouseClicked(VK_LBUTTON, elements_count, false))
			{
				checkbox_enabled[elements_count] = false;
			}
		}

		int active_hotkey = -1;
		bool already_pressed = false;
		std::string VirtualKeyCodeToString(UCHAR virtualKey)
		{
			UINT scanCode = LI_FN(MapVirtualKeyA).get()(virtualKey, MAPVK_VK_TO_VSC);

			if (virtualKey == VK_LBUTTON)
			{
				return skCrypt("MOUSE0").decrypt();
			}
			if (virtualKey == VK_RBUTTON)
			{
				return skCrypt("MOUSE1").decrypt();
			}
			if (virtualKey == VK_MBUTTON)
			{
				return skCrypt("MBUTTON").decrypt();
			}
			if (virtualKey == VK_XBUTTON1)
			{
				return skCrypt("XBUTTON1").decrypt();
			}
			if (virtualKey == VK_XBUTTON2)
			{
				return skCrypt("XBUTTON2").decrypt();
			}

			CHAR szName[128];
			int result = 0;
			switch (virtualKey)
			{
			case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
			case VK_RCONTROL: case VK_RMENU:
			case VK_LWIN: case VK_RWIN: case VK_APPS:
			case VK_PRIOR: case VK_NEXT:
			case VK_END: case VK_HOME:
			case VK_INSERT: case VK_DELETE:
			case VK_DIVIDE:
			case VK_NUMLOCK:
				scanCode |= KF_EXTENDED;
			default:
				result = LI_FN(GetKeyNameTextA).get()(scanCode << 16, szName, 128);
			}

			return szName;
		}
		void Hotkey(const char* name, FVector2D size, int* key)
		{
			elements_count++;

			FVector2D padding = FVector2D{ 100, 4 };
			FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
			if (sameLine)
			{
				pos.X = last_element_pos.X + last_element_size.X + padding.X;
				pos.Y = last_element_pos.Y + (last_element_size.Y / 2) - size.Y / 2;
			}
			if (pushY)
			{
				pos.Y = pushYvalue;
				pushY = false;
				pushYvalue = 0.0f;
				offset_y = pos.Y - menu_pos.Y;
			}
			bool isHovered = Input::MouseInZone(FVector2D{ pos.X, pos.Y }, size);

			//Bg
			if (isHovered)
			{
				this->renderer->draw_filled_rect(pos.X, pos.Y, size.X, size.Y, 0xff212124);
				hover_element = true;
			}
			else
			{
				this->renderer->draw_filled_rect(pos.X, pos.Y, size.X, size.Y, 0xff2D2D30);
			}

			if (!sameLine)
				offset_y += size.Y + padding.Y;

			if (active_hotkey == elements_count)
			{
				//Text
				FVector2D textPos = FVector2D{ pos.X + size.X / 2, pos.Y + 6 };
				this->renderer->draw_text(s2wc(skCrypt("[Press Key]")), textPos.X, textPos.Y, 0xffffffff);

				/*if (!Input::IsAnyMouseDown())
				{
					already_pressed = false;
				}*/

				if (!already_pressed)
				{
					for (int code = 0; code < 255; code++)
					{
						if (GetAsyncKeyState_Spoofed(code) & 1)
						{
							*key = code;
							active_hotkey = -1;
							already_pressed = true;
						}
					}
				}
			}
			else
			{
				//Text
				FVector2D textPos = FVector2D{ pos.X + size.X / 2, pos.Y + 6 };
				this->renderer->draw_text(s2wc((char*)VirtualKeyCodeToString(*key).c_str()), textPos.X, textPos.Y, 0xffffffff, true, true);

				if (isHovered)
				{
					if (Input::IsMouseClicked(VK_LBUTTON, elements_count, false))
					{
						already_pressed = false;
						active_hotkey = elements_count;

						//Queue Fix
						for (int code = 0; code < 255; code++)
							if (GetAsyncKeyState_Spoofed(code)) {}
					}
				}
				else
				{
					if (Input::IsMouseClicked(VK_LBUTTON, elements_count, false))
					{
						active_hotkey = -1;
					}
				}
			}

			sameLine = false;
			last_element_pos = pos;
			last_element_size = size;
			if (first_element_pos.X == 0.0f)
				first_element_pos = pos;
		}


	};
}