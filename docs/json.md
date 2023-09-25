## json Module
json is a builtin Module.

```ruby
import json
```

### parse
Parse a json string into saynaa object.

```ruby
json.parse(json_str:String) -> Var
```

### print
Render a saynaa value into text. Takes an optional argument pretty, if true it'll pretty print the output.

```ruby
json.print(value:Var, pretty:Bool=false)
```
