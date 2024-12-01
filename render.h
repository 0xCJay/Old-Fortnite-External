#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <d3dx9.h>
#include "protect/skCrypt.h"
#include "settings.h"
#include "spoofer.h"
#include "OVERLAY.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "draw.h"
#include "esp.hpp"
#include "shit.h"
#include "fonts.h"
#include "logo.h"

ImFont* LexendRegular;
ImFont* LexendLight;

ImFont* TabsFont;
ImFont* WeaponFont;
ImFont* iconfont = nullptr;

ImFont* MainFont;
IDirect3DTexture9* Logo;

bool  ShowMenu = true;

static float switch_alpha[3], open_alpha = 0;
static bool toggled = true;
static int m_tabs;
static int enemy_tab;
static int visual_tab;
static int world_visual_tab;
static int color_visual_tab;
static int misc_tab;


void RenderTab()
{
	ImGui::SetCursorPos(ImVec2{ 33, 427 });
	// ImGui::Image(Avatar, ImVec2(40, 40));//steam

	ImGui::PushFont(TabsFont);
	{
		ImGui::SetCursorPos(ImVec2{ 36, 100 });
		if (ImGui::Renderingtab("J", !m_tabs)) m_tabs = 0;

		ImGui::SetCursorPos(ImVec2{ 36, 151 });
		if (ImGui::Renderingtab("G", m_tabs == 1)) m_tabs = 1; // B

		ImGui::SetCursorPos(ImVec2{ 36, 207 });
		if (ImGui::Renderingtab("F", m_tabs == 2)) m_tabs = 2;

		ImGui::SetCursorPos(ImVec2{ 36, 260 });
		if (ImGui::Renderingtab("B", m_tabs == 3)) m_tabs = 3;




	}
	ImGui::PopFont();

	ImGui::PushFont(iconfont);
	{
		ImGui::SetCursorPos(ImVec2{ 36, 313 });

		if (ImGui::Renderingtab("C", m_tabs == 4)) m_tabs = 4;

		ImGui::SetCursorPos(ImVec2{ 36, 366 });
		if (ImGui::Renderingtab("E", m_tabs == 5)) m_tabs = 5;


	}
	ImGui::PopFont();

}

#define V2 ImVec2
#define F float
F k;
int i{};
F r() { return F(rand() / 32768.f) * 2.f - 1.f; };
struct P {
	F x, y, z, a, b, c; void A() {
		x += a * k; y += b * k; z += c * k;
		F ng{ 0.008f }; z -= 5.f; F xp = x * cosf(ng) - z * sinf(ng); F zp = x * sinf(ng) + z * cosf(ng);
		x = xp; z = zp + 5.f; a -= x * k + r() * k; b -= y * k + r() * k; c -= (z - 5.0f) * k + r() * k;
	}
};
P ps[64];
void Particles(ImDrawList* d, ImVec2 o, ImVec2 b, ImVec2 sz, ImVec4, float t)
{
	int j{};
	if (!i) { i = 1; for (P& a : ps) { a = { r(),r(),r() + 5.f,r(),r(),r() }; } }
	for (P& a : ps) {
		if (a.z < 0.001) continue;
		V2 s((a.x / a.z) * sz.x * 2.f + sz.x * 0.5f + o.x, (a.y / a.z) * sz.y * 2.f + sz.y * 0.5f + o.y);
		int x = (j++) % 16;
		k = cosf((j / 64.f) * 3.14592f) * 0.002f + 0.002f;
		F dist = fabsf(a.z - 5.f) / 2.5f, sc = (10.f + dist * 100.f) / a.z;
		int tr = int(ImMin(dist * 128.f, 128.f) + 127) << 24;
		ImColor col = ImVec4(94, 159, 255, 1.f);
		d->AddCircleFilled(s, sc, col, 12); a.A();
	}
}



