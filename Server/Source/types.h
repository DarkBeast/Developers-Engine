/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_TYPES_H
#define DE_TYPES_H

#include "globals.h"
#include "integer.h"

typedef enum stats_t stats_t;
typedef enum vitals_t vitals_t;
typedef enum equipment_t equipment_t;
typedef enum dir_t dir_t;
typedef enum movement_t movement_t;
typedef enum target_type target_type;

enum vitals_t{
	VITAL_HP = 0,
	VITAL_MP,
	VITAL_SP,
	VITAL_COUNT //always keep last
};

enum stats_t{
	STAT_STRENGTH = 0,
	STAT_DEFENSE,
	STAT_SPEED,
	STAT_MAGIC,
	STAT_COUNT //always keep last
};

enum equipment_t{
	EQUIPMENT_WEAPON = 0,
	EQUIPMENT_ARMOR,
	EQUIPMENT_HELMET,
	EQUIPMENT_SHIELD,
	EQUIPMENT_COUNT //always keep last
};

enum dir_t{
	DIR_NONE,
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_COUNT
};

enum movement_t{
	MOVEMENT_NONE,
	MOVEMENT_WALKING,
	MOVEMENT_RUNNING,
	MOVEMENT_COUNT
};

enum target_type{
	TARGET_NONE,
	TARGET_PLAYER,
	TARGET_NPC,
	TARGET_TYPE_COUNT
};

enum editor_type{
	EDITOR_NONE,
	EDITOR_MAP,
	EDITOR_ITEM,
	EDITOR_NPC,
	EDITOR_SPELL,
	EDITOR_SHOP
};

#endif