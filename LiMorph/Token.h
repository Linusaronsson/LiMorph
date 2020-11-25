#pragma once

#include <string_view>
#include <charconv>

namespace LiMorph {

enum class TokenType : uint8_t
{
	RESET,
	COMMANDS,
	MORPH,
	RACE,
	GENDER,
	ITEM,
	ENCHANT,
	TITLE,
	SHAPESHIFT,
	MOUNT,
	NUMBER,
	END,
	UNKNOWN
};

class Token
{
public:
	Token(TokenType type, const std::string_view& str)
		: m_type{ type }, m_str{ str } {}
	TokenType type() const { return m_type; }
	std::string toString() const { return std::string(m_str); }

	static std::string tokenTypeToString(TokenType type) {
		switch (type) {
		case TokenType::RESET:
			return "RESET";
		case TokenType::COMMANDS:
			return "COMMANDS";
		case TokenType::MORPH:
			return "MORPH";
		case TokenType::RACE:
			return "RACE";
		case TokenType::ITEM:
			return "ITEM";
		case TokenType::MOUNT:
			return "MOUNT";
		case TokenType::NUMBER:
			return "NUMBER";
		case TokenType::END:
			return "END";
		case TokenType::UNKNOWN:
			return "UNKNOWN";
		default:
			return "UNREACHABLE TOKENTYPE";
		}
	}
	int toNumber() const {
		int num;
		auto result = std::from_chars(m_str.data(), m_str.data() + m_str.size(), num);
		return num;
	}
private:
	TokenType m_type;
	std::string_view m_str;
};

} // namespace morph