void RenderSubTab()
{
	if (m_tabs == 0)
	{
		ImGui::SetCursorPos(ImVec2{ 90, 67 });
		if (ImGui::Renderingsubtab(E("   Aim"), !enemy_tab)) enemy_tab = 0;


	}
	if (m_tabs == 1)
	{
		ImGui::SetCursorPos(ImVec2{ 90, 67 });
		if (ImGui::Renderingsubtab(E("   Visuals"), !visual_tab)) visual_tab = 0;

	}
	if (m_tabs == 2)
	{
		ImGui::SetCursorPos(ImVec2{ 90, 67 });
		if (ImGui::Renderingsubtab(E("   World"), !world_visual_tab)) world_visual_tab = 0;

	}
	if (m_tabs == 4)
	{
		ImGui::SetCursorPos(ImVec2{ 90, 67 });
		if (ImGui::Renderingsubtab(E("   Colors"), !color_visual_tab)) color_visual_tab = 0;

	}
	if (m_tabs == 5)
	{
		ImGui::SetCursorPos(ImVec2{ 90, 67 });
		if (ImGui::Renderingsubtab(E("   Misc"), !misc_tab)) misc_tab = 0;

	}
}

void RenderRagetab()
{
	if (enemy_tab == 0)
	{
		ImGui::SetCursorPos(ImVec2{ 125, 30 });
		//ImGui::Image(Main, ImVec2(190, 40));//steam

		auto p = ImGui::GetWindowPos();
		ImGui::GetWindowDrawList()->AddText(LexendRegular, 40, ImVec2(p.x + 100, p.y + 30), ImColor(245, 245, 245, int(255 * ImGui::GetStyle().Alpha)), E("Aim Assitance"));

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, switch_alpha[0]);
		ImGui::SetCursorPos(ImVec2{ 100, 115 });
		{
			ImGui::MenuChild(E("Aim Bot"), ImVec2(240, 180), true);
			{
				ImGui::Spacing(); ImGui::Spacing();

				static bool active = true;
				static bool inactive = false;
				static int procent = 60;

				static KeyBind_t bind;

				static float color[4] = { 1.f, 1.f, 1.f, 1.f };

				ImGui::CoolCheckbox(E("Aimbot"), &aimbot);
				ImGui::CoolCheckbox(E("Draw Fov"), &draw_fov);
				ImGui::CoolCheckbox(E("Prediction"), &prediction);

				ImGui::SliderInt(E("Aim Fov"), &fov, 0, 500, E("%d"));
				ImGui::Spacing(); ImGui::Spacing();
				ImGui::SliderInt(E("Smoothing"), &smoothing, 0, 50, E("%d"));

				ImGui::SetCursorPosX(10.5);
				ImGui::SetCursorPosY(153);
				HotkeyButton(aimkey, ChangeKey, keystatus);
			}
			ImGui::EndChild();

			ImGui::SameLine(0, 15);
			ImGui::MenuChild(E("Trigger Bot"), ImVec2(240, 180), true);
			{
				ImGui::Spacing(); ImGui::Spacing();

				ImGui::CoolCheckbox(E("Triggerbot"), &triggerbot);

				ImGui::CoolCheckbox(E("Shotgun Only"), &shotgun_only);
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::SliderInt(E("Max Distance"), &max_trig_distance, 0, 50, E("%d"));
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::SliderInt(E("Delay"), &trig_delay, 0, 5, E("%d"));
				ImGui::Spacing();


			}
			ImGui::EndChild();
		}

		ImGui::SetCursorPos(ImVec2{ 100, 310 });
		{
			ImGui::MenuChild(E("Hit Box"), ImVec2(240, 150), true);
			{
				ImGui::Spacing(); ImGui::Spacing();

				if (ImGui::CoolCheckbox(E("Head "), &HeadBone))
				{
					NeckBbone = false;
					ChestBone = false;
					PelvisBone = false;

				}
				if (ImGui::CoolCheckbox(E("Neck "), &NeckBbone))
				{
					HeadBone = false;
					ChestBone = false;
					PelvisBone = false;
				}
				if (ImGui::CoolCheckbox(E("Chest"), &ChestBone))
				{
					HeadBone = false;
					NeckBbone = false;
					PelvisBone = false;
				}
				if (ImGui::CoolCheckbox(E("Pelvis"), &PelvisBone))
				{
					HeadBone = false;
					NeckBbone = false;
					ChestBone = false;
				}

			}
			ImGui::EndChild();

			ImGui::SameLine(0, 15);
			ImGui::MenuChild(E("Visuals"), ImVec2(240, 150), true);
			{
				ImGui::Spacing(); ImGui::Spacing();

				ImGui::CoolCheckbox(E("Target Line"), &targetline);
				ImGui::CoolCheckbox(E("Target Circle"), &targetcirlce);

			}
			ImGui::EndChild();
		}
		ImGui::PopStyleVar();
	}
}

DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;


void RenderEsptab()
{
	if (visual_tab == 0)
	{
		ImGui::SetCursorPos(ImVec2{ 125, 30 });
		//ImGui::Image(Main, ImVec2(190, 40));//steam

		auto p = ImGui::GetWindowPos();
		ImGui::GetWindowDrawList()->AddText(LexendRegular, 40, ImVec2(p.x + 100, p.y + 30), ImColor(245, 245, 245, int(255 * ImGui::GetStyle().Alpha)), E("Visuals"));

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, switch_alpha[0]);
		ImGui::SetCursorPos(ImVec2{ 100, 115 });
		{
			ImGui::MenuChild(E("Visual"), ImVec2(240, 340), true);
			{
				ImGui::Spacing(); ImGui::Spacing();

				ImGui::CoolCheckbox(E("Enable"), &esp);
				if (ImGui::CoolCheckbox(E("2D Box "), &box_esp))
				{
					threed = false;
					Corner = false;
					box_round = false;

				}
				if (ImGui::CoolCheckbox(E("3D Box "), &threed))
				{
					box_esp = false;
					Corner = false;
					box_round = false;
				}
				if (ImGui::CoolCheckbox(E("Corner Box"), &Corner))
				{
					threed = false;
					box_round = false;
					box_esp = false;
				}
				if (ImGui::CoolCheckbox(E("Rounded Box"), &box_round))
				{
					box_esp = false;
					Corner = false;
					threed = false;
				}
				ImGui::CoolCheckbox(E("Skeleton"), &skeleton);
				ImGui::CoolCheckbox(E("Head"), &head);
				ImGui::CoolCheckbox(E("Distance"), &draw_distance);
				ImGui::CoolCheckbox(E("Username"), &draw_username);
				ImGui::CoolCheckbox(E("Weapon"), &draw_held_weapon);
				ImGui::CoolCheckbox(E("Custom Image"), &image_esp);
				ImGui::CoolCheckbox(E("Rainbow"), &rainbow);



			}
			ImGui::EndChild();

		}

		ImGui::SetCursorPos(ImVec2{ 100, 310 });
		{


			ImGui::SameLine(0, 15);

			ImGui::MenuChild(E("Radar"), ImVec2(240, 150), true);
			{
				ImGui::Spacing(); ImGui::Spacing();

				if (ImGui::CoolCheckbox(E("Mini Map"), &mini_map_radar))
				{
					radar = false;
				}
				if (ImGui::CoolCheckbox(E("Normal Radar"), &radar))
				{
					mini_map_radar = false;
				}
				ImGui::SliderInt(E("Pos X"), &radar_position_x, 0, 2500, E("%d"));
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::SliderInt(E("Pos Y"), &radar_position_y, 0, 2000, E("%d"));
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::SliderInt(E("Size"), &radar_size, 0, 2000, E("%d"));



			}
			ImGui::EndChild();
			ImGui::SetCursorPos(ImVec2{ 355, 270 });


			ImGui::MenuChild(E("Settings"), ImVec2(240, 185), true);
			{
				ImGui::Spacing(); ImGui::Spacing();

				ImGui::CoolCheckbox(E("Box Outline"), &box_outline);
				ImGui::CoolCheckbox(E("Skeleton Outline"), &skeleton_outline);
				ImGui::CoolCheckbox(E("Text Outline"), &text_outline);
				ImGui::SliderInt(E("Skeleton Thickness"), &skelthick, 0, 10, E("%d"));
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::SliderInt(E("Box Thickness"), &box_thickness, 0, 10, E("%d"));
				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::SliderInt(E("Font Size"), &font_size, 0, 20, E("%d"));



			}
			ImGui::EndChild();
		}
		ImGui::PopStyleVar();
	}
}

