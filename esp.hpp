	#pragma once
#include <memory>
#include "cache.h"

#include "draw.h"
#include "aimkey.h"
#include "driver.hpp"
#include "protect/skCrypt.h"
#include "imgui/imgui_internal.h"
#include "test menu.h"
#include "controlelr.h"
#pragma warning(disable : 4996)

bool has_clicked;
std::chrono::steady_clock::time_point tb_begin;
std::chrono::steady_clock::time_point tb_end;
int tb_time_since;
__int64 TargetedBuild;
bool bTargetedBuild;

HWND Fort_windwo = NULL;
IDirect3DTexture9* custom_image;

ImFont* NoramlFOnt;
ImFont* ESPFont;
ImFont* FortniteFont;
ImFont* Arial;
ImFont* Verdana;
ImFont* proggy;
ImFont* poppins;
ImFont* roboto;

vector3 PredictLocation(vector3 target, vector3 targetVelocity, float projectileSpeed, float projectileGravityScale, float distance)
{
	float horizontalTime = distance / projectileSpeed;
	float verticalTime = distance / projectileSpeed;

	target.x += targetVelocity.x * horizontalTime;
	target.y += targetVelocity.y * horizontalTime;
	target.z += targetVelocity.z * verticalTime +
		abs(-980 * projectileGravityScale) * 0.5f * (verticalTime * verticalTime);

	return target;
}

enum EFortWeaponType : int
{
	Rifle = 0,
	Shotgun = 1,
	Smg = 2,
	Sniper = 3
};

static const size_t BUFF_SIZE = 1024;
static const size_t BUFF_SIZE_test = 3024;

//
//static std::string GetNameFromIndex(int key) {
//	if (global->local_pawn)
//	{
//		uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
//		uint16_t NameOffset = (uint16_t)key;
//		uint64_t NamePoolChunk = request->read<uint64_t>(Bypass::GameBase + 0x12894800 + (8 * ChunkOffset) + 0x10) + (unsigned int)(2 * NameOffset);
//		uint16_t nameEntry = request->read<uint16_t>(NamePoolChunk);
//		int nameLength = nameEntry >> 6;
//		char buff[1024] = {};
//
//		char* v2; // rdi
//		int v4; // ebx
//		__int16 result; // ax
//		int v6; // edx
//		int v7; // ecx
//		int v8; // ecx
//		__int16 v9; // ax
//
//		v2 = buff;
//		v4 = nameLength;
//		result = nameLength;
//		v6 = 0;
//		v7 = 30;
//
//		if (v4)
//		{
//			Bypass::read(PVOID(NamePoolChunk + 2), (PVOID)buff, 2 * nameLength);
//
//			do
//			{
//				v8 = v6++ | v7;
//				v9 = v8;
//				v7 = 2 * v8;
//				result = ~v9;
//				*v2++ ^= result;
//			} while (v6 < v4);
//			buff[nameLength] = '\0';
//			return std::string(buff);
//		}
//		return std::string("");
//	}
//	else
//	{
//		return std::string("");
//
//	}
//}

static std::string GetNameFromIndex(int key) {
	try {
	
		uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
				uint16_t NameOffset = (uint16_t)key;
				uint64_t NamePoolChunk = request->read<uint64_t>(Bypass::GameBase + 0x12894800 + (8 * ChunkOffset) + 0x10) + (unsigned int)(2 * NameOffset);
				

				if (!request->IsValidAddress(NamePoolChunk)) {
					return "nan";
				}

				uint16_t nameEntry = request->read<uint16_t>(NamePoolChunk);
				if (!request->IsValidAddress(NamePoolChunk)) {
					return "nan";
				}

				int nameLength = nameEntry >> 6;

				if (nameLength <= 0 || nameLength >= BUFF_SIZE) {
					return "nan";
				}
				char buff[BUFF_SIZE] = {};

				char* v2; // rdi
				int v4; // ebx
				__int16 result; // ax
				int v6; // edx
				int v7; // ecx
				int v8; // ecx
				__int16 v9; // ax


		
				v2 = buff;
				v4 = nameLength;
				result = nameLength;
				v6 = 0;
				v7 = 30;
		
				if (v4)
				{
					Bypass::read(PVOID(NamePoolChunk + 2), (PVOID)buff, 2 * nameLength);
		
					do
					{
						v8 = v6++ | v7;
						v9 = v8;
						v7 = 2 * v8;
						result = ~v9;
						*v2++ ^= result;
					} while (v6 < v4);
					buff[nameLength] = '\0';
					return std::string(buff);
				}
				return std::string("");
	}
	catch (const std::exception& e) {
		// Handle standard exceptions
		return "nan";
	}
	catch (...) {
		// Handle non-standard exceptions
		return "nan";
	}
}

static std::string GetNameFromFName(int key)
{
	uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
	uint16_t NameOffset = (uint16_t)key;

	if (global->local_pawn)
	{

		uint64_t NamePoolChunk = request->read<uint64_t>(Bypass::GameBase + 0x12894800 + (8 * ChunkOffset) + 16) + (unsigned int)(2 * NameOffset); //((ChunkOffset + 2) * 8) ERROR_NAME_SIZE_EXCEEDED
		if (request->read<uint16_t>(NamePoolChunk) < 64)
		{
			auto a1 = request->read<DWORD>(NamePoolChunk + 2);
			return GetNameFromIndex(a1);
		}
		else
		{
			return GetNameFromIndex(key);
		}
	}
	else
	{
		return "";
	}
}




uintptr_t rootcomp(uintptr_t actor) {

	return request->read<uintptr_t>(actor + 0x198);
}
vector3 get_location(uintptr_t actor) {

	return request->read<vector3>(rootcomp(actor) + 0x120);
}


std::string filterInvalidChars(const std::string& input) {
	std::string filteredText;
	for (char c : input) {
		// Check if the character is within the printable ASCII range and not null ('\0')
		if (c >= 0x20 && c <= 0x7E && c != '\0') {
			// Append valid characters to the filtered text
			filteredText += c;
		}
	}
	return filteredText;
}

std::wstring filterInvalidChars1(const std::wstring& input) {
	std::wstring filteredText;
	for (char c : input) {
		// Check if the character is within the printable ASCII range and not null ('\0')
		if (c >= 0x20 && c <= 0x7E && c != '\0') {
			// Append valid characters to the filtered text
			filteredText += c;
		}
	}
	return filteredText;
}

vector3 chicken_bone(uintptr_t mesh, int id) {
	SPOOF_FUNC
		
	uintptr_t bone_array = request->read<uintptr_t>(mesh + 0x5B0);
	if (bone_array == NULL) bone_array = request->read<uintptr_t>(mesh + 0x5B0 + 0x10);
	FTransform bone = request->read<FTransform>(bone_array + (id * 0x60));
	FTransform component_to_world = request->read<FTransform>(mesh + 0x1c0);
	D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());
	return vector3(matrix._41, matrix._42, matrix._43);


}


class NIGGERITEMS {
public:
	uintptr_t
		Actor;

	std::string
		Name;
	float
		distance;
};
std::vector<NIGGERITEMS> item_pawns;
std::mutex lootMutex;


namespace Offsets
{
	namespace UFortMovementComp_Character {
		auto bWantsToSwing = 0x14cc;
		auto bSwingInstantVelocity = 0x14cf;
		auto SwingAttachLocation = 0x14d0;
		auto SwingLaunch = 0x14e8;
	}
}
ImColor VIsible_color;


std::mutex worlddly_mtx;
class loott
{
public:


	static auto levels_call() -> void {
		std::thread([&]() {
			for (;;)
			{
				try
				{
					CacheLevels();
				}
				catch (...) {
				}
			}
			}).detach();
	}


