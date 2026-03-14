# Range

Ranges are immutable sequences of numbers.

## Creation

Ranges are created using the `..` operator.

```ruby
r = 1..10
```

## Properties

### length

The number of items in the range.

```ruby
print((1..10).length) ## 9
```

### first

The start of the range (inclusive).

```ruby
f = r.first
```

### last

The end of the range (inclusive).

```ruby
l = r.last
```

### as_list

Returns the range as a list of numbers.

```ruby
l = r.as_list
## [1, 2, ..., 10]
```
