#include "Assembler.h"
#include "Opcodes.h"
uint8_t* Assembler::compile(std::string _source)
{
	source = _source;
	std::vector<Token>* tokens = scanner.scan(source);
	uint8_t* program = parser.parse(tokens);
	//return nullptr;
	return program;
}

Assembler::Assembler()
{
	opcodes["NOOP"] = Opcodes::NOOP;
	opcodes["ADD"] = Opcodes::ADD;
	opcodes["SUB"] = Opcodes::SUB;
	opcodes["NEG"] = Opcodes::NEG;
	opcodes["MUL"] = Opcodes::MUL;
	opcodes["DIV"] = Opcodes::DIV;
	opcodes["DIVS"] = Opcodes::DIVS;
	opcodes["MOD"] = Opcodes::MOD;
	opcodes["MODS"] = Opcodes::MODS;
	opcodes["INC"] = Opcodes::INC;
	opcodes["DEC"] = Opcodes::DEC;
	opcodes["AND"] = Opcodes::AND;
	opcodes["OR"] = Opcodes::OR;
	opcodes["XOR"] = Opcodes::XOR;
	opcodes["NOT"] = Opcodes::NOT;
	opcodes["ASL"] = Opcodes::ASL;
	opcodes["ASR"] = Opcodes::ASR;
	opcodes["DUP"] = Opcodes::DUP;
	opcodes["PUSH"] = Opcodes::PUSH;
	opcodes["POP"] = Opcodes::POP;
	opcodes["LIT"] = Opcodes::LIT;
	opcodes["JSR"] = Opcodes::JSR;
	opcodes["RTS"] = Opcodes::RTS;
	opcodes["MOV"] = Opcodes::MOV;
	opcodes["LOAD"] = Opcodes::LOAD;
	opcodes["SAVE"] = Opcodes::SAVE;
	opcodes["MCPY"] = Opcodes::MCPY;
	opcodes["JMP"] = Opcodes::JMP;
	opcodes["JEZ"] = Opcodes::JEZ;
	opcodes["JLZ"] = Opcodes::JLZ;
	opcodes["JGZ"] = Opcodes::JGZ;
	opcodes["JNZ"] = Opcodes::JNZ;
	opcodes["VBL"] = Opcodes::VBL;
	opcodes["DUMP "] = Opcodes::DUMP;
	opcodes["SAVEG"] = Opcodes::SAVEG;
	opcodes["LOADG"] = Opcodes::LOADG;
	opcodes["SAVES"] = Opcodes::SAVES;
	opcodes["LOADS"] = Opcodes::LOADS;
	opcodes["RESET"] = Opcodes::RESET;
	opcodes["CLR"] = Opcodes::CLR;
	opcodes["SPR"] = Opcodes::SPR;
	opcodes["LOADV"] = Opcodes::LOADV;
	opcodes["SAVEV"] = Opcodes::SAVEV;

	registers["0"] = 0;
	registers["1"] = 1;
	registers["2"] = 2;
	registers["3"] = 3;
	registers["4"] = 4;
	registers["5"] = 5;
	registers["6"] = 6;
	registers["7"] = 7;
	registers["8"] = 8;
	registers["PAL"] = 9;
	registers["VRAM"] = 10;
	registers["OSET"] = 11;
	registers["STAT"] = 12;
	registers["ACC"] = 13;
	registers["SP"] = 14;
	registers["IP"] = 15;

	keywords.insert("MARK");

	scanner.init(&opcodes, &registers, &keywords);
}

void Scanner::scanToken()
{
	start = current;
	char c = advance();
	switch (c)
	{
	case ' ':
	case '\t':
	case '\r':
		break;
	case '\n':
		line++;
		break;
	case ';':
		comment();
		break;
	case '$':
		reg();
		break;
	case '#':
		label();
		break;
	case '"':
		str();
		break;
	default:
		if (isNumeric(c))number();
		else identifier();
		break;
	}
}

