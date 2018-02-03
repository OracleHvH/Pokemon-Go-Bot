#include "Resolver.h"
#include "Ragebot.h"
#include "Hooks.h"

void LowerBodyYawFix(IClientEntity* pEntity)
{
	if (Menu::Window.RageBotTab.LowerbodyFix.GetState())
	{
		if (!pEntity) return;
		if (pEntity->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer) return;
		if (!pEntity->IsAlive() || !pEntity->GetActiveWeaponHandle()) return;
		if (Interfaces::Engine->GetLocalPlayer()) return;

		auto EyeAngles = pEntity->GetEyeAnglesXY();
		if (pEntity->GetVelocity().Length() > 1 && (pEntity->GetFlags() & (int)pEntity->GetFlags() & FL_ONGROUND))
			EyeAngles->y = pEntity->GetLowerBodyYaw();
	}
}

void PitchCorrection()
{
	CUserCmd* pCmd;
	for (int i = 0; i < Interfaces::Engine->GetMaxClients(); ++i)
	{
		IClientEntity* pLocal = hackManager.pLocal();
		IClientEntity *player = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);

		if (!player || player->IsDormant() || player->GetHealth() < 1 || (DWORD)player == (DWORD)pLocal)
			continue;

		if (!player)
			continue;

		if (pLocal)
			continue;

		if (pLocal && player && pLocal->IsAlive())
		{
			if (Menu::Window.RageBotTab.AdvancedResolver.GetState())
			{
				Vector* eyeAngles = player->GetEyeAnglesXY();
				if (eyeAngles->x < -179.f) eyeAngles->x += 360.f;
				else if (eyeAngles->x > 90.0 || eyeAngles->x < -90.0) eyeAngles->x = 89.f;
				else if (eyeAngles->x > 89.0 && eyeAngles->x < 91.0) eyeAngles->x -= 90.f;
				else if (eyeAngles->x > 179.0 && eyeAngles->x < 181.0) eyeAngles->x -= 180;
				else if (eyeAngles->x > -179.0 && eyeAngles->x < -181.0) eyeAngles->x += 180;
				else if (fabs(eyeAngles->x) == 0) eyeAngles->x = std::copysign(89.0f, eyeAngles->x);
			}
		}
	}
}

