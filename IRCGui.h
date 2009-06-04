#pragma once
#include "vgui/vgui.h"
#include "vgui_controls/Frame.h"
#include "ienginevgui.h"
#include "vgui/ischeme.h"
#include "IRCPanel.h"
#include <stdexcept>
#include "vgui/ILocalize.h"
#include <windows.h>


namespace SteamIRC {
	using namespace vgui;

	class CIRCEnvironment;
	class IRCGui
	{
	public:
		IRCGui(CreateInterfaceFn interfaceFactory, CIRCEnvironment& env) throw(std::runtime_error);
		virtual ~IRCGui(void);

		void CreatePanel() throw(std::logic_error);
		void Update();
		void DestroyPanel();
	private:
		IEngineVGui* vguiEngine_;
		ISchemeManager*	 vguiScheme_;
		ILocalize* vguiLocalize_;
		CIRCEnvironment& env_;
		CRITICAL_SECTION cs;
		CIRCPanel* panel_;
	};
}
