#include "common/type/date_type.h"
#include "common/log/log.h"
#include "common/value.h"

int DateType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::DATES, "left type must be date");
  ASSERT(right.attr_type() == AttrType::DATES, "right type must be date");
  date_t left_val  = left.get_date();
  date_t right_val = right.get_date();
  auto   cmp       = left_val <=> right_val;
  if (cmp < 0) {
    return -1;
  } else if (cmp > 0) {
    return 1;
  }
  return 0;
}

RC DateType::to_string(const Value &val, string &result) const
{
  result = val.get_date().to_string();
  return RC::SUCCESS;
}