std::string Scanner::getIdentifier()
{
	char c = source[current];
	while (!isAtEof() && !isReserved(c)) c = advance();
	current--;
	if (c == '$')
	{
		error("Character '$' is not allowed in an identifier.");
	}
	else if (c == '#')
	{
		error("Character '#' is not allowed in an identifier.");
	}
	else if (c == '"')
	{
		error("Character '\"' is not allowed in an identifier.");
	}
	else
	{
		return source.substr(start, current - start);
	}
	return "";
}

char Scanner::advance()
{
	current++;
	return source[current - 1];
}
bool Scanner::isAtEof()
{
	return current >= source.length();
}
bool Scanner::isNumeric(char c)
{
	return (c >= '0' && c <= '9');
}
bool Scanner::isReserved(char c)
{
	return !(c != ' ' && c != '\t' && c != '\r' && c != '\n' && c != ';' && c != '$' && c != '#' && c != '"');
}
void Scanner::error(std::string message)
{
	if (hasError) return;
	std::cout << "Error at line " << line << ": " << message << std::endl;
	hasError = true;
}
void Scanner::number()
{
	char c = source[current];
	if (c == '0')
	{
		if (current < source.length() - 1 && source[current + 1] == 'x') advance();
	}
	while (!isAtEof() && isNumeric(c)) c = advance();
	Token t = {};
	t.literal = source.substr(start, current - start);
	t.line = line;
	t.type = Token::TokenType::number;
	t.value = std::stoi(t.literal);
	tokens.push_back(t);
}
void Scanner::comment()
{
	while (source[current] != '\n' && !isAtEof()) advance();
}
void Scanner::reg()
{
	Token t = {};
	t.literal = getIdentifier();
	if (hasError) return;
	t.line = line;
	t.type = Token::TokenType::reg;
	std::string lit = t.literal.substr(1); // strips off starting character
	if (registers->count(lit) == 1)
	{
		t.value = registers->at(lit);
		tokens.push_back(t);
	}
	else
	{
		error("Invalid register name '" + lit + "'");
	}
}
void Scanner::label()
{
	Token t = {};
	t.literal = getIdentifier();
	if (hasError) return;
	t.line = line;
	t.type = Token::TokenType::label;
	t.literal = t.literal.substr(1); // strips off starting character
	t.value = 0;
	tokens.push_back(t);
}
void Scanner::identifier()
{
	Token t = {};
	t.literal = getIdentifier();
	if (hasError) return;
	t.line = line;

	std::string lit = t.literal;
	if (opcodes->count(lit) == 1)
	{
		t.type = Token::TokenType::opcode;
		t.value = opcodes->at(lit);
		tokens.push_back(t);
	}
	else if (keywords->count(lit) == 1)
	{
		t.type = Token::TokenType::keyword;
		t.value = 0;
		tokens.push_back(t);
	}
	else
	{
		error("Opcode not recognized '" + lit + "'");
	}
}

void Scanner::str()
{
	char c = advance();
	while (!isAtEof() && c != '"') c = advance();
	if (c != '"') error("Reached eof while searching for the end of a string, did you forget a '\"'");
	else
	{
		Token t = {};
		t.type = Token::TokenType::string;
		t.literal = source.substr(start + 1, current - start - 2);
		t.value = 0;
		t.line = line;
		tokens.push_back(t);
	}
}

std::vector<Token>* Scanner::scan(std::string _source)
{
	source = _source;
	tokens.clear();
	while (!hasError && !isAtEof())
	{
		scanToken();
	}
	Token t = {};
	t.type = Token::TokenType::eof;
	t.literal = "EOF";
	t.value = 0;
	t.line = line;
	tokens.push_back(t);
	/*for (size_t i = 0; i < tokens.size(); i++)
	{
		tokens[i].print();
	}*/
	return &tokens;
}

void Scanner::init(std::unordered_map<std::string, uint8_t>* _opcodes,
	std::unordered_map<std::string, uint8_t>* _registers,
	std::unordered_set<std::string>* _keywords)
{
	opcodes = _opcodes;
	registers = _registers;
	keywords = _keywords;
}

