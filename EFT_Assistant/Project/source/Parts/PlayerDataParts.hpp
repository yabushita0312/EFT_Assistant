#pragma once
#include"../Header.hpp"

namespace FPS_n2 {
	//
	class ItemLockData {
		std::string		m_ID;
		bool			m_IsLock{ false };
	public:
		const auto&		GetID() const noexcept { return m_ID; }
		const auto&		GetIsLock() const noexcept { return m_IsLock; }

		void	SetID(const char* Name) noexcept { m_ID = Name; }
		void	SetIsLock(bool value) noexcept { m_IsLock = value; }
	};
	//
	enum class EnumEdition : int {
		STANDARD,
		LEFT_BEHIND,
		PREPARE_FOR_ESCAPE,
		EDGE_OF_DARKNESS,
		Max,
	};
	static const char* EnumEditionStr[(int)EnumEdition::Max] = {
		"STANDARD",
		"LEFT BEHIND",
		"PREPARE FOR ESCAPE",
		"EDGE OF DARKNESS",
	};
	//
	class PlayerData : public SingletonBase<PlayerData> {
	private:
		friend class SingletonBase<PlayerData>;
	private:
		std::string					m_LastDataReceive;
		std::vector<ItemLockData>	m_ItemLockData;
		std::vector<std::string>	m_TaskClearData;
		std::vector<std::pair<std::string, int>>	m_HideoutClearData;
	private:
		EnumEdition					m_Edition{ EnumEdition::STANDARD };
		bool						m_IsNeedLightKeeper{ false };
		bool						m_IsNeedKappa{ false };
		int							m_MaxLevel{ 71 };
	private:
		bool						m_IsUSEC{ true };
	private:
		PlayerData() noexcept {
			Load();
		}
		~PlayerData() noexcept {
		}
	public:
		void			Load(void) noexcept {
			SetOutApplicationLogValidFlag(FALSE);
			int mdata = FileRead_open("data/PlayerData.txt", FALSE);
			while (true) {
				if (FileRead_eof(mdata) != 0) { break; }
				auto ALL = getparams::Getstr(mdata);
				//コメントアウト
				SubRIGHTStrs(&ALL, "//");
				//
				if (ALL == "") { continue; }
				auto LEFT = getparams::getleft(ALL);
				auto RIGHT = getparams::getright(ALL);
				auto Args = GetArgs(RIGHT);
				//アイテムデータ読みとり
				if (LEFT == "LastDataReceive") { m_LastDataReceive = RIGHT; }
				else if (LEFT == "ItemData") { SetItemLock(Args.at(0).c_str(), Args.at(1) == "true"); }
				else if (LEFT == "ClearTask") { m_TaskClearData.emplace_back(RIGHT); }
				else if (LEFT == "UnlockHideout") {
					auto L = RIGHT.rfind("x");
					if (L != std::string::npos) {
						m_HideoutClearData.emplace_back(std::make_pair(RIGHT.substr(0, L), std::stoi(RIGHT.substr(L + 1))));
					}
				}
				else if (LEFT == "Edition") { m_Edition = (EnumEdition)(std::stoi(RIGHT)); }
				else if (LEFT == "IsNeedLightKeeper") { m_IsNeedLightKeeper = (RIGHT == "TRUE"); }
				else if (LEFT == "IsNeedKappa") { m_IsNeedKappa = (RIGHT == "TRUE"); }
				else if (LEFT == "MaxLevel") { m_MaxLevel = std::stoi(RIGHT); }
				else if (LEFT == "PMCType") { m_IsUSEC = (RIGHT == "USEC"); }
			}
			FileRead_close(mdata);
			SetOutApplicationLogValidFlag(TRUE);
		}
		void			Save(void) noexcept {
			std::ofstream outputfile("data/PlayerData.txt");
			outputfile << "LastDataReceive=" + m_LastDataReceive + "\n";

			for (auto& LD : m_ItemLockData) {
				outputfile << "ItemData=[" + LD.GetID() + DIV_STR + (LD.GetIsLock() ? "true" : "false") + "]\n";
			}
			for (auto& LD : m_TaskClearData) {
				outputfile << "ClearTask=" + LD + "\n";
			}
			for (auto& LD : m_HideoutClearData) {
				outputfile << "UnlockHideout=" + LD.first + "x" + std::to_string(LD.second) + "\n";
			}

			outputfile << "Edition=" + std::to_string((int)m_Edition) + "\n";
			outputfile << "IsNeedLightKeeper=" + (std::string)(m_IsNeedLightKeeper ? "TRUE" : "FALSE") + "\n";
			outputfile << "IsNeedKappa=" + (std::string)(m_IsNeedKappa ? "TRUE" : "FALSE") + "\n";
			outputfile << "MaxLevel=" + std::to_string(m_MaxLevel) + "\n";
			outputfile << "PMCType=" + (std::string)(m_IsUSEC ? "USEC" : "BEAR") + "\n";

			//outputfile << "grass_level=" + std::to_string(grass_level) + "\n";
			outputfile.close();
		}
	public:
		void OnOffItemLock(const char* ID) noexcept {
			auto Point = std::find_if(m_ItemLockData.begin(), m_ItemLockData.end(), [&](const ItemLockData& Data) {return (Data.GetID() == ID); });
			if (Point != m_ItemLockData.end()) {
				Point->SetIsLock(!Point->GetIsLock());
			}
			else {
				m_ItemLockData.resize(m_ItemLockData.size() + 1);
				m_ItemLockData.back().SetID(ID);
				m_ItemLockData.back().SetIsLock(!m_ItemLockData.back().GetIsLock());
			}
		}
		void SetItemLock(const char* ID, bool value) noexcept {
			auto Point = std::find_if(m_ItemLockData.begin(), m_ItemLockData.end(), [&](const ItemLockData& Data) {return (Data.GetID() == ID); });
			if (Point != m_ItemLockData.end()) {
				Point->SetIsLock(value);
			}
			else {
				m_ItemLockData.resize(m_ItemLockData.size() + 1);
				m_ItemLockData.back().SetID(ID);
				m_ItemLockData.back().SetIsLock(value);
			}
		}
		const auto GetItemLock(const char* ID) noexcept {
			auto Point = std::find_if(m_ItemLockData.begin(), m_ItemLockData.end(), [&](const ItemLockData& Data) {return (Data.GetID() == ID); });
			if (Point != m_ItemLockData.end()) {
				return Point->GetIsLock();
			}
			else {
				m_ItemLockData.resize(m_ItemLockData.size() + 1);
				m_ItemLockData.back().SetID(ID);
				m_ItemLockData.back().SetIsLock(false);
				return m_ItemLockData.back().GetIsLock();
			}
		}

