#include "pch.h"
#include "Lexer.h"

namespace morph {

	void Lexer::setText(const std::string& source) {
		m_source_code = source;
		m_cursor = m_start = m_source_code.c_str();
		m_current_char = m_source_code[0];
		m_current = 0;
		m_finished = false;
	}

	void Lexer::finish() {
		m_finished = true;
	}

	char Lexer::peek(unsigned int distance) {
		return m_source_code[m_current + distance];
	}

	bool Lexer::isDigit(char d) { return d >= '0' && d <= '9'; }
	bool Lexer::isAlpha(char d) {
		return (d >= 'a' && d <= 'z') || (d >= 'A' && d <= 'Z') || (d == '_');
	}

	size_t Lexer::offsetFromCursor() const {
		return (m_start + m_current) - m_cursor;
	}

	void Lexer::syncCursor() { m_cursor += offsetFromCursor(); }

	Token Lexer::constructToken(TokenType type) {
		std::string_view sv{ m_cursor, offsetFromCursor() };
		syncCursor();
		return Token(type, sv);
	}

	void Lexer::skipWhitespace() {
		while (m_current_char == ' ' || m_current_char == '\t' ||
			m_current_char == '\r' || m_current_char == '\n') {
			advance();
		}
	}

	Token Lexer::nextToken() {
		skipWhitespace();
		syncCursor();
		return getNextToken();
	}

	Token Lexer::getNextToken() {
		while (m_current_char != '\0' && !m_finished) {
			if (isDigit(m_current_char)) {
				return number();
			}

			if (isAlpha(m_current_char)) {
				return identifier();  // reserved keyword or symbol
			}
			// not digit or alpha, just skip

			advance();
			syncCursor();
		}
		return Token(TokenType::END, "");
	}

	Token Lexer::number() {
		while (isDigit(m_current_char)) {
			advance();
		}

		return constructToken(TokenType::NUMBER);
		// Add support for numbers written in scientific notation
	}

	Token Lexer::identifier() {
		//while (isAlpha(m_current_char) || isDigit(m_current_char)) {
		while (isAlpha(m_current_char)) {
			advance();
		}

		std::string_view result{ m_cursor, offsetFromCursor() };

		// If it's a reserved keyword:
		if (result == "morph") return constructToken(TokenType::MORPH);
		if (result == "race") return constructToken(TokenType::RACE);
		if (result == "gender") return constructToken(TokenType::GENDER);
		if (result == "mount") return constructToken(TokenType::MOUNT);
		if (result == "item") return constructToken(TokenType::ITEM);
		if (result == "enchant") return constructToken(TokenType::ENCHANT);
		if (result == "title") return constructToken(TokenType::TITLE);
		if (result == "shapeshift") return constructToken(TokenType::SHAPESHIFT);
		if (result == "commands") return constructToken(TokenType::COMMANDS);
		if (result == "reset") return constructToken(TokenType::RESET);

		return constructToken(TokenType::UNKNOWN);
	}


	void Lexer::advance(unsigned int steps) {
		if (m_current_char == '\0') return;
		m_current += steps;
		m_current_char = m_source_code[m_current];
	}

} // namespace morph