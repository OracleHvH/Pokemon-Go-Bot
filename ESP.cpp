/*
Syn's AyyWare Framework 2015
*/

#include "ESP.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "Glowmanager.h"
DWORD GlowManager = *(DWORD*)(Utilities::Memory::FindPatternV2("client.dll", "0F 11 05 ?? ?? ?? ?? 83 C8 01 C7 05 ?? ?? ?? ?? 00 00 00 00") + 3);
IClientEntity *BombCarrier;
void CEsp::Init()
{
	BombCarrier = nullptr;
}
#define ccsplayer 35
// Yeah dude we're defo gunna do some sick moves for the esp yeah
void CEsp::Move(CUserCmd *pCmd,bool &bSendPacket) 
{

}

// Main ESP Drawing loop
void CEsp::Draw()
{
	IClientEntity *pLocal = hackManager.pLocal();

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
			player_info_t pinfo;

			if (pEntity &&  pEntity != pLocal && !pEntity->IsDormant())
			{
				if (Menu::Window.VisualsTab.OtherRadar.GetState())
				{
					DWORD m_bSpotted = NetVar.GetNetVar(0x839EB159);
					*(char*)((DWORD)(pEntity)+m_bSpotted) = 1;
				}

				if (Menu::Window.VisualsTab.FiltersPlayers.GetState() && Interfaces::Engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive())
				{
					DrawPlayer(pEntity, pinfo);
				}

				ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();

				if (Menu::Window.VisualsTab.FiltersWeapons.GetState() && cClass->m_ClassID != (int)CSGOClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)CSGOClassID::CDEagle || cClass->m_ClassID == (int)CSGOClassID::CAK47)))
				{
					DrawDrop(pEntity, cClass);
				}

				if (Menu::Window.VisualsTab.FiltersC4.GetState())
				{
					if (cClass->m_ClassID == (int)CSGOClassID::CPlantedC4)
						DrawBombPlanted(pEntity, cClass);

					if (cClass->m_ClassID == (int)CSGOClassID::CC4)
						DrawBomb(pEntity, cClass);
				}

				if (Menu::Window.VisualsTab.FiltersChickens.GetState())
				{
					if (cClass->m_ClassID == (int)CSGOClassID::CChicken)
						DrawChicken(pEntity, cClass);
				}
			}
		}

		if (Menu::Window.MiscTab.OtherSpectators.GetState())
		{
			SpecList();
		}

}
#include "game.h"
#define M_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608640344181598136297747713099605187072113499999983729780499510597317328160963185950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989380952572010654858632788659361533818279682303019520353018529689957736225994138912497217752834791315155748572424541506959508295331168617278558890750983817546374649393192550604009277016711390098488240128583616035637076601047101819429555961989467678374494482553797747268471040475346462080466842590694912933136770289891521047521620569660240580

void CEsp::direction_arrow(const Vector& origin) {
	if (Menu::Window.VisualsTab.DirectionArrow.GetState()) {
		constexpr float radius = 360.0f;
		int width, height;
		Interfaces::Engine->GetScreenSize(width, height);

		Vector vRealAngles;
		Interfaces::Engine->GetViewAngles(vRealAngles);

		Vector vForward, vRight, vUp(0.0f, 0.0f, 1.0f);

		AngleVectors(vRealAngles, &vForward, NULL, NULL);

		vForward.z = 0.0f;
		VectorNormalize(vForward);
		CrossProduct(vUp, vForward, vRight);

		float flFront = DotProduct(origin, vForward);
		float flSide = DotProduct(origin, vRight);
		float flXPosition = radius * -flSide;
		float flYPosition = radius * -flFront;

		float rotation = game::globals.UserCmd->viewangles.y;

		rotation = atan2(flXPosition, flYPosition) + M_PI;
		rotation *= 180.0f / M_PI;

		float flYawRadians = -(rotation)* M_PI / 180.0f;
		float flCosYaw = cos(flYawRadians);
		float flSinYaw = sin(flYawRadians);

		flXPosition = (int)((width / 2.0f) + (radius * flSinYaw));
		flYPosition = (int)((height / 2.0f) - (radius * flCosYaw));
		float DirectionAlpha = 1;
		if (DirectionAlpha <= 1)
			DirectionAlpha = min(DirectionAlpha + 8, 255);
		else if (DirectionAlpha >= 255)
			DirectionAlpha = max(DirectionAlpha - 255, 1);
		Render::DrawRect(flXPosition, flYPosition, 10, 10, Color(255, 255, 255, DirectionAlpha));
	}
}
void CEsp::SpecList()
{
	IClientEntity *pLocal = hackManager.pLocal();

	RECT scrn = Render::GetViewport();
	int ayy = 0;

	// Loop through all active entitys
	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		player_info_t pinfo;

		// The entity isn't some laggy peice of shit or something
		if (pEntity &&  pEntity != pLocal)
		{
			if (Interfaces::Engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
			{
				HANDLE obs = pEntity->GetObserverTargetHandle();

				if (obs)
				{
					IClientEntity *pTarget = Interfaces::EntList->GetClientEntityFromHandle(obs);
					player_info_t pinfo2;
					if (pTarget)
					{
						if (Interfaces::Engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
						{
							char buf[255]; sprintf_s(buf, "%s: %s", pinfo.name, pinfo2.name);
							RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, buf);
							Render::Clear(scrn.right - 260, (scrn.bottom / 2) + (16 * ayy), 260, 16, Color(0, 0, 0, 0));
							Render::Text(scrn.right - TextSize.right - 4, (scrn.bottom / 2) + (16 * ayy), pTarget->GetIndex() == pLocal->GetIndex() ? Color(240, 0, 0, 200) : Color(255, 255, 255, 200), Render::Fonts::ESP, buf);
							ayy++;
						}
					}
				}
			}
		}
	}

	Render::Outline(scrn.right - 261, (scrn.bottom / 2) - 1, 262, (16 * ayy) + 2, Color(23, 23, 23, 0));
	Render::Outline(scrn.right - 260, (scrn.bottom / 2), 260, (16 * ayy), Color(90, 90, 90, 0));
}

