#ifndef LEXICAL_REPORT_H
#define LEXICAL_REPORT_H

#include <string>
#include <vector>

#include "../lexer/Token.h"
#include "../lexer/Lexer.h"

bool writeLexicalHtmlReport(
    const std::string& outputPath,
    const std::vector<Token>& tokens,
    const std::vector<LexicalError>& errors
);

#endif