void RenderWorldtab()
{
	if (world_visual_tab == 0)
	{
		ImGui::SetCursorPos(ImVec2{ 125, 30 });
		//ImGui::Image(Main, ImVec2(190, 40));//steam

		auto p = ImGui::GetWindowPos();
		ImGui::GetWindowDrawList()->AddText(LexendRegular, 40, ImVec2(p.x + 100, p.y + 30), ImColor(245, 245, 245, int(255 * ImGui::GetStyle().Alpha)), E("World"));

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, switch_alpha[0]);
		ImGui::SetCursorPos(ImVec2{ 100, 115 });
		{
			ImGui::MenuChild(E("World "), ImVec2(240, 340), true);
			{
				ImGui::Spacing(); ImGui::Spacing();


					ImGui::CoolCheckbox(E("Chests"), &show_chests);
					ImGui::CoolCheckbox(E("Ammo Box"), &show_ammo_box);
					ImGui::CoolCheckbox(E("MC Nugget"), &show_mcnugget);
					ImGui::CoolCheckbox(E("Pickups"), &show_pickups);
					ImGui::CoolCheckbox(E("Vehicles"), &show_vehicles);
					ImGui::CoolCheckbox(E("Cooler"), &show_cooler);
					ImGui::CoolCheckbox(E("Food"), &food_stuff);
					ImGui::CoolCheckbox(E("Gold"), &Gold);

					ImGui::CoolCheckbox(E("Mod Station"), &mod_station);
					ImGui::CoolCheckbox(E("Bullshit Esp"), &bullshit);

					

					ImGui::CoolCheckbox(E("Developer"), &developer);




			}
			ImGui::EndChild();

		}

		ImGui::SetCursorPos(ImVec2{ 100, 310 });
		{


			ImGui::SameLine(0, 15);

			ImGui::MenuChild(E("Settings"), ImVec2(240, 290), true);
			{
				ImGui::Spacing(); ImGui::Spacing();

				ImGui::Spacing();
				ImGui::CoolCheckbox(E("3D Box"), &loot_boxes);
				ImGui::Spacing();

							ImGui::SliderInt(E("Chests Distance"), &chest_render_distance, 0, 500, E("%d"));
							ImGui::Spacing();
							ImGui::Spacing();
							ImGui::SliderInt(E("Ammo Distance"), &ammo_render_distance, 0, 500, E("%d"));
							ImGui::Spacing();
							ImGui::Spacing();
							ImGui::SliderInt(E("MC Nugget Distance"), &mcnugget_render_distance, 0, 500, E("%d"));
							ImGui::Spacing();
							ImGui::Spacing();
							ImGui::SliderInt(E("Pickups Distance"), &pickup_render_distance, 0, 500, E("%d"));
							ImGui::Spacing();
							ImGui::Spacing();
							ImGui::SliderInt(E("Vehicles"), &vehicle_render_distance, 0, 500, E("%d"));
							ImGui::Spacing();
							ImGui::Spacing();
							ImGui::SliderInt(E("Cooler"), &cooler_render_distance, 0, 500, E("%d"));
							ImGui::Spacing();
							ImGui::Spacing();
							ImGui::SliderInt(E("Food"), &food_render_distance, 0, 500, E("%d"));
							ImGui::Spacing();
							ImGui::Spacing();
							ImGui::SliderInt(E("Gold"), &gold_render_distance, 0, 500, E("%d"));
							ImGui::Spacing();
							ImGui::Spacing();
							ImGui::SliderInt(E("Mod Station"), &mod_render_distance, 0, 500, E("%d"));

							
			}
			ImGui::EndChild();
			ImGui::SetCursorPos(ImVec2{ 355, 270 });

		}
		ImGui::PopStyleVar();
	}
}

