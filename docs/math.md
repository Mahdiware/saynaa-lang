# math

The `math` module provides basic mathematical functions and constants.

## Constants

### pi

The ratio of a circle's circumference to its diameter (approximately 3.14159).

```ruby
print(math.pi)
```

## Functions

### abs

Returns the absolute value of `x`.

```ruby
math.abs(x)
```

**Parameters:**
- `x` (Number): The number.

### ceil

Returns the smallest integer greater than or equal to `x`.

```ruby
math.ceil(x)
```

**Parameters:**
- `x` (Number): The number.

### floor

Returns the largest integer less than or equal to `x`.

```ruby
math.floor(x)
```

**Parameters:**
- `x` (Number): The number.

### round

Rounds the number to the nearest integer.

```ruby
math.round(x)
```

**Parameters:**
- `x` (Number): The number.

### sign

Returns the sign of `x`. output is -1, 0, or 1.

```ruby
math.sign(x)
```

**Parameters:**
- `x` (Number): The number.

### sqrt

Returns the square root of `x`.

```ruby
math.sqrt(x)
```

**Parameters:**
- `x` (Number): The number.

### pow

Returns `base` to the power of `exponent`.

```ruby
math.pow(base, exponent)
```

**Parameters:**
- `base` (Number): The base.
- `exponent` (Number): The exponent.

### log10

Returns the base-10 logarithm of `x`.

```ruby
math.log10(x)
```

**Parameters:**
- `x` (Number): The number.

### rand

Returns a random integer between 0 and 32767.

```ruby
math.rand()
```

### random

Returns a random integer between `min` and `max` (inclusive).

```ruby
math.random(min, max)
```

**Parameters:**
- `min` (Number): The minimum value.
- `max` (Number): The maximum value.

### Trigonometric Functions

All trigonometric functions typically work with radians.

#### sin, cos, tan

Returns the sine, cosine, or tangent of `x` (in radians).

```ruby
math.sin(x)
math.cos(x)
math.tan(x)
```

#### asin, acos, atan

Returns the arc sine, arc cosine, or arc tangent of `x`.

```ruby
math.asin(x)
math.acos(x)
math.atan(x)
```

#### atan2

Returns the arc tangent of `y/x`, accounting for the quadrant.

```ruby
math.atan2(y, x)
```

#### sinh, cosh, tanh

Returns the hyperbolic sine, cosine, or tangent of `x`.

```ruby
math.sinh(x)
math.cosh(x)
math.tanh(x)
```
