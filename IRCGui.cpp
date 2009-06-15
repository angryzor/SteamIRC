#include "IRCGui.h"
#include "CSLock.h"
#include "logging.h"
#include <boost/logging/format/named_write.hpp>

#include "tier0/memdbgon.h"

namespace SteamIRC {

	IRCGui::IRCGui(CreateInterfaceFn interfaceFactory, CIRCEnvironment& env) throw(std::runtime_error) : panel_(NULL), vguiEngine_(NULL), vguiScheme_(NULL), env_(env)
	{
		InitializeCriticalSection(&cs);
		vguiEngine_ = (IEngineVGui *)interfaceFactory(VENGINE_VGUI_VERSION, NULL);
		if(vguiEngine_)
			Msg(" Got reference to VGUI engine! (%x)\r\n", vguiEngine_);
		else throw std::runtime_error(" Unable to acquire VGUI engine reference!\r\n");

		vguiScheme_ = (ISchemeManager *)interfaceFactory(VGUI_SCHEME_INTERFACE_VERSION, NULL);
		if(vguiScheme_)
			Msg(" Got reference to VGUI scheme interface! (%x)\r\n", vguiScheme_);
		else throw std::runtime_error(" Unable to acquire VGUI scheme interface reference!\r\n");

		vguiLocalize_ = (ILocalize *)interfaceFactory(VGUI_LOCALIZE_INTERFACE_VERSION, NULL);
		if(vguiLocalize_)
			Msg(" Got reference to VGUI localization interface! (%x)\r\n", vguiLocalize_);
		else throw std::runtime_error(" Unable to acquire VGUI localization interface reference!\r\n");


		// Load localizations
		vguiLocalize_->AddFile("resource/steamirc_%language%.txt", "GAME");
	}

	IRCGui::~IRCGui(void)
	{
		DestroyPanel();
		DeleteCriticalSection(&cs);
	}

	void IRCGui::CreatePanel(void) throw(std::logic_error) {
		L_ << "CIRCGui::CreatePanel() <-";
		{
			CSLock csl(cs);
			if(panel_) throw std::logic_error("Panel already created");

			Msg( "vguiEngine PANEL_GAMEUIDLL = %d\r\n", vguiEngine_->GetPanel( PANEL_GAMEUIDLL ));

			panel_ = new CIRCPanel(vguiEngine_->GetPanel( PANEL_GAMEUIDLL ), vguiEngine_, vguiScheme_, env_, vguiLocalize_, *this);
		}
		L_ << "CIRCGui::CreatePanel() ->";
	}

	void IRCGui::DestroyPanel(void) {
		L_ << "CIRCGui::DestroyPanel() <-";
		{
			CSLock csl(cs);
			if(!panel_) return;

			delete panel_;
			panel_ = NULL;
		}
		L_ << "CIRCGui::DestroyPanel() ->";
	}

	void IRCGui::Update() {
		L_ << "CIRCGui::Update() <-";
		{
			CSLock csl(cs);
			if(panel_)
				panel_->Update();
		}
		L_ << "CIRCGui::Update() ->";
	}

}
