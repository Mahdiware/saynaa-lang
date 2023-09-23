## Map

Maps are associative containers implemented as pairs each of which maps a key to a value. You can create a map by placing a series of comma-separated entries inside brace. Each entry is a key and a value separated by a colon:

```ruby
  // create a new map with 4 entries
  d = {"Banana":10, "Apple":20, "Lime":30, "Orange":40};

  // map has a length attribute
  n = d.length;  // n is 4

  // create an empty map
  map = {};
```

### Looking up values
You can access an element from a list by calling the subscript operator [] on it with the key of the element you want:
      
```ruby
  names = {"Banana":10, "Apple":20, "Lime":30, "Orange":40};
  names["Banana"];      // 10
  names["Apple"];       // 20
```

### Iterating items
The subscript operator works well for finding values when you know the key you’re looking for, but sometimes you want to see everything that’s in the map. Since the Map class implements the iterator method (through the keys method), you can easily use it in a for loop:
```ruby
  fruits = {"Banana":10, "Apple":20, "Lime":30, "Orange":40};
  for name in fruits.keys do
    print("Current name is " + name);
  end
```

### Adding items
An item can be added to a map by simply setting a key/value:
```ruby
  fruits = {"Banana":10, "Apple":20, "Lime":30, "Orange":40};
  fruits["Grape"] = 50;  // fruits now contains the "Grape" key with value 50
```

### Retrieving keys
The keys method has been added to the map class as a conveniente way to get access to all keys:
```ruby
  fruits = {"Banana":10, "Apple":20, "Lime":30, "Orange":40};
  fruits.keys; // ["Banana", "Orange", "Lime", "Apple"]
```