	static auto CacheLevels() -> void {


		constexpr size_t max_items = 800; // Maximum number of items to limit vector growth and prevent potential buffer overflow.
		constexpr size_t max_items_count = 8000; // Maximum number of items to limit vector growth and prevent potential buffer overflow.

		for (;;) {

			if (!(u)global->local_pawn) {

				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				//std::cout << "invalid u acknowledged_pawn!" << std::endl;
				continue;
			}

			if (!(show_chests || show_pickups || show_ammo_box || show_mcnugget || show_llamas || mod_station || food_stuff || show_vehicles || show_cooler || Gold)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				//std::cout << "invalid u world!" << std::endl;
				continue;
			}


			std::vector<NIGGERITEMS> mrxd;
			uintptr_t ItemLevels = request->read<uintptr_t>((u)global->world + 0x178);
			if (!request->IsValidAddress(ItemLevels)) {
				//std::cout << "invalid u ItemLevels!" << std::endl;

				continue; // Skip iteration if ItemLevels is invalid
			}
			//std::cout << "got ItemLevels!" << std::endl;

			DWORD itemSize = request->read<DWORD>((u)global->world + (0x178 + sizeof(PVOID)));
			//std::cout << "item size: " << itemSize << std::endl;
			if (itemSize == NULL) {
				continue; // Skip to next item if ItemLevel is invalid
			}
			itemSize = min(itemSize, max_items); // Limit the number of items processed


			for (DWORD i = 0; i < itemSize; ++i) {

				if (!(u)global->local_pawn) {
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
					continue;
				}
				//if (!entity_pointers->acknowledged_pawn)goto startfa;

				uintptr_t ItemLevel = request->read<uintptr_t>(ItemLevels + (i * sizeof(uintptr_t)));
				if (!request->IsValidAddress(ItemLevel)) {
					continue; // Skip to next item if ItemLevel is invalid
				}

				DWORD itemCount = request->read<DWORD>(ItemLevel + (0xA0 + sizeof(PVOID)));
			//	std::cout << "item count: " << itemCount << std::endl;


				if (itemCount == NULL) {
					continue; // Skip to next item if ItemLevel is invalid
				}

				itemCount = min(itemCount, max_items_count); // Limit the number of items processed

				uintptr_t ItemsPawns = request->read<uintptr_t>(ItemLevel + 0xA0);
				if (!request->IsValidAddress(ItemsPawns)) {
					continue; // Skip if ItemsPawns is invalid
				}

				for (DWORD j = 0; j < itemCount; ++j) {
					uintptr_t CurrentItemPawn = request->read<uintptr_t>(ItemsPawns + (j * sizeof(uintptr_t)));
					if (!request->IsValidAddress(CurrentItemPawn)) {
						continue; // Skip if CurrentItemPawn is invalid
					}

					vector3 ItemPosition = get_location(CurrentItemPawn);
					float ItemDist = camera_postion.location.distance(ItemPosition) / 100.f;
				//	std::cout << "item Dist: " << ItemDist << std::endl;


					int ItemIndex = request->read<int>(CurrentItemPawn + 0x18);

				//	std::cout << "item Index: " << ItemIndex << std::endl;

					auto CurrentItemPawnName = GetNameFromFName(ItemIndex);

					//std::cout << CurrentItemPawnName << std::endl;
					bool bIsPickup = show_chests || show_pickups || show_ammo_box || show_mcnugget || show_llamas || mod_station || food_stuff || show_vehicles || show_cooler || Gold;



					if (bIsPickup) {
						NIGGERITEMS newItem;
						newItem.Actor = CurrentItemPawn;
						newItem.Name = CurrentItemPawnName;
						newItem.distance = ItemDist;

						// Efficiently insert in a sorted position by distance
						auto it = std::lower_bound(mrxd.begin(), mrxd.end(), newItem, [](const NIGGERITEMS& a, const NIGGERITEMS& b) {
							return a.distance < b.distance;
							});
						mrxd.insert(it, newItem);

						// Keep the vector size limited to max_items
						if (mrxd.size() > max_items) {
							mrxd.resize(max_items); // Efficiently trim to max_items
						}
					}
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(200));

			std::lock_guard<std::mutex> lock(worlddly_mtx);
			item_pawns.clear();
			item_pawns = std::move(mrxd);
		}

	}




	void LevelDrawing() {
		// Copy the list of cached items
		std::vector<NIGGERITEMS> levelListCopy;
		// Copy the list of cached items
		{
			std::lock_guard<std::mutex> lock(worlddly_mtx);
			levelListCopy = item_pawns;
		}

		if (Proggy)
		{
			ImGui::PushFont(proggy);
		}

		if (fort_font)
		{
			ImGui::PushFont(FortniteFont);
		}

		if (verdana)
		{
			ImGui::PushFont(Verdana);
		}
		if (arial)
		{
			ImGui::PushFont(Arial);
		}

		if (Poppins)
		{
			ImGui::PushFont(poppins);
		}

		if (Roboto)
		{
			ImGui::PushFont(roboto);
		}

		// Iterate over each item in the copied list and draw relevant information on the screen
		for (auto entity : levelListCopy)
		{


			if ((u)global->local_pawn && show_pickups || show_ammo_box || show_chests || show_mcnugget || show_llamas || show_vehicles || show_cooler || food_stuff || Gold)
			{
				// Check various conditions and draw information based on item type and distance
				if (show_chests && (strstr(entity.Name.c_str(), _xor_("Tiered_Chest").c_str()) or strstr(entity.Name.c_str(), _xor_("AlwaysSpawn_NormalChest").c_str()) or strstr(entity.Name.c_str(), _xor_("AlwaysSpawn_RareChest").c_str()) or strstr(entity.Name.c_str(), _xor_("OlympusFactionChest").c_str()))) {
					if (entity.distance <= chest_render_distance)
					{
						auto bAlreadySearched = (request->read<char>(entity.Actor + 0xD92) >> 1);
						if (bAlreadySearched) continue;

						auto root_comp = rootcomp(entity.Actor);
						

						vector3 Location = get_location(entity.Actor);
						vector3 bounds = vector3(Location.x, Location.y, + Location.z + 50);

						vector2 Screen = w2s(Location);
						if (in_screen(Screen))
						{
							std::string wtf2 = ("Chest [") + std::to_string((int)entity.distance) + ("m]");
							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());

						//	DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(245, 245, 0), true, true, wtf2.c_str());
							if (loot_boxes)
							{
								if (box_outline)
								{
									DrawThreeDBox(Location, bounds, 100, 100, box_thickness + 2.5, ImColor(0, 0, 0));
								}
								DrawThreeDBoxText(Location, bounds, 100, 100, box_thickness, ImColor(245, 245, 0), wtf2.c_str());

							}
							else
							{
								DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(245, 245, 0), true, text_outline, wtf2.c_str());
							}

						}
					}
				}

				if (show_ammo_box && strstr(entity.Name.c_str(), _xor_("Tiered_Ammo").c_str())) {
					if (entity.distance <= ammo_render_distance)
					{
						auto bAlreadySearched = (request->read<char>(entity.Actor + 0xD92) >> 1);
						if (bAlreadySearched) continue;

						vector3 Location = get_location(entity.Actor);

						vector3 bounds = vector3(Location.x, Location.y, +Location.z + 30);


						vector2 Screen = w2s(Location);

						if (in_screen(Screen))
						{
							std::string wtf2 = ("Ammobox [") + std::to_string((int)entity.distance) + ("m]");
							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
						//	DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(0, 245, 0), true, true, wtf2.c_str());
							if (loot_boxes)
							{
								if (box_outline)
								{
									DrawThreeDBox(Location, bounds, 50, 50, box_thickness + 2.5, ImColor(0, 0, 0));
								}
								DrawThreeDBoxText(Location, bounds, 50, 50, box_thickness, ImColor(0, 245, 0), wtf2.c_str());
							}
							else
							{
								DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(0, 245, 0), true, text_outline, wtf2.c_str());
							}


						}
					}
				}

				if (developer)
				{
					if (entity.distance <= 10)
					{
						vector3 Location = get_location(entity.Actor);
						vector2 Screen = w2s(Location);

						if (in_screen(Screen))
						{
							std::string wtf2 = entity.Name.c_str();
							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
							DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, text_outline, wtf2.c_str());


						}
					}
				}


				if (show_mcnugget && strstr(entity.Name.c_str(), _xor_("NPC_Pawn_Irwin_Prey_Nug").c_str())) {
					if (entity.distance <= mcnugget_render_distance) {
						vector3 Location = get_location(entity.Actor);
						vector2 Screen = w2s(Location);
						auto mesh = request->read<uintptr_t>(entity.Actor + 0x318);

			
						ImDrawList* drawList = ImGui::GetBackgroundDrawList();	
						auto top_bone = chicken_bone(mesh, 11);
						auto bottom_bone = chicken_bone(mesh, 0);
						auto head_bone = chicken_bone(mesh, 8);
						vector2 head_screen = w2s(head_bone);
						auto neck_bone = chicken_bone(mesh, 7);
						vector2 neck_screen = w2s(neck_bone);
						auto spine_bone = chicken_bone(mesh, 6);
						vector2 spine_screen = w2s(spine_bone);
						auto mid_spine_bone = chicken_bone(mesh, 4);
						vector2 mid_spine_screen = w2s(mid_spine_bone);
						auto lower_spine_bone = chicken_bone(mesh,2);
						vector2 lower_spine_screen = w2s(lower_spine_bone);
						auto top_arm_1_bone = chicken_bone(mesh,19);
						vector2 top_arm_1_screen = w2s(top_arm_1_bone);
						auto top_arm_2_bone = chicken_bone(mesh, 15);
						vector2 top_arm_2_screen = w2s(top_arm_2_bone);
						auto mid_arm_1_bone = chicken_bone(mesh, 19);
						vector2 mid_arm_1_screen = w2s(mid_arm_1_bone);
						auto mid_arm_3_bone = chicken_bone(mesh, 20);
						vector2 mid_arm_3_screen = w2s(mid_arm_3_bone);
						auto mid_arm_2_bone = chicken_bone(mesh, 16);
						vector2 mid_arm_2_screen = w2s(mid_arm_2_bone);
						auto bottom_arm_1_bone = chicken_bone(mesh, 21);
						vector2 bottom_arm_1screen = w2s(bottom_arm_1_bone);
						auto bottom_arm_2_bone = chicken_bone(mesh, 17);
						vector2 bottom_arm_2_screen = w2s(bottom_arm_2_bone);
						auto pelvis_bone = chicken_bone(mesh, 1);
						vector2 pelvis_screen = w2s(pelvis_bone);
						auto top_leg_1_bone = chicken_bone(mesh, 25);
						vector2 top_leg_1_screen = w2s(top_leg_1_bone);
						auto mid_leg_1_bone = chicken_bone(mesh, 28);
						vector2 mid_leg_1_screen = w2s(mid_leg_1_bone);
						auto bottom_leg_1_bone = chicken_bone(mesh, 27);
						vector2 bottom_leg_1_screen = w2s(bottom_leg_1_bone);
						auto top_leg_2_bone = chicken_bone(mesh, 38);
						vector2 top_leg_2_screen = w2s(top_leg_2_bone);
						auto mid_leg_2_bone = chicken_bone(mesh, 39);
						vector2 mid_leg_2_screen = w2s(mid_leg_2_bone);
						auto bottom_leg_2_bone = chicken_bone(mesh, 40);
						vector2 bottom_leg_2_screen = w2s(bottom_leg_2_bone);
						vector2 head_2d = w2s(vector3(head_bone.x, head_bone.y, head_bone.z));
						vector3 delta = head_bone - camera_postion.location;
						float distance = delta.length();

						const float constant_circle_size = 10;

						float circle_radius = constant_circle_size * (screen_height / (2.0f * distance * tanf(camera_postion.fov * (float)M_PI / 360.f)));

						int segments = 50;
						float thickness = 2.0f;

						if (head)
						{
							if (skeleton_outline)
							{
								ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, ImColor(0, 0, 0, 255), segments, skelthick + 2);
							}
							ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, ImColor(66, 245, 212), segments, skelthick);

						}

