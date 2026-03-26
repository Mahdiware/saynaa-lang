# String

Strings are immutable sequences of UTF-8 characters.

## Properties

### length

Returns the number of characters in the string.

```ruby
s = "Hello"
len = s.length ## 5
```

## Methods

### strip

Returns a copy of the string with leading and trailing whitespace removed.

```ruby
s = "  hello  "
trimmed = s.strip() ## "hello"
```

### lower

Returns a copy of the string converted to lowercase.

```ruby
s = "Hello"
low = s.lower() ## "hello"
```

### upper

Returns a copy of the string converted to uppercase.

```ruby
s = "Hello"
up = s.upper() ## "HELLO"
```

### find

Returns the lowest index in the string where substring `sub` is found. Returns -1 if not found.

```ruby
idx = "hello".find("l") ## 2
```

**Parameters:**
- `sub` (String): The substring to search for.
- `start` (Number): Optional. Start index.
- `end` (Number): Optional. End index.

### rfind

Returns the highest index in the string where substring `sub` is found. Returns -1 if not found.

```ruby
idx = "hello".rfind("l") ## 3
```

**Parameters:**
- `sub` (String): The substring to search for.
- `start` (Number): Optional. Start index.
- `end` (Number): Optional. End index.

### replace

Return a copy of the string with all occurrences of substring `old` replaced by `new`.

```ruby
s = "hello world"
new_s = s.replace("world", "saynaa")
```

**Parameters:**
- `old` (String): The substring to replace.
- `new` (String): The replacement string.
- `count` (Number): Optional. Maximum number of replacements.

### split

Return a list of the words in the string, using `sep` as the delimiter string.

```ruby
parts = "a,b,c".split(",") ## ["a", "b", "c"]
```

**Parameters:**
- `sep` (String): The delimiter.
- `maxsplit` (Number): Optional. Maximum number of splits.

### startswith

Returns true if the string starts with the specified prefix.

```ruby
"hello".startswith("he") ## true
```

**Parameters:**
- `prefix` (String): The prefix.

### endswith

Returns true if the string ends with the specified suffix.

```ruby
"hello".endswith("lo") ## true
```

**Parameters:**
- `suffix` (String): The suffix.

### sub

Returns the substring from `start` (inclusive) to `end` (exclusive).

```ruby
"hello".sub(1, 4) ## "ell"
"hello".sub(-2) ## "lo"
```

**Parameters:**
- `start` (Number): Start index (0-based). Negative values count from the end.
- `end` (Number): Optional end index (exclusive). Negative values count from the end.

### reverse

Returns a copy of the string with bytes reversed.

```ruby
"abc".reverse() ## "cba"
```

### rep

Returns a new string repeated `count` times.

```ruby
"ha".rep(3) ## "hahaha"
```

**Parameters:**
- `count` (Number): Repeat count (>= 0).

### byte

Returns the byte value at `index`.

```ruby
"abc".byte(1) ## 98
```

**Parameters:**
- `index` (Number): 0-based index. Negative values count from the end.

### format

Formats the string using printf-style specifiers.

```ruby
"name=%s age=%d".format("bob", 5)
```

### match

Returns the first substring match of `sub` starting at `start`, or `null`.

```ruby
"hello".match("ll") ## "ll"
```

### gsub

Replaces occurrences of `old` with `new`.

```ruby
"a-b-a".gsub("a", "x") ## "x-b-x"
```

### gmatch

Returns a list of all substring matches of `sub`.

```ruby
"a-b-a".gmatch("a") ## ["a", "a"]
```
