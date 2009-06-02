%option noyywrap nounput batch c++
%%
ANYTHING	.
COLORID		[:digit:][:digit:]?
CTRLC		\x03
CTRLO		\x15

%%
%{
    typedef yy::calcxx_parser::token token;
%}

COLORID		{	istringstream iss(YYText());
				int col(0);
				iss >> col;
				yylval->ival = col;
				return token::IRC_COLORID }
CTRLC		return token::IRC_CTRLC;
CTRLO		return token::IRC_CTRLO;
ANYTHING	{	yylval->cval = YYText()[0];
				return token::IRC_CDATA; }
