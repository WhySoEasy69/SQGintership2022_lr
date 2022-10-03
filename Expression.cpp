#include "Expression.h"

#include <set>
#include <stdexcept>
#include <unordered_map>
#include <numeric> // my add

namespace
{// почему не работает infinity 

static std::unordered_map<std::string, double> s_variables;

static const std::set<OperatorInfo> sc_operators = {
    { "+", 1, 1, 2, [](const std::vector<double>& args) { return args.size() == 2 ? args[0] + args[1] : args[0]; } },
    { "-", 1, 1, 2, [](const std::vector<double>& args) { return args.size() == 2 ? args[0] - args[1] + 0.0 : -args[0]; } },
    { "*", 2, 2, 2, [](const std::vector<double>& args) { return args[0] * args[1]; } },
    { "/", 2, 2, 2, [](const std::vector<double>& args) { return args[0] / args[1]; } },
    { "sin", 0, 1, 1, [](const std::vector<double>& args) { return std::sin(args[0]); } },
    { "cos", 0, 1, 1, [](const std::vector<double>& args) { return std::cos(args[0]); } },
    { "min", 0, 2, 2, [](const std::vector<double>& args) { return std::min(args[0], args[1]); } },
    { "max", 0, 2, 2, [](const std::vector<double>& args) { return std::max(args[0], args[1]); } },
    {"^", 3, 2, 2, [](const std::vector<double> &args) { return std::pow(args[0], args[1]); } }, 
    {"average", 0, 1, std::numeric_limits<unsigned>::infinity(),
     [](const std::vector<double> &args) {
         return std::accumulate(args.begin(), args.end(), 0.0) / (args.size() + 0.0);
     }}, //                  my add;
};
}

//ExpressionNode::~ExpressionNode(){}; // my add

const OperatorInfo* FindOperator(const std::string& op)
{
    const auto iter = sc_operators.find(op);
    return iter != sc_operators.end() ? &*iter : nullptr;
}

const OperatorInfo* FindOperator(char op)
{
    return FindOperator(std::string(1, op));
}

int GetPriority(char ch)
{
    const auto operatorInfo = FindOperator(ch);
    return operatorInfo ? operatorInfo->priority : 0;
}

double VariableExpressionNode::Compute() const
{
    const auto iter = s_variables.find(m_identifier);
    if (iter == s_variables.end())
    {
        throw std::runtime_error("Uninitialized variable \"" + m_identifier + "\"");
    }

    return iter->second;
}

double OperatorExpressionNode::Compute() const
{
    std::vector<double> values;
    for (const auto& child : m_children)
        values.push_back(child->Compute());
    return m_op(values);
}

double AssignmentExpressionNode::Compute() const
{
    const auto value = m_child->Compute();
    if (std::isnan(value) || std::isinf(value))
    {
        throw std::runtime_error("Result can not be nan or infinite"); // my add
    }
    s_variables[m_variable] = value;
    return value;
}

/* AssignmentExpressionNode::~AssignmentExpressionNode()
{
    m_child.reset(); 
 };

  OperatorExpressionNode::~OperatorExpressionNode() {
      for (auto ptr : m_children){
          ptr.reset(); 
      }
  };
  */