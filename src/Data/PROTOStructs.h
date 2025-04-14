#pragma once
#include "GenerallFiestaStructs.h"

#pragma pack(push,1 )
struct PROTO_EQUIPMENT
{
	struct type_upgrade
	{
		unsigned __int8 lefthand : 4;
		unsigned __int8 righthand : 4;
		unsigned __int8 body : 4;
		unsigned __int8 leg : 4;
		unsigned __int8 shoes : 4;
	};
	unsigned __int16 Equ_Head;
	unsigned __int16 Equ_Mouth;
	unsigned __int16 Equ_RightHand;
	unsigned __int16 Equ_Body;
	unsigned __int16 Equ_LeftHand;
	unsigned __int16 Equ_Pant;
	unsigned __int16 Equ_Boot;
	unsigned __int16 Equ_AccBoot;
	unsigned __int16 Equ_AccPant;
	unsigned __int16 Equ_AccBody;
	unsigned __int16 Equ_AccHeadA;
	unsigned __int16 Equ_MiniMon_R;
	unsigned __int16 Equ_Eye;
	unsigned __int16 Equ_AccLeftHand;
	unsigned __int16 Equ_AccRightHand;
	unsigned __int16 Equ_AccBack;
	unsigned __int16 Equ_CosEff;
	unsigned __int16 Equ_AccHip;
	unsigned __int16 Equ_Minimon;
	unsigned __int16 Equ_AccShield;
	type_upgrade upgrade;
};
#pragma pack(pop)