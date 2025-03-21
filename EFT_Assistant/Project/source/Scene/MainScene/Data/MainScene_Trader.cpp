#include "MainScene_Trader.hpp"
#include "../Data/MainScene_Base.hpp"
#include "../../../PartsHeader.hpp"

namespace FPS_n2 {
	//
	void	TraderList::Load_Sub() noexcept {
		for (auto& L : this->m_LvData) {
			for (auto& C : L.m_ItemBarters) {
				for (auto& T : C.m_TaskReq) {
					T.CheckID(DataBase::Instance()->GetTaskData().get());
				}
				for (auto& T : C.m_ItemReq) {
					T.CheckID(DataBase::Instance()->GetItemData().get());
				}
				for (auto& T : C.m_ItemReward) {
					T.CheckID(DataBase::Instance()->GetItemData().get());
				}
			}
		}

	}
	const int		TraderList::DrawBarter(WindowMySystem::WindowControl* window, unsigned int defaultcolor, int xp, int yp, int ysize, int Lv, int Select, bool isdrawName, bool isdrawAfter, int count) const noexcept {
		auto* WindowMngr = WindowMySystem::WindowManager::Instance();
		const auto& cf = this->m_LvData.at(static_cast<size_t>(Lv - 1)).m_ItemBarters.at(Select);
		int xofsbuf = DXDraw::Instance()->GetUIY(10);
		int xofsbuf2 = DXDraw::Instance()->GetUIY(10);
		int yofsbuf = 0;
		int xsize2 = isdrawName ? (ysize / 2) : ysize;
		int ysize2 = isdrawName ? (ysize / 2 - DXDraw::Instance()->GetUIY(3)) : ysize;
		if (isdrawName) {
			std::string Name = this->GetName() + " Lv" + std::to_string(Lv);
			if (count > 1) {
				Name += " x" + std::to_string(count);
			}
			WindowSystem::SetMsg(xp + xofsbuf, yp + yofsbuf + ysize2 / 2, ysize2, STRX_LEFT, White, Black, Name);
			xofsbuf2 = WindowSystem::GetMsgLen(ysize2, Name) + DXDraw::Instance()->GetUIY(30);
			for (const auto& w : cf.m_TaskReq) {
				auto* ptr = DataBase::Instance()->GetTaskData()->FindPtr(w.GetID());
				if (ptr) {
					xofsbuf2 += ptr->Draw(xp + xofsbuf2, yp + yofsbuf, xsize2 * 10, ysize2, 0, !WindowMngr->PosHitCheck(window)) + DXDraw::Instance()->GetUIY(5);
				}
			}
			yofsbuf += ysize2 + DXDraw::Instance()->GetUIY(5);
		}
		for (const auto& w : cf.m_ItemReq) {
			auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(w.GetID());
			if (ptr) {
				xofsbuf += ptr->Draw(xp + xofsbuf, yp + yofsbuf, xsize2, ysize2, w.GetValue() * std::max(1, count), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true) + DXDraw::Instance()->GetUIY(5);
			}
		}
		if (isdrawAfter) {
			xofsbuf = std::max(xofsbuf, xsize2 * 10);
			WindowSystem::SetMsg(xp + xofsbuf / 2, yp + ysize / 2, (ysize / 2 - DXDraw::Instance()->GetUIY(3)), STRX_LEFT, White, Black, "->");
			xofsbuf += WindowSystem::GetMsgLen((ysize / 2 - DXDraw::Instance()->GetUIY(3)), "->") + DXDraw::Instance()->GetUIY(30);
			for (const auto& w : cf.m_ItemReward) {
				auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(w.GetID());
				if (ptr) {
					xofsbuf += ptr->Draw(xp + xofsbuf, yp + yofsbuf, xsize2, ysize2, w.GetValue() * std::max(1, count), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true) + DXDraw::Instance()->GetUIY(5);
				}
			}
		}
		if (!isdrawName) {
			for (const auto& w : cf.m_TaskReq) {
				auto* ptr = DataBase::Instance()->GetTaskData()->FindPtr(w.GetID());
				if (ptr) {
					xofsbuf += ptr->Draw(xp + xofsbuf, yp + yofsbuf, xsize2 * 10, ysize2, 0, !WindowMngr->PosHitCheck(window)) + DXDraw::Instance()->GetUIY(5);
				}
			}
		}
		xofsbuf = std::max(xofsbuf, xofsbuf2);
		return xofsbuf;
	}
	//
	void TraderJsonData::GetJsonSub(const nlohmann::json& data) noexcept {
		PayItem = DataBase::Instance()->GetItemData()->FindID((std::string)(data["currency"]["name"]));
		m_LvData.clear();
		for (auto& Ld : data["levels"]) {
			m_LvData.resize(m_LvData.size() + 1);
			auto& L = this->m_LvData.back();

			L.NeedLv = Ld["requiredPlayerLevel"];
			L.Reputation = Ld["requiredReputation"];
			L.Commerce = Ld["requiredCommerce"];
			L.payRate = Ld["payRate"];

			if (!Ld["insuranceRate"].is_null()) {
				L.insuranceRate = Ld["insuranceRate"];
			}
			else {
				L.insuranceRate = -1.f;
			}

			if (!Ld["repairCostMultiplier"].is_null()) {
				L.repairCostMultiplier = Ld["repairCostMultiplier"];
			}
			else {
				L.repairCostMultiplier = -1.f;
			}

			L.m_ItemBarters.clear();
			if (Ld.contains("barters")) {
				if (!Ld["barters"].is_null()) {
					for (const auto& m : Ld["barters"]) {
						L.m_ItemBarters.resize(L.m_ItemBarters.size() + 1);
						auto& B = L.m_ItemBarters.back();
						if (!m["taskUnlock"].is_null()) {
							IDParents<TaskID> buf;
							buf.SetName(m["taskUnlock"]["name"]);
							B.m_TaskReq.emplace_back(buf);
						}
						for (const auto& I : m["requiredItems"]) {
							ItemGetData buf;
							buf.Set(I["item"]["name"], I["count"]);
							B.m_ItemReq.emplace_back(buf);
						}
						for (const auto& I : m["rewardItems"]) {
							ItemGetData buf;
							buf.Set(I["item"]["name"], I["count"]);
							B.m_ItemReward.emplace_back(buf);
						}
					}
				}
			}
			if (Ld.contains("cashOffers")) {
				if (!Ld["cashOffers"].is_null()) {
					for (const auto& m : Ld["cashOffers"]) {
						L.m_ItemBarters.resize(L.m_ItemBarters.size() + 1);
						auto& B = L.m_ItemBarters.back();
						if (!m["taskUnlock"].is_null()) {
							IDParents<TaskID> buf;
							buf.SetName(m["taskUnlock"]["name"]);
							B.m_TaskReq.emplace_back(buf);
						}
						{
							ItemGetData buf;
							buf.Set(m["currencyItem"]["name"], m["price"]);
							B.m_ItemReq.emplace_back(buf);
						}
						{
							ItemGetData buf;
							buf.Set(m["item"]["name"], 1);
							B.m_ItemReward.emplace_back(buf);
						}
					}
				}
			}
			for (auto& B : L.m_ItemBarters) {
				B.m_Sortvalue = 0;
				for (auto& T : B.m_TaskReq) {
					T.CheckID(DataBase::Instance()->GetTaskData().get());
					B.m_Sortvalue += T.GetID();
				}
				for (auto& T : B.m_ItemReq) {
					T.CheckID(DataBase::Instance()->GetItemData().get());
					B.m_Sortvalue += T.GetID();
				}
				for (auto& T : B.m_ItemReward) {
					T.CheckID(DataBase::Instance()->GetItemData().get());
					B.m_Sortvalue += T.GetID();
				}
			}

		}
	}
	void TraderJsonData::OutputDataSub(std::ofstream& outputfile) noexcept {
		{
			auto* ptr = DataBase::Instance()->GetTraderData()->FindPtr(DataBase::Instance()->GetTraderData()->FindID(this->m_name));
			if (ptr) {
				outputfile << "Color=[" + std::to_string(ptr->GetColorRGB(0)) + DIV_STR + std::to_string(ptr->GetColorRGB(1)) + DIV_STR + std::to_string(ptr->GetColorRGB(2)) + "]\n";
			}
		}
		outputfile << "PayItem=" + DataBase::Instance()->GetItemData()->FindPtr(this->PayItem)->GetName() + "\n";
		for (auto& L2 : this->m_LvData) {
			auto LV = "Lv" + std::to_string((&L2 - &this->m_LvData.front()) + 1);

			outputfile << LV + "NeedLv=" + std::to_string(L2.NeedLv) + "\n";
			outputfile << LV + "Reputation=" + std::to_string(L2.Reputation) + "\n";
			outputfile << LV + "Commerce=" + std::to_string(L2.Commerce) + "\n";
			outputfile << LV + "payRate=" + std::to_string(L2.payRate) + "\n";
			if (L2.insuranceRate >= 0.f) {
				outputfile << LV + "insuranceRate=" + std::to_string(L2.insuranceRate) + "\n";
			}
			if (L2.repairCostMultiplier >= 0.f) {
				outputfile << LV + "repairCostMultiplier=" + std::to_string(L2.repairCostMultiplier) + "\n";
			}
			std::sort(L2.m_ItemBarters.begin(), L2.m_ItemBarters.end(), [&](const BartersData& a, const BartersData& b) { return a.m_Sortvalue < b.m_Sortvalue; });
			for (auto& c : L2.m_ItemBarters) {
				//m_TaskReq
				outputfile << LV + "taskUnlock=[";
				for (auto& m : c.m_TaskReq) {
					outputfile << m.GetName();
					if (&m != &c.m_TaskReq.back()) {
						outputfile << DIV_STR;
					}
				}
				outputfile << "]\n";
				{
					outputfile << LV + "BarteritemReq=[";
					for (auto& m : c.m_ItemReq) {
						outputfile << m.GetOutputStr();
						if (&m != &c.m_ItemReq.back()) {
							outputfile << DIV_STR;
						}
					}
					outputfile << "]\n";
				}
				{
					outputfile << LV + "BarteritemReward=[";
					for (auto& m : c.m_ItemReward) {
						outputfile << m.GetOutputStr();
						if (&m != &c.m_ItemReward.back()) {
							outputfile << DIV_STR;
						}
					}
					outputfile << "]\n";
				}
			}
		}
	}
	//
	void TraderBartersJsonData::GetJsonSub(const nlohmann::json& data) noexcept {
		m_ItemBarters.clear();
		m_ItemBarters.resize(m_ItemBarters.size() + 1);
		auto& B = m_ItemBarters.back();
		if (data.contains("trader") && !data["trader"].is_null()) {
			B.m_traderName = data["trader"]["name"];
		}
		if (data.contains("level") && !data["level"].is_null()) {
			B.m_level = data["level"];
		}
		if (data.contains("taskUnlock") && !data["taskUnlock"].is_null()) {
			IDParents<TaskID> buf;
			buf.SetName(data["taskUnlock"]["name"]);
			B.m_TaskReq.emplace_back(buf);
		}
		for (const auto& I : data["requiredItems"]) {
			ItemGetData buf;
			buf.Set(I["item"]["name"], I["count"]);
			B.m_ItemReq.emplace_back(buf);
		}
		for (const auto& I : data["rewardItems"]) {
			ItemGetData buf;
			buf.Set(I["item"]["name"], I["count"]);
			B.m_ItemReward.emplace_back(buf);
		}

		B.m_Sortvalue = 0;
		for (auto& T : B.m_TaskReq) {
			T.CheckID(DataBase::Instance()->GetTaskData().get());
			B.m_Sortvalue += T.GetID();
		}
		for (auto& T : B.m_ItemReq) {
			T.CheckID(DataBase::Instance()->GetItemData().get());
			B.m_Sortvalue += T.GetID();
		}
		for (auto& T : B.m_ItemReward) {
			T.CheckID(DataBase::Instance()->GetItemData().get());
			B.m_Sortvalue += T.GetID();
		}
	}
	void TraderBartersJsonData::OutputData2(const std::string& Path) noexcept {
		m_Path = Path;
		m_SetFinish = true;
	}
	void TraderBartersData::SaveTraderDataBack(std::string ParentPath) noexcept {
		std::string ChildPath = ParentPath + "/";
		for (auto& L : DataBase::Instance()->GetTraderData()->SetList()) {
			std::string Name = L.GetName() + ".txt";
			std::string Path = ChildPath + Name;
			SubStrs(&Path, ":");
			SubStrs(&Path, "*");
			SubStrs(&Path, "?");
			SubStrs(&Path, "\"");
			SubStrs(&Path, ">");
			SubStrs(&Path, "<");
			SubStrs(&Path, "|");
			std::ofstream outputfile(Path, std::ios_base::app);

			for (auto& jd : GetJsonDataList()) {
				auto* jd2 = (TraderBartersJsonData*)jd.get();
				std::sort(jd2->m_ItemBarters.begin(), jd2->m_ItemBarters.end(), [&](const BartersData& a, const BartersData& b) { return a.m_Sortvalue < b.m_Sortvalue; });
				for (auto& c : jd2->m_ItemBarters) {
					if (L.GetName() != c.m_traderName) { continue; }
					auto LV = "Lv" + std::to_string(c.m_level);
					//m_TaskReq
					outputfile << LV + "taskUnlock=[";
					for (auto& m : c.m_TaskReq) {
						outputfile << m.GetName();
						if (&m != &c.m_TaskReq.back()) {
							outputfile << DIV_STR;
						}
					}
					outputfile << "]\n";
					{
						outputfile << LV + "BarteritemReq=[";
						for (auto& m : c.m_ItemReq) {
							outputfile << m.GetOutputStr();
							if (&m != &c.m_ItemReq.back()) {
								outputfile << DIV_STR;
							}
						}
						outputfile << "]\n";
					}
					{
						outputfile << LV + "BarteritemReward=[";
						for (auto& m : c.m_ItemReward) {
							outputfile << m.GetOutputStr();
							if (&m != &c.m_ItemReward.back()) {
								outputfile << DIV_STR;
							}
						}
						outputfile << "]\n";
					}
				}
				jd2->OutputData2(Path);
			}

			outputfile.close();
		}
	}	//
};
