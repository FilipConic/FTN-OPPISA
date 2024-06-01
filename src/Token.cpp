/**
 * Autor: Filip Čonić
 * Datum: 27. 05. 2024.
 */

#include <iostream>
#include <iomanip>

#include "Token.h"
#include "FiniteStateMachine.h"

using namespace std;


TokenType Token::getType()
{
	return tokenType;
}


void Token::setType(TokenType t)
{
	tokenType = t;
}


string Token::getValue()
{
	return value;
}


void Token::setValue(string s)
{
	value = s;
}


void Token::makeToken(int begin, int end, std::vector<char>& programBuffer,  int lastFiniteState)
{
	string _value = "";
	for (int i = begin; i < end; i++)
	{
		_value += programBuffer[i];
	}
	value = _value;
	tokenType = FiniteStateMachine::getTokenType(lastFiniteState);
}


void Token::makeErrorToken(int pos, std::vector<char>& programBuffer)
{
	tokenType = T_ERROR;
	value = programBuffer[pos];
}


void Token::makeEofToken()
{
	tokenType = T_END_OF_FILE;
	value = "EOF";
}


void Token::printTokenInfo()
{
	cout << setw(LEFT_ALIGN) << left << tokenTypeToString(tokenType);
	cout << setw(RIGHT_ALIGN) << right << value << endl;
}


void Token::printTokenValue()
{
	cout << value << endl;
}


string Token::tokenTypeToString(TokenType t)
{
	switch (t)
	{
		case T_NO_TYPE:			return "[T_NO_TYPE]";
		case T_ID:				return "[T_ID]";
		case T_M_ID:			return "[T_M_ID]";
		case T_R_ID:			return "[T_R_ID]";
		case T_NUM:				return "[T_NUM]";
		case T_WHITE_SPACE:		return "[T_WHITE_SPACE]";
		case T_MEM:				return "[T_MEM]";
		case T_REG:				return "[T_REG]";
		case T_FUNC:			return "[T_FUNC]";
		case T_ADD:				return "[T_ADD]";
		case T_ADDI:			return "[T_ADDI]";
		case T_SUB:				return "[T_SUB]";
		case T_LA:				return "[T_LA]";
		case T_LI:				return "[T_LI]";
		case T_LW:				return "[T_LW]";
		case T_SW:				return "[T_SW]";
		case T_BLTZ:			return "[T_BLTZ]";
		case T_B:				return "[T_B]";
		case T_NOP:				return "[T_NOP]";
		case T_COMMA:			return "[T_COMMA]";
		case T_L_PARENT:		return "[T_L_PARENT]";
		case T_R_PARENT:		return "[T_R_PARENT]";
		case T_COL:				return "[T_COL]";
		case T_SEMI_COL:		return "[T_SEMI_COL]";
		case T_COMMENT:			return "[T_COMMENT]";
		case T_END_OF_FILE:		return "[T_END_OF_FILE]";
		case T_ERROR:			return "[T_ERROR]";
		case T_AND:             return "[T_AND]";
		case T_OR:              return "[T_OR]";
		case T_NOT:             return "[T_NOT]";
		default:				return "";
	}
}

string tokenTypeToString(TokenType t)
{
	switch (t)
	{
	case T_NO_TYPE:			return "[T_NO_TYPE]";
	case T_ID:				return "[T_ID]";
	case T_M_ID:			return "[T_M_ID]";
	case T_R_ID:			return "[T_R_ID]";
	case T_NUM:				return "[T_NUM]";
	case T_WHITE_SPACE:		return "[T_WHITE_SPACE]";
	case T_MEM:				return "[T_MEM]";
	case T_REG:				return "[T_REG]";
	case T_FUNC:			return "[T_FUNC]";
	case T_ADD:				return "[T_ADD]";
	case T_ADDI:			return "[T_ADDI]";
	case T_SUB:				return "[T_SUB]";
	case T_LA:				return "[T_LA]";
	case T_LI:				return "[T_LI]";
	case T_LW:				return "[T_LW]";
	case T_SW:				return "[T_SW]";
	case T_BLTZ:			return "[T_BLTZ]";
	case T_B:				return "[T_B]";
	case T_NOP:				return "[T_NOP]";
	case T_COMMA:			return "[T_COMMA]";
	case T_L_PARENT:		return "[T_L_PARENT]";
	case T_R_PARENT:		return "[T_R_PARENT]";
	case T_COL:				return "[T_COL]";
	case T_SEMI_COL:		return "[T_SEMI_COL]";
	case T_COMMENT:			return "[T_COMMENT]";
	case T_END_OF_FILE:		return "[T_END_OF_FILE]";
	case T_ERROR:			return "[T_ERROR]";
	case T_AND:             return "[T_AND]";
	case T_OR:              return "[T_OR]";
	case T_NOT:             return "[T_NOT]";
	default:				return "";
	}
}