//  Yeah m8
void CEsp::DrawPlayer(IClientEntity* pEntity, player_info_t pinfo)
{
	ESPBox Box;
	Color Color;

	// Show own team false? well gtfo teammate lol
	if (Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() && (pEntity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
		return;
	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++) 
	{
		if (pEntity->IsAlive()) {
			Vector Position;
			Position = pEntity->GetOrigin() - Vector(0, 0, 0);
			int EntityNum = pEntity->GetTeamNum();
			int LocalPlayerTeam = hackManager.pLocal()->GetTeamNum();
			if (!GetBox(pEntity, Box) && EntityNum != LocalPlayerTeam) {
				direction_arrow(Position);
			}

		}
	}
	if (GetBox(pEntity, Box))
	{
		Color = GetPlayerColor(pEntity);

		if (Menu::Window.VisualsTab.OptionsBox.GetIndex())
			DrawBox(Box, Color);

		if (Menu::Window.VisualsTab.OptionsName.GetState())
			DrawName(pinfo, Box);

		if (Menu::Window.VisualsTab.OptionsHealth.GetState())
			DrawHealth(Box, pEntity);

		if (Menu::Window.VisualsTab.OptionsInfo.GetState() || Menu::Window.VisualsTab.OptionsWeapon.GetState())
			DrawInfo(pEntity, Box);

		if (Menu::Window.VisualsTab.OptionsMoney.GetState())
			DrawMoney(pEntity, Box);

		if (Menu::Window.VisualsTab.OptionsAimSpot.GetState())
			DrawCross(pEntity);

		if (Menu::Window.VisualsTab.OptionsArmor.GetState())
			Armor(Box, pEntity);

		if (Menu::Window.VisualsTab.OptionsSkeleton.GetState())
			DrawSkeleton(pEntity);

		if (Menu::Window.VisualsTab.OptionsGlow.GetIndex())
			DrawGlow();

		if (Menu::Window.VisualsTab.OptionsBarrels.GetState())
			Barrel(Box, Color, pEntity);
	}
}


// Gets the 2D bounding box for the entity
// Returns false on failure nigga don't fail me
bool CEsp::GetBox(IClientEntity* pEntity, CEsp::ESPBox &result)
{
	// Variables
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = pEntity->GetOrigin();
	min = pEntity->collisionProperty()->GetMins() + vOrigin;
	max = pEntity->collisionProperty()->GetMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (!Render::WorldToScreen(points[3], flb) || !Render::WorldToScreen(points[5], brt)
		|| !Render::WorldToScreen(points[0], blb) || !Render::WorldToScreen(points[4], frt)
		|| !Render::WorldToScreen(points[2], frb) || !Render::WorldToScreen(points[1], brb)
		|| !Render::WorldToScreen(points[6], blt) || !Render::WorldToScreen(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;

	return true;
}

void CEsp::DrawGlow()
{
	CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)GlowManager;

	for (int i = 0; i < GlowObjectManager->size; ++i)
	{
		CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
		IClientEntity* Entity = glowEntity->getEntity();

		if (glowEntity->IsEmpty() || !Entity)
			continue;

		switch (Entity->GetClientClass()->m_ClassID)
		{
		case 35:
			if (Menu::Window.VisualsTab.OptionsGlow.GetIndex())
			{
				if (!Menu::Window.VisualsTab.FiltersPlayers.GetState() && !(Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
					break;
				if (Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() && (Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
					break;

				if (GameUtils::IsVisible(hackManager.pLocal(), Entity, 0))
				{
					glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(0, 255, 0, 120) : Color(0, 255, 0, 120));
				}

				else
				{
					glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(255, 255, 0, 120) : Color(255, 255, 0, 120));
				}
			}
		}
	}
}

void CEsp::Barrel(CEsp::ESPBox size, Color color, IClientEntity* pEntity)
{
	Vector src3D, src;
	src3D = pEntity->GetOrigin() - Vector(0, 0, 0);

	if (!Render::WorldToScreen(src3D, src))
		return;

	int ScreenWidth, ScreenHeight;
	Interfaces::Engine->GetScreenSize(ScreenWidth, ScreenHeight);

	int x = (int)(ScreenWidth * 0.5f);
	int y = 0;


	y = ScreenHeight;

	Render::Line((int)(src.x), (int)(src.y), x, y, Color(0, 0, 0, 255));
}

void CEsp::DrawMoney(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox ArmorBar = size;

	int MoneyEnemy = 100;
	MoneyEnemy = pEntity->GetMoney();
	char nameBuffer[512];
	sprintf_s(nameBuffer, "%d $", MoneyEnemy);

	RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, nameBuffer);
	Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 27, Color(255, 255, 0, 255), Render::Fonts::ESP, nameBuffer);
}

// Get an entities color depending on team and vis ect
Color CEsp::GetPlayerColor(IClientEntity* pEntity)
{
	int TeamNum = pEntity->GetTeamNum();
	bool IsVis = GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Head);

	Color color;

	if (TeamNum == TEAM_CS_T)
	{
		if (IsVis)
			color = Color(235, 200, 0, 255);
		else
			color = Color(235, 50, 0, 255);
	}
	else
	{
		if (IsVis)
			color = Color(120, 210, 26, 255);
		else
			color = Color(15, 110, 220, 255);
	}


	return color;
}

// 2D  Esp box
void CEsp::DrawBox(CEsp::ESPBox size, Color color)
{
	{
		{
			int VertLine;
			int HorzLine;
			// Corner Box
			//bool IsVis = GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Chest);  da dream
			int xd = Menu::Window.VisualsTab.OptionsBox.GetIndex();
			if (Menu::Window.VisualsTab.OptionsBox.GetIndex() == 1)
			{
				// Corner Box
				int VertLine = (((float)size.w) * (0.20f));
				int HorzLine = (((float)size.h) * (0.20f));

				Render::Clear(size.x, size.y - 1, VertLine, 1, Color(10, 10, 10, 240));
				Render::Clear(size.x + size.w - VertLine, size.y - 1, VertLine, 1, Color(10, 10, 10, 240));
				Render::Clear(size.x, size.y + size.h - 1, VertLine, 1, Color(10, 10, 10, 240));
				Render::Clear(size.x + size.w - VertLine, size.y + size.h - 1, VertLine, 1, Color(10, 10, 10, 240));

				Render::Clear(size.x - 1, size.y, 1, HorzLine, Color(10, 10, 10, 240));
				Render::Clear(size.x - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(10, 10, 10, 240));
				Render::Clear(size.x + size.w - 1, size.y, 1, HorzLine, Color(10, 10, 10, 240));
				Render::Clear(size.x + size.w - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(10, 10, 10, 240));

				Render::Clear(size.x, size.y, VertLine, 1, color);
				Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
				Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
				Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);

				Render::Clear(size.x, size.y, 1, HorzLine, color);
				Render::Clear(size.x, size.y + size.h - HorzLine, 1, HorzLine, color);
				Render::Clear(size.x + size.w, size.y, 1, HorzLine, color);
				Render::Clear(size.x + size.w, size.y + size.h - HorzLine, 1, HorzLine, color);
			}
			if (Menu::Window.VisualsTab.OptionsBox.GetIndex() == 2)
			{
				{
					Render::Outline(size.x, size.y, size.w, size.h, color);
					Render::Outline(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color(10, 10, 10, 240));
					Render::Outline(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(10, 10, 10, 240));
				}
			}
		}
	}
}


