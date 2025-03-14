#include "MainScene_PageItem.hpp"
#include "../Data/MainScene_Base.hpp"
#include "../../../PartsHeader.hpp"

namespace FPS_n2 {
	void ItemBG::Init_Sub(int*, int*, float*) noexcept {
		Items.clear();
		for (auto& L : DataBase::Instance()->GetItemData()->GetList()) {//todo
			Items.emplace_back(&L);
		}
		//
		m_RaidMode = false;
		m_TraderSort = true;
		m_ValueSort = true;
		m_ValuePerSort = true;
		//
		InitLists(2, DXDraw::Instance()->GetUIY(1920 - 10) - DXDraw::Instance()->GetUIY(400), LineHeight + DXDraw::Instance()->GetUIY(5), DXDraw::Instance()->GetUIY(400));

		m_SearchBox.Init();
		m_ReturnButtonPress = [&]() {
			auto* PageMngr = PageManager::Instance();
			if (!(!m_RaidMode && BackLists())) {
				PageMngr->TurnOnGoNextPage();
			}
			};
	}
	void ItemBG::Draw_Back_Sub(int, int, float) noexcept {
		auto* WindowMngr = WindowMySystem::WindowManager::Instance();
		auto* DrawParts = DXDraw::Instance();
		auto* Pad = PadControl::Instance();

		int xpos = DXDraw::Instance()->GetUIY(40);
		int ypos = LineHeight + DXDraw::Instance()->GetUIY(10) + LineHeight;
		int ysize = (DXDraw::Instance()->GetUIY(80));

		int xs = 400;
		int ScrPosX = DXDraw::Instance()->GetUIY(1920 - xs * 3 / 2 - 10) - DXDraw::Instance()->GetUIY(80);

		int ScrPxItem = this->m_RaidMode ? (xpos + DXDraw::Instance()->GetUIY(400)) : ScrPosX;

		int yp0 = ypos - (int)m_YNow;
		if (m_RaidMode) {
			int PrevSize = DXDraw::Instance()->GetUIY(400);
			int PerSize = PrevSize + DXDraw::Instance()->GetUIY(250);
			if (WindowSystem::SetMsgClickBox(xpos + PrevSize, yp0, xpos + PerSize, DrawParts->GetScreenY(1080), LineHeight, Gray75, false, true, "")) {
				std::sort(Items.begin(), Items.end(), [&](const ItemList* a, const ItemList* b) {
					TraderID IDA = InvalidID;
					int ValueA = -1;
					if (a->GetSellValue(&IDA, &ValueA)) {}
					TraderID IDB = InvalidID;
					int ValueB = -1;
					if (b->GetSellValue(&IDB, &ValueB)) {}
					return this->m_TraderSort ? (IDA > IDB) : (IDA < IDB);
					});
				m_TraderSort ^= 1;

			}
			if (IntoMouse(xpos + PrevSize, yp0, xpos + PerSize, DrawParts->GetScreenY(1080))) {
				WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
					FontPool::FontType::MS_Gothic, LineHeight,
					STRX_RIGHT, STRY_BOTTOM, Pad->GetMS_X(), Pad->GetMS_Y(), RedPop, Black,
					"�g���[�_�["
				);
			}

			PrevSize = PerSize + DXDraw::Instance()->GetUIY(25);
			PerSize += DXDraw::Instance()->GetUIY(250);
			if (WindowSystem::SetMsgClickBox(xpos + PrevSize, yp0, xpos + PerSize, DrawParts->GetScreenY(1080), LineHeight, Gray75, false, true, "")) {
				std::sort(Items.begin(), Items.end(), [&](const ItemList* a, const ItemList* b) {
					TraderID IDA = InvalidID;
					int ValueA = -1;
					if (a->GetSellValue(&IDA, &ValueA)) {}
					TraderID IDB = InvalidID;
					int ValueB = -1;
					if (b->GetSellValue(&IDB, &ValueB)) {}
					return this->m_ValueSort ? (ValueA > ValueB) : (ValueA < ValueB);
					});
				m_ValueSort ^= 1;
			}
			if (IntoMouse(xpos + PrevSize, yp0, xpos + PerSize, DrawParts->GetScreenY(1080))) {
				WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
					FontPool::FontType::MS_Gothic, LineHeight,
					STRX_RIGHT, STRY_BOTTOM, Pad->GetMS_X(), Pad->GetMS_Y(), RedPop, Black,
					"���l"
				);
			}

