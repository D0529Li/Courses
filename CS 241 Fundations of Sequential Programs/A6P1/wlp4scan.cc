#include <sstream>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <utility>
#include <set>
#include <array>
#include <vector>
#include <climits>
#include "wlp4scan.h"

/*
 * C++ Starter code for CS241 A3
 * All code requires C++14, so if you're getting compile errors make sure to
 * use -std=c++14.
 *
 * This file contains helpers for asm.cc and you don't need to modify it.
 * Furthermore, while this code may be helpful to understand starting with
 * the DFA assignments, you do not need to understand it to write the assembler.
 */

Token::Token(Token::Kind kind, std::string lexeme) : kind(kind), lexeme(std::move(lexeme)) {}

Token::Kind Token::getKind() const { return kind; }

const std::string& Token::getLexeme() const { return lexeme; }
void Token::modifyID() {
	if (kind == Kind::ID) {
		if (lexeme == "return") kind = RETURN;
		else if (lexeme == "if") kind = IF;
		else if (lexeme == "else") kind = ELSE;
		else if (lexeme == "while") kind = WHILE;
		else if (lexeme == "println") kind = PRINTLN;
		else if (lexeme == "wain") kind = WAIN;
		else if (lexeme == "int") kind = INT;
		else if (lexeme == "new") kind = NEW;
		else if (lexeme == "delete") kind = DELETE;
		else if (lexeme == "NULL") kind = NUL;
	}
}

std::ostream& operator<<(std::ostream& out, const Token& tok)
{
	switch (tok.getKind())
	{
	case Token::ID:
		out << "ID";
		break;
	case Token::NUM:
		out << "NUM";
		break;
	case Token::NUMZERO:
		out << "NUM";
		break;
	case Token::LPAREN:
		out << "LPAREN";
		break;
	case Token::RPAREN:
		out << "RPAREN";
		break;
	case Token::LBRACE:
		out << "LBRACE";
		break;
	case Token::RBRACE:
		out << "RBRACE";
		break;
	case Token::BECOMES:
		out << "BECOMES";
		break;
	case Token::EQ:
		out << "EQ";
		break;
	case Token::NE:
		out << "NE";
		break;
	case Token::LT:
		out << "LT";
		break;
	case Token::GT:
		out << "GT";
		break;
	case Token::LE:
		out << "LE";
		break;
	case Token::GE:
		out << "GE";
		break;
	case Token::PLUS:
		out << "PLUS";
		break;
	case Token::MINUS:
		out << "MINUS";
		break;
	case Token::STAR:
		out << "STAR";
		break;
	case Token::SLASH:
		out << "SLASH";
		break;
	case Token::PCT:
		out << "PCT";
		break;
	case Token::COMMA:
		out << "COMMA";
		break;
	case Token::SEMI:
		out << "SEMI";
		break;
	case Token::LBRACK:
		out << "LBRACK";
		break;
	case Token::RBRACK:
		out << "RBRACK";
		break;
	case Token::AMP:
		out << "AMP";
		break;
	case Token::WHITESPACE:
		out << "WHITESPACE";
		break;
	case Token::COMMENT:
		out << "COMMENT";
		break;
	case Token::RETURN:
		out << "RETURN";
		break;
	case Token::IF:
		out << "IF";
		break;
	case Token::ELSE:
		out << "ELSE";
		break;
	case Token::WHILE:
		out << "WHILE";
		break;
	case Token::PRINTLN:
		out << "PRINTLN";
		break;
	case Token::WAIN:
		out << "WAIN";
		break;
	case Token::INT:
		out << "INT";
		break;
	case Token::NEW:
		out << "NEW";
		break;
	case Token::DELETE:
		out << "DELETE";
		break;
	case Token::NUL:
		out << "NULL";
		break;
	}
	out << " " << tok.getLexeme();

	return out;
}

int64_t Token::toLong() const
{
	std::istringstream iss;
	int64_t result;

	if (kind == NUM || kind == NUMZERO)
	{
		iss.str(lexeme);
	}
	else
	{
		// This should never happen if the user calls this function correctly
		return 0;
	}

	iss >> result;
	return result;
}

bool Token::checkNum() const {
	if (kind == NUM) {
		if (abs(this->toLong()) > INT_MAX) return false;
	}
	return true;
}

ScanningFailure::ScanningFailure(std::string message) : message(std::move(message)) {}

const std::string& ScanningFailure::what() const { return message; }

/* Represents a DFA (which you will see formally in class later)
 * to handle the scanning
 * process. You should not need to interact with this directly:
 * it is handled through the starter code.
 */
class AsmDFA
{
public:
	enum State
	{
		// States that are also kinds
		ID = 0,
		NUM,
		NUMZERO,
		LPAREN,
		RPAREN,
		LBRACE,
		RBRACE,
		BECOMES,
		EQ,
		NE,
		LT,
		GT,
		LE,
		GE,
		PLUS,
		MINUS,
		STAR,
		SLASH,
		PCT,
		COMMA,
		SEMI,
		LBRACK,
		RBRACK,
		AMP,
		WHITESPACE,
		COMMENT,

