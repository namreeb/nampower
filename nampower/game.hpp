/*
    Copyright (c) 2017-2023, namreeb (legal@namreeb.org)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    The views and conclusions contained in the software and documentation are those
    of the authors and should not be interpreted as representing official policies,
    either expressed or implied, of the FreeBSD Project.
*/

#pragma once

#include <cstdint>

namespace game
{
#pragma pack(push, 1)
struct SpellRec
{
    unsigned int Id;
    unsigned int School;
    unsigned int Category;
    unsigned int castUI;
    unsigned int Dispel;
    unsigned int Mechanic;
    unsigned int Attributes;
    unsigned int AttributesEx;
    unsigned int AttributesEx2;
    unsigned int AttributesEx3;
    unsigned int AttributesEx4;
    unsigned int Stances;
    unsigned int StancesNot;
    unsigned int Targets;
    unsigned int TargetCreatureType;
    unsigned int RequiresSpellFocus;
    unsigned int CasterAuraState;
    unsigned int TargetAuraState;
    unsigned int CastingTimeIndex;
    unsigned int RecoveryTime;
    unsigned int CategoryRecoveryTime;
    unsigned int InterruptFlags;
    unsigned int AuraInterruptFlags;
    unsigned int ChannelInterruptFlags;
    unsigned int procFlags;
    unsigned int procChance;
    unsigned int procCharges;
    unsigned int maxLevel;
    unsigned int baseLevel;
    unsigned int spellLevel;
    unsigned int DurationIndex;
    unsigned int powerType;
    unsigned int manaCost;
    unsigned int manaCostPerlevel;
    unsigned int manaPerSecond;
    unsigned int manaPerSecondPerLevel;
    unsigned int rangeIndex;
    float speed;
    unsigned int modalNextSpell;
    unsigned int StackAmount;
    unsigned int Totem[2];
    int Reagent[8];
    unsigned int ReagentCount[8];
    int EquippedItemClass;
    int EquippedItemSubClassMask;
    int EquippedItemInventoryTypeMask;
    unsigned int Effect[3];
    int EffectDieSides[3];
    unsigned int EffectBaseDice[3];
    float EffectDicePerLevel[3];
    float EffectRealPointsPerLevel[3];
    int EffectBasePoints[3];
    unsigned int EffectMechanic[3];
    unsigned int EffectImplicitTargetA[3];
    unsigned int EffectImplicitTargetB[3];
    unsigned int EffectRadiusIndex[3];
    unsigned int EffectApplyAuraName[3];
    unsigned int EffectAmplitude[3];
    float EffectMultipleValue[3];
    unsigned int EffectChainTarget[3];
    unsigned int EffectItemType[3];
    int EffectMiscValue[3];
    unsigned int EffectTriggerSpell[3];
    float EffectPointsPerComboPoint[3];
    unsigned int SpellVisual;
    unsigned int SpellVisual2;
    unsigned int SpellIconID;
    unsigned int activeIconID;
    unsigned int spellPriority;
    const char *SpellName[8];
    unsigned int SpellNameFlag;
    unsigned int Rank[8];
    unsigned int RankFlags;
    unsigned int Description[8];
    unsigned int DescriptionFlags;
    unsigned int ToolTip[8];
    unsigned int ToolTipFlags;
    unsigned int ManaCostPercentage;
    unsigned int StartRecoveryCategory;
    unsigned int StartRecoveryTime;
    unsigned int MaxTargetLevel;
    unsigned int SpellFamilyName;
    unsigned __int64 SpellFamilyFlags;
    unsigned int MaxAffectedTargets;
    unsigned int DmgClass;
    unsigned int PreventionType;
    unsigned int StanceBarOrder;
    float DmgMultiplier[3];
    unsigned int MinFactionId;
    unsigned int MinReputation;
    unsigned int RequiredAuraVision;
};

template <typename T>
struct WowClientDB
{
    T **m_recordsById;
    int m_maxID;
    int Unk3;
    int Unk4;
    int Unk5;
};

struct SpellCast
{
    unsigned __int64 casterUnit;
    unsigned __int64 caster;
    int spellId;
    unsigned __int16 targets;
    unsigned __int16 unk;
    unsigned __int64 unitTarget;
    unsigned __int64 itemTarget;
    unsigned int Fields2[18];
    char targetString[128];
};
#pragma pack(pop)

enum SpellEffects
{
    SPELL_EFFECT_NONE = 0x0,
    SPELL_EFFECT_INSTAKILL = 0x1,
    SPELL_EFFECT_SCHOOL_DAMAGE = 0x2,
    SPELL_EFFECT_DUMMY = 0x3,
    SPELL_EFFECT_PORTAL_TELEPORT = 0x4,
    SPELL_EFFECT_TELEPORT_UNITS = 0x5,
    SPELL_EFFECT_APPLY_AURA = 0x6,
    SPELL_EFFECT_ENVIRONMENTAL_DAMAGE = 0x7,
    SPELL_EFFECT_POWER_DRAIN = 0x8,
    SPELL_EFFECT_HEALTH_LEECH = 0x9,
    SPELL_EFFECT_HEAL = 0xA,
    SPELL_EFFECT_BIND = 0xB,
    SPELL_EFFECT_PORTAL = 0xC,
    SPELL_EFFECT_RITUAL_BASE = 0xD,
    SPELL_EFFECT_RITUAL_SPECIALIZE = 0xE,
    SPELL_EFFECT_RITUAL_ACTIVATE_PORTAL = 0xF,
    SPELL_EFFECT_QUEST_COMPLETE = 0x10,
    SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL = 0x11,
    SPELL_EFFECT_RESURRECT = 0x12,
    SPELL_EFFECT_ADD_EXTRA_ATTACKS = 0x13,
    SPELL_EFFECT_DODGE = 0x14,
    SPELL_EFFECT_EVADE = 0x15,
    SPELL_EFFECT_PARRY = 0x16,
    SPELL_EFFECT_BLOCK = 0x17,
    SPELL_EFFECT_CREATE_ITEM = 0x18,
    SPELL_EFFECT_WEAPON = 0x19,
    SPELL_EFFECT_DEFENSE = 0x1A,
    SPELL_EFFECT_PERSISTENT_AREA_AURA = 0x1B,
    SPELL_EFFECT_SUMMON = 0x1C,
    SPELL_EFFECT_LEAP = 0x1D,
    SPELL_EFFECT_ENERGIZE = 0x1E,
    SPELL_EFFECT_WEAPON_PERCENT_DAMAGE = 0x1F,
    SPELL_EFFECT_TRIGGER_MISSILE = 0x20,
    SPELL_EFFECT_OPEN_LOCK = 0x21,
    SPELL_EFFECT_SUMMON_CHANGE_ITEM = 0x22,
    SPELL_EFFECT_APPLY_AREA_AURA_PARTY = 0x23,
    SPELL_EFFECT_LEARN_SPELL = 0x24,
    SPELL_EFFECT_SPELL_DEFENSE = 0x25,
    SPELL_EFFECT_DISPEL = 0x26,
    SPELL_EFFECT_LANGUAGE = 0x27,
    SPELL_EFFECT_DUAL_WIELD = 0x28,
    SPELL_EFFECT_SUMMON_WILD = 0x29,
    SPELL_EFFECT_SUMMON_GUARDIAN = 0x2A,
    SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER = 0x2B,
    SPELL_EFFECT_SKILL_STEP = 0x2C,
    SPELL_EFFECT_ADD_HONOR = 0x2D,
    SPELL_EFFECT_SPAWN = 0x2E,
    SPELL_EFFECT_TRADE_SKILL = 0x2F,
    SPELL_EFFECT_STEALTH = 0x30,
    SPELL_EFFECT_DETECT = 0x31,
    SPELL_EFFECT_TRANS_DOOR = 0x32,
    SPELL_EFFECT_FORCE_CRITICAL_HIT = 0x33,
    SPELL_EFFECT_GUARANTEE_HIT = 0x34,
    SPELL_EFFECT_ENCHANT_ITEM = 0x35,
    SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY = 0x36,
    SPELL_EFFECT_TAMECREATURE = 0x37,
    SPELL_EFFECT_SUMMON_PET = 0x38,
    SPELL_EFFECT_LEARN_PET_SPELL = 0x39,
    SPELL_EFFECT_WEAPON_DAMAGE = 0x3A,
    SPELL_EFFECT_OPEN_LOCK_ITEM = 0x3B,
    SPELL_EFFECT_PROFICIENCY = 0x3C,
    SPELL_EFFECT_SEND_EVENT = 0x3D,
    SPELL_EFFECT_POWER_BURN = 0x3E,
    SPELL_EFFECT_THREAT = 0x3F,
    SPELL_EFFECT_TRIGGER_SPELL = 0x40,
    SPELL_EFFECT_HEALTH_FUNNEL = 0x41,
    SPELL_EFFECT_POWER_FUNNEL = 0x42,
    SPELL_EFFECT_HEAL_MAX_HEALTH = 0x43,
    SPELL_EFFECT_INTERRUPT_CAST = 0x44,
    SPELL_EFFECT_DISTRACT = 0x45,
    SPELL_EFFECT_PULL = 0x46,
    SPELL_EFFECT_PICKPOCKET = 0x47,
    SPELL_EFFECT_ADD_FARSIGHT = 0x48,
    SPELL_EFFECT_SUMMON_POSSESSED = 0x49,
    SPELL_EFFECT_SUMMON_TOTEM = 0x4A,
    SPELL_EFFECT_HEAL_MECHANICAL = 0x4B,
    SPELL_EFFECT_SUMMON_OBJECT_WILD = 0x4C,
    SPELL_EFFECT_SCRIPT_EFFECT = 0x4D,
    SPELL_EFFECT_ATTACK = 0x4E,
    SPELL_EFFECT_SANCTUARY = 0x4F,
    SPELL_EFFECT_ADD_COMBO_POINTS = 0x50,
    SPELL_EFFECT_CREATE_HOUSE = 0x51,
    SPELL_EFFECT_BIND_SIGHT = 0x52,
    SPELL_EFFECT_DUEL = 0x53,
    SPELL_EFFECT_STUCK = 0x54,
    SPELL_EFFECT_SUMMON_PLAYER = 0x55,
    SPELL_EFFECT_ACTIVATE_OBJECT = 0x56,
    SPELL_EFFECT_SUMMON_TOTEM_SLOT1 = 0x57,
    SPELL_EFFECT_SUMMON_TOTEM_SLOT2 = 0x58,
    SPELL_EFFECT_SUMMON_TOTEM_SLOT3 = 0x59,
    SPELL_EFFECT_SUMMON_TOTEM_SLOT4 = 0x5A,
    SPELL_EFFECT_THREAT_ALL = 0x5B,
    SPELL_EFFECT_ENCHANT_HELD_ITEM = 0x5C,
    SPELL_EFFECT_SUMMON_PHANTASM = 0x5D,
    SPELL_EFFECT_SELF_RESURRECT = 0x5E,
    SPELL_EFFECT_SKINNING = 0x5F,
    SPELL_EFFECT_CHARGE = 0x60,
    SPELL_EFFECT_SUMMON_CRITTER = 0x61,
    SPELL_EFFECT_KNOCK_BACK = 0x62,
    SPELL_EFFECT_DISENCHANT = 0x63,
    SPELL_EFFECT_INEBRIATE = 0x64,
    SPELL_EFFECT_FEED_PET = 0x65,
    SPELL_EFFECT_DISMISS_PET = 0x66,
    SPELL_EFFECT_REPUTATION = 0x67,
    SPELL_EFFECT_SUMMON_OBJECT_SLOT1 = 0x68,
    SPELL_EFFECT_SUMMON_OBJECT_SLOT2 = 0x69,
    SPELL_EFFECT_SUMMON_OBJECT_SLOT3 = 0x6A,
    SPELL_EFFECT_SUMMON_OBJECT_SLOT4 = 0x6B,
    SPELL_EFFECT_DISPEL_MECHANIC = 0x6C,
    SPELL_EFFECT_SUMMON_DEAD_PET = 0x6D,
    SPELL_EFFECT_DESTROY_ALL_TOTEMS = 0x6E,
    SPELL_EFFECT_DURABILITY_DAMAGE = 0x6F,
    SPELL_EFFECT_SUMMON_DEMON = 0x70,
    SPELL_EFFECT_RESURRECT_NEW = 0x71,
    SPELL_EFFECT_ATTACK_ME = 0x72,
    SPELL_EFFECT_DURABILITY_DAMAGE_PCT = 0x73,
    SPELL_EFFECT_SKIN_PLAYER_CORPSE = 0x74,
    SPELL_EFFECT_SPIRIT_HEAL = 0x75,
    SPELL_EFFECT_SKILL = 0x76,
    SPELL_EFFECT_APPLY_AREA_AURA_PET = 0x77,
    SPELL_EFFECT_TELEPORT_GRAVEYARD = 0x78,
    SPELL_EFFECT_NORMALIZED_WEAPON_DMG = 0x79,
    SPELL_EFFECT_122 = 0x7A,
    SPELL_EFFECT_SEND_TAXI = 0x7B,
    SPELL_EFFECT_PLAYER_PULL = 0x7C,
    SPELL_EFFECT_MODIFY_THREAT_PERCENT = 0x7D,
    SPELL_EFFECT_126 = 0x7E,
    SPELL_EFFECT_127 = 0x7F,
    SPELL_EFFECT_APPLY_AREA_AURA_FRIEND = 0x80,
    SPELL_EFFECT_APPLY_AREA_AURA_ENEMY = 0x81,
    TOTAL_SPELL_EFFECTS = 0x82,
};

enum SpellFailedReason
{
    SPELL_FAILED_ERROR = 0x1C,
};

enum SpellAttributes
{
    SPELL_ATTR_UNK0 = 0x1,
    SPELL_ATTR_RANGED = 0x2,
    SPELL_ATTR_ON_NEXT_SWING_1 = 0x4,
    SPELL_ATTR_UNK3 = 0x8,
    SPELL_ATTR_ABILITY = 0x10,
    SPELL_ATTR_TRADESPELL = 0x20,
    SPELL_ATTR_PASSIVE = 0x40,
    SPELL_ATTR_HIDDEN_CLIENTSIDE = 0x80,
    SPELL_ATTR_HIDE_IN_COMBAT_LOG = 0x100,
    SPELL_ATTR_TARGET_MAINHAND_ITEM = 0x200,
    SPELL_ATTR_ON_NEXT_SWING_2 = 0x400,
    SPELL_ATTR_UNK11 = 0x800,
    SPELL_ATTR_DAYTIME_ONLY = 0x1000,
    SPELL_ATTR_NIGHT_ONLY = 0x2000,
    SPELL_ATTR_INDOORS_ONLY = 0x4000,
    SPELL_ATTR_OUTDOORS_ONLY = 0x8000,
    SPELL_ATTR_NOT_SHAPESHIFT = 0x10000,
    SPELL_ATTR_ONLY_STEALTHED = 0x20000,
    SPELL_ATTR_DONT_AFFECT_SHEATH_STATE = 0x40000,
    SPELL_ATTR_LEVEL_DAMAGE_CALCULATION = 0x80000,
    SPELL_ATTR_STOP_ATTACK_TARGET = 0x100000,
    SPELL_ATTR_IMPOSSIBLE_DODGE_PARRY_BLOCK = 0x200000,
    SPELL_ATTR_SET_TRACKING_TARGET = 0x400000,
    SPELL_ATTR_CASTABLE_WHILE_DEAD = 0x800000,
    SPELL_ATTR_CASTABLE_WHILE_MOUNTED = 0x1000000,
    SPELL_ATTR_DISABLED_WHILE_ACTIVE = 0x2000000,
    SPELL_ATTR_NEGATIVE = 0x4000000,
    SPELL_ATTR_CASTABLE_WHILE_SITTING = 0x8000000,
    SPELL_ATTR_CANT_USED_IN_COMBAT = 0x10000000,
    SPELL_ATTR_UNAFFECTED_BY_INVULNERABILITY = 0x20000000,
    SPELL_ATTR_UNK30 = 0x40000000,
    SPELL_ATTR_CANT_CANCEL = 0x80000000,
};

enum SpellAttributesEx3
{
    SPELL_ATTR_EX3_UNK0 = 0x1,
    SPELL_ATTR_EX3_UNK1 = 0x2,
    SPELL_ATTR_EX3_HIDE_NAME = 0x4,
    SPELL_ATTR_EX3_BLOCKABLE_SPELL = 0x8,
    SPELL_ATTR_EX3_IGNORE_RESURRECTION_TIMER = 0x10,
    SPELL_ATTR_EX3_UNK5 = 0x20,
    SPELL_ATTR_EX3_UNK6 = 0x40,
    SPELL_ATTR_EX3_STACK_FOR_DIFF_CASTERS = 0x80,
    SPELL_ATTR_EX3_TARGET_ONLY_PLAYER = 0x100,
    SPELL_ATTR_EX3_UNK9 = 0x200,
    SPELL_ATTR_EX3_MAIN_HAND = 0x400,
    SPELL_ATTR_EX3_BATTLEGROUND = 0x800,
    SPELL_ATTR_EX3_CAST_ON_DEAD = 0x1000,
    SPELL_ATTR_EX3_DONT_DISPLAY_CHANNEL_BAR = 0x2000,
    SPELL_ATTR_EX3_IS_HONORLESS_TARGET = 0x4000,
    SPELL_ATTR_EX3_UNK15 = 0x8000,
    SPELL_ATTR_EX3_UNK16 = 0x10000,
    SPELL_ATTR_EX3_NO_INITIAL_AGGRO = 0x20000,
    SPELL_ATTR_EX3_CANT_MISS = 0x40000,
    SPELL_ATTR_EX3_UNK19 = 0x80000,
    SPELL_ATTR_EX3_DEATH_PERSISTENT = 0x100000,
    SPELL_ATTR_EX3_UNK21 = 0x200000,
    SPELL_ATTR_EX3_REQ_WAND = 0x400000,
    SPELL_ATTR_EX3_UNK23 = 0x800000,
    SPELL_ATTR_EX3_REQ_OFFHAND = 0x1000000,
    SPELL_ATTR_EX3_UNK25 = 0x2000000,
    SPELL_ATTR_EX3_UNK26 = 0x4000000,
    SPELL_ATTR_EX3_DRAIN_SOUL = 0x8000000,
    SPELL_ATTR_EX3_UNK28 = 0x10000000,
    SPELL_ATTR_EX3_UNK29 = 0x20000000,
    SPELL_ATTR_EX3_DONT_DISPLAY_RANGE = 0x40000000,
    SPELL_ATTR_EX3_UNK31 = 0x80000000,
};

enum Events : std::uint32_t
{
    UNIT_NAME_UPDATE = 0xB7,
    UNIT_PORTRAIT_UPDATE = 0xB8,
    UNIT_MODEL_CHANGED = 0xB9,
    UNIT_INVENTORY_CHANGED = 0xBA,
    UNIT_CLASSIFICATION_CHANGED = 0xBB,
    ITEM_LOCK_CHANGED = 0xBC,
    PLAYER_XP_UPDATE = 0xBD,
    PLAYER_REGEN_DISABLED = 0xBE,
    PLAYER_REGEN_ENABLED = 0xBF,
    PLAYER_AURAS_CHANGED = 0xC0,
    PLAYER_ENTER_COMBAT = 0xC1,
    PLAYER_LEAVE_COMBAT = 0xC2,
    PLAYER_TARGET_CHANGED = 0xC3,
    PLAYER_CONTROL_LOST = 0xC4,
    PLAYER_CONTROL_GAINED = 0xC5,
    PLAYER_FARSIGHT_FOCUS_CHANGED = 0xC6,
    PLAYER_LEVEL_UP = 0xC7,
    PLAYER_MONEY = 0xC8,
    PLAYER_DAMAGE_DONE_MODS = 0xC9,
    PLAYER_COMBO_POINTS = 0xCA,
    ZONE_CHANGED = 0xCB,
    ZONE_CHANGED_INDOORS = 0xCC,
    ZONE_CHANGED_NEW_AREA = 0xCD,
    MINIMAP_ZONE_CHANGED = 0xCE,
    MINIMAP_UPDATE_ZOOM = 0xCF,
    SCREENSHOT_SUCCEEDED = 0xD0,
    SCREENSHOT_FAILED = 0xD1,
    ACTIONBAR_SHOWGRID = 0xD2,
    ACTIONBAR_HIDEGRID = 0xD3,
    ACTIONBAR_PAGE_CHANGED = 0xD4,
    ACTIONBAR_SLOT_CHANGED = 0xD5,
    ACTIONBAR_UPDATE_STATE = 0xD6,
    ACTIONBAR_UPDATE_USABLE = 0xD7,
    ACTIONBAR_UPDATE_COOLDOWN = 0xD8,
    UPDATE_BONUS_ACTIONBAR = 0xD9,
    PARTY_MEMBERS_CHANGED = 0xDA,
    PARTY_LEADER_CHANGED = 0xDB,
    PARTY_MEMBER_ENABLE = 0xDC,
    PARTY_MEMBER_DISABLE = 0xDD,
    PARTY_LOOT_METHOD_CHANGED = 0xDE,
    SYSMSG = 0xDF,
    UI_ERROR_MESSAGE = 0xE0,
    UI_INFO_MESSAGE = 0xE1,
    UPDATE_CHAT_COLOR = 0xE2,
    CHAT_MSG_ADDON = 0xE3,
    CHAT_MSG_SAY = 0xE4,
    CHAT_MSG_PARTY = 0xE5,
    CHAT_MSG_RAID = 0xE6,
    CHAT_MSG_GUILD = 0xE7,
    CHAT_MSG_OFFICER = 0xE8,
    CHAT_MSG_YELL = 0xE9,
    CHAT_MSG_WHISPER = 0xEA,
    CHAT_MSG_WHISPER_INFORM = 0xEB,
    CHAT_MSG_EMOTE = 0xEC,
    CHAT_MSG_TEXT_EMOTE = 0xED,
    CHAT_MSG_SYSTEM = 0xEE,
    CHAT_MSG_MONSTER_SAY = 0xEF,
    CHAT_MSG_MONSTER_YELL = 0xF0,
    CHAT_MSG_MONSTER_WHISPER = 0xF1,
    CHAT_MSG_MONSTER_EMOTE = 0xF2,
    CHAT_MSG_CHANNEL = 0xF3,
    CHAT_MSG_CHANNEL_JOIN = 0xF4,
    CHAT_MSG_CHANNEL_LEAVE = 0xF5,
    CHAT_MSG_CHANNEL_LIST = 0xF6,
    CHAT_MSG_CHANNEL_NOTICE = 0xF7,
    CHAT_MSG_CHANNEL_NOTICE_USER = 0xF8,
    CHAT_MSG_AFK = 0xF9,
    CHAT_MSG_DND = 0xFA,
    CHAT_MSG_COMBAT_LOG = 0xFB,
    CHAT_MSG_IGNORED = 0xFC,
    CHAT_MSG_SKILL = 0xFD,
    CHAT_MSG_LOOT = 0xFE,
    CHAT_MSG_MONEY = 0xFF,
    CHAT_MSG_RAID_LEADER = 0x100,
    CHAT_MSG_RAID_WARNING = 0x101,
    LANGUAGE_LIST_CHANGED = 0x102,
    TIME_PLAYED_MSG = 0x103,
    SPELLS_CHANGED = 0x104,
    CURRENT_SPELL_CAST_CHANGED = 0x105,
    SPELL_UPDATE_COOLDOWN = 0x106,
    SPELL_UPDATE_USABLE = 0x107,
    CHARACTER_POINTS_CHANGED = 0x108,
    SKILL_LINES_CHANGED = 0x109,
    ITEM_PUSH = 0x10A,
    LOOT_OPENED = 0x10B,
    LOOT_SLOT_CLEARED = 0x10C,
    LOOT_CLOSED = 0x10D,
    PLAYER_LOGIN = 0x10E,
    PLAYER_LOGOUT = 0x10F,
    PLAYER_ENTERING_WORLD = 0x110,
    PLAYER_LEAVING_WORLD = 0x111,
    PLAYER_ALIVE = 0x112,
    PLAYER_DEAD = 0x113,
    PLAYER_CAMPING = 0x114,
    PLAYER_QUITING = 0x115,
    LOGOUT_CANCEL = 0x116,
    PARTY_INVITE_REQUEST = 0x118,
    PARTY_INVITE_CANCEL = 0x119,
    GUILD_INVITE_REQUEST = 0x11A,
    GUILD_INVITE_CANCEL = 0x11B,
    GUILD_MOTD = 0x11C,
    TRADE_REQUEST = 0x11D,
    TRADE_REQUEST_CANCEL = 0x11E,
    RESURRECT_REQUEST = 0x117,
    LOOT_BIND_CONFIRM = 0x11F,
    EQUIP_BIND_CONFIRM = 0x120,
    AUTOEQUIP_BIND_CONFIRM = 0x121,
    USE_BIND_CONFIRM = 0x122,
    DELETE_ITEM_CONFIRM = 0x123,
    CURSOR_UPDATE = 0x124,
    ITEM_TEXT_BEGIN = 0x125,
    ITEM_TEXT_TRANSLATION = 0x126,
    ITEM_TEXT_READY = 0x127,
    ITEM_TEXT_CLOSED = 0x128,
    GOSSIP_SHOW = 0x129,
    GOSSIP_ENTER_CODE = 0x12A,
    GOSSIP_CLOSED = 0x12B,
    QUEST_GREETING = 0x12C,
    QUEST_DETAIL = 0x12D,
    QUEST_PROGRESS = 0x12E,
    QUEST_COMPLETE = 0x12F,
    QUEST_FINISHED = 0x130,
    QUEST_ITEM_UPDATE = 0x131,
    TAXIMAP_OPENED = 0x132,
    TAXIMAP_CLOSED = 0x133,
    QUEST_LOG_UPDATE = 0x134,
    TRAINER_SHOW = 0x135,
    TRAINER_UPDATE = 0x136,
    TRAINER_CLOSED = 0x137,
    CVAR_UPDATE = 0x138,
    TRADE_SKILL_SHOW = 0x139,
    TRADE_SKILL_UPDATE = 0x13A,
    TRADE_SKILL_CLOSE = 0x13B,
    MERCHANT_SHOW = 0x13C,
    MERCHANT_UPDATE = 0x13D,
    MERCHANT_CLOSED = 0x13E,
    TRADE_SHOW = 0x13F,
    TRADE_CLOSED = 0x140,
    TRADE_UPDATE = 0x141,
    TRADE_ACCEPT_UPDATE = 0x142,
    TRADE_TARGET_ITEM_CHANGED = 0x143,
    TRADE_PLAYER_ITEM_CHANGED = 0x144,
    TRADE_MONEY_CHANGED = 0x145,
    PLAYER_TRADE_MONEY = 0x146,
    BAG_OPEN = 0x147,
    BAG_UPDATE = 0x148,
    BAG_CLOSED = 0x149,
    BAG_UPDATE_COOLDOWN = 0x14A,
    LOCALPLAYER_PET_RENAMED = 0x14B,
    UNIT_ATTACK = 0x14C,
    UNIT_DEFENSE = 0x14D,
    PET_ATTACK_START = 0x14E,
    PET_ATTACK_STOP = 0x14F,
    UPDATE_MOUSEOVER_UNIT = 0x150,
    SPELLCAST_START = 0x151,
    SPELLCAST_STOP = 0x152,
    SPELLCAST_FAILED = 0x153,
    SPELLCAST_INTERRUPTED = 0x154,
    SPELLCAST_DELAYED = 0x155,
    SPELLCAST_CHANNEL_START = 0x156,
    SPELLCAST_CHANNEL_UPDATE = 0x157,
    SPELLCAST_CHANNEL_STOP = 0x158,
    PLAYER_GUILD_UPDATE = 0x159,
    QUEST_ACCEPT_CONFIRM = 0x15A,
    PLAYERBANKSLOTS_CHANGED = 0x15B,
    BANKFRAME_OPENED = 0x15C,
    BANKFRAME_CLOSED = 0x15D,
    PLAYERBANKBAGSLOTS_CHANGED = 0x15E,
    FRIENDLIST_UPDATE = 0x15F,
    IGNORELIST_UPDATE = 0x160,
    PET_BAR_UPDATE_COOLDOWN = 0x162,
    PET_BAR_UPDATE = 0x161,
    PET_BAR_SHOWGRID = 0x163,
    PET_BAR_HIDEGRID = 0x164,
    MINIMAP_PING = 0x165,
    CHAT_MSG_COMBAT_MISC_INFO = 0x166,
    CRAFT_SHOW = 0x167,
    CRAFT_UPDATE = 0x168,
    CRAFT_CLOSE = 0x169,
    MIRROR_TIMER_START = 0x16A,
    MIRROR_TIMER_PAUSE = 0x16B,
    MIRROR_TIMER_STOP = 0x16C,
    WORLD_MAP_UPDATE = 0x16D,
    WORLD_MAP_NAME_UPDATE = 0x16E,
    AUTOFOLLOW_BEGIN = 0x16F,
    AUTOFOLLOW_END = 0x170,
    CINEMATIC_START = 0x172,
    CINEMATIC_STOP = 0x173,
    UPDATE_FACTION = 0x174,
    CLOSE_WORLD_MAP = 0x175,
    OPEN_TABARD_FRAME = 0x176,
    CLOSE_TABARD_FRAME = 0x177,
    SHOW_COMPARE_TOOLTIP = 0x179,
    TABARD_CANSAVE_CHANGED = 0x178,
    GUILD_REGISTRAR_SHOW = 0x17A,
    GUILD_REGISTRAR_CLOSED = 0x17B,
    DUEL_REQUESTED = 0x17C,
    DUEL_OUTOFBOUNDS = 0x17D,
    DUEL_INBOUNDS = 0x17E,
    DUEL_FINISHED = 0x17F,
    TUTORIAL_TRIGGER = 0x180,
    PET_DISMISS_START = 0x181,
    UPDATE_BINDINGS = 0x182,
    UPDATE_SHAPESHIFT_FORMS = 0x183,
    WHO_LIST_UPDATE = 0x184,
    UPDATE_LFG = 0x185,
    PETITION_SHOW = 0x186,
    PETITION_CLOSED = 0x187,
    EXECUTE_CHAT_LINE = 0x188,
    UPDATE_MACROS = 0x189,
    UPDATE_TICKET = 0x18A,
    UPDATE_CHAT_WINDOWS = 0x18B,
    CONFIRM_XP_LOSS = 0x18C,
    CORPSE_IN_RANGE = 0x18D,
    CORPSE_IN_INSTANCE = 0x18E,
    CORPSE_OUT_OF_RANGE = 0x18F,
    UPDATE_GM_STATUS = 0x190,
    PLAYER_UNGHOST = 0x191,
    BIND_ENCHANT = 0x192,
    REPLACE_ENCHANT = 0x193,
    TRADE_REPLACE_ENCHANT = 0x194,
    PLAYER_UPDATE_RESTING = 0x195,
    UPDATE_EXHAUSTION = 0x196,
    PLAYER_FLAGS_CHANGED = 0x197,
    GUILD_ROSTER_UPDATE = 0x198,
    GM_PLAYER_INFO = 0x199,
    MAIL_SHOW = 0x19A,
    MAIL_CLOSED = 0x19B,
    SEND_MAIL_MONEY_CHANGED = 0x19C,
    SEND_MAIL_COD_CHANGED = 0x19D,
    MAIL_SEND_INFO_UPDATE = 0x19E,
    MAIL_SEND_SUCCESS = 0x19F,
    MAIL_INBOX_UPDATE = 0x1A0,
    BATTLEFIELDS_SHOW = 0x1A1,
    BATTLEFIELDS_CLOSED = 0x1A2,
    UPDATE_BATTLEFIELD_STATUS = 0x1A3,
    UPDATE_BATTLEFIELD_SCORE = 0x1A4,
    AUCTION_HOUSE_SHOW = 0x1A5,
    AUCTION_HOUSE_CLOSED = 0x1A6,
    NEW_AUCTION_UPDATE = 0x1A7,
    AUCTION_ITEM_LIST_UPDATE = 0x1A8,
    AUCTION_OWNED_LIST_UPDATE = 0x1A9,
    AUCTION_BIDDER_LIST_UPDATE = 0x1AA,
    PET_UI_UPDATE = 0x1AB,
    PET_UI_CLOSE = 0x1AC,
    ADDON_LOADED = 0x1AD,
    VARIABLES_LOADED = 0x1AE,
    MACRO_ACTION_FORBIDDEN = 0x1AF,
    ADDON_ACTION_FORBIDDEN = 0x1B0,
    MEMORY_EXHAUSTED = 0x1B1,
    MEMORY_RECOVERED = 0x1B2,
    START_AUTOREPEAT_SPELL = 0x1B3,
    STOP_AUTOREPEAT_SPELL = 0x1B4,
    PET_STABLE_SHOW = 0x1B5,
    PET_STABLE_UPDATE = 0x1B6,
    PET_STABLE_UPDATE_PAPERDOLL = 0x1B7,
    PET_STABLE_CLOSED = 0x1B8,
    CHAT_MSG_COMBAT_SELF_HITS = 0x1B9,
    CHAT_MSG_COMBAT_SELF_MISSES = 0x1BA,
    CHAT_MSG_COMBAT_PET_HITS = 0x1BB,
    CHAT_MSG_COMBAT_PET_MISSES = 0x1BC,
    CHAT_MSG_COMBAT_PARTY_HITS = 0x1BD,
    CHAT_MSG_COMBAT_PARTY_MISSES = 0x1BE,
    CHAT_MSG_COMBAT_FRIENDLYPLAYER_HITS = 0x1BF,
    CHAT_MSG_COMBAT_FRIENDLYPLAYER_MISSES = 0x1C0,
    CHAT_MSG_COMBAT_HOSTILEPLAYER_HITS = 0x1C1,
    CHAT_MSG_COMBAT_HOSTILEPLAYER_MISSES = 0x1C2,
    CHAT_MSG_COMBAT_CREATURE_VS_SELF_HITS = 0x1C3,
    CHAT_MSG_COMBAT_CREATURE_VS_SELF_MISSES = 0x1C4,
    CHAT_MSG_COMBAT_CREATURE_VS_PARTY_HITS = 0x1C5,
    CHAT_MSG_COMBAT_CREATURE_VS_PARTY_MISSES = 0x1C6,
    CHAT_MSG_COMBAT_CREATURE_VS_CREATURE_HITS = 0x1C7,
    CHAT_MSG_COMBAT_CREATURE_VS_CREATURE_MISSES = 0x1C8,
    CHAT_MSG_COMBAT_FRIENDLY_DEATH = 0x1C9,
    CHAT_MSG_COMBAT_HOSTILE_DEATH = 0x1CA,
    CHAT_MSG_COMBAT_XP_GAIN = 0x1CB,
    CHAT_MSG_COMBAT_HONOR_GAIN = 0x1CC,
    CHAT_MSG_SPELL_SELF_DAMAGE = 0x1CD,
    CHAT_MSG_SPELL_SELF_BUFF = 0x1CE,
    CHAT_MSG_SPELL_PET_DAMAGE = 0x1CF,
    CHAT_MSG_SPELL_PET_BUFF = 0x1D0,
    CHAT_MSG_SPELL_PARTY_DAMAGE = 0x1D1,
    CHAT_MSG_SPELL_PARTY_BUFF = 0x1D2,
    CHAT_MSG_SPELL_FRIENDLYPLAYER_DAMAGE = 0x1D3,
    CHAT_MSG_SPELL_FRIENDLYPLAYER_BUFF = 0x1D4,
    CHAT_MSG_SPELL_HOSTILEPLAYER_DAMAGE = 0x1D5,
    CHAT_MSG_SPELL_HOSTILEPLAYER_BUFF = 0x1D6,
    CHAT_MSG_SPELL_CREATURE_VS_SELF_DAMAGE = 0x1D7,
    CHAT_MSG_SPELL_CREATURE_VS_SELF_BUFF = 0x1D8,
    CHAT_MSG_SPELL_CREATURE_VS_PARTY_DAMAGE = 0x1D9,
    CHAT_MSG_SPELL_CREATURE_VS_PARTY_BUFF = 0x1DA,
    CHAT_MSG_SPELL_CREATURE_VS_CREATURE_DAMAGE = 0x1DB,
    CHAT_MSG_SPELL_CREATURE_VS_CREATURE_BUFF = 0x1DC,
    CHAT_MSG_SPELL_TRADESKILLS = 0x1DD,
    CHAT_MSG_SPELL_DAMAGESHIELDS_ON_SELF = 0x1DE,
    CHAT_MSG_SPELL_DAMAGESHIELDS_ON_OTHERS = 0x1DF,
    CHAT_MSG_SPELL_AURA_GONE_SELF = 0x1E0,
    CHAT_MSG_SPELL_AURA_GONE_PARTY = 0x1E1,
    CHAT_MSG_SPELL_AURA_GONE_OTHER = 0x1E2,
    CHAT_MSG_SPELL_ITEM_ENCHANTMENTS = 0x1E3,
    CHAT_MSG_SPELL_BREAK_AURA = 0x1E4,
    CHAT_MSG_SPELL_PERIODIC_SELF_DAMAGE = 0x1E5,
    CHAT_MSG_SPELL_PERIODIC_SELF_BUFFS = 0x1E6,
    CHAT_MSG_SPELL_PERIODIC_PARTY_DAMAGE = 0x1E7,
    CHAT_MSG_SPELL_PERIODIC_PARTY_BUFFS = 0x1E8,
    CHAT_MSG_SPELL_PERIODIC_FRIENDLYPLAYER_DAMAGE = 0x1E9,
    CHAT_MSG_SPELL_PERIODIC_FRIENDLYPLAYER_BUFFS = 0x1EA,
    CHAT_MSG_SPELL_PERIODIC_HOSTILEPLAYER_DAMAGE = 0x1EB,
    CHAT_MSG_SPELL_PERIODIC_HOSTILEPLAYER_BUFFS = 0x1EC,
    CHAT_MSG_SPELL_PERIODIC_CREATURE_DAMAGE = 0x1ED,
    CHAT_MSG_SPELL_PERIODIC_CREATURE_BUFFS = 0x1EE,
    CHAT_MSG_SPELL_FAILED_LOCALPLAYER = 0x1EF,
    CHAT_MSG_BG_SYSTEM_NEUTRAL = 0x1F0,
    CHAT_MSG_BG_SYSTEM_ALLIANCE = 0x1F1,
    CHAT_MSG_BG_SYSTEM_HORDE = 0x1F2,
    RAID_ROSTER_UPDATE = 0x1F3,
    UPDATE_PENDING_MAIL = 0x1F4,
    UPDATE_INVENTORY_ALERTS = 0x1F5,
    UPDATE_TRADESKILL_RECAST = 0x1F6,
    OPEN_MASTER_LOOT_LIST = 0x1F7,
    UPDATE_MASTER_LOOT_LIST = 0x1F8,
    START_LOOT_ROLL = 0x1F9,
    CANCEL_LOOT_ROLL = 0x1FA,
    CONFIRM_LOOT_ROLL = 0x1FB,
    INSTANCE_BOOT_START = 0x1FC,
    INSTANCE_BOOT_STOP = 0x1FD,
    LEARNED_SPELL_IN_TAB = 0x1FE,
    DISPLAY_SIZE_CHANGED = 0x1FF,
    CONFIRM_TALENT_WIPE = 0x200,
    CONFIRM_BINDER = 0x201,
    MAIL_FAILED = 0x202,
    CLOSE_INBOX_ITEM = 0x203,
    CONFIRM_SUMMON = 0x204,
    BILLING_NAG_DIALOG = 0x205,
    IGR_BILLING_NAG_DIALOG = 0x206,
    MEETINGSTONE_CHANGED = 0x207,
    PLAYER_SKINNED = 0x208,
    TABARD_SAVE_PENDING = 0x209,
    UNIT_QUEST_LOG_CHANGED = 0x20A,
    PLAYER_PVP_KILLS_CHANGED = 0x20B,
    PLAYER_PVP_RANK_CHANGED = 0x20C,
    INSPECT_HONOR_UPDATE = 0x20D,
    UPDATE_WORLD_STATES = 0x20E,
    AREA_SPIRIT_HEALER_IN_RANGE = 0x20F,
    AREA_SPIRIT_HEALER_OUT_OF_RANGE = 0x210,
    CONFIRM_PET_UNLEARN = 0x211,
    PLAYTIME_CHANGED = 0x212,
    UPDATE_LFG_TYPES = 0x213,
    UPDATE_LFG_LIST = 0x214,
    CHAT_MSG_COMBAT_FACTION_CHANGE = 0x215,
    START_MINIGAME = 0x216,
    MINIGAME_UPDATE = 0x217,
    READY_CHECK = 0x218,
    RAID_TARGET_UPDATE = 0x219,
    GMSURVEY_DISPLAY = 0x21A,
    UPDATE_INSTANCE_INFO = 0x21B,
    CHAT_MSG_RAID_BOSS_EMOTE = 0x21D,
    COMBAT_TEXT_UPDATE = 0x21E,
    LOTTERY_SHOW = 0x21F,
    CHAT_MSG_FILTERED = 0x220,
    QUEST_WATCH_UPDATE = 0x221,
    CHAT_MSG_BATTLEGROUND = 0x222,
    CHAT_MSG_BATTLEGROUND_LEADER = 0x223,
    LOTTERY_ITEM_UPDAT = 0x224,
};

void *GetObjectPtr(std::uint64_t guid);
std::uint32_t GetCastTime(void *unit, int spellId);
const SpellRec *GetSpellInfo(int spellId);
const char *GetSpellName(int spellId);
std::uint64_t ClntObjMgrGetActivePlayer();

class CDataStore
{
    private:
        virtual void InternalInitialize(int a2, int a3, int a4);
        virtual void InternalDestroy(unsigned __int8 **, unsigned int *, unsigned int *);
        virtual int InternalFetchRead(int start, int count, void *buff, int base, int alloc);
        virtual void InternalFetchWrite(unsigned int, int, unsigned __int8 **, unsigned int *, unsigned int *, int, int);
        virtual void Destructor();
        virtual int IsRead();
        virtual void Reset();
        virtual void Finalize();
        virtual void GetBufferParams();
        virtual void DetachBuffer();

    public:
        unsigned __int8 *m_buffer;
        unsigned int m_base;
        unsigned int m_alloc;
        unsigned int m_size;
        unsigned int m_read;

        template <typename T>
        T Get()
        {
            static_assert(std::is_pod<T>::value, "CDataStore::Get requires POD type");

            T ret = *reinterpret_cast<T *>(m_buffer + m_read);
            m_read += sizeof(T);
            return ret;
        }
};
}