						if (skeleton)
						{
							if (skeleton_outline)
							{

								drawList->AddLine(ImVec2(neck_screen.x, neck_screen.y), ImVec2(spine_screen.x, spine_screen.y), ImColor(0, 0, 0), 4.5);
								//SPINE
								drawList->AddLine(ImVec2(spine_screen.x, spine_screen.y), ImVec2(mid_spine_screen.x, mid_spine_screen.y), ImColor(0, 0, 0), 4.5);
								drawList->AddLine(ImVec2(mid_spine_screen.x, mid_spine_screen.y), ImVec2(lower_spine_screen.x, lower_spine_screen.y), ImColor(0, 0, 0), 4.5);
								// SHOULDER
								drawList->AddLine(ImVec2(spine_screen.x, spine_screen.y), ImVec2(top_arm_1_screen.x, top_arm_1_screen.y), ImColor(0, 0, 0), 4.5);
								drawList->AddLine(ImVec2(spine_screen.x, spine_screen.y), ImVec2(top_arm_2_screen.x, top_arm_2_screen.y), ImColor(0, 0, 0), 4.5);
								// ELBOW
								drawList->AddLine(ImVec2(top_arm_2_screen.x, top_arm_2_screen.y), ImVec2(mid_arm_2_screen.x, mid_arm_2_screen.y), ImColor(0, 0, 0), 4.5);
								drawList->AddLine(ImVec2(top_arm_1_screen.x, top_arm_1_screen.y), ImVec2(mid_arm_1_screen.x, mid_arm_1_screen.y), ImColor(0, 0, 0), 4.5);
								drawList->AddLine(ImVec2(mid_arm_1_screen.x, mid_arm_1_screen.y), ImVec2(mid_arm_3_screen.x, mid_arm_3_screen.y), ImColor(0, 0, 0), 4.5);
								// Hands
								drawList->AddLine(ImVec2(mid_arm_3_screen.x, mid_arm_3_screen.y), ImVec2(bottom_arm_1screen.x, bottom_arm_1screen.y), ImColor(0, 0, 0), 4.5);
								drawList->AddLine(ImVec2(mid_arm_2_screen.x, mid_arm_2_screen.y), ImVec2(bottom_arm_2_screen.x, bottom_arm_2_screen.y), ImColor(0, 0, 0), 4.5);
								// Pelvis
								drawList->AddLine(ImVec2(lower_spine_screen.x, lower_spine_screen.y), ImVec2(pelvis_screen.x, pelvis_screen.y), ImColor(0, 0, 0), 4.5);
								// LEG
								drawList->AddLine(ImVec2(pelvis_screen.x, pelvis_screen.y), ImVec2(top_leg_1_screen.x, top_leg_1_screen.y), ImColor(0, 0, 0), 4.5);
								drawList->AddLine(ImVec2(top_leg_1_screen.x, top_leg_1_screen.y), ImVec2(mid_leg_1_screen.x, mid_leg_1_screen.y), ImColor(0, 0, 0), 4.5);
								drawList->AddLine(ImVec2(mid_leg_1_screen.x, mid_leg_1_screen.y), ImVec2(bottom_leg_1_screen.x, bottom_leg_1_screen.y), ImColor(0, 0, 0), 4.5);

								drawList->AddLine(ImVec2(pelvis_screen.x, pelvis_screen.y), ImVec2(top_leg_2_screen.x, top_leg_2_screen.y), ImColor(0, 0, 0), 4.5);
								drawList->AddLine(ImVec2(top_leg_2_screen.x, top_leg_2_screen.y), ImVec2(mid_leg_2_screen.x, mid_leg_2_screen.y), ImColor(0, 0, 0), 4.5);
								drawList->AddLine(ImVec2(mid_leg_2_screen.x, mid_leg_2_screen.y), ImVec2(bottom_leg_2_screen.x, bottom_leg_2_screen.y), ImColor(0, 0, 0), 4.5);
							}
							//NECK
							//drawList->AddLine(ImVec2(head_screen.x, head_screen.y), ImVec2(neck_screen.x, neck_screen.y), ImColor(VIsible_color), 1.5f);
							drawList->AddLine(ImVec2(neck_screen.x, neck_screen.y), ImVec2(spine_screen.x, spine_screen.y), ImColor(66, 245, 212), 2.5f);
							//SPINE
							drawList->AddLine(ImVec2(spine_screen.x, spine_screen.y), ImVec2(mid_spine_screen.x, mid_spine_screen.y), ImColor(66, 245, 212), 2.5f);
							drawList->AddLine(ImVec2(mid_spine_screen.x, mid_spine_screen.y), ImVec2(lower_spine_screen.x, lower_spine_screen.y), ImColor(66, 245, 212), 2.5f);
							// SHOULDER
							drawList->AddLine(ImVec2(spine_screen.x, spine_screen.y), ImVec2(top_arm_1_screen.x, top_arm_1_screen.y), ImColor(66, 245, 212), 2.5f);
							drawList->AddLine(ImVec2(spine_screen.x, spine_screen.y), ImVec2(top_arm_2_screen.x, top_arm_2_screen.y), ImColor(66, 245, 212), 2.5f);
							// ELBOW
							drawList->AddLine(ImVec2(top_arm_2_screen.x, top_arm_2_screen.y), ImVec2(mid_arm_2_screen.x, mid_arm_2_screen.y), ImColor(66, 245, 212), 2.5f);
							drawList->AddLine(ImVec2(top_arm_1_screen.x, top_arm_1_screen.y), ImVec2(mid_arm_1_screen.x, mid_arm_1_screen.y), ImColor(66, 245, 212), 2.5f);
							drawList->AddLine(ImVec2(mid_arm_1_screen.x, mid_arm_1_screen.y), ImVec2(mid_arm_3_screen.x, mid_arm_3_screen.y), ImColor(66, 245, 212), 2.5f);
							// Hands
							drawList->AddLine(ImVec2(mid_arm_3_screen.x, mid_arm_3_screen.y), ImVec2(bottom_arm_1screen.x, bottom_arm_1screen.y), ImColor(66, 245, 212), 2.5f);
							drawList->AddLine(ImVec2(mid_arm_2_screen.x, mid_arm_2_screen.y), ImVec2(bottom_arm_2_screen.x, bottom_arm_2_screen.y), ImColor(66, 245, 212), 2.5f);
							// Pelvis
							drawList->AddLine(ImVec2(lower_spine_screen.x, lower_spine_screen.y), ImVec2(pelvis_screen.x, pelvis_screen.y), ImColor(66, 245, 212), 2.5f);
							// LEG
							drawList->AddLine(ImVec2(pelvis_screen.x, pelvis_screen.y), ImVec2(top_leg_1_screen.x, top_leg_1_screen.y), ImColor(66, 245, 212), 2.5f);
							drawList->AddLine(ImVec2(top_leg_1_screen.x, top_leg_1_screen.y), ImVec2(mid_leg_1_screen.x, mid_leg_1_screen.y), ImColor(66, 245, 212), 2.5f);
							drawList->AddLine(ImVec2(mid_leg_1_screen.x, mid_leg_1_screen.y), ImVec2(bottom_leg_1_screen.x, bottom_leg_1_screen.y), ImColor(66, 245, 212), 2.5f);

							drawList->AddLine(ImVec2(pelvis_screen.x, pelvis_screen.y), ImVec2(top_leg_2_screen.x, top_leg_2_screen.y), ImColor(66, 245, 212), 2.5f);
							drawList->AddLine(ImVec2(top_leg_2_screen.x, top_leg_2_screen.y), ImVec2(mid_leg_2_screen.x, mid_leg_2_screen.y), ImColor(66, 245, 212), 2.5f);
							drawList->AddLine(ImVec2(mid_leg_2_screen.x, mid_leg_2_screen.y), ImVec2(bottom_leg_2_screen.x, bottom_leg_2_screen.y), ImColor(66, 245, 212), 2.5f);

							/*for (int bone = 0; bone < 43; bone++)
							{
								auto BoneLocation = chicken_bone(mesh, bone);
								vector2 screen_bone = w2s(BoneLocation);
								DrawString(font_size, screen_bone.x, screen_bone.y, ImColor(192, 192, 192), true, true, (std::to_string(bone)).c_str());

							}*/
						}

						if (in_screen(Screen)) {
							std::string wtf2 = ("mcnugget [") + std::to_string((int)entity.distance) + ("m]");
							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());

