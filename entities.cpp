#include "Entities.h"
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"


CBaseCombatWeapon* IClientEntity::GetWeapon()
{
	static int iOffset = GET_NETVAR("DT_BaseCombatCharacter", "m_hActiveWeapon");
	ULONG pWeepEhandle = *(PULONG)((DWORD)this + iOffset);
	return (CBaseCombatWeapon*)(Interfaces::EntList->GetClientEntityFromHandle((HANDLE)pWeepEhandle));
}