// Unicode Conversions
static wchar_t* CharToWideChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs_s(NULL, wa, size/4, text, size);
	return wa;
}

// Player name
void CEsp::DrawName(player_info_t pinfo, CEsp::ESPBox size)
{
	RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
	Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16,
		Color(255, 255, 255, 255), Render::Fonts::ESP, pinfo.name);
}

void CEsp::Armor(ESPBox box, IClientEntity* pEntity)
{
	float armor = pEntity->ArmorValue();

	if (armor > 100)
		armor = 100;

	Interfaces::Surface->DrawSetColor(0, 0, 0, 150);
	if (Menu::Window.VisualsTab.OptionsArmor.GetState())
	{
		Interfaces::Surface->DrawOutlinedRect(box.x + box.w + 1, box.y - 1, box.x + box.w + 5, box.y + box.h + 1);

		int height = armor * box.h / 100;

		if (armor > 0) {
			Interfaces::Surface->DrawSetColor(80, 135, 190, 255);
			Interfaces::Surface->DrawFilledRect(box.x + box.w + 2,
				box.y + box.h - height,
				box.x + box.w + 4,
				box.y + box.h);
		}
	}
}

// Draw a health bar. For Tf2 when a bar is bigger than max health a second bar is displayed
void CEsp::DrawHealth(ESPBox box, IClientEntity* pEntity)
{
	float health = pEntity->GetHealth();

	if (health > 100)
		health = 100;

	int colors[2] = { 255 - (health * 2.55), health * 2.55 };

	Interfaces::Surface->DrawSetColor(0, 0, 0, 150);
	if (Menu::Window.VisualsTab.OptionsHealth.GetState())
	{
		Interfaces::Surface->DrawOutlinedRect(box.x - 5, box.y - 1, box.x - 1, box.y + box.h + 1);

		int height = health * box.h / 100;

		if (health > 0)
		{
			Interfaces::Surface->DrawSetColor(colors[0], colors[1], 0, 255);
			Interfaces::Surface->DrawFilledRect(box.x - 4,
				box.y + box.h - height,
				box.x - 2,
				box.y + box.h);
		}
	}
}

