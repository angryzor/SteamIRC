#pragma once
#include "vgui/vgui.h"
#include "vgui_controls/Frame.h"
#include "ienginevgui.h"
#include "vgui/ischeme.h"
#include "IRCPanel.h"
#include <stdexcept>
#include "vgui/ILocalize.h"


using namespace vgui;

class IRCGui
{
public:
	IRCGui(CreateInterfaceFn interfaceFactory) throw(std::runtime_error);
	virtual ~IRCGui(void);

	void CreatePanel() throw(std::logic_error);
	void DestroyPanel();
private:
	IEngineVGui* vguiEngine_;
	ISchemeManager*	 vguiScheme_;
	ILocalize* vguiLocalize_;

	CIRCPanel* panel_;
};