		// States that are not also kinds
		START,
		FAIL,
		EXCLM,

		// Hack to let this be used easily in arrays. This should always be the
		// final element in the enum, and should always point to the previous
		// element.

		LARGEST_STATE = EXCLM
	};

private:
	/* A set of all accepting states for the DFA.
	   * Currently non-accepting states are not actually present anywhere
	   * in memory, but a list can be found in the constructor.
	   */
	std::set<State> acceptingStates;

	/*
	   * The transition function for the DFA, stored as a map.
	   */

	std::array<std::array<State, 128>, LARGEST_STATE + 1> transitionFunction;

	/*
	   * Converts a state to a kind to allow construction of Tokens from States.
	   * Throws an exception if conversion is not possible.
	   */
	Token::Kind stateToKind(State s) const
	{
		try {
			switch (s)
			{
			case ID: return Token::ID;
			case NUM: return Token::NUM;
			case NUMZERO: return Token::NUMZERO;
			case LPAREN: return Token::LPAREN;
			case RPAREN: return Token::RPAREN;
			case LBRACE: return Token::LBRACE;
			case RBRACE: return Token::RBRACE;
			case BECOMES: return Token::BECOMES;
			case EQ: return Token::EQ;
			case NE: return Token::NE;
			case LT: return Token::LT;
			case GT: return Token::GT;
			case LE: return Token::LE;
			case GE: return Token::GE;
			case PLUS: return Token::PLUS;
			case MINUS: return Token::MINUS;
			case STAR: return Token::STAR;
			case SLASH: return Token::SLASH;
			case PCT: return Token::PCT;
			case COMMA: return Token::COMMA;
			case SEMI: return Token::SEMI;
			case LBRACK: return Token::LBRACK;
			case RBRACK: return Token::RBRACK;
			case AMP: return Token::AMP;
			case WHITESPACE: return Token::WHITESPACE;
			case COMMENT: return Token::COMMENT;
			default:
				throw ScanningFailure("ERROR: Cannot convert state to kind.");
			}
		}
		catch (ScanningFailure f) {
			std::cerr << f.what() << std::endl;
			exit(1);
		}
	}

public:
	/* Tokenizes an input string according to the SMM algorithm.
	   * You will learn the SMM algorithm in class around the time of Assignment 6.
	   */
	std::vector<Token> simplifiedMaximalMunch(const std::string& input) const
	{
		std::vector<Token> result;

		State state = start();
		std::string munchedInput;

		// We can't use a range-based for loop effectively here
		// since the iterator doesn't always increment.
		for (std::string::const_iterator inputPosn = input.begin();
			inputPosn != input.end();)
		{

			State oldState = state;
			state = transition(state, *inputPosn);

			if (!failed(state))
			{
				munchedInput += *inputPosn;
				oldState = state;

				++inputPosn;
			}

			try {
				if (inputPosn == input.end() || failed(state))
				{
					if (accept(oldState))
					{
						result.push_back(Token(stateToKind(oldState), munchedInput));

						munchedInput = "";
						state = start();
					}
					else
					{
						if (failed(state))
						{
							munchedInput += *inputPosn;
						}
						throw ScanningFailure("ERROR: Simplified maximal munch failed on input: " + munchedInput);
					}
				}
			}
			catch (ScanningFailure f) {
				std::cerr << f.what() << std::endl;
				exit(1);
			}
		}

		return result;
	}

