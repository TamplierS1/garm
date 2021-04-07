#include "error_handler.h"

namespace cpplox
{
const char* ParseError::what() const noexcept
{
    return "ParseError: unexpected character.";
}

const char* RuntimeError::what() const noexcept
{
    return m_msg.c_str();
}

void ErrorHandler::error(const Token& token, const std::string& msg)
{
    std::string formatted_line = format_error(token);

    if (token.get_token_type() == TokenType::cpplox_EOF)
        report(token.get_line(), token.get_column(), " at end", formatted_line, msg);
    else
        report(token.get_line(), token.get_column(), token.get_lexeme(), formatted_line, msg);
}

void ErrorHandler::error(int line, int column, char character, const std::string& src_str,
                         const std::string& msg)
{
    report(line, column, std::string(1, character), src_str, msg);
}

void ErrorHandler::runtime_error(const RuntimeError& error)
{
    this->error(error.m_op, error.m_msg);
    m_had_runtime_error = true;
}

void ErrorHandler::debug_error(const std::string& msg, int line)
{
    fmt::print("\n[line {}]", line);
    fmt::print(fmt::emphasis::italic | fg(fmt::color::red), " Internal error in ");
    fmt::print(fmt::emphasis::bold | fg(fmt::color::deep_pink), "{}:\n\n", __FILE__);
    fmt::print(fmt::emphasis::bold | fg(fmt::color::white), "\t{}\n\n", msg);
}

void ErrorHandler::warning(const std::string& msg) const
{
    fmt::print(fmt::emphasis::italic | fg(fmt::color::yellow), " Warning: {}\n\n", msg);
}

std::string ErrorHandler::format_error(const Token& token)
{
    std::string source_line = token.get_str_line();

    // I separated the line into multiple variables
    // because I wanted the token to be colored differently from the rest of the line

    // TODO: make the error token different color from the rest of the line.
    // part of the line before the token that caused the error
    int column = token.get_column();
    std::string before_token_line =
        fmt::format(fg(fmt::color::dark_olive_green), "{}", source_line.substr(0, column - token.get_lexeme().size()));
    std::string token_str = fmt::format(fg(fmt::color::red), "{}", token.get_lexeme());
    std::string after_token_line = fmt::format(fg(fmt::color::dark_olive_green), "{}", source_line.substr(column,
                                               source_line.size() - column));

    return before_token_line + token_str + after_token_line;
}

void ErrorHandler::report(int line, int column, const std::string& where, const std::string& src_str,
                          const std::string& msg)
{
    fmt::print("\n[{}, {}]", line, column);
    fmt::print(fmt::emphasis::italic | fg(fmt::color::red), " Error at '");
    fmt::print(fmt::emphasis::bold | fg(fmt::color::white), "{}", where);
    fmt::print(fmt::emphasis::italic | fg(fmt::color::red), "': {}\n\n", msg);
    fmt::print(fg(fmt::color::dark_olive_green), "\t{}\n\n", src_str);
    m_had_error = true;
}


}
