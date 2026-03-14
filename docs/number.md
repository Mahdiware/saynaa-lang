# Number

Numbers in Saynaa are 64-bit floating point values (double precision).

## Methods

### times

Executes a function `n` times, where `n` is the integer value of the number. The function receives the iteration index (0-based) as an argument.

```ruby
5.times(function(i) print(i) end)
```

**Parameters:**
- `fn` (Closure): The function to execute.

### isint

Returns true if the number is an integer.

```ruby
n = 3.14
print(n.isint()) ## false
m = 42
print(m.isint()) ## true
```

### isbyte

Returns true if the number is an integer between 0 and 255 (inclusive).

```ruby
n = 255
print(n.isbyte()) ## true
```
