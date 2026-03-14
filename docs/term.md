# term

The `term` module provides a portable interface for terminal manipulation, including colors, cursor movement, and screen buffers.

## Functions

### init

Initialize the terminal for control (e.g., enter raw mode).

```ruby
term.init()
```

### cleanup

Restore terminal state. Call this before exiting.

```ruby
term.cleanup()
```

### isatty

Returns true if the standard output is a terminal.

```ruby
is_tty = term.isatty()
```

### Screen Buffers

#### new_screen_buffer

Switch to the alternate screen buffer.

```ruby
term.new_screen_buffer()
```

#### restore_screen_buffer

Return to the main screen buffer.

```ruby
term.restore_screen_buffer()
```

#### flush

Flush the output buffer to the screen.

```ruby
term.flush()
```

#### clear

Clear the screen.

```ruby
term.clear()
```

#### clear_eol

Clear from cursor to end of line.

```ruby
term.clear_eol()
```

#### clear_eof

Clear from cursor to end of file (screen).

```ruby
term.clear_eof()
```

#### reset

Reset terminal attributes to default.

```ruby
term.reset()
```

### Cursor

#### getsize

Get the terminal size as a vector (columns, rows).

```ruby
size = term.getsize()
// size.x is columns, size.y is rows
```

#### getposition

Get the cursor position.

```ruby
pos = term.getposition()
// pos.x is column, pos.y is row
```

#### setposition

Set the cursor position.

```ruby
term.setposition(x, y)
```

**Parameters:**
- `x` (Number): Column.
- `y` (Number): Row.

#### hide_cursor

Hide the cursor.

```ruby
term.hide_cursor()
```

#### show_cursor

Show the cursor.

```ruby
term.show_cursor()
```

### Output & Styling

#### write

Write text to the terminal buffer.

```ruby
term.write("Hello")
```

#### set_title

Set the terminal window title.

```ruby
term.set_title("My App")
```

#### start_color / end_color

Set or unset foreground color.

```ruby
term.start_color(color_code)
term.end_color()
```

#### start_bg / end_bg

Set or unset background color.

```ruby
term.start_bg(color_code)
term.end_bg()
```

#### Styles

Functions to start and end text styles:

- `term.start_bold()` / `term.end_bold()`
- `term.start_dim()` / `term.end_dim()`
- `term.start_italic()` / `term.end_italic()`
- `term.start_underline()` / `term.end_underline()`
- `term.start_inverse()` / `term.end_inverse()`
- `term.start_hidden()` / `term.end_hidden()`
- `term.start_strikethrough()` / `term.end_strikethrough()`

#### Helper Colors

Functions to start/end specific colors easily:

- `term.start_color_black()` / `term.end_color_black()`
- `term.start_color_red()` / `term.end_color_red()`
- `term.start_color_green()` / `term.end_color_green()`
- `term.start_color_yellow()` / `term.end_color_yellow()`
- `term.start_color_blue()` / `term.end_color_blue()`
- `term.start_color_magenta()` / `term.end_color_magenta()`
- `term.start_color_cyan()` / `term.end_color_cyan()`
- `term.start_color_white()` / `term.end_color_white()`
- `term.start_color_default()` / `term.end_color_default()`

### Input

#### read_event

Read an event into the provided `Event` object. Returns true if an event was read.

```ruby
ev = term.Event()
if term.read_event(ev) {
  print(ev.type)
}
```

**Parameters:**
- `event` (term.Event): The event object to populate.

**Returns:**
- (Boolean): True if an event was read, false otherwise.

### binary_mode

Set stdout to binary mode (Windows specific).

```ruby
term.binary_mode()
```

## Classes

### term.Event

A class to hold terminal event data.

#### Properties

- `type` (Number): Event type.
- `key` (Number): Key code (if key event).
- `ch` (Number): Character code (if char event).
- `mod` (Number): Modifier keys.
- `w` (Number): Width (resize event).
- `h` (Number): Height (resize event).
- `x` (Number): Mouse X.
- `y` (Number): Mouse Y.

### term.Config

Configuration class (usage not fully documented yet).
