#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Parents.hpp"

namespace FPS_n2 {
	//Map
	typedef int MapID;
	//Trader
	typedef int TraderID;
	class TraderGetData : public GetDataParent<TraderID> {};
	class TraderLLData : public GetDataParent<TraderID> {};
	//ItemCategory
	typedef int ItemCategoryID;
	//ItemType
	typedef int ItemTypeID;
	//Item
	typedef int ItemID;
	class ItemGetData : public GetDataParent<ItemID> {};
	//Preset
	typedef int PresetID;
	//Enemy
	typedef int EnemyID;
	//Task
	typedef int TaskID;
	//Hideout
	typedef int HideoutID;
	class HideoutGetData : public GetDataParent<HideoutID> {};

	enum class EnumData {
		Map,
		Trader,
		ItemCategory,
		ItemType,
		Item,
		Preset,
		Enemy,
		Task,
		Hideout,
		Max,
	};
	static const char* EnumDataStr[(int)EnumData::Max] = {
		"Map",
		"Trader",
		"ItemCategory",
		"ItemType",
		"Item",
		"Preset",
		"Enemy",
		"Task",
		"Hideout",
	};
};