			PrevSize = PerSize + DXDraw::Instance()->GetUIY(25);
			PerSize += DXDraw::Instance()->GetUIY(250);
			if (WindowSystem::SetMsgClickBox(xpos + PrevSize, yp0, xpos + PerSize, DrawParts->GetScreenY(1080), LineHeight, Gray75, false, true, "")) {
				std::sort(Items.begin(), Items.end(), [&](const ItemList* a, const ItemList* b) {
					TraderID IDA = InvalidID;
					int ValueA = -1;
					if (a->GetSellValue(&IDA, &ValueA)) {}
					int ValueAP = ((a->Getwidth() * a->Getheight()) > 0) ? (ValueA / (a->Getwidth() * a->Getheight())) : 0;

					TraderID IDB = InvalidID;
					int ValueB = -1;
					if (b->GetSellValue(&IDB, &ValueB)) {}
					int ValueBP = ((b->Getwidth() * b->Getheight()) > 0) ? (ValueB / (b->Getwidth() * b->Getheight())) : 0;
					return this->m_ValuePerSort ? (ValueAP > ValueBP) : (ValueAP < ValueBP);
					});
				m_ValuePerSort ^= 1;
			}
			if (IntoMouse(xpos + PrevSize, yp0, xpos + PerSize, DrawParts->GetScreenY(1080))) {
				WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
					FontPool::FontType::MS_Gothic, LineHeight,
					STRX_RIGHT, STRY_BOTTOM, Pad->GetMS_X(), Pad->GetMS_Y(), RedPop, Black,
					"�}�X�P��"
				);
			}
		}
		for (auto& L : Items) {//todo
			if (ListsSel(1) == InvalidID) {
				bool isHit = false;
				for (auto& TL : DataBase::Instance()->GetItemTypeData()->GetList()) {
					if (TL.GetCategoryID() == ListsSel(0) || ListsSel(0) == InvalidID) {
						isHit = (L->GetTypeID() == TL.GetID());
						if (isHit) { break; }
					}
				}
				if (!isHit) { continue; }
			}
			if (L->GetIsPreset()) { continue; }

			if (L->GetTypeID() == ListsSel(1) || ListsSel(1) == InvalidID) {
				bool ishit = true;
				/*
				bool ishit = (ListsSel(2) == InvalidID);
				for (auto& m : L->GetMapID()) {
					if (m.GetID() == ListsSel(2)) {
						ishit = true;
						break;
					}
				}
				if (ListsSel(2) == ElseSelectID) {
					ishit = (L->GetMapID().size() == 0);
				}
				//*/
				if (ishit && m_SearchBox.GetIsHit(L->GetName())) {
					if (((0 - ysize) < yp0) && (yp0 < DrawParts->GetScreenY(1080))) {
						const_cast<ItemList*>(L)->Draw(xpos, yp0, ScrPxItem - xpos - DXDraw::Instance()->GetUIY(36), ysize, 0, Gray75, !WindowMngr->PosHitCheck(nullptr), false, !m_RaidMode, false);
						if (m_RaidMode) {
							TraderID ID = InvalidID;
							int Value = -1;
							if (L->GetSellValue(&ID, &Value)) {
								auto Color = Green;
								std::string TraderName = "Flea Market";
								if (ID != InvalidID) {
									auto* ptr = DataBase::Instance()->GetTraderData()->FindPtr(ID);
									if (ptr) {
										TraderName = ptr->GetName();
										Color = ptr->GetColors(50);
									}
								}
								int PerSize = DXDraw::Instance()->GetUIY(400) + DXDraw::Instance()->GetUIY(250);
								WindowSystem::SetMsg(xpos + PerSize, yp0 + ysize / 2, LineHeight * 9 / 10, STRX_RIGHT, Color, Black, "%s", TraderName.c_str());
								PerSize += DXDraw::Instance()->GetUIY(250);
								WindowSystem::SetMsg(xpos + PerSize, yp0 + ysize / 2, LineHeight * 9 / 10, STRX_RIGHT, Color, Black, "%6d", Value);
								PerSize += DXDraw::Instance()->GetUIY(250);
								WindowSystem::SetMsg(xpos + PerSize, yp0 + ysize / 2, LineHeight * 9 / 10, STRX_RIGHT, Color, Black, "%6d", Value / (L->Getwidth() * L->Getheight()));
							}
						}
					}
					yp0 += ysize;
				}
			}
		}
		yp0 -= ypos - (int)m_YNow;


		int ScrSizY = (DrawParts->GetScreenY(1080) - (DXDraw::Instance()->GetUIY(10) + LineHeight)) - ypos;
		m_Scroll.SetScrollBoxParam(xpos, ypos, ScrPosX, ypos + ScrSizY, (float)std::max(yp0, ScrSizY) / (float)ScrSizY, !WindowMngr->PosHitCheck(nullptr));
		m_Scroll.ScrollBox();

		m_YNow = std::max(0.f, this->m_Scroll.GetNowScrollYPer() * (float)(yp0 - ScrSizY));
		//List
		{
			int xgoal = 0;
			int xs_add = m_ListXSize + DXDraw::Instance()->GetUIY(50);
			bool isChild = false;
			//
			isChild |= MakeLists(0, true, [&](std::pair<int, bool>* IDs, bool IsChild, int XP, int YP, int XS) {
				if (IsChild) { xgoal -= xs_add; }
				DataBase::Instance()->GetItemCategoryData()->DrawList(XP + xgoal, YP, XS, "ItemCategory", &IDs->first, !IDs->second, false, true);
				});
			isChild |= MakeLists(1, ((ListsSel(1) != InvalidID)/* && (DataBase::Instance()->GetItemTypeData()->FindPtr(ListsSel(1))->GetName() == "mechanical-key")*/),
				[&](std::pair<int, bool>* IDs, bool IsChild, int XP, int YP, int XS) {
					if (IsChild) { xgoal -= xs_add; }
					DataBase::Instance()->GetItemTypeData()->DrawList(XP + xgoal, YP, XS, "ItemType", &IDs->first, !IDs->second, false, true, [&](const auto* ptr) { return (ptr->GetCategoryID() == ListsSel(1 - 1)); });
				});
			/*
			isChild |= MakeLists(2, false, [&](std::pair<int, bool>* IDs, bool IsChild, int XP, int YP, int XS) {
				if (IsChild) { xgoal -= xs_add; }
				DataBase::Instance()->GetMapData()->DrawList(XP + xgoal, YP, XS, "Map", &IDs->first, !IDs->second, true, true);
			});
			//*/
			ExecuteLists(isChild, xgoal);
		}
	}
	void ItemBG::DrawFront_Sub(int, int, float) noexcept {
		//
		{
			int xp = DXDraw::Instance()->GetUIY(1910) - LineHeight * 3;
			int yp = DXDraw::Instance()->GetUIY(900);
			m_RaidMode = WindowSystem::CheckBox(xp, yp, m_RaidMode);
			WindowSystem::SetMsg(xp, yp + LineHeight / 2, LineHeight, STRX_RIGHT, White, Black, "���p���i���[�h");
			yp += LineHeight + DXDraw::Instance()->GetUIY(6);
		}
		//����
		m_SearchBox.Draw(DXDraw::Instance()->GetUIY(1910) - DXDraw::Instance()->GetUIY(500), DXDraw::Instance()->GetUIY(850));
	}
};
