#pragma once

#include "vguihelpers.h"
#include <vgui/vgui.h>
#include <vgui_controls/Frame.h>
#include "ienginevgui.h"
#include "vgui/ischeme.h"

using namespace vgui;

class CIRCPanel :
	public Frame
{
	DECLARE_CLASS_SIMPLE(CIRCPanel, Frame);
public:
	CIRCPanel( vgui::VPANEL parent, IEngineVGui* vguiEngine,	ISchemeManager* vguiScheme );
	~CIRCPanel() {};
protected:
	virtual void OnCommand(const char* pcCommand);
};