Token Parser::advance()
{
	current++;
	return tokens->at(current - 1);
}
void Parser::error(std::string message, Token t)
{
	if (hasError)return;
	hasError = true;
	std::cout << "Error at line " << t.line << ": " << message << "\tToken: ";
	t.print();
}
void Parser::writeNumber(size_t value)
{
	uint8_t low = (uint8_t)value;
	uint8_t high = (uint8_t)(value >> 8);
	program.push_back(high);
	program.push_back(low);
}
uint8_t* Parser::parse(std::vector<Token>* _tokens)
{
	tokens = _tokens;
	current = 0;
	std::unordered_map<std::string, Label> labels;
	while (current < tokens->size() && !hasError)
	{
		Token t = advance();
		Token label;
		if (t.type == Token::TokenType::opcode)
		{
			program.push_back(t.value);
			switch (t.value)
			{
			case Opcodes::JMP:
			case Opcodes::JGZ:
			case Opcodes::JLZ:
			case Opcodes::JEZ:
			case Opcodes::JNZ:
				label = advance();
				if (label.type != Token::TokenType::label) error("Jump commands expect a label", label);
				if (labels.count(label.literal) == 0)
				{
					labels[label.literal] = {};
				}
				labels[label.literal].refs.push_back(&label);
				labels[label.literal].refPositions.push_back(program.size());
				program.push_back(0);
				program.push_back(0);
				//t = advance();
				if (t.value != Opcodes::JMP)
				{
					t = advance();
					if (t.type != Token::TokenType::reg) error("No register supplied to conditonal jump.", t);
					else program.push_back(t.value);
				}
				break;
			case Opcodes::PUSH:
			case Opcodes::POP:
				t = advance();
				if (t.type != Token::TokenType::reg) error("Push and pop reqire a valid register name.", t);
				else program.push_back(t.value);
				break;
			case Opcodes::INC:
			case Opcodes::DEC:
				t = advance();
				if (t.type != Token::TokenType::reg) error("Increment and Decrement reqire a valid register name.", t);
				else program.push_back(t.value);
				break;
			case Opcodes::LIT:
				t = advance();
				if (t.type != Token::TokenType::number) error("Token must be a number literal.", t);
				else writeNumber(t.value);
				break;
			case Opcodes::JSR:
				t = advance();
				if (t.type != Token::TokenType::label) error("JSR command expects a label", t);
				else writeNumber(t.value);
				break;
			default:
				break;
			}
		}
		else if (t.type == Token::TokenType::keyword)
		{
			// right now the only keyword is "MARK" to create a label.
			if (labels.count(t.literal) == 1)
			{
				error("Label '" + t.literal + " already exists", t);
			}
			else
			{
				t = advance();
				if (t.type != Token::TokenType::label)
				{
					error(t.literal + " is not a valid label", t);
				}
				else
				{
					labels[t.literal].head = &t;
					labels[t.literal].headPos = program.size();
				}
			}
		}
		else if (t.type == Token::TokenType::eof)break;
		else error("Expected an opcode", t);
	}
	for (std::pair<std::string, Label> pair : labels)
	{
		if (pair.second.head == nullptr)
		{
			error("Label " + pair.first + " is referenced but not defined.", *pair.second.refs[0]);
			break;
		}
		else
		{
			if (pair.second.refPositions.size() == 0)
			{
				if (!hasError) std::cout << "Warning: " + pair.first + " is defined but not referenced.\n";
			}
			else
			{
				uint8_t low = (uint8_t)pair.second.headPos;
				uint8_t high = (uint8_t)(pair.second.headPos >> 8);
				for (size_t i = 0; i < pair.second.refs.size(); i++)
				{
					program[pair.second.refPositions[i]] = high;
					program[pair.second.refPositions[i] + 1] = low;
				}
			}
		}
	}
	constexpr int BANK_SIZE = 65536;
	uint8_t* output = (uint8_t*)calloc(BANK_SIZE, 1);
	for (size_t i = 0; i < program.size(); i++)
	{
		output[i] = program[i];
	}
	return output;
}