void RenderColortab()
{
	if (color_visual_tab == 0)
	{
		ImGui::SetCursorPos(ImVec2{ 125, 30 });
		//ImGui::Image(Main, ImVec2(190, 40));//steam

		auto p = ImGui::GetWindowPos();
		ImGui::GetWindowDrawList()->AddText(LexendRegular, 40, ImVec2(p.x + 100, p.y + 30), ImColor(245, 245, 245, int(255 * ImGui::GetStyle().Alpha)), E("Colors"));

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, switch_alpha[0]);
		ImGui::SetCursorPos(ImVec2{ 100, 115 });
		{
			ImGui::MenuChild(E("Colors"), ImVec2(240, 350), true);
			{
				ImGui::Spacing(); ImGui::Spacing();


				ImGui::Spacing();
				ImGui::Text("Accent Color");
				ImGui::ColorPicker3(E("Menu Color "), accentcolor);







				//ImGui::CoolCheckbox(E("Developer"), &developer);




			}
			ImGui::EndChild();

		}

		ImGui::SetCursorPos(ImVec2{ 100, 310 });
		{


			ImGui::SameLine(0, 15);

			ImGui::MenuChild(E("Settings"), ImVec2(240, 350), true, ImGuiWindowFlags_NoScrollbar);
			{
				ImGui::Spacing(); ImGui::Spacing();
				ImGui::Text("Visible Color");
				ImGui::ColorPicker3(E("Visible Color "), PlayerColor::visible_color);
				ImGui::Text("Non Visible Color");
				ImGui::ColorPicker3(E("Non Visible Color "), PlayerColor::nonvisible_color);
			}
			ImGui::EndChild();
			ImGui::SetCursorPos(ImVec2{ 355, 270 });

		}
		ImGui::PopStyleVar();
	}
}

void RenderExtratab()
{
	if (color_visual_tab == 0)
	{
		ImGui::SetCursorPos(ImVec2{ 125, 30 });
		auto p = ImGui::GetWindowPos();
		ImGui::GetWindowDrawList()->AddText(LexendRegular, 40, ImVec2(p.x + 100, p.y + 30), ImColor(245, 245, 245, int(255 * ImGui::GetStyle().Alpha)), E("Extras"));

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, switch_alpha[0]);
		ImGui::SetCursorPos(ImVec2{ 100, 115 });
		{
			ImGui::MenuChild(E("Extras"), ImVec2(240, 350), true);
			{
				ImGui::Spacing(); ImGui::Spacing();

				ImGui::CoolCheckbox(E("Stream Proof"), &streamproof);
				ImGui::CoolCheckbox(E("Show FPS"), &draw_fps);
				ImGui::CoolCheckbox(E("Crosshair"), &draw_crosshair);

				ImGui::CoolCheckbox(E("Background"), &background);
				ImGui::Text(E("Fonts"));

				if (ImGui::CoolCheckbox(E("Fortnite "), &fort_font))
				{
					Proggy = false;
					verdana = false;
					arial = false;
					Roboto = false;
					Poppins = false;

				}
				if (ImGui::CoolCheckbox(E("Proggy "), &Proggy))
				{
					fort_font = false;
					verdana = false;
					arial = false;
					Roboto = false;
					Poppins = false;
				}
				if (ImGui::CoolCheckbox(E("Verdana"), &verdana))
				{
					Proggy = false;
					fort_font = false;
					arial = false;
					Roboto = false;
					Poppins = false;
				}
				if (ImGui::CoolCheckbox(E("Arial"), &arial))
				{
					fort_font = false;
					Proggy = false;
					verdana = false;
					Roboto = false;
					Poppins = false;
				}

				if (ImGui::CoolCheckbox(E("Robot"), &Roboto))
				{
					Proggy = false;
					fort_font = false;
					arial = false;
					Poppins = false;
					verdana = false;
				}
				if (ImGui::CoolCheckbox(E("Poppins"), &Poppins))
				{
					Proggy = false;
					fort_font = false;
					arial = false;
					Roboto = false;
					verdana = false;
				}
			}
			ImGui::EndChild();

		}


		ImGui::PopStyleVar();
	}
}


void CreateStyle()
{
	auto& style = ImGui::GetStyle();
	auto& color = style.Colors;
	style.FrameRounding = 0;
	style.WindowBorderSize = 0;
	style.ScrollbarRounding = 0;
	style.ScrollbarSize = 5;
}

