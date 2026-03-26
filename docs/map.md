# Map

Maps are key-value pairs (dictionaries). Keys must be hashable.

## Creation

```ruby
m = {"name": "Alice", "age": 30}
```

You can also create a map with value-only entries. Keys are auto-assigned
starting at 0 and increasing by 1 for each value.

```ruby
m = {10, 20, 30}
# => {0: 10, 1: 20, 2: 30}
```

If you mix explicit numeric keys, the next auto key continues after the
highest numeric key seen so far.

Maps preserve insertion order for printing, iteration, and the `keys`/`values`
properties.

## Properties

### length

The number of items in the map.

```ruby
l = m.length
```

### keys

Returns a list of the map's keys.

```ruby
k = m.keys
## ["name", "age"]
```

### values

Returns a list of the map's values.

```ruby
v = m.values
## ["Alice", 30]
```

## Methods

### get

Return the value for `key` if `key` is in the map, else `default`. If `default` is not given, it defaults to `null`.

```ruby
val = m.get("name")
val2 = m.get("missing", "default")
```

**Parameters:**
- `key` (Any): The key.
- `default` (Any): Optional.

### has

Returns true if the map has the key `key`.

```ruby
present = m.has("name")
```

**Parameters:**
- `key` (Any): The key.

### pop

If `key` is in the map, remove it and return its value. If not found, raise an error (Verify).

```ruby
val = m.pop("age")
```

**Parameters:**
- `key` (Any): The key.

### clear

Remove all items from the map.

```ruby
m.clear()
```
