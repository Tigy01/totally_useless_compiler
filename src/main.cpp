#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

enum class TokenType {
    _return,
    int_lit,
    new_line,
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

std::string parseKeyword(std::string str, int index)
{
    std::string keywordBuf;
    keywordBuf.push_back(str.at(index));

    index++;
    while (std::isalnum(str.at(index))) {
        keywordBuf.push_back(str.at(index));
        index++;
    }
    return keywordBuf;
}

std::string parseNumber(std::string str, int index)
{
    std::string keywordBuf;
    keywordBuf.push_back(str.at(index));

    index++;
    while (std::isdigit(str.at(index))) {
        keywordBuf.push_back(str.at(index));
        index++;
    }
    return keywordBuf;
}

std::optional<Token> getTokenFromKeyword(std::string keyword)
{
    if (keyword == "return") {
        return std::make_optional(Token {
            .type = TokenType::_return,
        });
    }
    return std::nullopt;
}

Token getTokenFromNumber(std::string value)
{
    return Token {
        .type = TokenType::int_lit,
        .value = value
    };
}

std::vector<Token> tokenize(const std::string& str)
{
    std::vector<Token> tokens {};

    for (int i = 0; i < str.length(); i++) {
        char c = str.at(i);
        if (std::isalpha(c)) {
            std::string keyword = parseKeyword(str, i);
            i += keyword.length() - 1;

            auto token = getTokenFromKeyword(keyword);

            if (token.has_value()) {
                tokens.push_back(token.value());
            } else {
                std::cerr << "Unknown Token: " << keyword << std::endl;
                exit(EXIT_FAILURE);
            }
        } else if (std::isdigit(c)) {
            std::string num = parseNumber(str, i);
            i += num.length() - 1;

            tokens.push_back(getTokenFromNumber(num));
        } else if (c == '\n') {
            tokens.push_back(Token { .type = TokenType::new_line });
        } else if (std::isspace(c)) {
            continue;
        } else {
            std::cerr << "Unknown Token: " << c << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    return tokens;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Incorrect Usage. Correct usage is:" << std::endl;
        std::cerr << "\ttotalUser <input.tu>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string contents;
    std::fstream input(argv[1], std::ios::in);
    {
        std::stringstream contents_stream;
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    auto tokens = tokenize(contents);

    std::cout << "[ ";
    for (Token tok : tokens) {
        switch (tok.type) {
        case TokenType::_return:
            std::printf("return, ");
            break;
        case TokenType::int_lit:
            std::printf("%s, ", tok.value.value_or("").c_str());
            break;
        case TokenType::new_line:
            std::printf("newline, ");
            break;
        }
    }
    std::cout << "]" << std::endl;

    return EXIT_SUCCESS;
}
