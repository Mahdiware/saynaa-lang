# Data Types

Saynaa supports several built-in data types, divided into primitives and objects.

## Primitive Types

*   **Null**: Represents the absence of a value.
*   **Boolean**: `true` or `false`.
*   **Number**: 64-bit floating point numbers (and optimized 32-bit integers internally).

## Object Types

*   **String**: UTF-8 immutable text.
*   **[List](list.md)**: Dynamic array of values.
*   **[Map](map.md)**: Key-value hash map.
*   **[Range](range.md)**: A sequence of numbers.
*   **Function / Closure**: Executable code blocks.
*   **Class / Instance**: User-defined types.

## Type Checking

You can check the type name of any value using the `type()` built-in function or the `.typename()` method.

```ruby
a = 42
print(type(a))      # Number
print(a.typename()) # Number
```