		void OnOffTaskClear(const char* ID) noexcept {
			auto Point = std::find_if(m_TaskClearData.begin(), m_TaskClearData.end(), [&](const std::string& Data) {return (Data == ID); });
			if (Point != m_TaskClearData.end()) {
				m_TaskClearData.erase(Point);
			}
			else {
				m_TaskClearData.resize(m_TaskClearData.size() + 1);
				m_TaskClearData.back() = ID;
			}
		}
		void SetTaskClear(const char* ID, bool value) noexcept {
			auto Point = std::find_if(m_TaskClearData.begin(), m_TaskClearData.end(), [&](const std::string& Data) {return (Data == ID); });
			if (Point != m_TaskClearData.end()) {
				if (!value) {
					m_TaskClearData.erase(Point);
				}
			}
			else {
				if (value) {
					m_TaskClearData.resize(m_TaskClearData.size() + 1);
					m_TaskClearData.back() = ID;
				}
			}
		}
		void ResetTaskClear() noexcept {
			m_TaskClearData.clear();
		}
		const auto GetTaskClear(const char* ID) noexcept {
			auto Point = std::find_if(m_TaskClearData.begin(), m_TaskClearData.end(), [&](const std::string& Data) {return (Data == ID); });
			return (Point != m_TaskClearData.end());
		}

		void OnOffHideoutClear(const char* ID, int Lv) noexcept {
			auto Point = std::find_if(m_HideoutClearData.begin(), m_HideoutClearData.end(), [&](const std::pair<std::string, int>& Data) {return (Data.first == ID); });
			if (Point != m_HideoutClearData.end()) {
				if (Point->second >= Lv) {
					if (Lv > 1) {
						Point->second = Lv - 1;
					}
					else {
						m_HideoutClearData.erase(Point);
					}
				}
				else {
					Point->second = Lv;
				}
			}
			else {
				m_HideoutClearData.resize(m_HideoutClearData.size() + 1);
				m_HideoutClearData.back().first = ID;
				m_HideoutClearData.back().second = Lv;
			}
		}
		void ResetHideoutClear() noexcept {
			m_HideoutClearData.clear();
		}
		const auto GetHideoutClear(const char* ID, int Lv) noexcept {
			auto Point = std::find_if(m_HideoutClearData.begin(), m_HideoutClearData.end(), [&](const std::pair<std::string, int>& Data) {return (Data.first == ID && Data.second >= Lv); });
			if (Point != m_HideoutClearData.end()) {
				return true;
			}
			else {
				std::string IDStr = ID;
				if (IDStr.find("Stash") != std::string::npos) {
					Point = std::find_if(m_HideoutClearData.begin(), m_HideoutClearData.end(), [&](const std::pair<std::string, int>& Data) {return (Data.first == ID); });
					if (Point != m_HideoutClearData.end()) {
						Point->second = (int)m_Edition + 1;
					}
					else {
						m_HideoutClearData.resize(m_HideoutClearData.size() + 1);
						m_HideoutClearData.back().first = ID;
						m_HideoutClearData.back().second = (int)m_Edition + 1;
					}
					return (((int)m_Edition + 1) >= Lv);
				}
				else {
					return false;
				}
			}
		}

		void SetLastDataReceive(const char* date) noexcept { m_LastDataReceive = date; }
		const auto& GetLastDataReceive() const noexcept { return m_LastDataReceive; }

		void SetIsNeedLightKeeper(bool date) noexcept { m_IsNeedLightKeeper = date; }
		const auto& GetIsNeedLightKeeper() const noexcept { return m_IsNeedLightKeeper; }

		void SetEdition(EnumEdition date) noexcept { m_Edition = date; }
		const auto& GetEdition() const noexcept { return m_Edition; }

		void SetIsNeedKappa(bool date) noexcept { m_IsNeedKappa = date; }
		const auto& GetIsNeedKappa() const noexcept { return m_IsNeedKappa; }
		void SetMaxLevel(int date) noexcept { m_MaxLevel = date; }
		const auto& GetMaxLevel() const noexcept { return m_MaxLevel; }
		void SetIsUSEC(bool date) noexcept { m_IsUSEC = date; }
		const auto& GetIsUSEC() const noexcept { return m_IsUSEC; }
	};
	//
	static void GetDirList(const char* DirPath, const std::function<void(const char*)>& Doing) noexcept {
		auto data_t = GetFileNamesInDirectory(DirPath);
		for (auto& d : data_t) {
			if (d.cFileName[0] != '.') {
				Doing(d.cFileName);
			}
		}
	}
	//
};
