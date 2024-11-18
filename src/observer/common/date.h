#pragma once

#include <compare>
#include <cstdint>
#include <type_traits>
#include <string>

class Date
{
public:
  Date() = default;
  explicit Date(int32_t days_since_epoch) : days_since_epoch_(days_since_epoch) {}

  // 使用C++20的默认三路比较运算符
  std::strong_ordering operator<=>(const Date &other) const = default;

  std::string to_string() const;

  // 尝试将字符串str转换为date, 若成功返回true, 否则返回false
  static bool from_string(const char *str, Date &date);

private:
  // 用32位有符号整数表示自UNIX epoch(1970-01-01)到某个日期经过的天数
  // yacc_sql.y中attr_def的定义指定类型的默认大小为4, 这里使用32位整数也是为了不破坏这一约定
  int32_t days_since_epoch_;
};

static_assert(std::is_trivial<Date>::value, "Date should be trivial");
static_assert(sizeof(Date) == 4, "sizeof(Date) should be 4");

using date_t = Date;