							if (loot_boxes)
							{
								if (box_outline)
								{
									DrawThreeDBox(bottom_bone, top_bone, 50, 50, box_thickness + 2.5, ImColor(0, 0, 0));
								}
								DrawThreeDBoxText(bottom_bone, top_bone, 50, 50, box_thickness, ImColor(66, 245, 212), wtf2.c_str());
							}
							else
							{
								DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(66, 245, 212), true, text_outline, wtf2.c_str());
							}

							
						}
					}
				}

				if (show_llamas && strstr(entity.Name.c_str(), _xor_("AthenaSupplyDrop_Llama_C").c_str()))
				{
					if (entity.distance <= llama_render_distance)
					{
						vector3 Location = get_location(entity.Actor);
						vector2 Screen = w2s(Location);
						if (in_screen(Screen))
						{
							std::string wtf2 = ("Lamma [") + std::to_string((int)entity.distance) + ("m]");
							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
							DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 0, 255), true, text_outline, wtf2.c_str());

						}
					}
				}


				if (show_vehicles && strstr(entity.Name.c_str(), _xor_("Valet_SportsCar_Vehicle_C").c_str())) {
					if (entity.distance <= vehicle_render_distance)
					{
						vector3 Location = get_location(entity.Actor);
						vector2 Screen = w2s(Location);

						if (in_screen(Screen))
						{
							std::string wtf2 = ("Sports Car [") + std::to_string((int)entity.distance) + ("m]");
							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
							DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, text_outline, wtf2.c_str());


						}
					}
				}

				if (show_vehicles && strstr(entity.Name.c_str(), _xor_("Vehicle_Hoverboard_C").c_str())) {
					if (entity.distance <= vehicle_render_distance)
					{
						vector3 Location = get_location(entity.Actor);
						vector2 Screen = w2s(Location);

						if (in_screen(Screen))
						{
							std::string wtf2 = ("Hoverboard [") + std::to_string((int)entity.distance) + ("m]");
							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
							DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, text_outline, wtf2.c_str());
						}
					}
				}

				if (show_vehicles && strstr(entity.Name.c_str(), _xor_("Motorcycle_DirtBike_Vehicle_C").c_str())) {
					if (entity.distance <= vehicle_render_distance)
					{
						vector3 Location = get_location(entity.Actor);
						vector2 Screen = w2s(Location);


						if (in_screen(Screen))
						{
							std::string wtf2 = ("Motorcycle [") + std::to_string((int)entity.distance) + ("m]");
							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
							DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, text_outline, wtf2.c_str());


						}
					}
				}



				if (show_vehicles && strstr(entity.Name.c_str(), _xor_("Valet_BasicSUV_Vehicle_C").c_str()))
				{
					if (entity.distance <= vehicle_render_distance)
					{
						vector3 Location = get_location(entity.Actor);
						vector2 Screen = w2s(Location);

						if (in_screen(Screen))
						{
							std::string wtf2 = ("SUV [") + std::to_string((int)entity.distance) + ("m]");
							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
							DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, text_outline, wtf2.c_str());


						}
					}
				}

				if (show_cooler && strstr(entity.Name.c_str(), _xor_("Cooler_Container_C").c_str()))
				{
					if (entity.distance <= cooler_render_distance)
					{

						auto bAlreadySearched = (request->read<char>(entity.Actor + 0xD92) >> 1);
						if (bAlreadySearched) continue;

						vector3 Location = get_location(entity.Actor);
						vector2 Screen = w2s(Location);


						if (in_screen(Screen))
						{
							std::string wtf2 = ("Cooler Container [") + std::to_string((int)entity.distance) + ("m]");
							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
							DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(186, 242, 239), true, text_outline, wtf2.c_str());


						}
					}
				}

				if (food_stuff && (strstr(entity.Name.c_str(), _xor_("FoodBox").c_str()) || strstr(entity.Name.c_str(), _xor_("FlopperSpawn").c_str())))
				{
					if (entity.distance <= food_render_distance)
					{

						auto bAlreadySearched = (request->read<char>(entity.Actor + 0xD92) >> 1);
						if (bAlreadySearched) continue;

						vector3 Location = get_location(entity.Actor);
						vector2 Screen = w2s(Location);
						vector3 bounds = vector3(Location.x, Location.y, +Location.z + 15);


						if (in_screen(Screen))
						{
							std::string wtf2 = ("Food [") + std::to_string((int)entity.distance) + ("m]");
							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());


							if (loot_boxes)
							{
								if (box_outline)
								{
									DrawThreeDBox(Location, bounds, 50, 80, box_thickness + 2.5, ImColor(0, 0, 0));
								}
								DrawThreeDBoxText(Location, bounds, 50, 80, box_thickness, ImColor(186, 242, 239), wtf2.c_str());
							}
							else
							{
								DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(186, 242, 239), true, text_outline, wtf2.c_str());

							}


						}
					}
				}

				if (bullshit && strstr(entity.Name.c_str(), _xor_("Rock").c_str()))
				{
					if (entity.distance <= 30)
					{
						vector3 Location = get_location(entity.Actor);
						vector2 Screen = w2s(Location);
						vector3 bounds = vector3(Location.x, Location.y, +Location.z + 200);

						if (in_screen(Screen))
						{
							std::string wtf2 = ("Rock [") + std::to_string((int)entity.distance) + ("m]");
							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());


							if (loot_boxes)
							{
								if (box_outline)
								{
									DrawThreeDBox(Location, bounds, 350, 350, box_thickness + 2.5, ImColor(0, 0, 0));
								}
								DrawThreeDBoxText(Location, bounds, 350, 350, box_thickness, ImColor(255, 255, 255), wtf2.c_str());
							}
							else
							{
								DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), true, text_outline, wtf2.c_str());

							}

						}
					}
				}


				if (bullshit && strstr(entity.Name.c_str(), _xor_("Tree").c_str()))
				{
					if (entity.distance <= 30)
					{
						vector3 Location = get_location(entity.Actor);
						vector2 Screen = w2s(Location);
						vector3 bounds = vector3(Location.x, Location.y, +Location.z + 400);
						
						if (in_screen(Screen))
						{
							std::string wtf2 = ("Tree [") + std::to_string((int)entity.distance) + ("m]");
							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());

							if (loot_boxes)
							{
								if (box_outline)
								{
									DrawThreeDBox(Location, bounds, 250, 250, box_thickness + 2.5, ImColor(0, 0, 0));
								}
								DrawThreeDBoxText(Location, bounds, 250, 250, box_thickness, ImColor(0, 255, 0), wtf2.c_str());
							}
							else
							{
								DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(0, 255, 0), true, text_outline, wtf2.c_str());

							}

						}
					}
				}

				if (mod_station && strstr(entity.Name.c_str(), _xor_("WeaponModStation").c_str()))
				{
					if (entity.distance <= mod_render_distance)
					{
						vector3 Location = get_location(entity.Actor);
						vector2 Screen = w2s(Location);


						if (in_screen(Screen))
						{
							std::string wtf2 = ("Mod Station [") + std::to_string((int)entity.distance) + ("m]");
							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
							DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, text_outline, wtf2.c_str());


						}
					}
				}


				if (Gold && strstr(entity.Name.c_str(), _xor_("CashRegister").c_str()))
				{
					if (entity.distance <= gold_render_distance)
					{

						auto bAlreadySearched = (request->read<char>(entity.Actor + 0xD92) >> 1);
						if (bAlreadySearched) continue;

						vector3 Location = get_location(entity.Actor);
						vector2 Screen = w2s(Location);


						if (in_screen(Screen))
						{
							std::string wtf2 = ("Gold [") + std::to_string((int)entity.distance) + ("m]");
							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
							DrawString(font_size, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(230, 230, 0), true, text_outline, wtf2.c_str());


						}
					}
				}


				if (show_pickups && (strstr(entity.Name.c_str(), _xor_("FortPickupAthena").c_str()) || strstr(entity.Name.c_str(), _xor_("B_Pickups_Default_C").c_str()) || strstr(entity.Name.c_str(), _xor_("Fort_Pickup_Creative_C").c_str())))
				{
					if (entity.distance <= pickup_render_distance)
					{
						auto definition = request->read <uint64_t>(entity.Actor + 0x350 + 0x18); // AFortPickup::PrimaryPickupItemEntry : FFortItemEntry::ItemDefinition -> 0x18
						if (request->IsValidAddress(definition))
						{
							vector3 Location = get_location(entity.Actor);
							vector2 Screen = w2s(Location);
							if (in_screen(Screen))
							{
								BYTE tier = request->read <BYTE>(definition + 0x0EB); //UFortItemDefinition::Tier
								ImColor Color, RGBAColor;
								vector3 Location = get_location(entity.Actor);
								vector2 ChestPosition = w2s(Location);
								uint64_t ftext_ptr = request->read <uint64_t>(definition + 0x40);

								if (request->IsValidAddress(ftext_ptr))
								{
									uint64_t ftext_data = request->read <uint64_t>(ftext_ptr + 0x28);
									int ftext_length = request->read <int>(ftext_ptr + 0x30);
									if (ftext_length > 0 && ftext_length < 50)
									{
										wchar_t* ftext_buf = new wchar_t[ftext_length];
										Bypass::read((PVOID)ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));
										wchar_t* WeaponName = ftext_buf;
										delete[] ftext_buf;

										std::string Text = wchar_to_char(WeaponName);
										std::string wtf2 = Text + " [" + std::to_string((int)entity.distance) + ("m]");
										if (tier == 1 && (Common))
										{
											Color = ImColor(0, 255, 0);
										}
										else if ((tier == 2) && (rare))
										{
											Color = ImColor(0, 150, 255);
										}
										else if ((tier == 3) && (purple))
										{
											Color = ImColor(255, 0, 255);
										}
										else if ((tier == 4) && (gold))
										{
											Color = ImColor(255, 255, 0);
										}
										else if ((tier == 5) && (mythic))
										{
											Color = ImColor(245, 245, 0);
										}

										else if ((tier == 0) && (UnCommon))
										{
											Color = ImColor(255, 255, 255);
										}

										ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
										DrawString(font_size, ChestPosition.x - (text_size.x / 2), ChestPosition.y - 18, Color, true, text_outline, wtf2.c_str());


									}
								}
							}
						}
					}
				}

			}

		}
	}
}; static loott* floot = new loott;




std::string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

void DrawNewTextCustomWithOutline(int x, int y, int R, int G, int B, int A, const char* str)
{
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);
	float outlineThickness = 1.5f; // Adjust the outline thickness as needed

	// Draw the text with black outline
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 18.0f, ImVec2(x - outlineThickness, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, A / 255.0f)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 18.0f, ImVec2(x + outlineThickness, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, A / 255.0f)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 18.0f, ImVec2(x, y - outlineThickness), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, A / 255.0f)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 18.0f, ImVec2(x, y + outlineThickness), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, A / 255.0f)), utf_8_2.c_str());

	// Draw the main text with the specified color
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 18.0f, ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f)), utf_8_2.c_str());
}

uintptr_t TargetedFortPawn;

WeaponInformation WINFO;


int HeldWeaponType;


