#pragma once
#include "ImGui/imgui.h"
#include <string>
#define SHN2k16 true
#define SHNV2 false
struct SHNRow
{
	unsigned short ColLen;
};
#if SHNV2
#pragma pack(push,1)

struct Abstate : SHNRow {
	short ID;
	char InxName[32];
	unsigned int AbStataIndex;
	unsigned int KeepTimeRatio;
	char KeepTimePower;
	char StateGrade;
	char PartyState1[32];
	char PartyState2[32];
	char PartyState3[32];
	char PartyState4[32];
	char PartyState5[32];
	unsigned int PartyRange;
	unsigned int PartyEnchantNumber;
	char SubAbState[32];
	unsigned int DispelIndex;
	unsigned int SubDispelIndex;
	char IsSave;
	char MainStateInx[32];
	char Duplicate;
};
struct AbStateView : SHNRow {
	short ID;
	char inxName[32];
	unsigned int icon;
	char iconFile[32];
	char Descript[256];
	char R;
	char G;
	char B;
	char AniIndex[32];
	char effName[32];
	unsigned int EffNamePos;
	char EffRefresh;
	char LoopEffect[32];
	unsigned int LoopEffPos;
	char LastEffect[32];
	unsigned int LastEffectPos;
	char DOTEffect[32];
	unsigned int DOTEffectPos;
	char IconSort[16];
	unsigned int TypeSort;
	char View;
};
struct ActionEffectInfo : SHNRow {
	unsigned int ID;
	char GrapicFile[32];
	char GEffectPos[17];
	char GEffectKeep;
	char Geffectdirection;
	char SoundFile[32];
};
struct ActionViewInfo : SHNRow {
	char nIndex;
	char InxName[32];
	char ActionName[32];
	short LinkActionIndex;
	char IconFileName[32];
	short nIconNum;
	unsigned int eActionType;
	unsigned int nEventCode;
	unsigned int nAfterCode;
};
struct ActiveSkill : SHNRow {
	short ID;
	char InxName[32];
	char Name[64];
	unsigned int Grade;
	unsigned int Step;
	unsigned int MaxStep;
	unsigned int DemandType;
	char DemandSk[32];
	short UseItem;
	unsigned int ItemNumber;
	unsigned int ItemOption;
	short DemandItem1;
	short DemandItem2;
	unsigned int SP;
	unsigned int HP;
	unsigned int Range;
	unsigned int First;
	unsigned int Last;
	char IsMovingSkill;
	short UsableDegree;
	short DirectionRotate;
	short SkillDegree;
	unsigned int SkillTargetState;
	unsigned int CastTime;
	unsigned int DlyTime;
	unsigned int DlyGroupNum;
	unsigned int DlyTimeGroup;
	unsigned int MinWC;
	unsigned int MaxWC;
	unsigned int MinMA;
	unsigned int MaxMA;
	unsigned int AC;
	unsigned int MR;
	unsigned int Area;
	unsigned int TargetNumber;
	unsigned int DemandClass;
	char StaNameA[32];
	unsigned int StaStrengthA;
	unsigned int StaSucRateA;
	char StaNameB[32];
	unsigned int StaStrengthB;
	unsigned int StaSucRateB;
	char StaNameC[32];
	unsigned int StaStrengthC;
	unsigned int StaSucRateC;
	char StaNameD[32];
	unsigned int StaStrengthD;
	unsigned int StaSucRateD;
	unsigned int nIMPT;
	unsigned int Unk_49;
	unsigned int Unk_50;
	unsigned int Unk_51;
	unsigned int nT0;
	unsigned int Unk_53;
	unsigned int Unk_54;
	unsigned int Unk_55;
	unsigned int Unk_56;
	unsigned int nT1;
	unsigned int Unk_58;
	unsigned int Unk_59;
	unsigned int Unk_60;
	unsigned int Unk_61;
	unsigned int nT2;
	unsigned int Unk_63;
	unsigned int Unk_64;
	unsigned int Unk_65;
	unsigned int Unk_66;
	unsigned int nT3;
	unsigned int Unk_68;
	unsigned int Unk_69;
	unsigned int Unk_70;
	unsigned int Unk_71;
	unsigned int EffectType;
	unsigned int SpecialIndexA;
	unsigned int SpecialValueA;
	unsigned int SpecialIndexB;
	unsigned int SpecialValueB;
	unsigned int SpecialIndexC;
	unsigned int SpecialValueC;
	unsigned int SpecialIndexD;
	unsigned int SpecialValueD;
	unsigned int SpecialIndexE;
	unsigned int SpecialValueE;
	char SkillClassifierA[32];
	char SkillClassifierB[32];
	char SkillClassifierC[32];
	char CannotInside;
	char DemandSoul;
	short HitID;
};
struct ActiveSkillGroup : SHNRow {
	char InxName[32];
	unsigned int ActiveSkillGroupIndex;
};
struct ActiveSkillInfoServer : SHNRow {
	short ID;
	char InxName[32];
	char UsualAttack;
	unsigned int SkilPyHitRate;
	unsigned int SkilMaHitRate;
	unsigned int PsySucRate;
	unsigned int MagSucRate;
	char StaLevel;
	unsigned int DmgIncRate;
	short DmgIncValue;
	unsigned int SkillHitType;
	char ItremUseSkill;
	unsigned int AggroPerDamage;
	unsigned int AbsoluteAggro;
	char AttackStart;
	char AttackEnd;
	short SwingTime;
	short HitTime;
	char AddSoul;
};
struct ActiveSkillView : SHNRow {
	short ID;
	char InxName[32];
	unsigned int CancelCasting;
	char TargetChange;
	unsigned int IconIndex;
	char IconFile[32];
	unsigned int R;
	unsigned int G;
	unsigned int B;
	unsigned int CastingType;
	unsigned int ActionType;
	char CasRdyAction[32];
	char CasAction[32];
	char SwingAction[32];
	unsigned int ShoEfSpd;
	char ShoEffect[32];
	char ShoSnd[32];
	char LastAction[32];
	char LastEffectA[32];
	unsigned int eLastEffPos;
	char LastEfASnd[32];
	char LastAreaEf[32];
	char LastAEfWhe[32];
	char LastAESnd[32];
	char DOTRageEft[32];
	char DOTRageEftSnd[32];
	char DOTRageEftLoop[32];
	char DOTRageEftLoopSnd[32];
	char Descript[256];
	unsigned int uiDemandLv;
	char Function[128];
};
struct BadNameFilter : SHNRow {
	char BadName[32];
	unsigned int Type;
};
struct CharacterTitleData : SHNRow {
	unsigned int Type;
	char Title0[32];
	unsigned int Value0;
	unsigned int Fame0;
	char Title1[32];
	unsigned int Value1;
	unsigned int Fame1;
	char Title2[32];
	unsigned int Value2;
	unsigned int Fame2;
	char Title3[32];
	unsigned int Value3;
	unsigned int Fame3;
};
struct ChargedEffect : SHNRow {
	short Handle;
	char ItemID[32];
	short KeepTime_Hour;
	unsigned int EffectEnum;
	short EffectValue;
	char StaStrength;
};
struct ClassName : SHNRow {
	char ClassID;
	unsigned int acPrefix;
	char acEngName[16];
	char acLocalName[32];
};
struct ColorInfo : SHNRow {
	unsigned int eColorID;
	char ColorR;
	char ColorG;
	char ColorB;
};
struct DamageEffect : SHNRow {
	short ID;
	unsigned int DamageType;
	char INDEX[32];
	char IsDamagedObject;
	short EffectID;
};
struct DamageSoundInfo : SHNRow {
	char acDmgSndMapByArmorType[32];
	char Unk_1[32];
	char Unk_2[32];
	char Unk_3[32];
	char Unk_4[32];
	char Unk_5[32];
	char Unk_6[32];
	char Unk_7[32];
	char Unk_8[32];
	char Unk_9[32];
	char Unk_10[32];
};
struct DiceDividind : SHNRow {
	short DividendRate;
	short Unk_1;
	short Unk_2;
	short Unk_3;
	short Unk_4;
	short Unk_5;
	short Unk_6;
	short Unk_7;
	short Unk_8;
	short Unk_9;
	short Unk_10;
	short Unk_11;
	short Unk_12;
	short Unk_13;
	short Unk_14;
	char AnyTriple;
};
struct DiceGame : SHNRow {
	short ItemID;
	short UseMinLv;
	short GetSysRate;
	short GetMasterRate;
	unsigned int MinGetMoney;
	unsigned int MaxBetMoney;
	short CastTime;
	short DelayTime;
	unsigned int WinCode;
	unsigned int Unk_9;
	unsigned int Unk_10;
	unsigned int LoseCode;
	unsigned int Unk_12;
	unsigned int Unk_13;
};
struct DiceRate : SHNRow {
	unsigned int Rate;
};
struct EffectViewInfo : SHNRow {
	short ID;
	unsigned int Amplitude;
	unsigned int Period;
	unsigned int Damp;
	unsigned int Axis;
};
struct EmotionFilter : SHNRow {
	char String[32];
	char Action[32];
};
struct ErrorCodeTable : SHNRow {
	unsigned int ErrorCode;
	char ErrorMessageIndex[64];
};
struct FaceInfo : SHNRow {
	char ID;
	char FaceName[32];
	char Grade;
	char FM_F_Male;
	char FT_F_Male;
	char FM_F_Female;
	char FT_F_Female;
	char FM_C_Male;
	char FT_C_Male;
	char FM_C_Female;
	char FT_C_Female;
	char FM_A_Male;
	char FT_A_Male;
	char FM_A_Female;
	char FT_A_Female;
	char FM_M_Male;
	char FT_M_Male;
	char FM_M_Female;
	char FT_M_Female;
	char FM_J_Male;
	char FT_J_Male;
	char FM_J_Female;
	char FT_J_Female;
};
struct Gather : SHNRow {
	short GatherID;
	char Index[32];
	unsigned int Type;
	char NeededTool0[32];
	char NeededTool1[32];
	char NeededTool2[32];
	char EqipItemView[32];
	unsigned int AniNumber;
	unsigned int Gauge;
};
struct GradeItemOption : SHNRow {
	char ItemIndex[32];
	short STR;
	short CON;
	short DEX;
	short INT;
	short MEN;
	short ResistPoison;
	short ResistDeaseas;
	short ResistCurse;
	short ResistMoveSpdDown;
	short Critical;
	short ToHitRate;
	short ToHitPlus;
	short ToBlockRate;
	short ToBlockPlus;
	short MaxHP;
	short MaxSP;
	short MoveSpdRate;
	short AbsoluteAttack;
	short PickupLimit;
};
struct GTIView : SHNRow {
	char ID;
	char InxName[33];
	char Name[32];
	char Advantage;
};
struct GuildGradeData : SHNRow {
	char Type;
	unsigned int NeedFame;
	short MaxOfMember;
	short MaxOfGradeMember;
	short Unk_4;
	short Unk_5;
	short Unk_6;
	short Unk_7;
	short Unk_8;
	short Unk_9;
};
struct GuildTournamentRequire : SHNRow {
	char MinLv;
	short MinMem;
	unsigned int JoinMoney;
};
struct GuildTournamentSkill : SHNRow {
	short MAP_TYPE;
	short Index;
	short DeathPoint;
	char StaName[32];
	unsigned int TargetType;
	unsigned int DlyTime;
};
struct GuildTournamentSkillDesc : SHNRow {
	short MAP_TYPE;
	short Index;
	unsigned int IconIndex;
	char IconFile[32];
	char Name[32];
	char Description[64];
};
struct HairColorInfo : SHNRow {
	char ID;
	char IndexName[15];
	char Name[30];
	char ColorTextureName[15];
	char Grade;
};
struct HairInfo : SHNRow {
	char ID;
	char IndexName[32];
	char HairName[32];
	char Grade;
	unsigned int fighter;
	unsigned int archer;
	unsigned int cleric;
	unsigned int mage;
	unsigned int Joker;
	char ucIsLink_Front;
	char acModelName_Front[32];
	char FrontTex[32];
	char ucIsLink_Bottom;
	char acModelName_Bottom[32];
	char BottomTex[32];
	char ucIsLink_Top;
	char acModelName_Top[32];
	char TopTex[32];
	unsigned int Exception1;
	unsigned int Exception2;
	char ucIsLink_Acc;
	char acModelName_Acc[32];
	char Acc1Tex[32];
	char ucIsLink_Acc2;
	char acModelName_Acc2[32];
	char Acc2Tex[32];
	char ucIsLink_Acc3;
	char acModelName_Acc3[32];
	char Acc3Tex[32];
};
struct HolyPromiseReward : SHNRow {
	char Level;
	char Class;
	char ItemIndex[33];
	short Lot;
	char Upgrade;
	unsigned int IO_Str;
	unsigned int IO_Con;
	unsigned int IO_Dex;
	unsigned int IO_Int;
	unsigned int IO_Men;
	unsigned int Res1;
	unsigned int Res2;
	unsigned int Res3;
};
struct ItemDismantle : SHNRow {
	char ID;
	char Grade;
	unsigned int Armor;
	unsigned int Unk_3;
	unsigned int Unk_4;
	unsigned int Unk_5;
	unsigned int Unk_6;
	unsigned int Boot;
	unsigned int Unk_8;
	unsigned int Unk_9;
	unsigned int Unk_10;
	unsigned int Unk_11;
	unsigned int Shield;
	unsigned int Unk_13;
	unsigned int Unk_14;
	unsigned int Unk_15;
	unsigned int Unk_16;
	unsigned int Weapon;
	unsigned int Unk_18;
	unsigned int Unk_19;
	unsigned int Unk_20;
	unsigned int Unk_21;
	unsigned int Amulet;
	unsigned int Unk_23;
	unsigned int Unk_24;
	unsigned int Unk_25;
	unsigned int Unk_26;
};
struct ItemInfo : SHNRow {
	short ID;
	char InxName[32];
	char Name[64];
	unsigned int Type;
	unsigned int Class;
	unsigned int MaxLot;
	unsigned int Equip;
	char TwoHand;
	unsigned int AtkSpeed;
	unsigned int DemandLv;
	unsigned int Grade;
	unsigned int MinWC;
	unsigned int MaxWC;
	unsigned int AC;
	unsigned int MinMA;
	unsigned int MaxMA;
	unsigned int MR;
	unsigned int WCRate;
	unsigned int MARate;
	unsigned int ACRate;
	unsigned int MRRate;
	unsigned int CriRate;
	unsigned int CriMinWc;
	unsigned int CriMaxWc;
	unsigned int CriMinMa;
	unsigned int CriMaxMa;
	unsigned int MaxHP;
	unsigned int MaxSP;
	unsigned int MaxAP;
	unsigned int WhoEquip;
	unsigned int BuyPrice;
	unsigned int SellPrice;
	unsigned int BuyFame;
	unsigned int BuyGToken;
	unsigned int WeaponType;
	unsigned int ArmorType;
	char UpLimit;
	short UpSucRatio;
	short UpLuckRatio;
	char UpResource;
	short BasicUpInx;
	short AddUpInx;
	unsigned int TH;
	unsigned int TB;
	unsigned int ShieldAC;
	unsigned int HitRatePlus;
	unsigned int EvaRatePlus;
	unsigned int MACriPlus;
	unsigned int CriDamPlus;
	unsigned int MagCriDamPlus;
	char Belonged;
	char NoDrop;
	char NoSell;
	char NoStorage;
	char NoTrade;
	char NoDelete;
	char TitleName[32];
	unsigned int ItemGradeType;
	char ItemUseSkill[32];
	char SetItemIndex[32];
	unsigned int ItemFunc;
	char AutoMon;
};
struct ItemShopView : SHNRow {
	unsigned int goodsNo;
	char Name[256];
	unsigned int IconIndex;
	char IconFile[32];
	unsigned int SubIconIndex;
	char SubIconFile[32];
	unsigned int PeriodIconIndex;
	char PeriodIconFile[32];
	unsigned int R;
	unsigned int G;
	unsigned int B;
	char Descript[256];
};
struct ItemViewInfo : SHNRow {
	short ID;
	char InxName[32];
	unsigned int IconIndex;
	char IconFile[32];
	unsigned int SubIconIndex;
	char SubIconFile[32];
	unsigned int PeriodIconIndex;
	char PeriodIconFile[32];
	unsigned int R;
	unsigned int G;
	unsigned int B;
	unsigned int SUB_R;
	unsigned int SUB_G;
	unsigned int SUB_B;
	unsigned int EquipType;
	char LinkFile[32];
	char TextureFile[32];
	unsigned int MSetNo;
	unsigned int FSetNo;
	unsigned int GrnItemSize;
	char GrnItemTex[32];
	char Descript[256];
	short UpEffect;
	char DropSnd[32];
	char EquSnd[32];
	char PutSnd[32];
	unsigned int MDummyType01;
	char MNif01[32];
	char MArmorTexture01[32];
	unsigned int MDummyType02;
	char MNif02[32];
	char MArmorTexture02[32];
	unsigned int MDummyType03;
	char MNif03[32];
	char MArmorTexture03[32];
	unsigned int MDummyType04;
	char MNif04[32];
	char MArmorTexture04[32];
	unsigned int MDummyType05;
	char MNif05[32];
	char MArmorTexture05[32];
	unsigned int MDummyType06;
	char MNif06[32];
	char MArmorTexture06[32];
	unsigned int MDummyType07;
	char MNif07[32];
	char MArmorTexture07[32];
	unsigned int FDummyType01;
	char FNif01[32];
	char FArmorTexture01[32];
	unsigned int FDummyType02;
	char FNif02[32];
	char FArmorTexture02[32];
	unsigned int FDummyType03;
	char FNif03[32];
	char FArmorTexture03[32];
	unsigned int FDummyType04;
	char FNif04[32];
	char FArmorTexture04[32];
	unsigned int FDummyType05;
	char FNif05[32];
	char FArmorTexture05[32];
	unsigned int FDummyType06;
	char FNif06[32];
	char FArmorTexture06[32];
	unsigned int FDummyType07;
	char FNif07[32];
	char FArmorTexture07[32];
	char CosArmor;
	char CosPants;
	char CosBoots;
	char HairVisibleState;
};
struct KingdomQuestDesc : SHNRow {
	char Desc;
};
struct MapInfo : SHNRow {
	short ID;
	char MapName[12];
	char Name[32];
	unsigned int RegenX;
	unsigned int RegenY;
	char KingdomMap;
	char MapFolderName[12];
	char InSide;
	unsigned int Sight;
};
struct MapViewInfo : SHNRow {
	short ID;
	char MapName[12];
	char BGM_Name_01[32];
	char BGM_Name_02[32];
	char BGM_Name_03[32];
	short BGMDlyTime;
	unsigned int MiniMapScale;
	char Enlargement;
	char KingdomMap;
	char MapFolderName[12];
	char MinimapView;
	char WorldMapView;
	char Loading[32];
	unsigned int BGMVol;
	char LoadLocation;
	short StartX;
	short StartY;
	short EndX;
	short EndY;
	char WeatherEffect[32];
	unsigned int SightRevise;
	char MiniMapSort;
};
struct MHEmotion : SHNRow {
	short MHEmotionID;
	char ActionViewInfo;
	char ActionIndex;
	unsigned int EventCode;
	unsigned int LegEventCode;
	char Name[32];
	char IconIndex;
	char IconFile[16];
};
struct MiniHouse : SHNRow {
	short Handle;
	char ItemID[32];
	char DummyType[32];
	char Backimage[16];
	short KeepTime_Hour;
	short HPTick;
	short SPTick;
	short HPRecovery;
	short SPRecovery;
	short Casting;
	char Slot;
};
struct MiniHouseDummy : SHNRow {
	short No;
	char Index[32];
	char IconFileName[32];
	short nIconNum;
	short HPTick;
	short SPTick;
	short HPRecovery;
	short SPRecovery;
	short Casting;
	char Slot;
	char HouseAType[32];
	unsigned int HouseALoc;
	char HouseBType[32];
	unsigned int HouseBLoc;
	char HouseCType[32];
	unsigned int HouseCLoc;
};
struct MiniHouseEndure : SHNRow {
	short Handle;
	short Endure;
	short Unk_2;
	short Unk_3;
	short Unk_4;
};
struct MiniHouseFurniture : SHNRow {
	short Handle;
	char ItemID[32];
	char FurnitureType[32];
	char InvenType[32];
	unsigned int GameType;
	char CanSet;
	char Backimage[32];
	char WALL;
	char BOTTOM;
	char CEILING;
	char IsAnimation;
	short Weight;
	short KeepTime_Hour;
	short KeepTime_Endure;
	char Grip;
	char MaxSlot;
	char MHEmotionID;
};
struct MiniHouseFurnitureObjEffect : SHNRow {
	short Handle;
	char ItemID[32];
	unsigned int EffectEnum;
	char EffectIndex[32];
	unsigned int ApplyRange;
	unsigned int Unk_5;
	unsigned int Unk_6;
	unsigned int Unk_7;
	unsigned int Unk_8;
	unsigned int UseRange;
	unsigned int Unk_10;
	unsigned int Unk_11;
	unsigned int Unk_12;
	unsigned int Unk_13;
	char NeedItem[32];
	unsigned int NeedMoney;
	char EffectName[32];
	char EffectSound[32];
};
struct MiniHouseObjAni : SHNRow {
	short Handle;
	short ItemID;
	char AniGroupIDMaxNum;
	short AniGroupID;
	unsigned int EventCode;
	short NextAniHandle;
	char ActorMaxNum;
	unsigned int Actor01;
	unsigned int Actor02;
	unsigned int Actor03;
	char ActeeMaxNum;
	unsigned int Actee01;
	unsigned int Actee02;
	unsigned int Actee03;
};
struct MobCoordinate : SHNRow {
	short MC_ID;
	short Mob_ID;
	char MapName[12];
	unsigned int CenterX;
	unsigned int CenterY;
	unsigned int Width;
	unsigned int Height;
	unsigned int RangeDegree;
};
struct MobInfo : SHNRow {
	short ID;
	char InxName[32];
	char Name[32];
	unsigned int Level;
	unsigned int MaxHP;
	unsigned int WalkSpeed;
	unsigned int RunSpeed;
	char IsNPC;
	unsigned int Size;
	unsigned int WeaponType;
	unsigned int ArmorType;
	unsigned int GradeType;
	unsigned int Type;
	char IsPlayerSide;
	unsigned int AbsoluteSize;
};
struct MobKillAnnounce : SHNRow {
	unsigned int MobID;
	unsigned int TextIndex;
};
struct MobKillAnnounceText : SHNRow {
	unsigned int TextIndex;
	char Text[128];
};
struct MobSpecies : SHNRow {
	short ID;
	char MobName[33];
	char Unk_2[33];
	char Unk_3[33];
	char Unk_4[33];
};
struct MobViewInfo : SHNRow {
	short ID;
	char InxName[32];
	char FileName[32];
	char Texture[32];
	unsigned int AttackType;
	char ShotEffect[32];
	char MobPortrait[32];
	unsigned int ChrMarkSize;
	unsigned int MiniMapIcon;
	short NpcViewIndex;
	short BoundingBox;
	short EffectViewID;
	char SpectralGlow;
};
struct MobWeapon : SHNRow {
	unsigned int ID;
	char InxName[33];
	char Skill[32];
	short AtkSpd;
	short BlastRate;
	short AtkDly;
	short CastTime;
	short SwingTime;
	short HitTime;
	short HitID;
	unsigned int AtkType;
	unsigned int MinWC;
	unsigned int MaxWC;
	short TH;
	unsigned int MinMA;
	unsigned int MaxMA;
	short MH;
	short Range;
	unsigned int MopAttackTarget;
	unsigned int HitType;
	char StaName[33];
	short StaStrength;
	short StaRate;
	short AggroInitialize;
};
struct NpcDialogData : SHNRow {
	short ID;
	char Dialog;
};
struct NPCViewInfo : SHNRow {
	short TypeIndex;
	unsigned int Class;
	short Gender;
	char FaceShape;
	char HairType;
	char HairColor;
	unsigned int BaseActionCode;
	unsigned int PeriodActionCode;
	unsigned int ActionDelayTime;
	char bUseEventAction;
	char Equ_RightHand[32];
	char Equ_LeftHand[32];
	char Equ_Body[32];
	char Equ_Leg[32];
	char Equ_Shoes[32];
	char Equ_AccBody[32];
	char Equ_AccLeg[32];
	char Equ_AccShoes[32];
	char Equ_AccShoulderA[32];
	char Equ_AccShoulderB[32];
	char Equ_AccHeadA[32];
	char Equ_AccHeadB[32];
	char Equ_AccHeadC[32];
	char Equ_AccLeftHand[32];
	char Equ_AccRightHand[32];
	char Equ_AccBack[32];
	char Equ_AccWeast[32];
	char Equ_AccHip[32];
	char Equ_AccFoot[32];
};
struct PassiveSkill : SHNRow {
	short ID;
	char InxName[32];
	char Name[32];
	unsigned int WeaponMastery;
	char DemandSk[32];
	unsigned int MstRtTmp;
	unsigned int MstRtSword1;
	unsigned int MstRtHammer1;
	unsigned int MstRtSword2;
	unsigned int MstRtAxe2;
	unsigned int MstRtMace1;
	unsigned int MstRtBow2;
	unsigned int MstRtCrossBow2;
	unsigned int MstRtWand2;
	unsigned int MstRtStaff2;
	unsigned int MstRtClaw;
	unsigned int MstRtDSword;
	unsigned int MstPlTmp;
	unsigned int MstPlSword1;
	unsigned int MstPlHammer1;
	unsigned int MstPlSword2;
	unsigned int MstPlAxe2;
	unsigned int MstPlMace1;
	unsigned int MstPlBow2;
	unsigned int MstPlCrossBow2;
	unsigned int MstPlWand2;
	unsigned int MstPlStaff2;
	unsigned int MstPlClaw;
	unsigned int MstPlDSword;
	unsigned int SPRecover;
	unsigned int TB;
	unsigned int MaxSP;
	unsigned int Intel;
	unsigned int CastingTime;
	short MACriRate;
};
struct PassiveSkillView : SHNRow {
	short ID;
	char InxName[32];
	unsigned int Icon;
	char IconFile[32];
	unsigned int R;
	unsigned int G;
	unsigned int B;
	char Descript[256];
	unsigned int DemandLv;
	unsigned int DemandClass;
};
struct Produce : SHNRow {
	short ProductID;
	char ProduceIndex[32];
	char Name[32];
	char Product[32];
	unsigned int Lot;
	char Raw0[32];
	unsigned int Quantity0;
	char Raw1[32];
	unsigned int Quantity1;
	char Raw2[32];
	unsigned int Quantity2;
	char Raw3[32];
	unsigned int Quantity3;
	char Raw4[32];
	unsigned int Quantity4;
	char Raw5[32];
	unsigned int Quantity5;
	char Raw6[32];
	unsigned int Quantity6;
	char Raw7[32];
	unsigned int Quantity7;
	unsigned int MasteryType;
	unsigned int MasteryGain;
	unsigned int NeededMasteryType;
	unsigned int NeededMasteryGain;
};
struct ProduceView : SHNRow {
	short ID;
	char InxName[32];
	char Name[32];
	short IconIndex;
	char IconFile[32];
	short SubIconIndex;
	char SubIconFile[32];
	unsigned int MasteryType;
	unsigned int RowMasteryGain;
	unsigned int NorMasteryGain;
	unsigned int HighMasteryGain;
	unsigned int BestMasteryGain;
};
struct QuestDialog : SHNRow {
	short ID;
	char Dialog;
};
struct RaceNameInfo : SHNRow {
	char RaceID;
	unsigned int acPrefix;
	char acEngName[16];
	char acLocalName[16];
};
struct Riding : SHNRow {
	short Handle;
	char ItemID[32];
	char Name[32];
	char BodyType[32];
	char Shape[32];
	short UseTime;
	char FeedType[32];
	char Texture[32];
	short FeedGauge;
	short HGauge;
	short InitHgauge;
	short Tick;
	short UGauge;
	short RunSpeed;
	short FootSpeed;
	short CastingTime;
	unsigned int CoolTime;
	char IconFileN[16];
	short IconIndex;
	char ImageN[16];
	char ImageH[16];
	char ImageE[16];
	char DummyA[32];
	char DummyB[32];
};
struct SetItem : SHNRow {
	char Index[32];
	char Piece;
	char Effect[32];
};
struct SetItemEffect : SHNRow {
	char Effect[32];
	char Desc[128];
	char UseSubject;
	char SkillGroup[64];
	unsigned int From;
	unsigned int To;
	unsigned int Index;
	unsigned int Argument;
};
struct SetItemView : SHNRow {
	char Index[32];
	char Name[32];
};
struct SlanderFilter : SHNRow {
	char Slander[32];
	char Alternate[32];
};
struct SubAbState : SHNRow {
	short ID;
	char InxName[32];
	unsigned int Strength;
	unsigned int Type;
	char SubType;
	unsigned int KeepTime;
	unsigned int ActionIndexA;
	unsigned int ActionArgA;
	unsigned int ActionIndexB;
	unsigned int ActionArgB;
	unsigned int ActionIndexC;
	unsigned int ActionArgC;
	unsigned int ActionIndexD;
	unsigned int ActionArgD;
};
struct TextData : SHNRow {
	unsigned int eTextID;
	char acVoice[32];
	char acSoundEff[32];
	char acString;
};
struct TextData2 : SHNRow {
	unsigned int eTextID;
	char acVoice[32];
	char acSoundEff[32];
	char acString;
};
struct TextData3 : SHNRow {
	unsigned int eTextID;
	char acVoice[32];
	char acSoundEff[32];
	char acString;
};
struct TownPortal : SHNRow {
	char Index;
	char MinLevel;
	char MapName[32];
	unsigned int X;
	unsigned int Y;
};
struct UpEffect : SHNRow {
	short ID;
	char UpEffect;
	char Unk_2;
	char Unk_3;
	char Unk_4;
	char Unk_5;
	char Unk_6;
	char Unk_7;
	char Unk_8;
	char Unk_9;
	char Unk_10;
	char Unk_11;
	char Unk_12;
	char UpTexture;
	char Unk_14;
	char Unk_15;
	char Unk_16;
	char Unk_17;
	char Unk_18;
	char Unk_19;
	char Unk_20;
	char Unk_21;
	char Unk_22;
	char Unk_23;
	char Unk_24;
	char UpAfterImgEff;
	char Unk_26;
	char Unk_27;
	char Unk_28;
	char Unk_29;
	char Unk_30;
	char Unk_31;
	char Unk_32;
	char Unk_33;
	char Unk_34;
	char Unk_35;
	char Unk_36;
};
struct UpgradeInfo : SHNRow {
	short ID;
	char InxName[32];
	unsigned int UpFactor;
	short Updata;
	short Unk_4;
	short Unk_5;
	short Unk_6;
	short Unk_7;
	short Unk_8;
	short Unk_9;
	short Unk_10;
	short Unk_11;
	short Unk_12;
	short Unk_13;
	short Unk_14;
};
struct WeaponAttrib : SHNRow {
	unsigned int WeaponType;
	short UsableDegree;
	char IsUsableInMoving;
	short HitRate;
	short Unk_4;
	short Unk_5;
};
struct WeaponTitleData : SHNRow {
	short MobID;
	char Level;
	char Prefix[16];
	char Suffix[16];
	unsigned int MobKillCount;
	short MinAdd;
	short MaxAdd;
	char SP1_Reference;
	short SP1_Type;
	unsigned int SP1_Value;
	char SP2_Reference;
	short SP2_Type;
	unsigned int SP2_Value;
	char SP3_Reference;
	short SP3_Type;
	unsigned int SP3_Value;
};
struct WorldMapAvatarInfo : SHNRow {
	char MapName[12];
	unsigned int LocalX;
	unsigned int LocalY;
	char NormalDn;
};
#pragma pack(pop)

