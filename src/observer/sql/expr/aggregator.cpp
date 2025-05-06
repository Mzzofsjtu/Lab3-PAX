/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2024/05/29.
//

#include "sql/expr/aggregator.h"
#include "common/log/log.h"

RC SumAggregator::accumulate(const Value &value)
{
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_ = value;
    return RC::SUCCESS;
  }

  ASSERT(value.attr_type() == value_.attr_type(), "type mismatch. value type: %s, value_.type: %s", 
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));

  Value::add(value, value_, value_);
  return RC::SUCCESS;
}

RC SumAggregator::evaluate(Value &result)
{
  result = value_;
  return RC::SUCCESS;
}

RC MaxAggregator::accumulate(const Value &value)
{
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_ = value;
    return RC::SUCCESS;
  }

  ASSERT(value.attr_type() == value_.attr_type(), "type mismatch. value type: %s, value_.type: %s", 
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));

  int cmp = value_.compare(value);
  if (cmp < 0) {
    value_ = value;
  }
  return RC::SUCCESS;
}

RC MaxAggregator::evaluate(Value &result)
{
  result = value_;
  return RC::SUCCESS;
}

RC MinAggregator::accumulate(const Value &value) {
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_ = value;
    return RC::SUCCESS;
  }
 
  ASSERT(value.attr_type() == value_.attr_type(), "type mismatch. value type: %s, value_.type: %s", 
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));
 
  int cmp = value_.compare(value);
  if (cmp > 0) {
    value_ = value;
  }
  return RC::SUCCESS;
}
 
RC MinAggregator::evaluate(Value &result) {
  result = value_;
  return RC::SUCCESS;
}

RC CountAggregator::accumulate(const Value &value) {
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_ = value;
    count_++;
    return RC::SUCCESS;
  }
 
  ASSERT(value.attr_type() == value_.attr_type(), "type mismatch. value type: %s, value_.type: %s", 
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));
 
  count_++;
 
  return RC::SUCCESS;
}
 
RC CountAggregator::evaluate(Value &result) {
  result.set_type(AttrType::INTS);
  result.set_data(reinterpret_cast<const char *>(&count_), sizeof(count_));
  ASSERT(result.get_int() == count_, "Error! The results are not set correctly, with value: %d", result.get_int());
  return RC::SUCCESS;
}
 
RC AvgAggregator::accumulate(const Value &value) {
  if (value_.attr_type() == AttrType::UNDEFINED) {
    value_ = value;
    count_++;
    return RC::SUCCESS;
  }
 
  ASSERT(value.attr_type() == value_.attr_type(), "type mismatch. value type: %s, value_.type: %s", 
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));
 
  count_++;
  value_.add(value, value_, value_);
 
  return RC::SUCCESS;
}
RC AvgAggregator::evaluate(Value &result) {
  if(count_ == 0) {
    return RC::VARIABLE_NOT_VALID;    // Divide by 0!
  }
  Value tmp = value_;
  Value value_count;
  value_count.set_type(AttrType::FLOATS);
  result.set_type(AttrType::FLOATS);
  value_count.set_data(reinterpret_cast<const char *>(&count_), sizeof(count_));
  result.divide(tmp, value_count, result);
  return RC::SUCCESS;
}