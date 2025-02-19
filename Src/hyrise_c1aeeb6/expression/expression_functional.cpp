#include "expression_functional.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "all_type_variant.hpp"
#include "expression/abstract_expression.hpp"
#include "expression/exists_expression.hpp"
#include "expression/interval_expression.hpp"
#include "expression/lqp_column_expression.hpp"
#include "expression/placeholder_expression.hpp"
#include "expression/pqp_column_expression.hpp"
#include "expression/value_expression.hpp"
#include "expression/window_expression.hpp"
#include "expression/window_function_expression.hpp"
#include "null_value.hpp"
#include "../types.hpp"

namespace hyrise::expression_functional {

std::shared_ptr<AbstractExpression> to_expression(const std::shared_ptr<AbstractExpression>& expression) {
  return expression;
}

std::shared_ptr<ValueExpression> to_expression(const AllTypeVariant& value) {
  return std::make_shared<ValueExpression>(value);
}

std::shared_ptr<ValueExpression> value_(const AllTypeVariant& value) {
  return std::make_shared<ValueExpression>(value);
}

std::shared_ptr<ValueExpression> null_() {
  return std::make_shared<ValueExpression>(NullValue{});
}

std::shared_ptr<PlaceholderExpression> placeholder_(const ParameterID parameter_id) {
  return std::make_shared<PlaceholderExpression>(parameter_id);
}

std::shared_ptr<LQPColumnExpression> lqp_column_(const std::shared_ptr<const AbstractLQPNode>& original_node,
                                                 const ColumnID original_column_id) {
  return std::make_shared<LQPColumnExpression>(original_node, original_column_id);
}

std::shared_ptr<PQPColumnExpression> pqp_column_(const ColumnID column_id, const DataType data_type,
                                                 const bool nullable, const std::string& column_name) {
  return std::make_shared<PQPColumnExpression>(column_id, data_type, nullable, column_name);
}

std::shared_ptr<WindowFunctionExpression> count_star_(const std::shared_ptr<AbstractLQPNode>& lqp_node) {
  const auto column_expression = std::make_shared<LQPColumnExpression>(lqp_node, INVALID_COLUMN_ID);
  return std::make_shared<WindowFunctionExpression>(WindowFunction::Count, column_expression);
}

std::shared_ptr<ExistsExpression> exists_(const std::shared_ptr<AbstractExpression>& subquery_expression) {
  return std::make_shared<ExistsExpression>(subquery_expression, ExistsExpressionType::Exists);
}

std::shared_ptr<ExistsExpression> not_exists_(const std::shared_ptr<AbstractExpression>& subquery_expression) {
  return std::make_shared<ExistsExpression>(subquery_expression, ExistsExpressionType::NotExists);
}

std::shared_ptr<IntervalExpression> interval_(const int64_t duration, const DatetimeComponent unit) {
  return std::make_shared<IntervalExpression>(duration, unit);
}

std::shared_ptr<WindowExpression> window_(std::vector<std::shared_ptr<AbstractExpression>>&& partition_by_expressions,
                                          std::vector<std::shared_ptr<AbstractExpression>>&& order_by_expressions,
                                          std::vector<SortMode>&& sort_modes, FrameDescription frame_description) {
  return std::make_shared<WindowExpression>(std::move(partition_by_expressions), std::move(order_by_expressions),
                                            std::move(sort_modes), frame_description);
}

}  // namespace hyrise::expression_functional