// Cleans the internal class name up to something human readable and nice
std::string CleanItemName(std::string name)
{
	std::string Name = name;
	// Tidy up the weapon Name
	if (Name[0] == 'C')
		Name.erase(Name.begin());

	// Remove the word Weapon
	auto startOfWeap = Name.find("Weapon");
	if (startOfWeap != std::string::npos)
		Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);

	return Name;
}

// Anything else: weapons, class state? idk
void CEsp::DrawInfo(IClientEntity* pEntity, CEsp::ESPBox size)
{
	std::vector<std::string> Info;

	// Player Weapon ESP
	IClientEntity* pWeapon = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)pEntity->GetActiveWeaponHandle());
	if (Menu::Window.VisualsTab.OptionsWeapon.GetState() && pWeapon)
	{
		ClientClass* cClass = (ClientClass*)pWeapon->GetClientClass();
		if (cClass)
		{
			// Draw it
			Info.push_back(CleanItemName(cClass->m_pNetworkName));
		}
	}

	if (Menu::Window.VisualsTab.OptionsInfo.GetState() && pEntity == BombCarrier)
	{
		Info.push_back("Bomb Carrier");
	}

	static RECT Size = Render::GetTextSize(Render::Fonts::Menu, "Text");
	int i = 0;
	for (auto Text : Info)
	{
		Render::Text(size.x + size.w + 3, size.y + (i*(Size.bottom + 2)), Color(255, 255, 255, 255), Render::Fonts::Menu, Text.c_str());
		i++;
	}
}


// Little cross on their heads
void CEsp::DrawCross(IClientEntity* pEntity)
{
	Vector cross = pEntity->GetHeadPos(), screen;
	static int Scale = 2;
	if (Render::WorldToScreen(cross, screen))
	{
		Render::Clear(screen.x - Scale, screen.y - (Scale * 2), (Scale * 2), (Scale * 4), Color(20, 20, 20, 160));
		Render::Clear(screen.x - (Scale * 2), screen.y - Scale, (Scale * 4), (Scale * 2), Color(20, 20, 20, 160));
		Render::Clear(screen.x - Scale - 1, screen.y - (Scale * 2) - 1, (Scale * 2) - 2, (Scale * 4) - 2, Color(250, 250, 250, 160));
		Render::Clear(screen.x - (Scale * 2) - 1, screen.y - Scale - 1, (Scale * 4) - 2, (Scale * 2) - 2, Color(250, 250, 250, 160));
	}
}

