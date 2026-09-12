# fmtstr-hpp

A quickly-made unoptimized and limited recreation of Python formatted strings in a C++ header. Requires [nlohmann/json](https://json.nlohmann.me/).

### Usage
Include `fmtstr.hpp` and use the `fmtstr()` function.

### Data Types
- Integers `int` (int)
- Booleans `bool` (bool)
- Strings `str` (std::string)

### Operator Precedance
Operator precedence is similar to that of C++. However, operators of the same precedence are always associated from left to right.

| Precedence | Operators | Description
| --- | --- | ---
| 1   | `a * b`, `a / b`, `a % b` | Multiplication, division, and modulo
| 2   | `a + b`, `a - b` | Addition and subtraction
| 3   | `a << b`, `a >> b` | Bitwise left shift and right shift
| 4   | `a < b`, `a <= b`, `a > b`, `a >= b` | Less than, less than or equal to, greater than, and greater than or equal to
| 5   | `a == b`, `a != b` | Equal to and not equal to
| 6   | `a & b` | Bitwise AND
| 7   | `a ^ b` | Bitwise XOR
| 7   | `a \| b` | Bitwise OR
| 8   | `a && b` | Logical AND
| 9   | `a \|\| b` | Logical OR
| 10  | `a ? b : c` | Ternary conditional

### Supported data types for operators
Operator precedence is similar to that of C++. However, operators of the same precedence are always associated from left to right.

| Operator | `int`, `int` OR `bool`, `bool` OR `int`, `bool` | `str`, `str` | `bool`, `any`, `any`
| --- | --- | --- | ---
| `a * b` | ✅ | ❌ | ❌
| `a / b` | ✅ | ❌ | ❌
| `a % b` | ✅ | ❌ | ❌
| `a + b` | ✅ | ✅ | ❌
| `a - b` | ✅ | ❌ | ❌
| `a << b`| ✅ | ❌ | ❌
| `a >> b`| ✅ | ❌ | ❌
| `a < b` | ✅ | ❌ | ❌
| `a <= b`| ✅ | ❌ | ❌
| `a > b` | ✅ | ❌ | ❌
| `a >= b`| ✅ | ❌ | ❌
| `a == b`| ✅ | ✅ | ❌
| `a != b`| ✅ | ✅ | ❌
| `a & b` | ✅ | ❌ | ❌
| `a ^ b` | ✅ | ❌ | ❌
| `a \| b`| ✅ | ❌ | ❌
| `a && b`| ✅ | ❌ | ❌
| `a \|\| b`| ✅ | ❌ | ❌
| `a ? b : c` | ❌ | ❌ | ✅
