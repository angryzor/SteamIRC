#pragma once

#include "vguihelpers.h"
#include <vgui/vgui.h>
#include <vgui_controls/Frame.h>
#include "ienginevgui.h"
#include "vgui/ischeme.h"
#include <string>

namespace SteamIRC {
	using namespace vgui;

	class CIRCEnvironment;
	class vgui::RichText;
	class IRCGui;
	class CIRCPanel :
		public Frame
	{
		DECLARE_CLASS_SIMPLE(CIRCPanel, Frame);
	public:
		CIRCPanel( vgui::VPANEL parent, IEngineVGui* vguiEngine,	ISchemeManager* vguiScheme , CIRCEnvironment& env, ILocalize* loc, IRCGui& gui_);
		virtual ~CIRCPanel();
		void Update();
		void UpdateChannelName();
		void UpdateOutputField();
		void UpdateChanFolkList();
		virtual void PerformLayout();
		Color ColorPick(std::string& tmp, std::string::size_type& loc);
		void ConvertAndMoveToOF(RichText* out, std::string txt);
		virtual void OnClose();
	protected:
		virtual void OnCommand(const char* pcCommand);
	private:
		CIRCEnvironment& env_;
		ILocalize* loc_;
		ISchemeManager* scheme_;
		IRCGui& gui_;
	};

}
