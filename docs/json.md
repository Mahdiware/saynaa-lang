# json

The `json` module provides functions for parsing and generating JSON data.

## Functions

### parse

Parses a JSON string into a valid Saynaa object (Map, List, String, Number, Boolean, or Null).

```ruby
data = json.parse('{"name": "Saynaa", "version": 1.0}')
```

**Parameters:**
- `text` (String): The JSON string to parse.

**Returns:**
- (Any): The parsed value.

### print

Converts a Saynaa object into a JSON string.

```ruby
json_str = json.print(data, true)
```

**Parameters:**
- `value` (Any): The value to serialize.
- `pretty` (Boolean): Optional. If true, the output will be pretty-printed. Default is `false`.

**Returns:**
- (String): The JSON string.