	/* Initializes the accepting states for the DFA.
	   */
	AsmDFA()
	{
		acceptingStates = { ID, NUM, NUMZERO, LPAREN, RPAREN, LBRACE, RBRACE,
						   BECOMES, EQ, NE, LT, GT, LE, GE, PLUS, MINUS, STAR,
						   SLASH, PCT, COMMA, SEMI, LBRACK, RBRACK, AMP, COMMENT, WHITESPACE };
		//Non-accepting states are start, EXCLM

		// Initialize transitions for the DFA
		for (size_t i = 0; i < transitionFunction.size(); ++i)
		{
			for (size_t j = 0; j < transitionFunction[0].size(); ++j)
			{
				transitionFunction[i][j] = FAIL;
			}
		}
		registerTransition(START, "abcdefghijklmnopqrstuvwxyz", ID);
		registerTransition(START, "(", LPAREN);
		registerTransition(START, ")", RPAREN);
		registerTransition(START, "{", LBRACE);
		registerTransition(START, "}", RBRACE);
		registerTransition(START, "=", BECOMES);
		registerTransition(BECOMES, "=", EQ);
		registerTransition(START, "!", EXCLM);
		registerTransition(EXCLM, "=", NE);
		registerTransition(START, "<", LT);
		registerTransition(START, ">", GT);
		registerTransition(LT, "=", LE);
		registerTransition(GT, "=", GE);
		registerTransition(START, "+", PLUS);
		registerTransition(START, "-", MINUS);
		registerTransition(START, "*", STAR);
		registerTransition(START, "/", SLASH);
		registerTransition(START, "%", PCT);
		registerTransition(START, ",", COMMA);
		registerTransition(START, ";", SEMI);
		registerTransition(START, "[", LBRACK);
		registerTransition(START, "]", RBRACK);
		registerTransition(START, "&", AMP);
		registerTransition(START, "0", NUMZERO);
		registerTransition(START, "123456789", NUM);
		registerTransition(NUM, "0123456789", NUM);
		registerTransition(ID, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", ID);
		registerTransition(SLASH, "/", COMMENT);
		registerTransition(START, isspace, WHITESPACE);
		registerTransition(COMMENT, [](int c) -> int { return c != '\n'; }, COMMENT);
		registerTransition(WHITESPACE, isspace, WHITESPACE);
	}

	// Register a transition on all chars in chars
	void registerTransition(State oldState, const std::string& chars,
		State newState)
	{
		for (char c : chars)
		{
			transitionFunction[oldState][c] = newState;
		}
	}

	// Register a transition on all chars matching test
	// For some reason the cctype functions all use ints, hence the function
	// argument type.
	void registerTransition(State oldState, int (*test)(int), State newState)
	{

		for (int c = 0; c < 128; ++c)
		{
			if (test(c))
			{
				transitionFunction[oldState][c] = newState;
			}
		}
	}

	/* Returns the state corresponding to following a transition
	   * from the given starting state on the given character,
	   * or a special fail state if the transition does not exist.
	   */
	State transition(State state, char nextChar) const
	{
		return transitionFunction[state][nextChar];
	}

	/* Checks whether the state returned by transition
	   * corresponds to failure to transition.
	   */
	bool failed(State state) const { return state == FAIL; }

	/* Checks whether the state returned by transition
	   * is an accepting state.
	   */
	bool accept(State state) const
	{
		return acceptingStates.count(state) > 0;
	}

	/* Returns the starting state of the DFA
	   */
	State start() const { return START; }
};

std::vector<Token> scan(const std::string& input)
{
	static AsmDFA theDFA;

	std::vector<Token> tokens = theDFA.simplifiedMaximalMunch(input);

	// Remove WHITESPACE and COMMENT tokens entirely.

	std::vector<Token> newTokens;

	bool first = false;
	Token::Kind prevKind;
	if (!tokens.empty()) prevKind = tokens[0].getKind();
	try {
		for (auto& token : tokens)
		{
			if (first)
			{
				if (token.getKind() == Token::Kind::EQ ||
					token.getKind() == Token::Kind::NE ||
					token.getKind() == Token::Kind::LT ||
					token.getKind() == Token::Kind::LE ||
					token.getKind() == Token::Kind::GT ||
					token.getKind() == Token::Kind::GE ||
					token.getKind() == Token::Kind::BECOMES)
				{
					if (prevKind == Token::Kind::EQ ||
						prevKind == Token::Kind::NE ||
						prevKind == Token::Kind::LT ||
						prevKind == Token::Kind::LE ||
						prevKind == Token::Kind::GT ||
						prevKind == Token::Kind::GE ||
						prevKind == Token::Kind::BECOMES)
					{
						if (!((prevKind == Token::Kind::LT || prevKind == Token::Kind::GT) && token.getKind() == Token::Kind::BECOMES))
						{
							throw ScanningFailure("ERROR: Missing critical whitespace.");
						}
					}
				}
				else if (token.getKind() == Token::Kind::ID && (prevKind == Token::Kind::NUM || prevKind == Token::Kind::NUMZERO))
					throw ScanningFailure("ERROR: A number followed by an ID without whitespace is not allowed.");
				else if (token.getKind() == Token::Kind::NUM && prevKind == Token::Kind::NUMZERO)
					throw ScanningFailure("ERROR: A zero followed by a number without whitespace is not allowed.");
			}
			first = true;
			if (token.getKind() != Token::WHITESPACE && token.getKind() != Token::Kind::COMMENT)
			{
				token.modifyID();
				if (!token.checkNum()) throw ScanningFailure("ERROR: The numeric value exceeds 2^31-1.");
				newTokens.push_back(token);
			}
			prevKind = token.getKind();
		}
	}
	catch (ScanningFailure f) {
		std::cerr << f.what() << std::endl;
	}

	return newTokens;
}

int main() {
	std::string s;
	while (getline(std::cin, s)) {
		std::vector<Token> vec;
		vec = scan(s);
		for (Token token : vec) {
			std::cout << token << std::endl;
		}
	}
	return 0;
}