#endif

#define ShowNumber(Col,Variable) ImGui::TableSetColumnIndex(Col);ImGui::Text(std::to_string(Variable).c_str())
#define ShowText(Col,Variable) ImGui::TableSetColumnIndex(Col);ImGui::Text(Variable)
#if SHN2k16
#pragma pack(push,1)
struct MapInfo : SHNRow
{

	short ID;
	char MapName[12];
	char Name[32];
	int IsWMLink;
	unsigned int RegenX;
	unsigned int RegenY;
	char KingdomMap;
	char MapFolderName[12];
	char InSide;
	unsigned int Sight;

	void DrawTableRow() 
	{
		ImGui::TableNextRow(0, 30.f);
		ShowNumber(0, ID);
		ShowText(1, MapName);
		ShowText(2, Name);
		ShowNumber(3, IsWMLink);
		ShowNumber(4, RegenX);
		ShowNumber(5, RegenY);
		ShowNumber(6, KingdomMap);
		ShowText(7, MapFolderName);
		ShowNumber(8, InSide);
		ShowNumber(9, Sight);
		
		ImGui::TableSetColumnIndex(10);
	}
};
enum WeaponTypeEnum : int
{
	WT_NONE = 0x0,
	WT_SWORD = 0x1,
	WT_BOW = 0x2,
	WT_STAFF = 0x3,
	WT_AXE = 0x4,
	WT_MACE = 0x5,
	WT_SPIKE = 0x6,
	WT_FIST = 0x7,
	WT_BODY = 0x8,
	WT_STONE = 0x9,
	WT_CROSSBOW = 0xA,
	WT_WAND = 0xB,
	WT_SPEAR = 0xC,
	WT_HAMMER = 0xD,
	WT_SPECIAL = 0xE,
	WT_PRODUCTIONTOOL = 0xF,
	WT_INVINCIBLEHAMMER = 0x10,
	WT_DSWORD = 0x11,
	WT_CLAW = 0x12,
	WT_BLADE = 0x13,
	WT_RANGE_PY = 0x14,
	WT_TSWORD = 0x15,
	MAX_WEAPONTYPEENUM = 0x16,
};
enum ArmorTypeEnum : int
{
	AT_NONE = 0x0,
	AT_CLOTH = 0x1,
	AT_LEATHER = 0x2,
	AT_SCALE = 0x3,
	AT_PLATE = 0x4,
	AT_BONE = 0x5,
	AT_HARDSKIN = 0x6,
	AT_WEAKSKIN = 0x7,
	AT_BARTSKIN = 0x8,
	AT_GELSKIN = 0x9,
	AT_FURSKIN = 0xA,
	AT_SPECIAL = 0xB,
	MAX_ARMORTYPEENUM = 0xC,
};
enum MobGradeType : int
{
	MGT_NORMAL = 0x0,
	MGT_CHIEF = 0x1,
	MGT_BOSS = 0x2,
	MGT_HERO = 0x3,
	MGT_ELITE = 0x4,
	MGT_NONE = 0x5,
	MAX_MOBGRADETYPE = 0x6,
};
enum MobType : int
{
	MT_HUMAN = 0x0,
	MT_MAGICLIFE = 0x1,
	MT_SPIRIT = 0x2,
	MT_BEAST = 0x3,
	MT_ELEMENTAL = 0x4,
	MT_UNDEAD = 0x5,
	MT_NPC = 0x6,
	MT_OBJECT = 0x7,
	MT_MINE = 0x8,
	MT_HERB = 0x9,
	MT_WOOD = 0xA,
	MT_NONAME = 0xB,
	MT_NOTARGET = 0xC,
	MT_NOTARGET2 = 0xD,
	MT_GLDITEM = 0xE,
	MT_FLAG = 0xF,
	MT_DEVIL = 0x10,
	MT_META = 0x11,
	MT_NODAMAGE = 0x12,
	MT_NODAMAGE2 = 0x13,
	MT_NONAMEGATE = 0x14,
	MT_BOX_HERB = 0x15,
	MT_BOX_MINE = 0x16,
	MT_GB_DICE = 0x17,
	MT_NODAMAGE3 = 0x18,
	MT_FRIEND = 0x19,
	MT_GB_SLOTMACHINE = 0x1A,
	MT_FRIENDDMGABSORB = 0x1B,
	MT_DEVILDOM = 0x1C,
	MT_NOTARGET3 = 0x1D,
	MT_META2 = 0x1E,
	MT_DWARF = 0x1F,
	MT_MACHINE = 0x20,
	MAX_MOBTYPE = 0x21,
};
struct MobInfo : SHNRow
{
	unsigned short ID;
	char InxName[32];
	char Name[32];
	unsigned int Level;
	unsigned int MaxHP;
	unsigned int WalkSpeed;
	unsigned int RunSpeed;
	unsigned char IsNPC;
	unsigned int Size;
	WeaponTypeEnum WeaponType;
	ArmorTypeEnum ArmorType;
	MobGradeType GradeType;
	MobType Type;
	unsigned char IsPlayerSide;
	unsigned int AbsoluteSize;
};
enum MobAttackType : int
{
	MAT_MELEE = 0x0,
	MAT_RANGE = 0x1,
	MAX_MOBATTACKTYPE = 0x2,
};
enum MiniMapIconType : int
{
	MMIT_NPCFuniture = 0x0,
	MMIT_NPCGuild = 0x1,
	MMIT_NPCHelp = 0x2,
	MMIT_NPCItem = 0x3,
	MMIT_NPCKingdom = 0x4,
	MMIT_NPCLink = 0x5,
	MMIT_NPCMine = 0x6,
	MMIT_NPCQuest = 0x7,
	MMIT_NPCSkill = 0x8,
	MMIT_NPCSmith = 0x9,
	MMIT_NPCSoulStone = 0xA,
	MMIT_NPCStore = 0xB,
	MMIT_NPCTempler = 0xC,
	MMIT_NPCNormal = 0xD,
	MMIT_NPCWTitle = 0xE,
	MMIT_NPCHouseZone = 0xF,
	MMIT_NPCRedFlag = 0x10,
	MMIT_NPCBlueFlag = 0x11,
	MMIT_NPCNormalFlag = 0x12,
	MMIT_NPCBlueGate = 0x13,
	MMIT_NPCRedGate = 0x14,
	MMIT_NPCNormalGate = 0x15,
	MMIT_NPCNone = 0x16,
	MMIT_NPCLinkTown = 0x17,
	MMIT_NPCIDGate = 0x18,
	MMIT_MobChief = 0x19,
	MMIT_MobDmg = 0x1A,
	MMIT_MobNormal = 0x1B,
	MMIT_MobNotDamaged = 0x1C,
	MMIT_MobAlreadyDamaged = 0x1D,
	MMIT_MobChrLocFlag = 0x1E,
	MAX_MINIMAPICONTYPE = 0x1F,
};
struct MobViewInfo : SHNRow
{
	unsigned short ID;
	char InxName[32];
	char FileName[32];
	char Texture[32];
	MobAttackType AttackType;
	char ShotEffect[32];
	char MobPortrait[32];
	unsigned int ChrMarkSize;
	MiniMapIconType MiniMapIcon;
	unsigned short NpcViewIndex;
	unsigned short BoundingBox;
	unsigned short EffectViewID;
	unsigned short SpectralGlow;
	char Group1[32];
	char Group2[32];
	char Group3[32];
	char GroupS[32];
};
enum BaseCharClass : unsigned __int32
{
	CC_FIGHT = 0x0,
	CC_CLERIC = 0x1,
	CC_ARCHER = 0x2,
	CC_MAGE = 0x3,
	CC_CHARCOPY = 0x4,
	CC_MOBCOPY = 0x5,
	CC_JOKER = 0x6,
	CC_SENTINEL = 0x7,
	MAX_BASECHARCLASS = 0x8,
};
struct NPCViewLoopDummy : SHNRow 
{
	
