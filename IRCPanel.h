#pragma once

#include "vguihelpers.h"
#include <vgui/vgui.h>
#include <vgui_controls/Frame.h>
#include "ienginevgui.h"
#include "vgui/ischeme.h"

namespace SteamIRC {
	using namespace vgui;

	class CIRCEnvironment;
	class CIRCPanel :
		public Frame
	{
		DECLARE_CLASS_SIMPLE(CIRCPanel, Frame);
	public:
		CIRCPanel( vgui::VPANEL parent, IEngineVGui* vguiEngine,	ISchemeManager* vguiScheme , CIRCEnvironment& env, ILocalize* loc);
		~CIRCPanel() {};
		void Update();
	protected:
		virtual void OnCommand(const char* pcCommand);
	private:
		CIRCEnvironment& env_;
		ILocalize* loc_;
	};

}
