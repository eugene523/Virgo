##
In this test we are testing basic arithmetic operations:
(+, -, *, /, ^)
(+=, -=, *=, /=, ^=)
(> , >=, <, <=)
and some tricky expressions performed on integer and real value types.
Sometimes we are adding some extra parentheses and plus/minus signs before terms
in order to test parsing too.
##

# This parameter is used to campare real numbers.
delta = 1e-10

# Plus (+) --------------------------------------------------------------------
assert( 1 + 2       =  3 )
assert( 1 + 2 + 3   =  6 )
assert((1 + 2) + 3  =  6 )
assert( 1 + -2      = -1 )
assert(-1 + -2 + -3 = -6 )
assert(-(1 + 2) + 3 =  0 )

a = 1 + 2
assert(a = 3)

a = 1
b = 2
c = a + b
assert(c = 3)

c = (a + b)
assert(c = 3)

assert((a) + b = (b) + a)

c = ((((a) + (b))))
assert(c = 3)

a = 3
b = 4.5
assert((a + b) = 7.5)

c = 1.2
d = 1.57
assert((c + d) = 2.77)

a = 1
b = 1.2
c = -3
d = 5.9
q = ((a
      +
      b)
      +
     (c
      +
      d))
assert(near(q, 5.1, delta))

assert(near(((a + b) + c) + d, 5.1, delta))
assert(near((a + (b + (c + (d)))), 5.1, delta))

# Minus (-) -------------------------------------------------------------------
a = 1 - 2
assert(a = -1)

a = 1
b = 2
c = a - b
assert(c = -1)

c = (a - b)
assert(c = -1)

c = (a) - b
assert(c = -1)

c = ((((a) - (b))))
assert(c = -1)

a = 3
b = 4.5
assert(near((a - b), -1.5, delta))

c = 1.2
d = 1.57
assert(near((c - d), -0.37, delta))

a = 1
b = 1.2
c = -3
d = 5.9
q = (a - b) - (c - d)

assert(near(q, 8.7, delta))
assert(near(((a - b) - c) - d, -3.1, delta))
assert(near((a - (b - (c - (d)))), -9.1, delta))

# Multiply (*) ----------------------------------------------------------------
assert(-1 * -1 = 1)
assert((((((1) * 2) * 3) * 4) * 5) = 120)

a = 3 * 4
assert(a = 12)

a = 5
b = -5
c = a * b
assert(c = -25)

a = -6
b = -7
c = (a * b * 2)
assert(c = 84)

c = (((((a))) * (((b)))))
assert(c = 42)

c = ((((a) * (b))))
assert(c = 42)

a = -3
b = 4.5
assert(near((a * b), -13.5, delta))

c = 1.2
d = 1.57
assert(near((c * d), 1.884, delta))

a = -1.367892
b =  1.2
c = -3
d =  5.9
q = (a * b) * (c * d)

assert(near(q, 29.05402608, delta))
assert(near(((a * b) * c) * d, 29.05402608, delta))
assert(near((a * (b * (c * (d)))), 29.05402608, delta))

# Divide (/) ------------------------------------------------------------------
assert(-1 / -1 = 1)
assert(near(1 / -2, -0.5, delta))

a = 3 / 4
assert(near(a, 0.75, delta))
a = 5.6
b = -5.7
c = a / b
assert(near(c, -0.982456140350877, delta))

a = -6
b = -7
c = ((a / b) / 2)
assert(near(c, 0.4285714285714285, delta))

c = (((((a))) / (((b)))))
assert(near(c, 0.857142857142857, delta))

# Remainder (%) ---------------------------------------------------------------
assert(rem(5, 2) = 1)
assert(rem(-5, 3) = 1)

# Power (^) -------------------------------------------------------------------
assert(near(3 ^ 2, 9, delta))
assert(near(2 ^ 0.5, 1.414213562373095, delta))

# Plus-equal (+=) -------------------------------------------------------------
a = 3.5
b = 1.7
a += -b
assert(near(a, 1.8, delta))

# Minus-equal (-=) ------------------------------------------------------------
a = 3.5
b = 1.7
a -= -b
assert(near(a, 5.2, delta))

# Multiply-equal (*=) ---------------------------------------------------------
a = 3.5
b = 1.7
a *= -b
assert(near(a, -5.95, delta))

# Divide-equal (/=) -----------------------------------------------------------
a = -3.5
b = 1.7
a /= -b
assert(near(a, 2.05882352941176, delta))

# Power-equal (^=) ------------------------------------------------------------
a = 3.0
a ^= 3
assert(near(a, 27, delta))

# Greater (>), Greater or equal (>=) ------------------------------------------
assert(3.5 > 1)
assert((3.1 + 5) >= (1 + 2.0))

# Less (<), Less or equal (<=) ------------------------------------------------
assert(3.5 < 8 ^ 2)
assert(3.3^3.5 <= 3.3^3.5)
