#include "IRCPanel.h"
#include "IRCEnvironment.h"
#include "vgui_controls/TextEntry.h"
#include "vgui_controls/RichText.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/ListPanel.h"
#include "vgui_controls/Button.h"

#include "IRCContext.h"
#include "IRCGui.h"
#include "IRCChannel.h"
#include <string>
#include <sstream>
#include "IRCGui.h"
// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"

namespace SteamIRC {
	struct nullstream:
	  std::ostream {
		nullstream(): std::ios(0), std::ostream(0) {}
	};

	CIRCPanel::CIRCPanel( vgui::VPANEL parent, IEngineVGui* vguiEngine,	ISchemeManager* vguiScheme, CIRCEnvironment& env, ILocalize* loc, IRCGui& gui)
		: BaseClass( NULL, "IRCPanel" ), scheme_(vguiScheme), env_(env), loc_(loc), gui_(gui)
	{
		SetParent(parent);
		SetAutoDelete(false);
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
			cfolk->SetColumnHeaderHeight(vguiScheme->GetProportionalScaledValueEx(GetScheme(), 20));
		}
	}

	CIRCPanel::~CIRCPanel() {}
	void CIRCPanel::OnCommand(const char* pcCommand) {
		if(strcmp(pcCommand, "Close") == 0) {
			Close();
		}
		if(!env_.GetActiveContext()) return;
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

	void CIRCPanel::PerformLayout() {
		Frame::PerformLayout();
		ListPanel* cfolk = static_cast<ListPanel*>(FindChildByName("ChanFolkList"));
		TextEntry* msgf = static_cast<TextEntry*>(FindChildByName("MessageField"));
		Button* send = static_cast<Button*>(FindChildByName("send"));
		Button* closer = static_cast<Button*>(FindChildByName("CloseBtn"));
		if(!cfolk || !msgf || !send) return;

		int wide = this->GetWide();
		int tall = this->GetTall();
		int x;
		int y;

		cfolk->GetPos(x,y);
		cfolk->SetPos(wide - scheme_->GetProportionalScaledValueEx(GetScheme(), 120),y);
		msgf->GetPos(x,y);
		msgf->SetPos(x,tall - scheme_->GetProportionalScaledValueEx(GetScheme(), 24));
		send->SetPos(wide - scheme_->GetProportionalScaledValueEx(GetScheme(), 95), tall - scheme_->GetProportionalScaledValueEx(GetScheme(), 24));
		closer->GetPos(x,y);
		closer->SetPos(wide - scheme_->GetProportionalScaledValueEx(GetScheme(), 20), y);
	}


	void CIRCPanel::Update() {
		UpdateChannelName();
		UpdateOutputField();
		UpdateChanFolkList();
	}

	void CIRCPanel::UpdateChannelName() {
		Label* lbl = static_cast<Label*>(FindChildByName("ChannelName"));
		if(!lbl) return;
		if(env_.GetActiveContext()) {		
			lbl->SetText(env_.GetActiveContext()->GetTitle().c_str());
		}
		else {
			lbl->SetText("#SteamIRC_IRC_Empty");
		}
	}
 
	Color CIRCPanel::ColorPick(std::string& tmp, std::string::size_type& loc) {
		switch(tmp[loc]) {
		case '0':
			switch(tmp[loc+1]) {
				case '0':
					loc += 2; return Color(0xff,0xff,0xff,0xff);
				case '1':
					loc += 2; return Color(0x00,0x00,0x00,0xff);
				case '2':
					loc += 2; return Color(0x00,0x00,0x7f,0xff);
				case '3':
					loc += 2; return Color(0x00,0x93,0x00,0xff);
				case '4':
					loc += 2; return Color(0xff,0x00,0x00,0xff);
				case '5':
					loc += 2; return Color(0x7f,0x00,0x00,0xff);
				case '6':
					loc += 2; return Color(0x9c,0x00,0x9c,0xff);
				case '7':
					loc += 2; return Color(0xfc,0x7f,0x00,0xff);
				case '8':
					loc += 2; return Color(0xff,0xff,0x00,0xff);
				case '9':
					loc += 2; return Color(0x00,0xfc,0x00,0xff);
				default:
					loc += 1; return Color(0xff,0xff,0xff,0xff);
			}
		case '1':
			switch(tmp[loc+1]) {
			case '0':
				loc += 2; return Color(0x00,0x93,0x93,0xff);
			case '1':
				loc += 2; return Color(0x00,0xff,0xff,0xff);
			case '2':
				loc += 2; return Color(0x00,0x00,0xfc,0xff);
			case '3':
				loc += 2; return Color(0xff,0x00,0xff,0xff);
			case '4':
				loc += 2; return Color(0x7f,0x7f,0x7f,0xff);
			case '5':
				loc += 2; return Color(0xd2,0xd2,0xd2,0xff);
			default:
				loc += 1; return Color(0x00,0x00,0x00,0xff);
			}
		case '2':
			loc += 1; return Color(0x00,0x00,0x7f,0xff);
		case '3':
			loc += 1; return Color(0x00,0x93,0x00,0xff);
		case '4':
			loc += 1; return Color(0xff,0x00,0x00,0xff);
		case '5':
			loc += 1; return Color(0x7f,0x00,0x00,0xff);
		case '6':
			loc += 1; return Color(0x9c,0x00,0x9c,0xff);
		case '7':
			loc += 1; return Color(0xfc,0x7f,0x00,0xff);
		case '8':
			loc += 1; return Color(0xff,0xff,0x00,0xff);
		case '9':
			loc += 1; return Color(0x00,0xfc,0x00,0xff);
		default:
			return Color(0xb2,0xb2,0xb2,0xff);
		}
	}

	void CIRCPanel::UpdateOutputField() {
		RichText* out = static_cast<RichText*>(FindChildByName("OutputField"));
		if(!out) return;

		out->SetText("");
		out->InsertColorChange(Color(0xb2,0xb2,0xb2,0xff));
		if(!env_.GetActiveContext()) return;


		std::string tmp(env_.GetActiveContext()->GetBuffer());
		std::ostringstream oss;

		std::string::size_type loc(0);

		while(loc < tmp.size()) {
			switch(tmp[loc]) {
			case 3:
				ConvertAndMoveToOF(out, oss.str());
				oss.str("");
				out->InsertColorChange(ColorPick(tmp, ++loc));
				if(tmp[loc] == ',')
					 ColorPick(tmp,++loc);
				break;
			case 15:
				ConvertAndMoveToOF(out, oss.str());
				oss.str("");
				out->InsertColorChange(Color(0xb2,0xb2,0xb2,0xff));
				loc++;
				break;
			case '\n':
				oss << '\n';
				ConvertAndMoveToOF(out, oss.str());
				oss.str("");
				out->InsertColorChange(Color(0xb2,0xb2,0xb2,0xff));
				loc++;
				break;
			default:
				oss << tmp[loc++];
				break;
			}
		}
		ConvertAndMoveToOF(out, oss.str());
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

	void CIRCPanel::OnClose() {
		gui_.DestroyPanel();
	}

	void CIRCPanel::UpdateChanFolkList() {
		ListPanel* cfolk = static_cast<ListPanel*>(FindChildByName("ChanFolkList"));
		if(!cfolk) return;

		cfolk->DeleteAllItems();
		if(!env_.GetActiveContext()) return;	

		CIRCChannel* chan;
		if(!(chan = dynamic_cast<CIRCChannel*>(env_.GetActiveContext()))) return;
		
		CIRCChannel::chanfolk_set* rcfolk(chan->GetChanFolk());

		for(CIRCChannel::chanfolk_set::const_iterator i = rcfolk->begin(); i != rcfolk->end(); i++) {
			KeyValues* kv = new KeyValues(i->GetDecoratedNick().c_str(), "Names", i->GetDecoratedNick().c_str());
			cfolk->AddItem(kv, 0, false, false);
			kv->deleteThis();
		}
//		cfolk->SortList();
	}

	
}
