#pragma once

#include <string>

#include "Token.h"

namespace morph {

class Lexer
{
public:
	Lexer() {}
	~Lexer() {}
	void setText(const std::string& source);
	void finish();
	Token nextToken();
private:
	Token number();
	Token identifier();

	void advance(unsigned int steps = 1);
	void skipWhitespace();
	inline char peek(unsigned int distance = 1);
	inline bool isDigit(char d);
	inline bool isAlpha(char d);
	Token getNextToken();
	inline size_t offsetFromCursor() const;
	inline void syncCursor();
	inline Token constructToken(TokenType type);

	std::string m_source_code;
	const char* m_start;
	const char* m_cursor;
	size_t m_current = 0;
	char m_current_char;
	bool m_finished;
};
} // namsspace morph