camera_position_s get_camera()
{
	camera_position_s camera;

	auto location_pointer = request->read<uintptr_t>((u)global->world + 0x110);
	auto rotation_pointer = request->read<uintptr_t>((u)global->world + 0x120);

	struct FNRot
	{
		double a; //0x0000
		char pad_0008[24]; //0x0008
		double b; //0x0020
		char pad_0028[424]; //0x0028
		double c; //0x01D0
	}fnRot;

	fnRot.a = request->read<double>(rotation_pointer);
	fnRot.b = request->read<double>(rotation_pointer + 0x20);
	fnRot.c = request->read<double>(rotation_pointer + 0x1d0);

	camera.location = request->read<vector3>(location_pointer);
	camera.rotation.x = asin(fnRot.c) * (180.0 / M_PI);
	camera.rotation.y = ((atan2(fnRot.a * -1, fnRot.b) * (180.0 / M_PI)) * -1) * -1;
	camera.fov = request->read<float>((uintptr_t)(u)global->player_controller + 0x394) * 90.f;

	return camera;
}



int radar_position_x = 10.0f ;
int radar_position_y = 10.0f ;
int radar_size = 250.0f ;


namespace radar_style {
	bool circle = true;
	bool rectangle = false;
}
namespace radar_text_mode {
	bool outlined = true;
	bool background = false;
}

enum bones : int {
	HumanBase = 0,
	HumanPelvis = 2,
	HumanLThigh1 = 71,
	HumanLThigh2 = 77,
	HumanLThigh3 = 72,
	HumanLCalf = 74,
	HumanLFoot = 86,
	HumanLToe = 76,
	HumanRThigh1 = 78,
	HumanRThigh2 = 84,
	HumanRThigh3 = 79,
	HumanRCalf = 81,
	HumanRFoot = 87,
	HumanRToe = 83,
	HumanSpine1 = 7,
	HumanSpine2 = 5,
	HumanSpine3 = 2,
	HumanLCollarbone = 9,
	HumanLUpperarm = 35,
	HumanLForearm1 = 36,
	HumanLForearm23 = 10,
	HumanLForearm2 = 34,
	HumanLForearm3 = 33,
	HumanLPalm = 32,
	HumanRCollarbone = 98,
	HumanRUpperarm = 64,
	HumanRForearm1 = 65,
	HumanRForearm23 = 39,
	HumanRForearm2 = 63,
	HumanRForearm3 = 62,
	HumanRPalm = 58,
	HumanNeck = 67,
	HumanHead = 68,
	HumanChest = 66
};

namespace Bounds {
	struct PlayerBounds {
		float left, right, top, bottom;
	};

	struct PlayerBones {
		vector2 screen;
		int index;
		bool on_screen;
		bool head_bone = false;
	};

	static std::array<PlayerBones, 26> BonesArray = {
	PlayerBones{ vector2{}, bones::HumanHead, false, true }, PlayerBones{ vector2{}, bones::HumanNeck, false },
	PlayerBones{ vector2{}, bones::HumanSpine3, false },  PlayerBones{ vector2{}, bones::HumanSpine2, false },
	PlayerBones{ vector2{}, bones::HumanSpine1, false },  PlayerBones{ vector2{}, bones::HumanPelvis, false },
	PlayerBones{ vector2{}, bones::HumanLToe, false },  PlayerBones{ vector2{}, bones::HumanLFoot, false },
	PlayerBones{ vector2{}, bones::HumanLCalf, false },  PlayerBones{ vector2{}, bones::HumanLThigh2, false },
	PlayerBones{ vector2{}, bones::HumanLThigh1, false },  PlayerBones{ vector2{}, bones::HumanPelvis, false },
	PlayerBones{ vector2{}, bones::HumanRThigh1, false },  PlayerBones{ vector2{}, bones::HumanRThigh2, false },
	PlayerBones{ vector2{}, bones::HumanRCalf, false },  PlayerBones{ vector2{}, bones::HumanRFoot, false },
	PlayerBones{ vector2{}, bones::HumanRToe, false },  PlayerBones{ vector2{}, bones::HumanLPalm, false },
	PlayerBones{ vector2{}, bones::HumanLForearm1, false },  PlayerBones{ vector2{}, bones::HumanLUpperarm, false },
	PlayerBones{ vector2{}, bones::HumanRUpperarm, false }, PlayerBones{ vector2{}, bones::HumanLForearm23, false },
	PlayerBones{ vector2{}, bones::HumanLForearm2, false },
	PlayerBones{ vector2{}, bones::HumanLForearm3, false },
	PlayerBones{ vector2{}, bones::HumanRForearm2, false },
	PlayerBones{ vector2{}, bones::HumanRForearm23, false }
	};
}


bool GetPlayerBoundings(uskeletalmeshcomponent* Mesh, Bounds::PlayerBounds* Boundings) {
	const auto get_bounds = [&](Bounds::PlayerBounds& out, float expand = 0.f) -> bool {
		Bounds::PlayerBounds Bounds = { FLT_MAX, FLT_MIN, FLT_MAX, FLT_MIN };

		for (auto& bone : Bounds::BonesArray) {
			vector3 bone_pos = Mesh->get_bone_3d(bone.index);

			bone.screen = vector2{  };

			if (bone.index == bones::HumanHead)
				bone_pos.z += 15;

			if (bone.index == bones::HumanLCalf ||
				bone.index == bones::HumanRCalf) {
				bone_pos.z -= 8.5;
			}

			if (bone.index == bones::HumanRPalm ||
				bone.index == bones::HumanRForearm23) {
				bone_pos.x -= 11;
			}

			if (bone.index == bones::HumanLPalm ||
				bone.index == bones::HumanLForearm23) {
				bone_pos.x += 11;
			}
			bone.screen = w2s(bone_pos);

			if (&bone.screen) {
				if (bone.screen.x < Bounds.left)
					Bounds.left = bone.screen.x;
				else if (bone.screen.x > Bounds.right)
					Bounds.right = bone.screen.x;
				if (bone.screen.y < Bounds.top)
					Bounds.top = bone.screen.y;
				else if (bone.screen.y > Bounds.bottom)
					Bounds.bottom = bone.screen.y;

				bone.on_screen = true;
			}
		}

		if (Bounds.left == FLT_MAX)
			return false;
		if (Bounds.right == FLT_MIN)
			return false;
		if (Bounds.top == FLT_MAX)
			return false;
		if (Bounds.bottom == FLT_MIN)
			return false;

		Bounds.left -= expand;
		Bounds.right += expand;
		Bounds.top -= expand;
		Bounds.bottom += expand;

		out = Bounds;

		return true;
		};

	Bounds::PlayerBounds Bounds;

	if (get_bounds(Bounds, 4.f)) {
		*Boundings = Bounds;
		return true;
	}

	return false;

}

std::string username(std::uintptr_t playerstate)
{
	if ((u)global->local_pawn != NULL )
	{
		if (playerstate) {
			auto fstring = (request->read<__int64>)(playerstate + 0xab0);
			auto length = (request->read<int>)(fstring + 16);

			auto v6 = (__int64)length;
			if (!v6) return std::string(("BOT"));

			auto ftext = (uintptr_t)(request->read<__int64>)(fstring + 8);

			wchar_t* buffer = new wchar_t[length];
			Bypass::read((PVOID)ftext, buffer, length * sizeof(wchar_t));

			char v21;
			int v22;
			int i;

			int v25;
			UINT16* v23;

			v21 = v6 - 1;
			if (!(UINT32)v6)
				v21 = 0;
			v22 = 0;
			v23 = (UINT16*)buffer;
			for (i = (v21) & 3; ; *v23++ += i & 7)
			{
				v25 = v6 - 1;
				if (!(UINT32)v6)
					v25 = 0;
				if (v22 >= v25)
					break;
				i += 3;
				++v22;
			}

			std::wstring username{ buffer };
			auto string = filterInvalidChars1(username);
			return std::string(string.begin(), string.end());
		}
	}

	return std::string(E("HIII"));
}




float color_red = 1.;
float color_green = 0;
float color_blue = 0;
float color_random = 0.0;
float color_speed = 2.0;

void ColorChange()
{
	static float Color[3];
	static DWORD Tickcount = 0;
	static DWORD Tickcheck = 0;
	ImGui::ColorConvertRGBtoHSV(color_red, color_green, color_blue, Color[0], Color[1], Color[2]);
	if (GetTickCount() - Tickcount >= 1)
	{
		if (Tickcheck != Tickcount)
		{
			Color[0] += 0.001f * color_speed;
			Tickcheck = Tickcount;
		}
		Tickcount = GetTickCount();
	}
	if (Color[0] < 0.0f) Color[0] += 1.0f;
	ImGui::ColorConvertHSVtoRGB(Color[0], Color[1], Color[2], color_red, color_green, color_blue);
}

float BOG_TO_GRD(float BOG) {
	return (180 / M_PI) * BOG;
}

float GRD_TO_BOG(float GRD) {
	return (M_PI / 180) * GRD;
}

void gaybow(ImGuiWindow& window) {
	auto RGB = ImGui::GetColorU32({ color_red, color_green, color_blue, 255 });


}




float RadarDistance = { 20.f };


inline float custom_sinf(float _X) {
	return _mm_cvtss_f32(_mm_sin_ps(_mm_set_ss(_X)));
}

inline float custom_cosf(float _X) {
	return _mm_cvtss_f32(_mm_cos_ps(_mm_set_ss(_X)));
}



inline float custom_fabsf(float x) {
	__m128 x_vec = _mm_set_ss(x);
	x_vec = _mm_and_ps(x_vec, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));
	return _mm_cvtss_f32(x_vec);
}


void calc_range(float* x, float* y, float range) {
	float AbsX = custom_fabsf(*x);
	float AbsY = custom_fabsf(*y);

	if (AbsX > range || AbsY > range) {
		float ratio = AbsX > AbsY ? (range / AbsX) : (range / AbsY);

		*x *= ratio * ((*x > 0) ? 1 : -1);
		*y *= ratio * ((*y > 0) ? 1 : -1);
	}
}

