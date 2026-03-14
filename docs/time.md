# time

The `time` module provides functions to work with time.

## Functions

### epoch

Returns the number of seconds since the Epoch (1970-01-01 00:00:00 UTC).

```ruby
seconds = time.epoch()
```

**Returns:**
- (Number): Seconds since epoch.

### nano

Returns the current time in nanoseconds.

```ruby
ns = time.nano()
```

**Returns:**
- (Number): Nanoseconds.

### sleep

Sleep for `ms` milliseconds.

```ruby
time.sleep(1000) ## Sleep for 1 second
```

**Parameters:**
- `ms` (Number): Milliseconds to sleep.

### clock

Returns the processor time consumed by the program in seconds (clocks / CLOCKS_PER_SEC).

```ruby
cpu_time = time.clock()
```

**Returns:**
- (Number): CPU time in seconds.
