%skeleton "lalr1.cc"                          /*  -*- C++ -*- */
%require "2.4.1"
%defines
%define parser_class_name "irc_color_parser"
%parse-param { RichText* out }

%code requires {
	class RichText;
}

%union
{
	char cval;
	int ival;
}

%code {
#include "vgui_controls/RichText.h"
}


%token IRC_CTRLC;
%token IRC_CTRLO;
%token <cval> IRC_CDATA;
%token <ival> IRC_COLORID;

%%
%start content

content: /* empty */
		| colorchange content
		| IRC_CDATA content { out->InsertChar(static_cast<wchar_t>($1)); }
		| normalreturn content

colorchange: IRC_CTRLC IRC_COLORID { switch($2) {
	case 0:
	    out->InsertColorChange(Color(0xff,0xff,0xff));
	case 1:
		out->InsertColorChange(Color(0x00,0x00,0x00));
	case 2:
		out->InsertColorChange(Color(0x00,0x00,0x7f));
	case 3:
		out->InsertColorChange(Color(0x00,0x93,0x00));
	case 4:
		out->InsertColorChange(Color(0xff,0x00,0x00));
	case 5:
		out->InsertColorChange(Color(0x7f,0x00,0x00));
	case 6:
		out->InsertColorChange(Color(0x9c,0x00,0x9c));
	case 7:
		out->InsertColorChange(Color(0xfc,0x7f,0x00));
	case 8:
		out->InsertColorChange(Color(0xff,0xff,0x00));
	case 9:
		out->InsertColorChange(Color(0x00,0xfc,0x00));
	case 10:
		out->InsertColorChange(Color(0x00,0x93,0x93));
	case 11:
		out->InsertColorChange(Color(0x00,0xff,0xff));
	case 12:
		out->InsertColorChange(Color(0x00,0x00,0xfc));
	case 13:
		out->InsertColorChange(Color(0xff,0x00,0xff));
	case 14:
		out->InsertColorChange(Color(0x7f,0x7f,0x7f));
	case 15:
		out->InsertColorChange(Color(0xd2,0xd2,0xd2));
}
			| IRC_CTRLC IRC_COLORID ',' IRC_COLORID { switch($2) {
	case 0:
	    out->InsertColorChange(Color(0xff,0xff,0xff));
	case 1:
		out->InsertColorChange(Color(0x00,0x00,0x00));
	case 2:
		out->InsertColorChange(Color(0x00,0x00,0x7f));
	case 3:
		out->InsertColorChange(Color(0x00,0x93,0x00));
	case 4:
		out->InsertColorChange(Color(0xff,0x00,0x00));
	case 5:
		out->InsertColorChange(Color(0x7f,0x00,0x00));
	case 6:
		out->InsertColorChange(Color(0x9c,0x00,0x9c));
	case 7:
		out->InsertColorChange(Color(0xfc,0x7f,0x00));
	case 8:
		out->InsertColorChange(Color(0xff,0xff,0x00));
	case 9:
		out->InsertColorChange(Color(0x00,0xfc,0x00));
	case 10:
		out->InsertColorChange(Color(0x00,0x93,0x93));
	case 11:
		out->InsertColorChange(Color(0x00,0xff,0xff));
	case 12:
		out->InsertColorChange(Color(0x00,0x00,0xfc));
	case 13:
		out->InsertColorChange(Color(0xff,0x00,0xff));
	case 14:
		out->InsertColorChange(Color(0x7f,0x7f,0x7f));
	case 15:
		out->InsertColorChange(Color(0xd2,0xd2,0xd2));
}
			
normalreturn: IRC_CTRLO

%%
void
     yy::irc_color_parser::error (const yy::calcxx_parser::location_type& l,
                               const std::string& m)
     {
     }