void CalcRadarPoint(vector3 vOrigin, int& screenx, int& screeny) {
	vector3 Rotation = camera_postion.rotation;
	float Yaw = Rotation.y * static_cast<float>(M_PI) / 180.0f;
	float DX = vOrigin.x - camera_postion.location.x;
	float DY = vOrigin.y - camera_postion.location.y;

	float SinYaw = custom_sinf(Yaw);
	float MinusCosYaw = -custom_cosf(Yaw);

	float x = DY * MinusCosYaw + DX * SinYaw;
	x = -x;
	float y = DX * MinusCosYaw - DY * SinYaw;

	float Range = RadarDistance * 1000;

	calc_range(&x, &y, Range);

	ImVec2 DrawPos = ImVec2(radar_position_x, radar_position_y);
	ImVec2 DrawSize = ImVec2(radar_size, radar_size);

	int RadX = static_cast<int>(DrawPos.x);
	int RadY = static_cast<int>(DrawPos.y);

	float RadSizeX = DrawSize.x;
	float RadSizeY = DrawSize.y;

	int MaxX = static_cast<int>(RadSizeX + RadX - 5);
	int MaxY = static_cast<int>(RadSizeY + RadY - 5);

	screenx = RadX + static_cast<int>((RadSizeX / 2) + (x / Range * RadSizeX));
	screeny = RadY + static_cast<int>((RadSizeY / 2) + (y / Range * RadSizeY));

	screenx = clamp(screenx, RadX, MaxX);
	screeny = clamp(screeny, RadY, MaxY);
}
inline float radar_circle_size = 3.0f;



vector2 RotatePoint(vector2 radar_pos, vector2 radar_size, vector3 LocalLocation, vector3 TargetLocation) {
	float DX = TargetLocation.x - LocalLocation.x;
	float DY = TargetLocation.y - LocalLocation.y;

	float x = DY * -1;
	x = -x;
	float y = DX * -1;

	float Range = 34 * 1000;

	calc_range(&x, &y, Range);

	int RadX = radar_pos.x;
	int RadY = radar_pos.y;

	float RadSizeX = radar_size.x;
	float RadSizeY = radar_size.y;

	int MaxX = RadSizeX + RadX - 5;
	int MaxY = RadSizeY + RadY - 5;

	vector2 ReturnValue = vector2();

	ReturnValue.x = RadX + (RadSizeX / 2) + (x / Range * RadSizeX);
	ReturnValue.y = RadY + (RadSizeY / 2) + (y / Range * RadSizeY);

	ReturnValue.x = clamp(static_cast<int>(ReturnValue.x), RadX, MaxX);
	ReturnValue.y = clamp(static_cast<int>(ReturnValue.y), RadY, MaxY);

	return ReturnValue;
}


void add_to_minimap(vector3 WorldLocation, vector3 LocalLocation, ImColor Color, vector2 RadarPos, vector2 RadarSize, float Distance) {
	vector2 Screen = RotatePoint(RadarPos, RadarSize, LocalLocation, WorldLocation);


	ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(Screen.x, Screen.y), radar_circle_size, Color, 12);
}



void fortnite_radar(float x, float y, float size, bool rect = false)
{
	if (radar)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Once);
		static const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
		ImGui::Begin(("##radar"), nullptr, flags);

		float render_size = 5;
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->AddRectFilled(ImVec2(x, y), ImVec2(x + size, y + size), ImColor(20, 20, 20, 180), 0.0f, 0);
		drawList->AddRect(ImVec2(x, y), ImVec2(x + size, y + size), ImColor(0, 0, 0, 255), 0.0f, 0);

		drawList->AddLine(ImVec2(radar_position_x + (radar_size / 2), radar_position_y + (radar_size / 2)), ImVec2(radar_position_x, radar_position_y), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), 1.f);
		drawList->AddLine(ImVec2(radar_position_x + (radar_size / 2), radar_position_y + (radar_size / 2)), ImVec2(radar_position_x + radar_size, radar_position_y), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), 1.f);
		drawList->AddLine(ImVec2(radar_position_x, radar_position_y + (radar_size / 2)), ImVec2(radar_position_x + radar_size, radar_position_y + (radar_size / 2)), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), 1.f);
		drawList->AddLine(ImVec2(radar_position_x + (radar_size / 2), radar_position_y + (radar_size / 2)), ImVec2(radar_position_x + (radar_size / 2), radar_position_y), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), 1.f);

		drawList->AddLine(ImVec2(radar_position_x + (radar_size / 2), radar_position_y + (radar_size / 2)), ImVec2(radar_position_x + (radar_size / 2), radar_position_y + radar_size), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), 1.f);
		drawList->AddCircleFilled(ImVec2(x + size / 2, y + size / 2), render_size / 2 * sqrt(2), ImGui::GetColorU32({ 1.0f, 1.0f, 1.0f, 1.0f }), 1000);

		ImGui::End();
	}
}
void add_players_radar(vector3 WorldLocation, ImColor color)
{
	if (radar)
	{
		static const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
		ImGui::Begin(("##radar"), nullptr, flags);

		int ScreenX, ScreenY = 0;
		const auto& GetWindowDrawList = ImGui::GetWindowDrawList();
		ImDrawList* drawList = ImGui::GetWindowDrawList();




		CalcRadarPoint(WorldLocation, ScreenX, ScreenY);

	
		
		drawList->AddCircleFilled(ImVec2(ScreenX, ScreenY), 5, ImColor(color), 7);
		

		ImGui::End();
	}
}








void camera_thread()
{

}

void normal_box(const ImVec2& position, const float width, const float height, const uint32_t color, float thickness) {
	ImGui::GetBackgroundDrawList()->AddRect(position, ImVec2(position.x + width, position.y + height), color, 0, 0, thickness);
}