// Draws a dropped CS:GO Item
void CEsp::DrawDrop(IClientEntity* pEntity, ClientClass* cClass)
{
	Vector Box;
	CBaseCombatWeapon* Weapon = (CBaseCombatWeapon*)pEntity;
	IClientEntity* plr = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)Weapon->GetOwnerHandle());
	if (!plr && Render::WorldToScreen(Weapon->GetOrigin(), Box))
	{
		if (Menu::Window.VisualsTab.OptionsBox.GetIndex())
		{
			Render::Outline(Box.x - 2, Box.y - 2, 4, 4, Color(255, 255, 255, 255));
			Render::Outline(Box.x - 3, Box.y - 3, 6, 6, Color(10, 10, 10, 150));
		}

		if (Menu::Window.VisualsTab.OptionsInfo.GetState())
		{
			std::string ItemName = CleanItemName(cClass->m_pNetworkName);
			RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, ItemName.c_str());
			Render::Text(Box.x - (TextSize.right / 2), Box.y - 16, Color(255, 255, 255, 255), Render::Fonts::ESP, ItemName.c_str());
		}
	}
}

// Draws a chicken
void CEsp::DrawChicken(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;

	if (GetBox(pEntity, Box))
	{
		player_info_t pinfo; strcpy_s(pinfo.name, "Chicken");
		if (Menu::Window.VisualsTab.OptionsBox.GetIndex())
			DrawBox(Box, Color(255,255,255,255));

		if (Menu::Window.VisualsTab.OptionsName.GetState())
			DrawName(pinfo, Box);
	}
}

// Draw the planted bomb and timer
void CEsp::DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass) 
{
	BombCarrier = nullptr;

	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)pEntity;

	if (Render::WorldToScreen(vOrig, vScreen))
	{
		float flBlow = Bomb->GetC4BlowTime();
		float TimeRemaining = flBlow - (Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase());
		char buffer[64];
		sprintf_s(buffer, "C4: %.1f", TimeRemaining);
		if (TimeRemaining >= 0.0f)
			Render::Text(vScreen.x, vScreen.y, Color(208, 215, 25, 255), Render::Fonts::Menu, buffer);
	}
}

void CEsp::DrawBomb(IClientEntity* pEntity, ClientClass* cClass)
{
	BombCarrier = nullptr;
	CBaseCombatWeapon *BombWeapon = (CBaseCombatWeapon *)pEntity;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	bool adopted = true;
	HANDLE parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		IClientEntity* pParentEnt = (Interfaces::EntList->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{
			BombCarrier = pParentEnt;
			adopted = false;
		}
	}

	if (adopted)
	{
		if (Render::WorldToScreen(vOrig, vScreen))
		{
			Render::Text(vScreen.x, vScreen.y, Color(208, 215, 25, 255), Render::Fonts::Menu, "C4");
		}
	}
}

void DrawBoneArray(int* boneNumbers, int amount, IClientEntity* pEntity, Color color)
{
	Vector LastBoneScreen;
	for (int i = 0; i < amount; i++)
	{
		Vector Bone = pEntity->GetBonePos(boneNumbers[i]);
		Vector BoneScreen;

		if (Render::WorldToScreen(Bone, BoneScreen))
		{
			if (i>0)
			{
				Render::Line(LastBoneScreen.x, LastBoneScreen.y, BoneScreen.x, BoneScreen.y, color);
			}
		}
		LastBoneScreen = BoneScreen;
	}
}

void DrawBoneTest(IClientEntity *pEntity)
{
	for (int i = 0; i < 127; i++)
	{
		Vector BoneLoc = pEntity->GetBonePos(i);
		Vector BoneScreen;
		if (Render::WorldToScreen(BoneLoc, BoneScreen))
		{
			char buf[10];
			_itoa_s(i, buf, 10);
			Render::Text(BoneScreen.x, BoneScreen.y, Color(255, 255, 255, 180), Render::Fonts::ESP, buf);
		}
	}
}

void CEsp::DrawSkeleton(IClientEntity* pEntity)
{
	studiohdr_t* pStudioHdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			if (Render::WorldToScreen(vParent, sParent) && Render::WorldToScreen(vChild, sChild))
			{
				Render::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(0, 0, 0,255));
			}
		}
	}
}