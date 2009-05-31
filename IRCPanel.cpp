#include "IRCPanel.h"
#include "IRCEnvironment.h"
#include "vgui_controls/TextEntry.h"
#include "vgui_controls/RichText.h"
#include "StdString.h"
// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"

namespace SteamIRC {
	CIRCPanel::CIRCPanel( vgui::VPANEL parent, IEngineVGui* vguiEngine,	ISchemeManager* vguiScheme, CIRCEnvironment& env)
		: BaseClass( NULL, "IRCPanel" ), env_(env)
	{
		SetParent(parent);
		SetProportional( true );
		SetTitleBarVisible( true );
		SetMinimizeButtonVisible( false );
		SetMaximizeButtonVisible( false );
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
			if(msgf) {
				char msg[501];
				msgf->GetText(msg, 500);
			}

		}
	}

	void CIRCPanel::Update() {
		RichText* out = static_cast<RichText*>(FindChildByName("OutputField"));
		if(out) {
			out->SetText(env_.GetActiveContext()->GetBuffer().c_str());
			out->GotoTextEnd();
		}
	}
}
