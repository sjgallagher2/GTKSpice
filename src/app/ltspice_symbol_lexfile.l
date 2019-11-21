// flexc++ file for LTSpice symbol file parsing
%case-insensitive
%filenames =  "ltspice_symbol_lexer"
%class-name = "LTSpiceSymbolLexer"
%lex-source = "ltspice_symbol_lexer.cpp"
//%print-tokens

%%


<<EOF>>                     {return once_++ ? 0 : '\n';} // Stop parser (see https://stackoverflow.com/questions/1756275/bison-end-of-file)
[ \t]+                      return ' '; // Reduce whitespace


version              {sval_ = matched(); return LTSpiceSymbolParser::VERSION;}
symboltype           {sval_ = matched(); return LTSpiceSymbolParser::SYMBOLTYPE;}
line                 {sval_ = matched(); return LTSpiceSymbolParser::LINE;}
rectangle            {sval_ = matched(); return LTSpiceSymbolParser::RECTANGLE;}
circle               {sval_ = matched(); return LTSpiceSymbolParser::CIRCLE;}
arc                  {sval_ = matched(); return LTSpiceSymbolParser::ARC;}
text                 {sval_ = matched(); return LTSpiceSymbolParser::TEXT;}
window               {sval_ = matched(); return LTSpiceSymbolParser::WINDOW;}
symattr              {sval_ = matched(); return LTSpiceSymbolParser::SYMATTR;}
pin                  {sval_ = matched(); return LTSpiceSymbolParser::PIN;}
pinattr              {sval_ = matched(); return LTSpiceSymbolParser::PINATTR;}

// Strings
[a-zA-Z_\+\=\(\)\*\<\>\/\[\]\!\@\#\$\%\^\&\'\"][a-zA-Z0-9_\+\-\=\(\)\*\<\>\/\[\]\!\@\#\$\%\^\&\'\"\.]*       {sval_ = matched(); return LTSpiceSymbolParser::STRING;}
[-]/[ ]  {sval_ = matched(); return LTSpiceSymbolParser::STRING;}
[\n]+                       return matched()[0];

// Integers 
[\-]?[0-9]+   {ival_ = std::stoi(matched()); return LTSpiceSymbolParser::INT;}
