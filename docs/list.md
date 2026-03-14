# List

Lists are dynamic arrays of values.

## Creation

```ruby
l = [1, 2, 3]
```

## Properties

### length

The number of elements in the list.

```ruby
n = l.length
```

## Methods

### append

Adds an item to the end of the list.

```ruby
l.append(4)
```

### insert

Inserts an item at a given position.

```ruby
l.insert(0, "start")
```

**Parameters:**
- `index` (Number): The index to insert at.
- `value` (Any): The value to insert.

### pop

Removes the item at the given position in the list, and returns it. If no index is specified, `pop()` removes and returns the last item in the list.

```ruby
last = l.pop()
first = l.pop(0)
```

**Parameters:**
- `index` (Number): Optional. Default is -1 (last).

### find

Return the index of the first item whose value is equal to `value`. Raises an error if no such item is found (Verify this behavior).

```ruby
idx = l.find(2)
```

**Parameters:**
- `value` (Any): The value to search for.

### clear

Remove all items from the list.

```ruby
l.clear()
```

### join

Return a string which is the concatenation of the strings in the list. `sep` is the separator between elements.

```ruby
s = ["a", "b", "c"].join("-") ## "a-b-c"
```

**Parameters:**
- `sep` (String): The separator string.

### resize

Resize the list to contain `size` elements.

```ruby
l.resize(10)
```

**Parameters:**
- `size` (Number): The new size. If smaller, elements are removed. If larger, `null` is padded.

