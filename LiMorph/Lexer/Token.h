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
	FLOAT,
	STRING,
	SCALE,
	NPC,
	CUSTOMIZATIONS,
	DISABLEMETA,
	NPCID,
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
		case TokenType::FLOAT:
			return "FLOAT";
		case TokenType::STRING:
			return "STRING";
		case TokenType::NPC:
			return "NPC";
		case TokenType::NPCID:
			return "NPCID";
		case TokenType::CUSTOMIZATIONS:
			return "CUSTOMIZATONS";
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

	float toFloat() const {
		float dbl;
		auto result = std::from_chars(m_str.data(), m_str.data() + m_str.size(), dbl);
		return dbl;
	}

private:
	TokenType m_type;
	std::string_view m_str;
};

} // namespace morph

