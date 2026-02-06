# Terminal Module (`term`)

The `term` module provides a portable interface for terminal manipulation, including colors, cursor movement, and screen buffers.

```ruby
import term
```

## Functions

### init
Initialize the terminal for control (raw mode, etc.).

```ruby
term.init() -> Bool
```

### cleanup
Restore terminal state.

```ruby
term.cleanup() -> Null
```

### clear
Clear the screen.

```ruby
term.clear() -> Null
```

### clear_eol
Clear to end of line.

```ruby
term.clear_eol() -> Null
```

### clear_eof
Clear to end of file (screen).

```ruby
term.clear_eof() -> Null
```

### getsize
Return the size of the terminal as `[rows, cols]`.

```ruby
term.getsize() -> List
```

### getposition
Return the current cursor position as `[row, col]`.

```ruby
term.getposition() -> List
```

### setposition
Set the cursor position.

```ruby
term.setposition(row: Int, col: Int) -> Null
```

### write
Write text to the terminal.

```ruby
term.write(text: String) -> Null
```

### flush
Flush the output buffer.

```ruby
term.flush() -> Null
```

### hide_cursor
Hide the cursor.

```ruby
term.hide_cursor() -> Null
```

### show_cursor
Show the cursor.

```ruby
term.show_cursor() -> Null
```

## Colors and Styles

### start_color
Set foreground color. `color` is an integer index (0-255).

```ruby
term.start_color(color: Int) -> Null
```

### start_bg
Set background color.

```ruby
term.start_bg(color: Int) -> Null
```

### Predefined Colors

*   `term.start_color_red()` / `term.end_color_red()`
*   `term.start_color_green()` / `term.end_color_green()`
*   `term.start_color_yellow()` / `term.end_color_yellow()`
*   `term.start_color_blue()` / `term.end_color_blue()`
*   `term.start_color_magenta()` / `term.end_color_magenta()`
*   `term.start_color_cyan()` / `term.end_color_cyan()`
*   `term.start_color_white()` / `term.end_color_white()`
*   `term.start_color_black()` / `term.end_color_black()`

### Styles

*   `term.start_bold()` / `term.end_bold()`
*   `term.start_dim()` / `term.end_dim()`
*   `term.start_italic()` / `term.end_italic()`
*   `term.start_underline()` / `term.end_underline()`
*   `term.start_inverse()` / `term.end_inverse()`
*   `term.start_strikethrough()` / `term.end_strikethrough()`

## Screen Buffers

### new_screen_buffer
Switch to a new screen buffer (alternate screen).

```ruby
term.new_screen_buffer() -> Null
```

### restore_screen_buffer
Return to the main screen buffer.

```ruby
term.restore_screen_buffer() -> Null
```

## Input

### read_event
Read a single input event. Returns a dictionary describing the event (key press, etc.) or `null`.

```ruby
term.read_event(timeout_ms: Int) -> Map|Null
```