void actor_loop() {

	ImGui::PushFont(ESPFont);
	auto RGB = ImGui::GetColorU32({ color_red, color_green, color_blue, 255 });
	if (debug) {
		printf(("\n [+] ActorLoop called"));
	}

	std::unique_lock<std::mutex> lock(global->data_mutex);
	camera_postion = get_camera();


	ImDrawList* draw_list = ImGui::GetForegroundDrawList();


	int dynamicfov = dynamicfovval - (80 - fov);

	if (Proggy)
	{
		ImGui::PushFont(proggy);
	}
		
	if (fort_font)
	{
		ImGui::PushFont(FortniteFont);
	}
		
	if (verdana)
	{
		ImGui::PushFont(Verdana);
	}
	if (arial)
	{
		ImGui::PushFont(Arial);
	}
		
	if (Poppins)
	{
		ImGui::PushFont(poppins);
	}

	if (Roboto)
	{
		ImGui::PushFont(roboto);
	}

	if (draw_fov) {

		if (rainbow)
		{
			draw_list->AddCircle(ImVec2(screen_width / 2, screen_height / 2), dynamicfov, ImColor(RGB), 33, 1);
			draw_list->AddCircle(ImVec2(screen_width / 2, screen_height / 2), dynamicfov + 1, ImColor(20, 0, 0, 255), 33, 1);
		}
		else
		{
			draw_list->AddCircle(ImVec2(screen_width / 2, screen_height / 2), dynamicfov, ImColor(0, 0, 0, 255), 100, 2.5);

				draw_list->AddCircle(ImVec2(screen_width / 2, screen_height / 2), dynamicfov, ImColor(255, 255, 255, 255), 100, 1);
			

		}
	}

	if (draw_crosshair)
	{
		constexpr long CrosshairSize = 10.0f;
		ImVec2 Center = ImVec2(screen_width / 2, screen_height /2);

		draw_list->AddLine(ImVec2(Center.x, Center.y - CrosshairSize), ImVec2(Center.x, Center.y + CrosshairSize), ImColor(255, 255, 255, 255), 1.0f);
		draw_list->AddLine(ImVec2(Center.x - CrosshairSize, Center.y), ImVec2(Center.x + CrosshairSize, Center.y), ImColor(255, 255, 255, 255), 1.0f);
	}
	
	if (radar) 
	{


		char fpsinfo[256];
		if (draw_fps)
		{
			sprintf_s(fpsinfo, E("DTC SOURCEEEEE NOTUDUDUDUDUUDUDD | FPS: %.0f"), ImGui::GetIO().Framerate);

		}
		else
		{
			sprintf_s(fpsinfo, (("DTC SOURCEEEEE NOTUDUDUDUDUUDUDD ")));
		}
		draw_list->AddText(ImVec2(2400, 15), ImColor(255, 255, 255, 255), fpsinfo);
	}
	else
	{
		char fpsinfo[256];
		if (draw_fps)
		{
			sprintf_s(fpsinfo, E("DTC SOURCEEEEE NOTUDUDUDUDUUDUDD | FPS: %.0f"), ImGui::GetIO().Framerate);

		}
		else
		{
			sprintf_s(fpsinfo, (("DTC SOURCEEEEE NOTUDUDUDUDUUDUDD ")));
		}		
		draw_list->AddText(ImVec2(15, 15), ImColor(255, 255, 255, 255), fpsinfo);
	}


	if (global->players.size() < 1) return;

	vector3 base_3d;

	bool DoingAimbot{ false };
	bool ResetAngles{ false };

	float target_dist = FLT_MAX;
	std::shared_ptr<player_data> closest_player = std::make_shared<player_data>();
	for (auto player : global->players) {
		vector3 head_3d = player->mesh->get_head();
		vector2 head_2d = w2s(vector3(head_3d.x, head_3d.y, head_3d.z + 20));
		base_3d = player->mesh->get_base();
		vector2 base_2d = w2s(base_3d);

	

		float box_hight = abs(head_2d.y - base_2d.y);
		float box_width = box_hight * 0.60;


		bool inlobby;


		if (global->local_pawn == NULL)
		{
			inlobby = true;
		}
		else
		{
			inlobby = false;
		}

		VIsible_color = ImVec4(1.f, 1.f, 1.f, 1.0f);

		if (rainbow)
		{
			VIsible_color = ImColor(RGB);


		}

		if (player->mesh->visible()) {
			if (!rainbow)
			{
				VIsible_color = ImVec4(PlayerColor::visible_color[0], PlayerColor::visible_color[1], PlayerColor::visible_color[2], 1.0f);



			}


		}
		else if (!player->mesh->visible()) {

			if (!rainbow)
			{

				VIsible_color = ImVec4(PlayerColor::nonvisible_color[0], PlayerColor::nonvisible_color[1], PlayerColor::nonvisible_color[2], 1.0f);


			}
		}


	

		if (mini_map_radar)
		{
			if (global->local_pawn)
			{
				float HUDScale = 0.f;

				if (UFortClientSettingsRecord* ClientSettingsRecord = global->local_player->get_ClientSettingsRecord()) {
					HUDScale = ClientSettingsRecord->HUDScale();
				}

				float RadarSize = (15.0f * screen_width / 2 * HUDScale / 100.0f) * 2.0f;
				float RadarPositionOffset = RadarSize / 30.0f;
				vector2 RadarPosition = vector2(screen_width - RadarSize - RadarPositionOffset, RadarPositionOffset);

				add_to_minimap(base_3d, camera_postion.location, VIsible_color, RadarPosition, vector2(RadarSize, RadarSize), player->player_distance);
			}
		}

		if (radar)
		{
			if ((u)global->local_pawn)
			{

				add_players_radar(base_3d, VIsible_color);
			}
		}

		if (global->local_pawn)
		{

			uint64_t player_weapon = request->read<uint64_t>((u)global->local_pawn + 0xa68); // 	struct AFortWeapon* CurrentWeapon


			if (request->IsValidAddress(player_weapon)) {


				uint64_t weapon_data = request->read<uint64_t>(player_weapon + 0x500);	//struct UFortWeaponItemDefinition* WeaponData;



				if (request->IsValidAddress(weapon_data)) {

					uint64_t ftext_ptr = request->read<uint64_t>(weapon_data + 0x40);

					if (request->IsValidAddress(ftext_ptr)) {
						uint64_t ftext_data = request->read<uint64_t>(ftext_ptr + 0x28);
						int ftext_length = request->read<int>(ftext_ptr + 0x30);
						if (ftext_length > 0 && ftext_length < 50) {
							wchar_t* ftext_buf = new wchar_t[ftext_length];

							Bypass::read((PVOID)ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));
							std::wstring wstr_buf(ftext_buf);



							LocalPlayerWeapon = std::string(wstr_buf.begin(), wstr_buf.end());

							wchar_t* WeaponName = ftext_buf;

							delete[] ftext_buf;



						//	std::cout << E("Current Local Player Weapon ->") << LocalPlayerWeapon << std::endl;



							if (wcsstr(WeaponName, E(L"Assault Rifle")) || wcsstr(WeaponName, E(L"Havoc Suppressed Assault Rifle")) || wcsstr(WeaponName, E(L"Red-Eye Assault Rifle"))
								|| wcsstr(WeaponName, E(L"Suppressed Assault Rifle")) || wcsstr(WeaponName, E(L"Striker Burst Rifle")) || wcsstr(WeaponName, E(L"Burst Assault Rifle"))
								|| wcsstr(WeaponName, E(L"Ranger Assault Rifle")) || wcsstr(WeaponName, E(L"Flapjack Rifle")) || wcsstr(WeaponName, E(L"Heavy Assault Rifle"))
								|| wcsstr(WeaponName, E(L"MK-Seven Assault Rifle")) || wcsstr(WeaponName, E(L"MK-Alpha Assault Rifle")) || wcsstr(WeaponName, E(L"Combat Assault Rifle"))
								|| wcsstr(WeaponName, E(L"Tactical Assault Rifle")) || wcsstr(WeaponName, E(L"Hammer Assault Rifle")) || wcsstr(WeaponName, E(L"Striker AR")) || wcsstr(WeaponName, E(L"Sideways Rifle")) || wcsstr(WeaponName, E(L"AR")) || wcsstr(WeaponName, E(L"Nemesis AR")) || wcsstr(WeaponName, E(L"Makeshift Rifle"))) {
								HeldWeaponType = EFortWeaponType::Rifle;

							}
							if (wcsstr(WeaponName, E(L"Shotgun"))) {

								HeldWeaponType = EFortWeaponType::Shotgun;



							}
							if (wcsstr(WeaponName, E(L"Smg")) || wcsstr(WeaponName, E(L"Submachine Gun")) || wcsstr(WeaponName, E(L"Hyper SMG")) || wcsstr(WeaponName, E(L"Thunder Burst SMG")) || wcsstr(WeaponName, E(L"SMG")) || wcsstr(WeaponName, E(L"Combat Smg")) || wcsstr(WeaponName, E(L"Ranger Pistol")) || wcsstr(WeaponName, E(L"Pistol"))) {
								HeldWeaponType = EFortWeaponType::Smg;


							}
							if (wcsstr(WeaponName, E(L"Hunting Rifle")) || wcsstr(WeaponName, E(L"Heavy Sniper Rifle")) || wcsstr(WeaponName, E(L"Suppressed Sniper Rifle"))
								|| wcsstr(WeaponName, E(L"Storm Scout")) || wcsstr(WeaponName, E(L"Bolt-Action Sniper Rifle")) || wcsstr(WeaponName, E(L"Automatic Sniper Rifle"))
								|| wcsstr(WeaponName, E(L"DMR")) || wcsstr(WeaponName, E(L"Thermal DMR")) || wcsstr(WeaponName, E(L"Hunter Bolt-Action Sniper"))
								|| wcsstr(WeaponName, E(L"Crossbow")) || wcsstr(WeaponName, E(L"Reaper Sniper Rifle")) || wcsstr(WeaponName, E(L"Mechanical Bow"))) {
								HeldWeaponType = EFortWeaponType::Sniper;



							}
						}
					}
				}
			}
		}

		
		if (!head_2d.valid_location())
			continue;

	

		if (esp) {

			if (draw_distance) {
				std::string dist = std::to_string(int(player->player_distance)).c_str(); std::string final = ("(") + dist + ("m)"); ImVec2 TextSize = ImGui::CalcTextSize(final.c_str());

				DrawString(font_size, (base_2d.x) - (TextSize.x / 2), (base_2d.y + 5), VIsible_color, false, text_outline, final.c_str());

			}

			if (image_esp)
			{
				ImGui::GetBackgroundDrawList()->AddImage(custom_image, ImVec2(head_2d.x - (box_width / 2), head_2d.y), ImVec2(head_2d.x + (box_width / 2), head_2d.y + box_hight));
			}

			if (draw_held_weapon)
			{
				if ((u)global->local_pawn)
				{
					WeaponInformation held_weapon{ };

					uint64_t player_weapon = request->read<uint64_t>((u)player->player + 0xa68);

					if (request->IsValidAddress(player_weapon)) {

						held_weapon.ammo_count = request->read <int32_t>(player_weapon + 0xEDC);

						uint64_t weapon_data = request->read <uint64_t>(player_weapon + 0x500);

						if (request->IsValidAddress(weapon_data)) {

							held_weapon.tier = request->read <uint8>(weapon_data + 0x0EB);

							uint64_t ftext_ptr = request->read <uint64_t>(weapon_data + 0x40);

							if (request->IsValidAddress(ftext_ptr)) {

								uint64_t ftext_data = request->read <uint64_t>(ftext_ptr + 0x28);

								int ftext_length = request->read <int>(ftext_ptr + 0x30);

								if (ftext_length > 0 && ftext_length < 50) {

									wchar_t* ftext_buf = new wchar_t[ftext_length + 1];

									Bypass::read((PVOID)ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));

									if (ftext_buf[0] != L'\0') {
										std::wstring wstr_buf(ftext_buf);

										held_weapon.weapon_name = std::string(wstr_buf.begin(), wstr_buf.end());
									}
									else {

										held_weapon.weapon_name = "Unkown";
									}

									delete[] ftext_buf;
								}
							}
						}

						WINFO = held_weapon;

						std::string final = ("(") + WINFO.weapon_name + " [" + std::to_string(WINFO.ammo_count) + "])";

						ImVec2 TextSize = ImGui::CalcTextSize(final.c_str());

						ImVec2 text_size = ImGui::CalcTextSize(final.c_str());

						if (held_weapon.weapon_name.length() > 2)
						{
							{
								if (WINFO.tier == 7)
								{
									if (strstr(WINFO.weapon_name.c_str(), ("(Pickaxe)")) != nullptr)
									{
										DrawString(font_size, (base_2d.x) - (TextSize.x / 2), (base_2d.y + 20), ImColor(VIsible_color), false, text_outline, final.c_str());
									}
									else
									{
										std::string fina1l = ("(Building Plan)");
										ImVec2 TextSiz1e = ImGui::CalcTextSize(fina1l.c_str());
										DrawString(font_size, (base_2d.x) - (TextSiz1e.x / 2), (base_2d.y + 20), ImColor(VIsible_color), false, text_outline, fina1l.c_str());
									}
								}
								else
								{
									DrawString(font_size, base_2d.x - (text_size.x / 2), base_2d.y + 20, ImColor(VIsible_color), false, text_outline, final.c_str());
								}
							}
						}
					}
				}
			}
		


			if (draw_username) {


				auto name = username((u)player->player_state);



				ImVec2 TextSize = ImGui::CalcTextSize(name.c_str());
				DrawString(font_size, (head_2d.x) - (TextSize.x / 2), (head_2d.y - 15), VIsible_color, false, text_outline, name.c_str());

			}


			if (head) {
				
					vector2 head_2d = w2s(vector3(head_3d.x, head_3d.y, head_3d.z));

					vector3 delta = head_3d - camera_postion.location;
					float distance = delta.length();

					const float constant_circle_size = 10;

					float circle_radius = constant_circle_size * (screen_height / (2.0f * distance * tanf(camera_postion.fov * (float)M_PI / 360.f)));

					int segments = 50;
					float thickness = 2.0f;

					if (skeleton_outline)
					{
						ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, ImColor(0, 0, 0, 255), segments, skelthick + 2);
					}
					ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, VIsible_color, segments, skelthick);
				
			}



			if (skeleton) {
				drawskeleton(player->mesh, skeleton_outline, VIsible_color, feet, hand, head);
			}

			
			if (snapline_top) {


				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen_width / 2, 0), ImVec2(head_2d.x, head_2d.y), ImColor(VIsible_color), 2);
			}
			if (snapline_middle) {
				ImVec2 center(screen_width / 2,screen_height / 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(center.x, center.y), ImVec2(head_2d.x, head_2d.y),  ImColor(VIsible_color), 2);
			}

		    if (snapline_bottom) {
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen_width / 2, screen_height - 5), ImVec2(head_2d.x, head_2d.y), ImColor(VIsible_color), 2);
			}
			


			/*Bounds::PlayerBounds o{ };
			if (GetPlayerBoundings(player->mesh, &o)) {*/


				if (box_esp)
				{


					if (box_outline)
					{
						normal_box(ImVec2(head_2d.x - (box_width / 2), head_2d.y), box_width, box_hight, ImColor(0, 0, 0, 255), box_thickness + 2.5);
					}

					normal_box(ImVec2(head_2d.x - (box_width / 2), head_2d.y), box_width, box_hight, VIsible_color, box_thickness);

				}
				if (box_round)
				{
					if (box_outline)
					{
						draw_list->AddRect(ImVec2(head_2d.x - box_width / 2, head_2d.y), ImVec2((head_2d.x - box_width / 2) + box_width, head_2d.y + box_hight), ImColor(0, 0, 0), 10, ImDrawCornerFlags_All, box_thickness + 2.5	);
					}

					draw_list->AddRect(ImVec2(head_2d.x - box_width / 2, head_2d.y), ImVec2((head_2d.x - box_width / 2) + box_width, head_2d.y + box_hight), VIsible_color, 10, ImDrawCornerFlags_All, box_thickness);
				}
				if (Corner)
				{
					if (box_outline)
					{
						draw_cornered_box(head_2d.x - (box_width / 2), head_2d.y, box_width, box_hight,  ImColor(0, 0, 0), box_thickness + 2.5);
					}

					draw_cornered_box(head_2d.x - (box_width / 2), head_2d.y, box_width, box_hight, VIsible_color, box_thickness);
				}

				if (threed)
				{

					if (box_outline)
					{
						DrawThreeDBox(base_3d, head_3d, 100, 100, box_thickness + 2.5, ImColor(0, 0, 0));
					}
					DrawThreeDBox(base_3d, head_3d, 100, 100, box_thickness, VIsible_color);
				}

		//	}

		}

		POINT cursor;
		GetCursorPos(&cursor);
	
		if (triggerbot) {

			if (global->local_pawn)
			{
				if (!TargetedFortPawn)
				{
					TargetedFortPawn = request->read<uintptr_t>((u)global->player_controller + 0x1888); // TargetedFortPawn
				}
				if (TargetedFortPawn) {


					if (shotgun_only)
					{
						if (HeldWeaponType == EFortWeaponType::Shotgun)
						{

							if (closest_player->player_distance <= max_trig_distance) {
								if (has_clicked) {
									tb_begin = std::chrono::steady_clock::now();
									has_clicked = 0;
								}
								tb_end = std::chrono::steady_clock::now();
								tb_time_since = std::chrono::duration_cast<std::chrono::milliseconds>(tb_end - tb_begin).count();
								if (tb_time_since >= trig_delay) {
									TargetedFortPawn = NULL;

									PostMessage(Fort_windwo, WM_LBUTTONDOWN | MK_LBUTTON, 0, MAKELPARAM(cursor.x, cursor.y));
									PostMessage(Fort_windwo, WM_LBUTTONUP, 0, MAKELPARAM(cursor.x, cursor.y));
									has_clicked = 1;

								}
							}
						}
					}
					else
					{
						if (closest_player->player_distance <= max_trig_distance) {
							if (has_clicked) {
								tb_begin = std::chrono::steady_clock::now();
								has_clicked = 0;
							}
							tb_end = std::chrono::steady_clock::now();
							tb_time_since = std::chrono::duration_cast<std::chrono::milliseconds>(tb_end - tb_begin).count();
							if (tb_time_since >= trig_delay) {
								TargetedFortPawn = NULL;

								PostMessage(Fort_windwo, WM_LBUTTONDOWN | MK_LBUTTON, 0, MAKELPARAM(cursor.x, cursor.y));
								PostMessage(Fort_windwo, WM_LBUTTONUP, 0, MAKELPARAM(cursor.x, cursor.y));
								has_clicked = 1;

							}
						}
					}
				}
			}
		}

		if (aimbot) {
			auto dx = head_2d.x - (screen_width / 2);
			auto dy = head_2d.y - (screen_height / 2);
			auto dist = sqrtf(dx * dx + dy * dy);

			if (dist < fov && dist < target_dist) {
				target_dist = dist;
				closest_player = player;
			}
		}

	

		if (closest_player->player && aimbot) {


			vector3 Velocity = request->read<vector3>((u)closest_player->root_component + 0x168);
			vector3 hitbox;
			float projectileSpeed = 0;
			float projectileGravityScale = 0;

				if (HeadBone) {
					hitbox = closest_player->mesh->get_bone_3d(110);
				}
				else if (NeckBbone) {
					hitbox = closest_player->mesh->get_bone_3d(66);
				}
				else if (ChestBone) {
					hitbox = closest_player->mesh->get_bone_3d(7);
				}
				else if (PelvisBone) {
					hitbox = closest_player->mesh->get_bone_3d(2);
				}
			

			if (prediction) {

				 if (strstr(LocalPlayerWeapon.c_str(), (E("Reaper Sniper Rifle"))))
				{
					projectileSpeed = 60000;
					projectileGravityScale = 4.5;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (E("Nemesis AR"))))
				{
					projectileSpeed = 80000;
					projectileGravityScale = 3.5;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (E("Warforged"))))
				{
					projectileSpeed = 80000;
					projectileGravityScale = 3.5;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (E("Striker AR"))))
				{
					projectileSpeed = 80000;
					projectileGravityScale = 3.5;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (E("SMG"))))
				{
					projectileSpeed = 75000;
					projectileGravityScale = 3;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (E("Pistol"))))
				{
					projectileSpeed = 60000;
					projectileGravityScale = 2;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (E("AR"))))
				{
					projectileSpeed = 80000;
					projectileGravityScale = 3.5;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (E("DMR"))))
				 {
					 projectileSpeed = 80000;
					 projectileGravityScale = 3.5;
				 }
				else {
					projectileSpeed = 0;
					projectileGravityScale = 0;
				}
			}
			int bone;
			if (projectileSpeed) {
				bone = 110;
			}

			//std::cout << projectileSpeed << std::endl;
			auto Distance = camera_postion.location.distance(hitbox);
			if (projectileSpeed) {


				hitbox = PredictLocation(hitbox, Velocity, projectileSpeed, projectileGravityScale, Distance); //dmr
			}

			vector2 hitbox_screen = w2s(hitbox);


			if (hitbox.x != 0 || hitbox.y != 0 && (GetCrossDistance(hitbox.x, hitbox.y, screen_width / 2, screen_height / 2) <= fov))
			{

				
					if (closest_player->mesh->visible()) {

						if (targetline) {
							ImGui::GetForegroundDrawList()->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(hitbox_screen.x, hitbox_screen.y), ImColor(255, 255, 255, 255), 1);
						}
						

				

				

						if (targetcirlce)
						{
							vector2 head_2d = w2s(vector3(hitbox.x, hitbox.y, hitbox.z));

							vector3 delta = hitbox - camera_postion.location;
							float distance = delta.length();

							const float constant_circle_size = 5;

							float circle_radius = constant_circle_size * (screen_height / (2.0f * distance * tanf(camera_postion.fov * (float)M_PI / 360.f)));

							int segments = 50;
							float thickness = 2.0f;

							
							ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, ImColor(0, 0, 0, 255), segments, skelthick + 2);
							
							ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, ImColor(255, 0, 0, 255), segments, skelthick);
						}

						if (GetAsyncKeyState_Spoofed(aimkey))
						{
							//mouse(hitbox_screen.x, hitbox_screen.y);

							vector2 screen_center = { static_cast<double>(screen_width) / 2, static_cast<double>(screen_height) / 2 };
							vector2 target{};


							target.x = (hitbox_screen.x > screen_center.x) ? -(screen_center.x - hitbox_screen.x) : hitbox_screen.x - screen_center.x;
							target.x /= smoothing;
							target.x = (target.x + screen_center.x > screen_center.x * 2 || target.x + screen_center.x < 0) ? 0 : target.x;

							target.y = (hitbox_screen.y > screen_center.y) ? -(screen_center.y - hitbox_screen.y) : hitbox_screen.y - screen_center.y;
							target.y /= smoothing;
							target.y = (target.y + screen_center.y > screen_center.y * 2 || target.y + screen_center.y < 0) ? 0 : target.y;
							vector3 Angles(-target.y / smoothing, target.x / smoothing, 0);


							request->write<double>((u)global->player_controller + 0x520, Angles.x);
							request->write<double>((u)global->player_controller + 0x528, Angles.y);

						}
					

					}
			
			

			}

		}
		else {
			target_dist = FLT_MAX;
			closest_player->player = NULL;
		}
	
	}
	lock.unlock();
}
	
