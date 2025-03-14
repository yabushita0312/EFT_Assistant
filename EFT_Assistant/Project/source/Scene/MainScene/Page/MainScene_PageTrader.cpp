#include "MainScene_PageTrader.hpp"
#include "../Data/MainScene_Base.hpp"
#include "../../../PartsHeader.hpp"

namespace FPS_n2 {
	void TraderBG::Init_Sub(int*, int*, float*) noexcept {
		m_EditTraderID = InvalidID;
		m_ReturnButtonPress = [&]() {
			auto* PageMngr = PageManager::Instance();
			if (m_EditTraderID != InvalidID) {
				m_EditTraderID = InvalidID;
			}
			else {
				PageMngr->TurnOnGoNextPage();
			}
			};
	}
	void TraderBG::Draw_Back_Sub(int, int, float) noexcept {
		auto* WindowMngr = WindowMySystem::WindowManager::Instance();
		//
		if (m_EditTraderID != InvalidID) {
			auto* ptr = DataBase::Instance()->GetTraderData()->FindPtr(m_EditTraderID);
			for (auto& LL : ptr->GetLvData()) {
				int index = (int)(&LL - &ptr->GetLvData().front());
				int xp = DXDraw::Instance()->GetUIY(100) + (DXDraw::Instance()->GetUIY(100) + DXDraw::Instance()->GetUIY(10)) * index;
				int yp = LineHeight + DXDraw::Instance()->GetUIY(50);
				if (WindowSystem::SetMsgClickBox(xp, yp, xp + DXDraw::Instance()->GetUIY(100), yp + LineHeight, LineHeight, (m_LLSelect == index) ? Gray15 : Gray50, false, true, "Lv%d", index + 1)) {
					m_LLSelect = index;
				}
			}
			m_LLSelect = std::min(m_LLSelect, (int)(ptr->GetLvData().size()) - 1);
			{
				int xp = DXDraw::Instance()->GetUIY(60);
				int yp = LineHeight + DXDraw::Instance()->GetUIY(50) + DXDraw::Instance()->GetUIY(100);

				int ysize = (DXDraw::Instance()->GetUIY(48));
				int ysizeAdd = ysize + DXDraw::Instance()->GetUIY(5);
				int yofs = LineHeight;

				int Max = (int)(ptr->GetLvData().at(m_LLSelect).m_ItemBarters.size());

				int ofset = (int)(this->m_Scroll.GetNowScrollYPer() * (std::max(0, Max - 10 + 1) * ysizeAdd));

				int ypMin = yp;
				int ypMax = (DXDraw::Instance()->GetUIY(1080) - DXDraw::Instance()->GetUIY(100));
				int yp1 = yp + yofs - ofset;
				for (auto& Bar : ptr->GetLvData().at(m_LLSelect).m_ItemBarters) {
					int xofs_buf = DXDraw::Instance()->GetUIY(10);
					if (ypMin - ysizeAdd < yp1 && yp1 < ypMax) {
						if (ypMin < yp1 && yp1 < ypMax - ysizeAdd) {
							WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
						}
						else {
							if (yp1 <= ypMin) {
								WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255 - std::clamp(255 * (ypMin - yp1) / ysizeAdd, 0, 255));
							}
							else {
								WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255 - std::clamp(255 * (yp1 - (ypMax - ysizeAdd)) / ysizeAdd, 0, 255));
							}
						}
						ptr->DrawBarter(nullptr, Gray15, xp + xofs_buf, yp1, ysize, m_LLSelect + 1, (int)(&Bar - &ptr->GetLvData().at(m_LLSelect).m_ItemBarters.front()), false, true, 0);
					}
					yofs += ysizeAdd;
					yp1 += ysizeAdd;
				}
				WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
				//スクロールバー
				{
					float Total = (float)yofs / (ypMax - ypMin);
					if (Total > 1.f) {
						m_Scroll.SetScrollBoxParam(xp, ypMin, DXDraw::Instance()->GetUIY(1920) - DXDraw::Instance()->GetUIY(600), ypMax, Total, !WindowMngr->PosHitCheck(nullptr));
						m_Scroll.ScrollBox();
					}
				}
			}
		}
	}
	void TraderBG::DrawFront_Sub(int, int, float) noexcept {
		DataBase::Instance()->GetTraderData()->DrawList(DXDraw::Instance()->GetUIY(1920 - 400 - 10), LineHeight + DXDraw::Instance()->GetUIY(50), DXDraw::Instance()->GetUIY(400), "Trader", (int*)&m_EditTraderID, true, false, false);
	}
};
