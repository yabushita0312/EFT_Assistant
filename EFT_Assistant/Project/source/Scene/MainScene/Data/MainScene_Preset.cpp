#include "MainScene_Preset.hpp"
#include "../Data/MainScene_Base.hpp"

namespace FPS_n2 {
	void	PresetList::Load_Sub() noexcept {
		m_Base = DataBase::Instance()->GetItemData()->FindPtr(DataBase::Instance()->GetItemData()->FindID(m_BaseIDBuffer.c_str()));
		if (!m_Base) { return; }
		for (auto& P : this->m_PartsIDBuffer) {
			m_Parts.emplace_back(DataBase::Instance()->GetItemData()->FindPtr(DataBase::Instance()->GetItemData()->FindID(P.c_str())));
		}
		if (GetName().find("Default") != std::string::npos) {
			SetShortName("デフォルト");
		}
		else if (m_Base->GetName().length() < GetName().length()) {
			SetShortName(GetName().substr(m_Base->GetName().length()));
		}
	}
};
