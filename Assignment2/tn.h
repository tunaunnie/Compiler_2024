#pragma once
enum tokentypes { TEOF, TCONST, TELSE, TIF, TINT, TFLOAT, TRETURN, TVOID, TWHILE, TEQUAL, 
	TNOTEQU, TLESS, TGREAT, TAND, TOR, TINC, TDEC, TADDASSIGN, TSUBASSIGN, 
	TMULASSIGN, TDIVASSIGN, TMODASSIGN, TPLUS, TMINUS, TSTAR, TSLASH, TMOD, 
	TASSIGN, TIDENT, TNUMBER, TERROR1,TERROR2, TERROR3, TERROR4, LINEPROCESS, TPAREN, 
	TLBRACE, TRBRACE, TSEMICOLON,TCOMMA, TMAIN,TPRINT, TSTRING, TCHAR
};