void ResolverSetup::Resolve(IClientEntity* pEntity)
{
	bool MeetsLBYReq;
	if (pEntity->GetFlags() & FL_ONGROUND)
		MeetsLBYReq = true;
	else
		MeetsLBYReq = false;

	bool IsMoving;
	if (pEntity->GetVelocity().Length2D() >= 0.5)
		IsMoving = true;
	else
		IsMoving = false;

	ResolverSetup::NewANgles[pEntity->GetIndex()] = *pEntity->GetEyeAnglesXY();
	ResolverSetup::newlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
	ResolverSetup::newsimtime = pEntity->GetSimulationTime();
	ResolverSetup::newdelta[pEntity->GetIndex()] = pEntity->GetEyeAnglesXY()->y;
	ResolverSetup::newlbydelta[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
	ResolverSetup::finaldelta[pEntity->GetIndex()] = ResolverSetup::newdelta[pEntity->GetIndex()] - ResolverSetup::storeddelta[pEntity->GetIndex()];
	ResolverSetup::finallbydelta[pEntity->GetIndex()] = ResolverSetup::newlbydelta[pEntity->GetIndex()] - ResolverSetup::storedlbydelta[pEntity->GetIndex()];
	if (newlby == storedlby)
		ResolverSetup::lbyupdated = false;
	else
		ResolverSetup::lbyupdated = true;

	if (Menu::Window.RageBotTab.AimbotResolver.GetIndex() == 0)
	{

	}
	else if (Menu::Window.RageBotTab.AimbotResolver.GetIndex() == 1)//level 1
	{
		if (MeetsLBYReq && lbyupdated)
			pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
		else if (!MeetsLBYReq && lbyupdated)
		{
			switch (Globals::Shots % 2)
			{
			case 1:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 15;
				break;
			case 2:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 40;
				break;
			case 3:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 15;
				break;
			case 4:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 40;
				break;
			case 5:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 90;
				break;
			case 6:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 180;
				break;
			case 7:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 90;
				break;
			case 8:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 180;
				break;
			case 9:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 15;
				break;
			case 10:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 47;
				break;
			case 11:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 77;
				break;
			case 12:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 47;
				break;
			case 13:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 47;
				break;
			case 14:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 58;
				break;
			case 15:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 14;
				break;
			case 16:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 25;
				break;
			case 17:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 32;
				break;
			case 18:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 24;
				break;
			case 19:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 180;
				break;
			case 20:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 90;
				break;
			case 21:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 14;
				break;
			case 22:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 25;
				break;
			case 23:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 34;
				break;
			case 25:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 78;
				break;
			case 26:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 90;
				break;
			case 27:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 47;
				break;
			case 28:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 91;
				break;
			case 29:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 150;
				break;
			case 30:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 75;
				break;
			case 31:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 78;
				break;
			case 32:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 50;
				break;
			case 33:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 70;
				break;
			case 34:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 140;
				break;
			case 35:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 78;
				break;
			case 36:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 150;
				break;
			case 37:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 130;
				break;
			case 39:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 176;
				break;
			case 40:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 120;
				break;
			case 41:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 120;
				break;
			case 42:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 123;
				break;
			case 44:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 56;
				break;
			case 45:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 24;
				break;
			case 46:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 70;
				break;
			case 47:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 58;
				break;
			case 48:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 170;
				break;
			case 49:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 40;
				break;
			case 50:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 123;
				break;
			case 51:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 20;
				break;
			case 100:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 90;
				break;
			case 52:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 90;
				break;
			case 53:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 45;
				break;
			case 54:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 45;
				break;
			case 55:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 70;
				break;
			case 56:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 70;
				break;
			case 57:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 120;
				break;
			case 58:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 130;
				break;
			case 59:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 34;
				break;
			case 60:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 64;
				break;
			case 61:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 78;
				break;
			case 62:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 42;
				break;
			case 63:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 17;
				break;
			case 64:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 42;
				break;
			case 65:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 170;
				break;
			case 66:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 170;
				break;
			case 67:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 58;
				break;
			case 68:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 140;
				break;
			case 69:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 78;
				break;
			case 70:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 40;
				break;
			case 71:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 78;
				break;
			case 72:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 40;
				break;
			case 73:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 74;
				break;
			case 74:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 74;
				break;
			case 75:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 78;
				break;
			case 76:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 45;
				break;
			case 77:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 74;
				break;
			case 78:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 90;
				break;
			case 79:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 80;
				break;
			case 80:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 70;
				break;
			case 81:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 47;
				break;
			case 82:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 145;
				break;
			case 83:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 178;
				break;
			case 84:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 147;
				break;
			case 85:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 170;
				break;
			case 86:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 170;
				break;
			case 87:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 160;
				break;
			case 88:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 160;
				break;
			case 89:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 150;
				break;
			case 90:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 150;
				break;
			case 91:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 140;
				break;
			case 92:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 140;
				break;
			case 93:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 130;
				break;
			case 94:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 130;
				break;
			case 95:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 120;
				break;
			case 96:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 120;
				break;
			case 97:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 110;
				break;
			case 98:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 110;
				break;
			case 99:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 100;
				break;
			case 156:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 100;
				break;
			case 101:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 80;
				break;
			case 102:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 80;
				break;
			case 103:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 70;
				break;
			case 104:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 70;
				break;
			case 105:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 60;
				break;
			case 106:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 60;
				break;
			case 107:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 50;
				break;
			case 108:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 50;
				break;
			case 109:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 40;
				break;
			case 110:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 40;
				break;
			case 111:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 30;
				break;
			case 112:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 30;
				break;
			case 113:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 20;
				break;
			case 114:
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 20;
				break;
			}
		}
			else if (MeetsLBYReq && !lbyupdated)
			{
				switch (Globals::Shots % 3)
				{
				case 1:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 15;
					break;
				case 2:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 40;
					break;
				case 3:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 15;
					break;
				case 4:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 40;
					break;
				case 5:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 90;
					break;
				case 6:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 180;
					break;
				case 7:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 90;
					break;
				case 8:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 180;
					break;
				case 9:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 15;
					break;
				case 10:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 47;
					break;
				case 11:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 77;
					break;
				case 12:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 47;
					break;
				case 13:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 47;
					break;
				case 14:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 58;
					break;
				case 15:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 14;
					break;
				case 16:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 25;
					break;
				case 17:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 32;
					break;
				case 18:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 24;
					break;
				case 19:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 180;
					break;
				case 20:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 90;
					break;
				case 21:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 14;
					break;
				case 22:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 25;
					break;
				case 23:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 34;
					break;
				case 25:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 78;
					break;
				case 26:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 90;
					break;
				case 27:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 47;
					break;
				case 28:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 91;
					break;
				case 29:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 150;
					break;
				case 30:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 75;
					break;
				case 31:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 78;
					break;
				case 32:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 50;
					break;
				case 33:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 70;
					break;
				case 34:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 140;
					break;
				case 35:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 78;
					break;
				case 36:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 150;
					break;
				case 37:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 130;
					break;
				case 38:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 176;
					break;
				case 39:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 120;
					break;
				case 40:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 120;
					break;
				case 41:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 123;
					break;
				case 42:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 56;
					break;
				case 43:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 24;
					break;
				case 44:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 70;
					break;
				case 45:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 58;
					break;
				case 46:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 170;
					break;
				case 47:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 40;
					break;
				case 48:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 123;
					break;
				case 49:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 20;
					break;
				}
			}
			else
				pEntity->GetEyeAnglesXY()->y = rand() % 360 - rand() % 5;
		}
		else if (Menu::Window.RageBotTab.AimbotResolver.GetIndex() == 2) //level 2 .
		{
			if (Resolver::didhitHS)
			{
				if (MeetsLBYReq && lbyupdated)
				{
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
				}
				switch (Globals::Shots % 6)
				{
				case 1:
					*pEntity->GetEyeAnglesXY() = StoredAngles[pEntity->GetIndex()];
					break;
				case 2:
					*pEntity->GetEyeAnglesXY() = StoredAngles[pEntity->GetIndex()];
					break;
				case 3:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 15;
					break;
				case 4:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y + 40;
					break;
				case 5:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y + 15;
					break;
				case 6:
					pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 40;
					break;
				}
			}
			else if (MeetsLBYReq && lbyupdated && !Resolver::didhitHS)
			{
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
			}
			else if (!MeetsLBYReq || !lbyupdated && !Resolver::didhitHS)
			{
				pEntity->GetEyeAnglesXY()->y = rand() % 180 - rand() % 35;
			}
			else
				pEntity->GetEyeAnglesXY()->y = rand() % 180;
		}
		else if (Menu::Window.RageBotTab.AimbotResolver.GetIndex() == 3)//level 3
		{
			if (Globals::missedshots > 3 && Globals::missedshots < 21)
			{
				if (MeetsLBYReq && lbyupdated)
				{
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
				}
				else if (!MeetsLBYReq && lbyupdated)
				{
					switch (Globals::Shots % 4)
					{
					case 1:
						pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 15;
						break;
					case 2:
						pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 40;
						break;
					case 3:
						pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 15;
						break;
					case 4:
						pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 40;
						break;
					}
				}
				else
					pEntity->GetEyeAnglesXY()->y = rand() % 180 - rand() % 35;
			}

			else if (Globals::missedshots >= 2 && Globals::missedshots <= 3)
			{
				if (MeetsLBYReq && lbyupdated)
				{
					pEntity->GetEyeAnglesXY()->y = ResolverSetup::finallbydelta[pEntity->GetIndex()];
				}
				else
					pEntity->GetEyeAnglesXY()->y = ResolverSetup::finaldelta[pEntity->GetIndex()];
			}
			else
			{
				if (MeetsLBYReq && lbyupdated)
				{
					bool timer;
					if (timer)
						pEntity->GetEyeAnglesXY()->y = ResolverSetup::finallbydelta[pEntity->GetIndex()] + rand() % 35;
					else
						pEntity->GetEyeAnglesXY()->y = ResolverSetup::finallbydelta[pEntity->GetIndex()] - rand() % 35;
					timer = !timer;
				}
				else
				{
					bool timer;
					if (timer)
						pEntity->GetEyeAnglesXY()->y = ResolverSetup::finaldelta[pEntity->GetIndex()] + rand() % 35;
					else
						pEntity->GetEyeAnglesXY()->y = ResolverSetup::finaldelta[pEntity->GetIndex()] - rand() % 35;
					timer = !timer;
				}
			}
		}
		LowerBodyYawFix(pEntity);
		PitchCorrection();
}

void ResolverSetup::StoreFGE(IClientEntity* pEntity)
{
	ResolverSetup::storedanglesFGE = pEntity->GetEyeAnglesXY()->y;
	ResolverSetup::storedlbyFGE = pEntity->GetLowerBodyYaw();
	ResolverSetup::storedsimtimeFGE = pEntity->GetSimulationTime();
}

void ResolverSetup::StoreThings(IClientEntity* pEntity)
{
	ResolverSetup::StoredAngles[pEntity->GetIndex()] = *pEntity->GetEyeAnglesXY();
	ResolverSetup::storedlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
	ResolverSetup::storedsimtime = pEntity->GetSimulationTime();
	ResolverSetup::storeddelta[pEntity->GetIndex()] = pEntity->GetEyeAnglesXY()->y;
	ResolverSetup::storedlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
}

void ResolverSetup::CM(IClientEntity* pEntity)
{
	for (int x = 1; x < Interfaces::Engine->GetMaxClients(); x++)
	{

		pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(x);

		if (!pEntity
			|| pEntity == hackManager.pLocal()
			|| pEntity->IsDormant()
			|| !pEntity->IsAlive())
			continue;

		ResolverSetup::StoreThings(pEntity);
	}
}


void AngleCorrect(IClientEntity* pEntity)
{
	if (Menu::Window.RageBotTab.AccuracyResolver2.GetState())
	{
		auto delta = pEntity->GetLowerBodyYaw();
		switch (Globals::Shots % 0)

			if (fabsf(pEntity->GetVelocity().Length2D() < 32)) // Fakewalk fix
			{
				pEntity->GetEyeAnglesPointer()->y = pEntity->GetLowerBodyYaw();
			}
			else
			{
				pEntity->GetEyeAnglesPointer()->y = Globals::Shots * 45;
			}
	}
}
	
void ResolverSetup::FSN(IClientEntity* pEntity, ClientFrameStage_t stage)
{
	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		for (int i = 1; i < Interfaces::Engine->GetMaxClients(); i++)
		{

			pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);

			if (!pEntity
				|| pEntity == hackManager.pLocal()
				|| pEntity->IsDormant()
				|| !pEntity->IsAlive())
				continue;

			ResolverSetup::Resolve(pEntity);
		}
	}
}