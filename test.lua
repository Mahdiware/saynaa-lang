-- vec types
local Vec2 = {}
Vec2.__index = Vec2

function Vec2.new(x, y)
  return setmetatable({x = x, y = y}, Vec2)
end

local Vec3 = {}
Vec3.__index = Vec3

function Vec3.new(x, y, z)
  return setmetatable({x = x, y = y, z = z}, Vec3)
end

-- matrix
local Matrix2 = {}
Matrix2.__index = Matrix2

function Matrix2.new(a, b, c, d)
  return setmetatable({a=a, b=b, c=c, d=d}, Matrix2)
end

function Matrix2:mul(v)
  return Vec2.new(
    self.a * v.x + self.b * v.y,
    self.c * v.x + self.d * v.y
  )
end

-- math functions
local function dot2(a, b)
  return a.x * b.x + a.y * b.y
end

local function dot3(a, b)
  return a.x * b.x + a.y * b.y + a.z * b.z
end

local function clamp(v, lo, hi)
  if v < lo then return lo end
  if v > hi then return hi end
  return v
end

local function poly(x)
  return (((x * 3 + 7) * x - 11) * x + 23)
end

local function reduce_sum(items)
  local s = 0
  for i = 1, #items do
    s = s + items[i]
  end
  return s
end

local function make_points(n)
  local pts = {}
  for i = 0, n-1 do
    pts[#pts+1] = Vec2.new(i, i * 2)
  end
  return pts
end

-- benchmark
local function run()
  local points = make_points(32)
  local m = Matrix2.new(1,2,3,4)

  local acc = 0

  for _, p in ipairs(points) do
    local q = m:mul(p)
    acc = acc + dot2(q, Vec2.new(2,1))
  end

  local v3 = Vec3.new(1,2,3)
  acc = acc + dot3(v3, Vec3.new(3,2,1))
  acc = acc + poly(clamp(14,0,10))
  acc = acc + reduce_sum({1,2,3,4,5})

  assert(acc == 14054, "value: " .. acc)
  return acc
end

-- timing harness
local t0 = os.clock()
for i = 1, 200000 do
  run()
end
local t1 = os.clock()

print("Lua time:", t1 - t0)