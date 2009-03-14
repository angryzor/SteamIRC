#include "IRCPanel.h"
#include "vgui_controls/TextEntry.h"

// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"

CIRCPanel::CIRCPanel( vgui::VPANEL parent, IEngineVGui* vguiEngine,	ISchemeManager* vguiScheme)
	: BaseClass( NULL, "IRCPanel" )
{
	SetParent(parent);
	SetProportional( true );
	SetTitleBarVisible( true );
	SetMinimizeButtonVisible( true );
	SetMaximizeButtonVisible( true );
	SetCloseButtonVisible( true );
	SetSizeable( true );
	SetMoveable( true );
	SetVisible( true );
	SetPos(20,20);
	SetSize(500,300);
	SetScheme( vguiScheme->LoadSchemeFromFile("resource/ClientScheme.res", "ClientScheme") );
	LoadControlSettings("resource/UI/SteamIRC.res");
	SetPaintBackgroundType(2);

//	LoadControlSettings("Resource/UI/TestPanel.res");
//	CenterThisPanelOnScreen();//keep in mind, hl2 supports widescreen 

	//Other useful options
	//SetSizeable(false);
	//SetMoveable(false);
}

void CIRCPanel::OnCommand(const char* pcCommand) {
	if(strcmp(pcCommand, "Send") == 0) {
		TextEntry* msgf = static_cast<TextEntry*>(FindChildByName("MessageField"));
		char msg[501];
		msgf->GetText(msg, 500);

	}
}
