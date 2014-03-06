#ifndef NUMBER_PARSE_H_
#define NUMBER_PARSE_H_

template<class T>
inline void ipow(T base, int exp, T* result) {
  *result = 1;
  if (exp < 0) {
    exp = -exp;
    while (exp) {
      if (exp & 1)
        *result *= base;
      exp >>= 1;
      base *= base;
    }
    *result = 1/ *result;
  } else {
    while (exp) {
      if (exp & 1)
        *result *= base;
      exp >>= 1;
      base *= base;
    }
  }
}

template<class T>
inline T ipow(const T& base, int exp) {
  T result;
  ipow(base, exp, &result);
  return result;
}

inline bool IsDigit(char c) {
  return static_cast<unsigned char>(c - '0') <= 9;
}

// Requires T to be an integer variation.
template<class T>
inline bool IsOdd(const T& t) {
  return t & 1;
}

// Requires T to be an integer variation.
template<class T>
inline bool IsEven(const T& t) {
  return !IsOdd(t);
}

template<class T, class Iterator>
inline T ParseUInt(Iterator& str) {
  // // char c;
  // while (IsDigit(*str)) {
  // // while (IsDigit(c = *str)) {
  //   // result += c - '0';
  //   result += *str - '0';
  //   result *= 10;
  //   ++str;
  // }

  // T result = 0;
  // unsigned char d = *str - '0';
  // while (d <= 9) {
  //   // result *= 10;
  //   result += d;
  //   result *= 10;
  //   d = *++str - '0';
  // }
  // return result/10;
  // // return result;

  unsigned char d = *str - '0';
  if (d > 9)
    return 0;
  T result = d;
  d = *++str - '0';
  while (d <= 9) {
    result *= 10;
    result += d;
    d = *++str - '0';
  }
  return result;
}

template<class T, class Iterator>
inline T ExtractUInt(Iterator str) {
  return ParseUInt<T>(str);
}

template<class T, class Iterator>
inline T ParseInt(Iterator& str) {
  if (*str == '-')
    return -ParseUInt<T>(++str);
  return ParseUInt<T>(str);
}

template<class T, class Iterator>
inline T ExtractInt(Iterator str) {
  return ParseInt<T>(str);
}

template<class T, class Iterator>
inline T ParseDecimal(Iterator& str, const T& result) {
  if (*str != '.')
    return result;
  ++str;
  // TODO: Find a better way to do this.
  T decimal_part = 1;
  T ival = 0;
  // TODO: Try doing the offsetting at the end using ipow.
  while (*str == '0') {
    decimal_part /= static_cast<T>(10);
    ++str;
  }
  while (IsDigit(*str)) {
    decimal_part /= static_cast<T>(10);
    ival *= static_cast<T>(10);
    ival += *str - '0';
    ++str;
  }
  if ((*str | 0x20) == 'e')
    return (result + decimal_part * ival) * ipow<T>(10, ParseInt<int>(++str));
  return result + decimal_part * ival;
}

template<class T, class Iterator>
inline T ExtractDecimal(Iterator str, const T& result) {
  return ParseDecimal<T>(str, result);
}

template<class T, class Iterator>
inline T ParseUFloat(Iterator& str) {
  // TODO: Should I use int or T?
  T result = ParseUInt<T>(str);
  return ParseDecimal<T>(str, result);
  // if (*str != '.')
  //   return result;
  // ++str;
  // // TODO: Find a better way to do this.
  // T decimal_part = 1;
  // T ival = 0;
  // while (*str == '0') {
  //   decimal_part /= static_cast<T>(10);
  //   ++str;
  // }
  // while (IsDigit(*str)) {
  //   decimal_part /= static_cast<T>(10);
  //   ival *= static_cast<T>(10);
  //   ival += *str - '0';
  //   ++str;
  // }
  // if ((*str | 0x20) == 'e')
  //   return (result + decimal_part * ival) * ipow<T>(10, ParseInt<int>(++str));
  // return result + decimal_part * ival;
}

template<class T, class Iterator>
inline T ExtractUFloat(Iterator str) {
  return ParseUFloat<T>(str);
}

template<class T, class Iterator>
inline T ParseFloat(Iterator& str) {
  if (*str == '-')
    return -ParseUFloat<T>(++str);
  return ParseUFloat<T>(str);
}

template<class T, class Iterator>
inline T ExtractFloat(Iterator str) {
  return ParseFloat<T>(str);
}

#endif // NUMBER_PARSE_H_
