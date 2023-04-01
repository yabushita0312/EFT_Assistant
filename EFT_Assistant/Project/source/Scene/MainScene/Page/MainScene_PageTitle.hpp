#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class TitleBG :public BGParent {
	private:
		BGSelect m_Select{ (BGSelect)0 };
		std::string strResult;
		int ttt = -1;
	private:
		void DrawOnce(int xpos1, int ypos1, int xpos2, int ypos2, const char* Name, BGSelect SelWindow) noexcept {
			if (WindowSystem::ClickCheckBox(xpos1, ypos1, xpos2, ypos2, false, true, Gray25, Name)) {
				m_Select = SelWindow;
				TurnOnGoNextBG();
			}
		}
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
			m_Select = (BGSelect)0;
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {}
		void Draw_Back_Sub(int, int, float) noexcept override {
			int xsize = y_r(420);
			int ysize = y_r(52);
			int ypos = y_r(540);
			DrawOnce(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, "タスク", BGSelect::Task);
			ypos += y_r(100);
			DrawOnce(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, "ハイドアウト", BGSelect::HideOut);
			ypos += y_r(100);
			DrawOnce(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, "アイテム", BGSelect::Item);
			ypos += y_r(100);
			DrawOnce(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, "マップ", BGSelect::Map);
			ypos += y_r(100);
			DrawOnce(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, "カスタマイズ", BGSelect::Custom);
			ypos += y_r(100);
		}
		void DrawFront_Sub(int, int, float) noexcept override {
			int Xsize = y_r(400);
			int Ysize = LineHeight;
			int XPos = y_r(1920) - Xsize - y_r(10);
			int YPos = LineHeight + y_r(10);

			if (ttt != -1) {
				if ((GetNowCount() - ttt) > 1000 * 60 * 5) {
					ttt = -1;
				}
			}
			auto TimeCard = [&]() {
				time_t t = time(NULL);				// 現在日時を取得する
				tm local;							// 日時情報を格納する変数を用意する
				localtime_s(&local, &t);			// ローカル日時を変数に格納する
				char buffer[256];
				strftime(buffer, sizeof(buffer), "%Y %m/%d %H:%M", &local);
				PlayerData::Instance()->SetLastDataReceive(buffer);
				ttt = GetNowCount();
			};
			WindowSystem::SetMsg(XPos, YPos, XPos + Xsize, YPos + Ysize, Ysize, STRX_MID, White, Black, "データをAPIから更新");
			YPos += Ysize + y_r(5);
			if (WindowSystem::ClickCheckBox(XPos, YPos, XPos + Xsize, YPos + Ysize, false, (ttt == -1), Gray25, "アイテム更新")) {
				int count = 0;
				//ItemData::Instance()->InitDatabyJson();
				while (true) {
					if (ItemDataRequest(20 * count, 20, strResult)) {
						ProcessMessage();
						auto data = nlohmann::json::parse(strResult);
						ItemData::Instance()->GetJsonData(data);
						ItemData::Instance()->SaveDatabyJson();
						if (data["data"]["items"].size() != 20) {
							break;
						}
					}
					count++;
				}
				ItemData::Instance()->CheckThroughJson();
				TimeCard();
			}
			YPos += Ysize + y_r(5);
			if (WindowSystem::ClickCheckBox(XPos, YPos, XPos + Xsize, YPos + Ysize, false, (ttt == -1), Gray25, "タスク更新")) {
				int count = 0;
				TaskData::Instance()->InitDatabyJson();
				while (true) {
					if (TaskDataRequest(20 * count, 20, strResult)) {
						ProcessMessage();
						auto data = nlohmann::json::parse(strResult);
						TaskData::Instance()->GetJsonData(data);
						TaskData::Instance()->SaveDatabyJson();
						if (data["data"]["tasks"].size() != 20) {
							break;
						}
					}
					count++;
				}
				TaskData::Instance()->CheckThroughJson();
				TimeCard();
			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	public:
		const auto& GetNextSelect() const noexcept { return m_Select; }
	};
};