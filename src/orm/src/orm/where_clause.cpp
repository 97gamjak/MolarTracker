#include "orm/where_clause.hpp"

namespace orm
{
    std::string whereOperatorStr(filter::Operator operator_)
    {
        switch (operator_)
        {
            case filter::Operator::Equal:
                return "= ?";
            case filter::Operator::NotEqual:
                return "!= ?";
            case filter::Operator::Less:
                return "< ?";
            case filter::Operator::LessOrEqual:
                return "<= ?";
            case filter::Operator::Greater:
                return "> ?";
            case filter::Operator::GreaterOrEqual:
                return ">= ?";
            case filter::Operator::In:
                return "IN (?, ?)";
            case filter::Operator::NotIn:
                return "NOT IN (?, ?)";
            case filter::Operator::Between:
                return "BETWEEN ? AND ?";
            default:
                return "";
        }
    }

}   // namespace orm