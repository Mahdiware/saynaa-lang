# Regex Module (`re`)

Saynaa provides Perl Compatible Regular Expressions (PCRE) support through the `re` module.

```ruby
import re
```

## Functions

### match
Match a regular expression pattern to the **beginning** of a string.

```ruby
re.match(pattern: String, text: String) -> String|Null
```

### fullmatch
Match a regular expression pattern to **all** of a string.

```ruby
re.fullmatch(pattern: String, text: String) -> String|Null
```

### search
Scan through string looking for the **first location** where the regular expression pattern produces a match.

```ruby
re.search(pattern: String, text: String) -> String|Null
```

### findall
Return all non-overlapping matches of pattern in string, as a list of strings.

```ruby
re.findall(pattern: String, text: String) -> List
```

### split
Split string by the occurrences of pattern. If capturing parentheses are used in pattern, then the text of all groups in the pattern are also returned as part of the resulting list.

```ruby
re.split(pattern: String, text: String, maxsplit: Int = 0) -> List
```

### sub
Return the string obtained by replacing the leftmost non-overlapping occurrences of pattern in string by the replacement `repl`.

```ruby
re.sub(pattern: String, repl: String, text: String) -> String
```

### subn
Perform the same operation as `sub()`, but return a tuple `[new_string, number_of_subs_made]`.

```ruby
re.subn(pattern: String, repl: String, text: String) -> List
```

### escape
Escape special characters in a string.

```ruby
re.escape(pattern: String) -> String
```

### extract
Returns a list of captured groups from the first match.

```ruby
re.extract(pattern: String, text: String) -> List|Null
```

### purge
Clear the regular expression cache.

```ruby
re.purge() -> Null
```
