# fmtstr-hpp

A quickly-made unoptimized and limited recreation of Python formatted strings in a C++ header. Requires [nlohmann/json](https://json.nlohmann.me/).

### Usage
Include `fmtstr.hpp` and use the `fmtstr::fmtstr(<str>)` function on your desired `std::string`.

### Data Types
- Integers `int` (int)
  - You can define hexadecimal integers with `0x` (e.g. `0xFF` = `255`) but **only use capital letters** for alphanumeric digits
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

### Limitations
- You cannot use the unary operator `-a` to swap the sign of a value. Subtract into `0` instead (`0-a`).
- There is no NOT (`!a`) operator. Instead use `a == false` or `a != true`.
- You cannot use data types other than `int`, `bool` and `str`.
- You cannot directly typecast. You can cast a `bool` to an `int` using an operation (such as `true + 0`), and you can cast a `bool` or `int` to a `str` by placing it inside a formatted string (such as `{1+1}`)
- You cannot use the unary operator `+a`. I'm not sure why you would though...

### Examples
`test.cpp` has some test strings that showcase various operators in action.