	unsigned __int16 TypeIndex;
	BaseCharClass Class;
	unsigned __int16 Gender;
	unsigned __int8 FaceShape;
	unsigned __int8 HairType;
	unsigned __int8 HairColor;
	unsigned int BaseActionCode;
	unsigned int PeriodActionCode;
	unsigned int ActionDelayTime;
	unsigned __int8 bUseEventAction;
	char Equ_Dummy[32 * 19];
};
struct NPCViewInfo : SHNRow
{
	unsigned __int16 TypeIndex;
	BaseCharClass Class;
	unsigned __int16 Gender;
	unsigned __int8 FaceShape;
	unsigned __int8 HairType;
	unsigned __int8 HairColor;
	unsigned int BaseActionCode;
	unsigned int PeriodActionCode;
	unsigned int ActionDelayTime;
	unsigned __int8 bUseEventAction;
	char Equ_RightHand[32];
	char Equ_LeftHand[32];
	char Equ_Body[32];
	char Equ_Leg[32];
	char Equ_Shoes[32];
	char Equ_AccBody[32];
	char Equ_AccLeg[32];
	char Equ_AccShoes[32];
	char Equ_AccMouth[32];
	char Equ_AccHeadA[32];
	char Equ_AccEye[32];
	char Equ_AccHead[32];
	char Equ_AccLeftHand[32];
	char Equ_AccRightHand[32];
	char Equ_AccBack[32];
	char Equ_AccWeast[32];
	char Equ_AccHip[32];
	char Equ_MiniMon[32];
	char Equ_MiniMon_R[32];
};
enum EquipTypeEnum : __int32
{
	ICON = 0x0,
	LINK = 0x1,
	SET = 0x2,
	MAX_EQUIPTYPEENUM = 0x3,
};
enum IVEType : __int32
{
	IVET_ALLOFF = 0x0,
	IVET_HAIROFF = 0x1,
	IVET_BA3OFF = 0x2,
	IVET_FA12OFF = 0x3,
	IVET_FRONTOFF = 0x4,
	IVET_DEFAULT = 0x5,
	IVET_3PIECE_BOOTS = 0x6,
	IVET_3PIECE_PANTS = 0x7,
	IVET_2PIECE_PANTS = 0x8,
	IVET_3PIECE_AMOR = 0x9,
	IVET_1PIECE = 0xA,
	MAX_IVETYPE = 0xB,
};
struct ItemViewInfo :SHNRow
{
	unsigned __int16 ID;
	char InxName[32];
	unsigned int IconIndex;
	char IconFile[32];
	unsigned int SubIconIndex;
	char SubIconFile[32];
	unsigned int PeriodIconIndex;
	char PeriodIconFile[32];
	unsigned int R;
	unsigned int G;
	unsigned int B;
	unsigned int SUB_R;
	unsigned int SUB_G;
	unsigned int SUB_B;
	EquipTypeEnum EquipType;
	char LinkFile[32];
	char TextureFile[32];
	unsigned int MSetNo;
	unsigned int FSetNo;
	float GrnItemSize;
	char GrnItemTex[32];
	unsigned __int16 UpEffect;
	char DropSnd[32];
	char EquSnd[32];
	char PutSnd[32];
	IVEType IVET_Index;
	char Descript[256];
};
enum ItemEquipEnum : __int32
{
	ITEMEQUIP_NONE = 0x0,
	ITEMEQUIP_HAT = 0x1,
	ITEMEQUIP_NOUSE03 = 0x2,
	ITEMEQUIP_NOUSE01 = 0x3,
	ITEMEQUIP_NOUSE02 = 0x4,
	ITEMEQUIP_FACETATTOO = 0x5,
	ITEMEQUIP_NECKLACE = 0x6,
	ITEMEQUIP_BODY = 0x7,
	ITEMEQUIP_BODYACC = 0x8,
	ITEMEQUIP_BACK = 0x9,
	ITEMEQUIP_LEFTHAND = 0xA,
	ITEMEQUIP_LEFTHANDACC = 0xB,
	ITEMEQUIP_RIGHTHAND = 0xC,
	ITEMEQUIP_RIGHTHANDACC = 0xD,
	ITEMEQUIP_BRACELET = 0xE,
	ITEMEQUIP_LEFTRING = 0xF,
	ITEMEQUIP_RIGHTRING = 0x10,
	ITEMEQUIP_COSEFF = 0x11,
	ITEMEQUIP_TAIL = 0x12,
	ITEMEQUIP_LEG = 0x13,
	ITEMEQUIP_LEGACC = 0x14,
	ITEMEQUIP_SHOES = 0x15,
	ITEMEQUIP_SHOESACC = 0x16,
	ITEMEQUIP_EARRING = 0x17,
	ITEMEQUIP_MOUTH = 0x18,
	ITEMEQUIP_MINIMON = 0x19,
	ITEMEQUIP_EYE = 0x1A,
	ITEMEQUIP_HATACC = 0x1B,
	ITEMEQUIP_MINIMON_R = 0x1C,
	ITEMEQUIP_SHIELDACC = 0x1D,
	MAX_ITEMEQUIPENUM = 0x1E,
};
enum ItemTypeEnum : __int32
{
	ITEMTYPE_EQU = 0x0,
	ITEMTYPE_EXH = 0x1,
	ITEMTYPE_ETC = 0x2,
	ITEMTYPE_QUEST = 0x3,
	ITEMTYPE_STARTQUEST = 0x4,
	ITEMTYPE_VIP = 0x5,
	ITEMTYPE_CONFIRM = 0x6,
	MAX_ITEMTYPEENUM = 0x7,
};
enum ItemClassEnum : __int32
{
	ITEMCLASS_BYTELOT = 0x0,
	ITEMCLASS_WORDLOT = 0x1,
	ITEMCLASS_DWRDLOT = 0x2,
	ITEMCLASS_QUESTITEM = 0x3,
	ITEMCLASS_AMULET = 0x4,
	ITEMCLASS_WEAPON = 0x5,
	ITEMCLASS_ARMOR = 0x6,
	ITEMCLASS_SHIELD = 0x7,
	ITEMCLASS_BOOT = 0x8,
	ITEMCLASS_FURNITURE = 0x9,
	ITEMCLASS_DECORATION = 0xA,
	ITEMCLASS_SKILLSCROLL = 0xB,
	ITEMCLASS_RECALLSCROLL = 0xC,
	ITEMCLASS_BINDITEM = 0xD,
	ITEMCLASS_UPSOURCE = 0xE,
	ITEMCLASS_ITEMCHEST = 0xF,
	ITEMCLASS_WTLICENCE = 0x10,
	ITEMCLASS_KQ = 0x11,
	ITEMCLASS_HOUSESKIN = 0x12,
	ITEMCLASS_UPRED = 0x13,
	ITEMCLASS_UPBLUE = 0x14,
	ITEMCLASS_KQSTEP = 0x15,
	ITEMCLASS_FEED = 0x16,
	ITEMCLASS_RIDING = 0x17,
	ITEMCLASS_AMOUNT = 0x18,
	ITEMCLASS_UPGOLD = 0x19,
	ITEMCLASS_COSWEAPON = 0x1A,
	ITEMCLASS_ACTIONITEM = 0x1B,
	ITEMCLASS_GBCOIN = 0x1C,
	ITEMCLASS_CAPSULE = 0x1D,
	ITEMCLASS_CLOSEDCARD = 0x1E,
	ITEMCLASS_OPENCARD = 0x1F,
	ITEMCLASS_MONEY = 0x20,
	ITEMCLASS_NOEFFECT = 0x21,
	ITEMCLASS_ENCHANT = 0x22,
	ITEMCLASS_ACTIVESKILL = 0x23,
	ITEMCLASS_PUP = 0x24,
	ITEMCLASS_COSSHIELD = 0x25,
	ITEMCLASS_BRACELET = 0x26,
	MAX_ITEMCLASSENUM = 0x27,
};
enum AuctionGroup : __int32
{
	AG_ALL = 0x0,
	AG_M_WEAPON = 0x1,
	AG_M_ARMOR = 0x2,
	AG_M_ACCESSORY = 0x3,
	AG_M_PRODUCE = 0x4,
	AG_M_ENCHANT = 0x5,
	AG_M_RAW = 0x6,
	AG_M_ETC = 0x7,
	AG_S_ONEHANDSWORD = 0x8,
	AG_S_TWOHANDSWORD = 0x9,
	AG_S_AXE = 0xA,
	AG_S_MACE = 0xB,
	AG_S_HAMMER = 0xC,
	AG_S_BOW = 0xD,
	AG_S_CBOW = 0xE,
	AG_S_STAFF = 0xF,
	AG_S_WAND = 0x10,
	AG_S_CLAW = 0x11,
	AG_S_DSWORD = 0x12,
	AG_S_FIGHTER = 0x13,
	AG_S_CLERIC = 0x14,
	AG_S_ARCHER = 0x15,
	AG_S_MAGE = 0x16,
	AG_S_JOKER = 0x17,
	AG_S_NECK = 0x18,
	AG_S_EARRING = 0x19,
	AG_S_RING = 0x1A,
	AG_S_SCROLL = 0x1B,
	AG_S_POTION = 0x1C,
	AG_S_STONE = 0x1D,
	AG_S_FOOD = 0x1E,
	AG_S_ENCHANT = 0x1F,
	AG_S_PRODRAW = 0x20,
	AG_S_FARM = 0x21,
	AG_S_MOVER = 0x22,
	AG_S_MINIHOUSE = 0x23,
	AG_S_COSTUME = 0x24,
	AG_S_ABILLITY = 0x25,
	AG_S_EMOTION = 0x26,
	AG_S_ETC = 0x27,
	AG_S_BLADE = 0x28,
	AG_S_SENTINEL = 0x29,
	AG_S_BRACELET = 0x2A,
	MAX_AUCTIONGROUP = 0x2B,
};
enum GradeType : __int32
{
	GT_NORMAL = 0x0,
	GT_NAMED = 0x1,
	GT_RARE = 0x2,
	GT_UNIQUE = 0x3,
	GT_CHARGE = 0x4,
	GT_SET = 0x5,
	GT_LEGENDARY = 0x6,
	GT_MYTHIC = 0x7,
	MAX_GRADETYPE = 0x8,
};
enum UseClassType : __int32
{
	UCT_NONE = 0x0,
	UCT_ALL = 0x1,
	UCT_FIGHTER_ALL = 0x2,
	UCT_CLEVERFIGHTER_AND_OVER = 0x3,
	UCT_WARRIOR_AND_OVER = 0x4,
	UCT_WARRIOR_OVER = 0x5,
	UCT_GLADIATOR_ONLY = 0x6,
	UCT_KNIGHT_ONLY = 0x7,
	UCT_CLERIC_ALL = 0x8,
	UCT_HIGHCLERIC_AND_OVER = 0x9,
	UCT_PALADIN_AND_OVER = 0xA,
	UCT_GUARDIAN_ONLY = 0xB,
	UCT_HOLYKNIGHT_ONLY = 0xC,
	UCT_PALADIN_OVER = 0xD,
	UCT_ARCHER_ALL = 0xE,
	UCT_HAWKARCHER_AND_OVER = 0xF,
	UCT_SCOUT_AND_OVER = 0x10,
	UCT_RANGER_ONLY = 0x11,
	UCT_SHARPSHOOTER_ONLY = 0x12,
	UCT_SCOUT_OVER = 0x13,
	UCT_MAGE_ALL = 0x14,
	UCT_WIZMAGE_AND_OVER = 0x15,
	UCT_ENCHANTER_AND_OVER = 0x16,
	UCT_WIZARD_ONLY = 0x17,
	UCT_WARLOCK_ONLY = 0x18,
	UCT_ENCHANTER_OVER = 0x19,
	UCT_SENTINEL_EXCLUDE = 0x1A,
	UCT_JOKER_ALL = 0x1B,
	UCT_CHASER_AND_OVER = 0x1C,
	UCT_CRUEL_AND_OVER = 0x1D,
	UCT_ASSASSIN_ONLY = 0x1E,
	UCT_CLOSER_ONLY = 0x1F,
	UCT_CRUEL_OVER = 0x20,
	UCT_SENTINEL_ALL = 0x21,
	UCT_SAVIOR_ONLY = 0x22,
	UCT_DEEPER_SKILL = 0x23,
	UCT_SHIELD = 0x24,
	UCT_CLASS_CHANGE = 0x25,
	UCT_SHIELD_NOT_GLA = 0x26,
	MAX_USECLASSTYPE = 0x27,
};
enum E_BelongType : __int32
{
	BT_COMMON = 0x0,
	BT_NO_SELL = 0x1,
	BT_NO_DROP = 0x2,
	BT_NO_SELL_DROP = 0x3,
	BT_ACC = 0x4,
	BT_CHR = 0x5,
	BT_ONLY_DEL = 0x6,
	BT_NO_DEL = 0x7,
	BT_PUTON_ACC = 0x8,
	BT_PUTON_CHR = 0x9,
	BT_NO_STORAGE = 0xA,
	MAX_E_BELONGTYPE = 0xB,
};
enum ItemFuncEnum : __int32
{
	ITEMFUNC_NONE = 0x0,
	ITEMFUNC_ENDUREKIT_WC = 0x1,
	ITEMFUNC_ENDUREKIT_F = 0x2,
	ITEMFUNC_JUSTREVIVAL = 0x3,
	ITEMFUNC_CHANGE_NAME = 0x4,
	ITEMFUNC_CHANGE_RELATION = 0x5,
	ITEMFUNC_PUTON_CLEAR = 0x6,
	MAX_ITEMFUNCENUM = 0x7,
};

