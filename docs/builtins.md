# Built-in Functions

Saynaa provides a set of built-in functions that are always available.

## General

### `print(...)`
Prints values to standard output.

```ruby
print("Hello", "World")
```

### `str(value)`
Converts a value to its string representation.

```ruby
s = str(42) # "42"
```

### `int(value)`
Converts a value to an integer number.

```ruby
n = int("123") # 123
```

### `type(value)`
Returns the type name of the value as a string.

```ruby
t = type(42) # "Number"
```

### `help(value)`
Prints help information about a value (closure, method, class).

```ruby
help(print)
```

### `error(value)`
Raises a runtime error with the given value as message.

```ruby
error("Something went wrong")
```

### `eval(code)`
Evaluates a string of code and returns the result of the last expression.

```ruby
val = eval("1 + 2") # 3
```

### `define(name, value)`
Defines a global variable with the given name and value.

```ruby
define("my_global", 42)
print(my_global) # 42
```

## String & Character

### `chr(codepoint)`
Returns a string character from a numeric unicode codepoint.

```ruby
s = chr(65) # "A"
```

### `ord(char)`
Returns the numeric unicode codepoint of a character.

```ruby
n = ord("A") # 65
```

## Numeric

### `bin(number)`
Returns the binary string representation of a number.

```ruby
b = bin(10) # "0b1010"
```

### `hex(number)`
Returns the hexadecimal string representation of a number.

```ruby
h = hex(255) # "0xff"
```

### `min(a, b)`
Returns the smaller of two values.

```ruby
m = min(10, 20) # 10
```

### `max(a, b)`
Returns the larger of two values.

```ruby
m = max(10, 20) # 20
```