inline void DrawMenu() {

	if (GetAsyncKeyState_Spoofed(VK_INSERT))
	{
		menu_open = !menu_open;
		Sleep(300);
	}

	RECT screen_rect;
	::GetWindowRect(::GetDesktopWindow(), &screen_rect);

	ImGui::PushFont(NoramlFOnt);

	CreateStyle();

	if (menu_open)
	{

		// menu goes here just use this one or fucking make ur own 
	}
	else
	{
	}
}
__forceinline VOID directx_init()
{
	SPOOF_FUNC
		if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &DirectX9Interface::p_object)))
			exit(3);

	ZeroMemory(&DirectX9Interface::p_params, sizeof(DirectX9Interface::p_params));
	DirectX9Interface::p_params.Windowed = TRUE;
	DirectX9Interface::p_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	DirectX9Interface::p_params.hDeviceWindow = DirectX9Interface::hijacked_hwnd;
	DirectX9Interface::p_params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	DirectX9Interface::p_params.BackBufferFormat = D3DFMT_A8R8G8B8;
	DirectX9Interface::p_params.BackBufferWidth = screen_width;
	DirectX9Interface::p_params.BackBufferHeight = screen_height;
	DirectX9Interface::p_params.EnableAutoDepthStencil = TRUE;
	DirectX9Interface::p_params.AutoDepthStencilFormat = D3DFMT_D16;
	DirectX9Interface::p_params.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	if (FAILED(DirectX9Interface::p_object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DirectX9Interface::hijacked_hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &DirectX9Interface::p_params, 0, &DirectX9Interface::p_device)))
	{
		DirectX9Interface::p_object->Release();
		Sleep(1000);
		exit(1);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(DirectX9Interface::hijacked_hwnd);
	ImGui_ImplDX9_Init(DirectX9Interface::p_device);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	static const ImWchar icons_ranges[] = { 0xf000, 0xf5ff, 0 };
	ImFontConfig icons_config;

	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	ImFontConfig config;

	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 1;
	icons_config.OversampleV = 1;
	ImFontConfig CustomFont;
	CustomFont.FontDataOwnedByAtlas = false;

	D3DXCreateTextureFromFileInMemoryEx(DirectX9Interface::p_device, logomenu, sizeof(logomenu), 92.52f, 77.66f, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &Logo);
	D3DXCreateTextureFromFileEx(DirectX9Interface::p_device, "C:\\pic.png", 1000, 1000, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &custom_image);


	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	FortniteFont = io.Fonts->AddFontFromMemoryTTF(burbank_bold, sizeof(burbank_bold), 17, NULL, io.Fonts->GetGlyphRangesCyrillic());
	Arial = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Arial.ttf"), 15.f);
	Verdana = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Verdana.ttf"), 15.f);
	proggy = io.Fonts->AddFontFromMemoryTTF(proggyclean, sizeof(proggyclean), 15.f);
	roboto = io.Fonts->AddFontFromMemoryTTF(&roboto1, sizeof roboto1, 20, NULL, io.Fonts->GetGlyphRangesCyrillic());
	poppins = io.Fonts->AddFontFromMemoryTTF(poppins1, sizeof(poppins1), 15.f);
	MainFont = io.Fonts->AddFontFromMemoryTTF(MuseoSansCyrl3000, sizeof(MuseoSansCyrl3000), 16, NULL, io.Fonts->GetGlyphRangesCyrillic());
	TabsFont = io.Fonts->AddFontFromMemoryTTF(MuseoSansCyrl5000, sizeof(MuseoSansCyrl5000), 25, NULL, io.Fonts->GetGlyphRangesCyrillic());
	WeaponFont = io.Fonts->AddFontFromMemoryTTF(astriumwep, sizeof(astriumwep), 20, NULL, io.Fonts->GetGlyphRangesCyrillic());
	iconfont = io.Fonts->AddFontFromMemoryTTF((void*)icon, sizeof(icon), 25, NULL, io.Fonts->GetGlyphRangesCyrillic());

	LexendRegular = io.Fonts->AddFontFromMemoryTTF(LexendLightfont, sizeof(LexendLightfont), 40, NULL, io.Fonts->GetGlyphRangesCyrillic());
	LexendLight = io.Fonts->AddFontFromMemoryTTF(LexendLightfont, sizeof(LexendLightfont), 20, NULL, io.Fonts->GetGlyphRangesCyrillic());

	DirectX9Interface::p_object->Release();
}

