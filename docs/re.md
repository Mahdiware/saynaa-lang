# re

The `re` module provides Perl Compatible Regular Expressions (PCRE2) support.

## Functions

### match

Match a regular expression pattern to the **beginning** of a string.

```ruby
result = re.match(pattern, text)
```

**Parameters:**
- `pattern` (String): The regex pattern.
- `text` (String): The string to search.

**Returns:**
- (String|Null): The matched string if successful, otherwise `null`.

### fullmatch

Match a regular expression pattern to **all** of a string.

```ruby
result = re.fullmatch(pattern, text)
```

**Parameters:**
- `pattern` (String): The regex pattern.
- `text` (String): The string to search.

**Returns:**
- (String|Null): The matched string if successful, otherwise `null`.

### search

Scan through string looking for the first location where the regular expression pattern produces a match.

```ruby
result = re.search(pattern, text)
```

**Parameters:**
- `pattern` (String): The regex pattern.
- `text` (String): The string to search.

**Returns:**
- (String|Null): The matched string if successful, otherwise `null`.

### sub

Return the string obtained by replacing the leftmost non-overlapping occurrences of the pattern in string by the replacement `repl`.

```ruby
new_str = re.sub(pattern, repl, text)
```

**Parameters:**
- `pattern` (String): The regex pattern.
- `repl` (String): The replacement string.
- `text` (String): The text to process.

**Returns:**
- (String): The modified string.

### subn

Perform the same operation as `sub()`, but return a tuple (List) `[new_string, number_of_subs_made]`.

```ruby
result = re.subn(pattern, repl, text)
new_str = result[0]
count = result[1]
```

**Parameters:**
- `pattern` (String): The regex pattern.
- `repl` (String): The replacement string.
- `text` (String): The text to process.

**Returns:**
- (List): `[new_string, count]`

### split

Split string by the occurrences of pattern.

```ruby
parts = re.split(pattern, text, maxsplit)
```

**Parameters:**
- `pattern` (String): The regex pattern.
- `text` (String): The text to split.
- `maxsplit` (Number): Optional. The maximum number of splits. 0 for unlimited.

**Returns:**
- (List): The list of strings.

### extract

Find a pattern in the string and returns a list containing the full match as the first element, and captured groups as the subsequent elements.

```ruby
groups = re.extract("(\\d+)-(\\d+)", "123-456")
## groups -> ["123-456", "123", "456"]
```

**Parameters:**
- `pattern` (String): The regex pattern.
- `text` (String): The text to search.

**Returns:**
- (List|Null): The list of matches or null.

### findall

Return all non-overlapping matches of pattern in string, as a list of strings (or list of lists for groups).

```ruby
matches = re.findall(pattern, text)
```

**Parameters:**
- `pattern` (String): The regex pattern.
- `text` (String): The text to search.

**Returns:**
- (List): The list of matches.

### escape

Escape all the characters in pattern except ASCII letters, numbers and '_'.

```ruby
safe_pattern = re.escape(text)
```

**Parameters:**
- `text` (String): The text to escape.

**Returns:**
- (String): The escaped pattern.

### purge

Clear the regular expression cache.

```ruby
re.purge()
```