struct ItemInfo :SHNRow
{
	unsigned __int16 ID;
	char InxName[32];
	char Name[64];
	ItemTypeEnum Type;
	ItemClassEnum Class;
	unsigned int MaxLot;
	ItemEquipEnum Equip;
	AuctionGroup ItemAuctionGroup;
	GradeType ItemGradeType;
	unsigned __int8 TwoHand;
	unsigned int AtkSpeed;
	unsigned int DemandLv;
	unsigned int Grade;
	unsigned int MinWC;
	unsigned int MaxWC;
	unsigned int AC;
	unsigned int MinMA;
	unsigned int MaxMA;
	unsigned int MR;
	unsigned int TH;
	unsigned int TB;
	unsigned int WCRate;
	unsigned int MARate;
	unsigned int ACRate;
	unsigned int MRRate;
	unsigned int CriRate;
	unsigned int CriMinWc;
	unsigned int CriMaxWc;
	unsigned int CriMinMa;
	unsigned int CriMaxMa;
	unsigned int CrlTB;
	UseClassType UseClass;
	unsigned int BuyPrice;
	unsigned int SellPrice;
	unsigned __int8 BuyDemandLv;
	unsigned int BuyFame;
	unsigned int BuyGToken;
	unsigned int BuyGBCoin;
	WeaponTypeEnum WeaponType;
	ArmorTypeEnum ArmorType;
	unsigned __int8 UpLimit;
	unsigned __int16 BasicUpInx;
	unsigned __int16 UpSucRatio;
	unsigned __int16 UpLuckRatio;
	unsigned __int8 UpResource;
	unsigned __int16 AddUpInx;
	unsigned int ShieldAC;
	unsigned int HitRatePlus;
	unsigned int EvaRatePlus;
	unsigned int MACriPlus;
	unsigned int CriDamPlus;
	unsigned int MagCriDamPlus;
	E_BelongType BT_Inx;
	char TitleName[32];
	char ItemUseSkill[32];
	char SetItemIndex[32];
	ItemFuncEnum ItemFunc;
};
enum StateType : __int32
{
	Normal = 0x0,
	Skill = 0x1,
	Scroll = 0x2,
	Potion = 0x3,
	Debuff = 0x4,
	MAX_STATETYPE = 0x5,
};
struct AbnormalStateViewInfo : SHNRow
{
	enum EffPosEnum : __int32
	{
		None = 0x0,
		Ground = 0x1,
		Chest = 0x2,
		HairLink = 0x3,
		OP_Ground = 0x4,
		OP_Chest = 0x5,
		Back = 0x6,
		TopLink = 0x7,
		OP_TopLink = 0x8,
		MAX_EFFPOSENUM = 0x9,
	};
	unsigned __int16 ID;
	char inxName[32];
	unsigned int icon;
	char iconFile[32];
	char Descript[256];
	unsigned __int8 R;
	unsigned __int8 G;
	unsigned __int8 B;
	char AniIndex[32];
	char effName[32];
	EffPosEnum EffNamePos;
	unsigned __int8 EffRefresh;
	char LoopEffect[32];
	EffPosEnum LoopEffPos;
	char LastEffect[32];
	EffPosEnum LastEffectPos;
	char DOTEffect[32];
	EffPosEnum DOTEffectPos;
	char IconSort[16];
	StateType TypeSort;
	unsigned __int8 View;
};
typedef AbnormalStateViewInfo::EffPosEnum DummyTypeEnum;
struct ItemViewDummy : SHNRow
{
	char InxName[32];
	unsigned __int8 IsMale;
	DummyTypeEnum DummyType;
	char Nif[32];
	char ArmorTexture[32];
}; 
enum EquipType : __int32
{
	ET_NONE = 0x0,
	ET_MALE = 0x1,
	ET_FEMALE = 0x2,
	ET_BOTH = 0x3,
	MAX_EQUIPTYPE = 0x4,
};
enum HairDummyType : __int32
{
	HAIR_NONE = 0x0,
	HAIR_FRONT = 0x1,
	HAIR_BOTTOM = 0x2,
	HAIR_TOP = 0x3,
	HAIR_ACC = 0x4,
	HAIR_ACC2 = 0x5,
	HAIR_ACC3 = 0x6,
	MAX_HAIRDUMMYTYPE = 0x7,
};
struct HairInfo : SHNRow
{
	unsigned __int8 ID;
	char IndexName[32];
	char HairName[32];
	unsigned __int8 Grade;
	EquipType fighter;
	EquipType archer;
	EquipType cleric;
	EquipType mage;
	EquipType Joker;
	EquipType Sentinel;
	unsigned __int8 ucIsLink_Front;
	char acModelName_Front[32];
	char FrontTex[32];
	unsigned __int8 ucIsLink_Bottom;
	char acModelName_Bottom[32];
	char BottomTex[32];
	unsigned __int8 ucIsLink_Top;
	char acModelName_Top[32];
	char TopTex[32];
	HairDummyType Exception1;
	HairDummyType Exception2;
	unsigned __int8 ucIsLink_Acc;
	char acModelName_Acc[32];
	char Acc1Tex[32];
	unsigned __int8 ucIsLink_Acc2;
	char acModelName_Acc2[32];
	char Acc2Tex[32];
	unsigned __int8 ucIsLink_Acc3;
	char acModelName_Acc3[32];
	char Acc3Tex[32];
};
#pragma pack(pop)
#endif