VOID render_loop()
{
	SPOOF_FUNC;
	static RECT old_rc;
	RtlSecureZeroMemory(&DirectX9Interface::message, sizeof(MSG));
	while (DirectX9Interface::message.message != WM_QUIT)
	{
		if (PeekMessageA(&DirectX9Interface::message, DirectX9Interface::hijacked_hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&DirectX9Interface::message);
			DispatchMessageA(&DirectX9Interface::message);
		}

		RECT rc;
		POINT xy;
		RtlSecureZeroMemory(&rc, sizeof(RECT));
		RtlSecureZeroMemory(&xy, sizeof(POINT));
		GetClientRect(DirectX9Interface::game_wnd, &rc);
		ClientToScreen(DirectX9Interface::game_wnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = DirectX9Interface::game_wnd;
		screen_width = io.DisplaySize.x;
		screen_height = io.DisplaySize.y;
		center_x = io.DisplaySize.x / 2;
		center_y = io.DisplaySize.y / 2;

		POINT p;
		GetCursorPosA(&p);
		long styleProc = GetWindowLongA_Spoofed(DirectX9Interface::hijacked_hwnd, GWL_EXSTYLE);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;
		if (menu_open)
		{
			styleProc &= ~WS_EX_LAYERED;
			SetWindowLongA_Spoofed(DirectX9Interface::hijacked_hwnd,
				GWL_EXSTYLE, styleProc);
			SetForegroundWindow(DirectX9Interface::hijacked_hwnd);
		}
		else
		{
			styleProc |= WS_EX_LAYERED;
			SetWindowLong(DirectX9Interface::hijacked_hwnd,
				GWL_EXSTYLE, styleProc);
		}
		if (GetAsyncKeyState_Spoofed(0x1))
		{
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
		{
			io.MouseDown[0] = false;
		}
		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{
			old_rc = rc;
			DirectX9Interface::p_params.BackBufferWidth = screen_width;
			DirectX9Interface::p_params.BackBufferHeight = screen_height;
			SetWindowPos_Spoofed(DirectX9Interface::hijacked_hwnd, (HWND)0, xy.x, xy.y, screen_width, screen_height, SWP_NOREDRAW);
			DirectX9Interface::p_device->Reset(&DirectX9Interface::p_params);
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ColorChange();
		gaybow;
		fortnite_radar(radar_position_x, radar_position_y, radar_size, false);

		actor_loop();

		floot->LevelDrawing();

		DrawMenu();
		if (!streamproof_once && streamproof) {
			hide_overlay();
			streamproof_once = true;
		}

		if (streamproof_once && !streamproof) {
			show_overlay();
			streamproof_once = false;
		}
		ImGui::EndFrame();

		DirectX9Interface::p_device->SetRenderState(D3DRS_ZENABLE, false);
		DirectX9Interface::p_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		DirectX9Interface::p_device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		DirectX9Interface::p_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		if (DirectX9Interface::p_device->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			DirectX9Interface::p_device->EndScene();
		}
		HRESULT result = DirectX9Interface::p_device->Present(NULL, NULL, NULL, NULL);
		if (result == D3DERR_DEVICELOST && DirectX9Interface::p_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			DirectX9Interface::p_device->Reset(&DirectX9Interface::p_params);
			ImGui_ImplDX9_CreateDeviceObjects();
		}

	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	if (DirectX9Interface::p_device != NULL)
	{
		DirectX9Interface::p_device->EndScene();
		DirectX9Interface::p_device->Release();
	}
	if (DirectX9Interface::p_object != NULL)
	{
		DirectX9Interface::p_object->Release();
	}
	DestroyWindow(DirectX9Interface::hijacked_hwnd);
}