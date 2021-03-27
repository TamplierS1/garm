#include "ast_printer.h"

namespace cpplox
{
std::string AstPrinter::print(expr::Expression* expr)
{
    return std::get<std::string>(expr->accept(this).value());
}

Value AstPrinter::visit(expr::Binary* expr)
{
    return static_cast<Value>(parenthesize(expr->m_op->get_lexeme(), {expr->m_left, expr->m_right}));
}

Value AstPrinter::visit(expr::Grouping* expr)
{
    return static_cast<Value>(parenthesize("group", {expr->m_expression}));
}

Value AstPrinter::visit(expr::Literal* expr)
{
    // TODO: introduce better error handling
    if (!expr->m_value->has_value()) return "nil";
    return *expr->m_value;
}

Value AstPrinter::visit(expr::Unary* expr)
{
    return static_cast<Value>(parenthesize(expr->m_op->get_lexeme(), {expr->m_right}));
}

Value AstPrinter::visit(expr::Variable* expr)
{
    return expr->m_name->get_literal();
}

std::string AstPrinter::parenthesize(const std::string& name, std::vector<ExpressionPtr>&& exprs)
{
    std::string out = "(" + name;

    for (const auto& expr : exprs)
    {
        out += " ";

        Value val = expr->accept(this).value();
        switch (val->index())
        {
            case 0:  // string
                out += std::get<std::string>(val.value());
                break;
            case 1:  // double
                out += std::to_string(std::get<double>(val.value()));
                break;
            case 2:  // bool
                out += std::to_string(std::get<bool>(val.value()));
                break;
            default:
                ErrorHandler::get_instance().debug_error("Not every type was handled");
                break;
        }
    }
    out += ")";

    return out;
}

}
