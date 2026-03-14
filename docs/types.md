# types

The `types` module provides additional data types and utility functions.

## Functions

### hashable

Returns a boolean indicating if the value can be hashed (used as a map key).

```ruby
can_hash = types.hashable(value)
```

**Parameters:**
- `value` (Any): The value to check.

**Returns:**
- (Boolean): True if hashable, false otherwise.

### hash

Returns the hash of the value. Raises an error if the value is not hashable.

```ruby
h = types.hash(value)
```

**Parameters:**
- `value` (Any): The value to hash.

**Returns:**
- (Number): The hash code.

## Classes

### ByteBuffer

A mutable byte buffer for efficient binary data manipulation and string construction. This class helps avoid excessive string allocations when building large strings.

#### Methods

##### ByteBuffer()

Creates a new empty byte buffer.

```ruby
buf = types.ByteBuffer()
```

##### reserve(count)

Reserves memory for the buffer to avoid frequent reallocations.

```ruby
buf.reserve(1024)
```

**Parameters:**
- `count` (Number): The number of bytes to reserve.

##### fill(value, count)

Fills the buffer with a byte value repeated `count` times.

```ruby
buf.fill(0, 100) ## Fill with 100 zero bytes
```

**Parameters:**
- `value` (Number): The byte value (0-255).
- `count` (Number): The number of times to write the value.

##### clear()

Clears the buffer content, resetting count to 0.

```ruby
buf.clear()
```

##### write(data)

Writes data to the buffer.

- If `data` is a **Number** (0-255), writes a single byte.
- If `data` is a **String**, writes all bytes of the string.
- If `data` is a **Boolean**, writes 1 for true, 0 for false.

```ruby
buf.write(65)      ## Writes 'A'
buf.write("Hello") ## Writes "Hello"
```

**Parameters:**
- `data` (Number|String|Boolean): The data to write.

**Returns:**
- (Number): The number of bytes written (1 for byte/bool, length for string).

##### string()

Returns the buffer content as a string.

```ruby
s = buf.string()
```

**Returns:**
- (String): The buffer content.

##### count()

Returns the number of bytes written to the buffer.

```ruby
c = buf.count()
```

**Returns:**
- (Number): The byte count.

##### operator [](index)

Gets the byte at the specified index.

```ruby
byte = buf[0]
```

**Parameters:**
- `index` (Number): The index.

**Returns:**
- (Number): The byte value (0-255).

##### operator []=(index, value)

Sets the byte at the specified index.

```ruby
buf[0] = 65
```

**Parameters:**
- `index` (Number): The index.
- `value` (Number): The byte value (0-255).

### Vector

A simple 3D vector type with x, y, and z components.

#### Properties

- `x`: (Number) X component. Default is 0.
- `y`: (Number) Y component. Default is 0.
- `z`: (Number) Z component. Default is 0.

#### Constructor

##### Vector(x, y, z)

Creates a new Vector.

```ruby
v = types.Vector(1.0, 2.0, 3.0)
v2 = types.Vector(1, 2) ## z defaults to 0
```

**Parameters:**
- `x` (Number): Optional. Default is 0.
- `y` (Number): Optional. Default is 0.
- `z` (Number): Optional. Default is 0.

#### String Representation

The vector can be printed directly.

```ruby
print(v) ## Output: [1, 2, 3]
```
