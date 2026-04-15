import time

# -------------------------
# Vector types
# -------------------------

class Vec2:
    __slots__ = ("x", "y")

    def __init__(self, x, y):
        self.x = x
        self.y = y


class Vec3:
    __slots__ = ("x", "y", "z")

    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z


# -------------------------
# Matrix
# -------------------------

class Matrix2:
    __slots__ = ("a", "b", "c", "d")

    def __init__(self, a, b, c, d):
        self.a = a
        self.b = b
        self.c = c
        self.d = d

    def mul(self, v: Vec2):
        return Vec2(
            self.a * v.x + self.b * v.y,
            self.c * v.x + self.d * v.y
        )


# -------------------------
# Math functions
# -------------------------

def dot2(a, b):
    return a.x * b.x + a.y * b.y


def dot3(a, b):
    return a.x * b.x + a.y * b.y + a.z * b.z


def clamp(v, lo, hi):
    if v < lo:
        return lo
    if v > hi:
        return hi
    return v


def poly(x):
    return (((x * 3 + 7) * x - 11) * x + 23)


def reduce_sum(items):
    s = 0
    for i in items:
        s += i
    return s


# -------------------------
# Benchmark workload
# -------------------------

def make_points(n):
    pts = []
    for i in range(n):
        pts.append(Vec2(i, i * 2))
    return pts


def main():
    points = make_points(32)
    m = Matrix2(1, 2, 3, 4)

    acc = 0

    for p in points:
        q = m.mul(p)
        acc += dot2(q, Vec2(2, 1))

    v3 = Vec3(1, 2, 3)
    acc += dot3(v3, Vec3(3, 2, 1))
    acc += poly(clamp(14, 0, 10))
    acc += reduce_sum([1, 2, 3, 4, 5])

    assert acc == 14054, f"value: {acc}"
    return acc


# -------------------------
# Benchmark loop
# -------------------------

N = 200000

t0 = time.perf_counter()

acc_total = 0
for _ in range(N):
    acc_total += main()

t1 = time.perf_counter()

print("Execution time:", t1 - t0, "seconds")