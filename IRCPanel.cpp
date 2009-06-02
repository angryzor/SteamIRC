#include "IRCPanel.h"
#include "IRCEnvironment.h"
#include "vgui_controls/TextEntry.h"
#include "vgui_controls/RichText.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/ListPanel.h"

#include "IRCContext.h"
#include "IRCGui.h"
#include "IRCChannel.h"
#include <string>
#include <math>
// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"

#define IS_NUMBER(x) (x >= 30 && x <= 39)

namespace SteamIRC {
	CIRCPanel::CIRCPanel( vgui::VPANEL parent, IEngineVGui* vguiEngine,	ISchemeManager* vguiScheme, CIRCEnvironment& env, ILocalize* loc)
		: BaseClass( NULL, "IRCPanel" ), env_(env), loc_(loc)
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
//		SetSize(500,300);
		SetScheme( vguiScheme->LoadSchemeFromFile("resource/ClientScheme.res", "ClientScheme") );
		LoadControlSettings("resource/UI/SteamIRC.res");
		SetPaintBackgroundType(2);

		ListPanel* cfolk = static_cast<ListPanel*>(FindChildByName("ChanFolkList"));
		if(cfolk) {
			cfolk->AddColumnHeader(0, "Names", "#SteamIRC_IRC_ChannelUserList", 73, 3);
		}
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
				env_.UserInput(std::string(msg));
				msgf->SetText("");
			}
		}
		if(strcmp(pcCommand, "Next") == 0) {
			CIRCEnvironment::context_set* ctxts = env_.GetContexts();
			CIRCEnvironment::context_set::iterator i = ctxts->find(env_.GetActiveContext());
			i++;
			if(i == ctxts->end()) i = ctxts->begin();
			env_.SetActiveContext(*i);
			env_.GetGui()->Update();
		}
		if(strcmp(pcCommand, "Prev") == 0) {
			CIRCEnvironment::context_set* ctxts = env_.GetContexts();
			CIRCEnvironment::context_set::iterator i = ctxts->find(env_.GetActiveContext());
			if(i == ctxts->begin()) {
				i = ctxts->end();
			}
			i--;
			env_.SetActiveContext(*i);
			env_.GetGui()->Update();
		}
	}


	void CIRCPanel::Update() {
		if(!env_.GetActiveContext()) return;

		UpdateChannelName();
		UpdateOutputField();
		UpdateChanFolkList();
	}

	void CIRCPanel::UpdateChannelName() {
		Label* lbl = static_cast<Label*>(FindChildByName("ChannelName"));
		if(!lbl) return;
		
		lbl->SetText(env_.GetActiveContext()->GetTitle().c_str());
	}

	void CIRCPanel::UpdateOutputField() {
		RichText* out = static_cast<RichText*>(FindChildByName("OutputField"));
		if(!out) return;

		std::string tmp(env_.GetActiveContext()->GetBuffer());
		std::string::size_type from(0);
		std::string::size_type loc(0);
		while((loc = min(tmp.find("\x03",from), tmp.find("\x15",from))) != tmp.npos) {
			ConvertAndMoveToOF(out, tmp.substr(from, loc-from));

			// UGLY UGLY UGLY
			if(tmp[loc] == '\x03') {
				std::string col;
				std::string colback;
				if(!IS_NUMBER(tmp[loc+1])) {
					col = "-1";
					colback = "-1";
				}
				else if(tmp[loc+2] == ',' && IS_NUMBER(tmp[loc+3])) {
					col = tmp.substr(loc+1,1);
					if(IS_NUMBER(tmp[loc+4])) {
						colback = tmp.substr(loc+3,2);
						from = loc+5;
					}
					else {
						colback = tmp.substr(loc+3,1);
						from = loc+4;
					}
				}
				else if(IS_NUMBER(tmp[loc+2]) && tmp[loc+3] == ',' && IS_NUMBER(tmp[loc+4])) col = tmp.substr(loc+1,2);

				

		}
		out->GotoTextEnd();
	}

	void CIRCPanel::ConvertAndMoveToOF(RichText* out, std::string txt) {
		wchar_t* buf;
        std::string::size_type len = txt.length() + 1;
		buf = new wchar_t [len];
		loc_->ConvertANSIToUnicode(txt.c_str(), buf, len*2);
		out->InsertString(buf);
		delete [] buf;
	}

	void CIRCPanel::UpdateChanFolkList() {
		ListPanel* cfolk = static_cast<ListPanel*>(FindChildByName("ChanFolkList"));
		if(!cfolk) return;

		CIRCChannel* chan;
		cfolk->DeleteAllItems();
		if(!(chan = dynamic_cast<CIRCChannel*>(env_.GetActiveContext()))) return;
		
		CIRCChannel::chanfolk_set* rcfolk(chan->GetChanFolk());

		for(CIRCChannel::chanfolk_set::const_iterator i = rcfolk->begin(); i != rcfolk->end(); i++) {
			KeyValues* kv = new KeyValues(i->GetDecoratedNick().c_str(), "Names", i->GetDecoratedNick().c_str());
			cfolk->AddItem(kv, 0, false, false);
			kv->deleteThis();
		}
		cfolk->SortList();
	}

	
}
