# Testing function calls

delta = 1e-10

#------------------------------------------------------------------------------

cube[q]
  -> q^3

a = cube[3]
assert[a == 27]

#------------------------------------------------------------------------------

# Here we are testing 'cube' accessibility
# from internal context of 'twice_cube'.
twice_cube[n]
  -> 2 * cube[n]

a = twice_cube[3]
assert[a == 54]

#------------------------------------------------------------------------------

hypotenuse[a, b]
  -> (a^2 + b^2)^0.5

assert[eq[hypotenuse[3, 4], 5, delta]]

a = 3
b = 4
assert[eq[hypotenuse[a, b], 5, delta]]

#------------------------------------------------------------------------------

factorial[n]
  assert[n >= 0]
  if n == 0
    -> 1
  -> n * factorial[n - 1]

assert[factorial[0] == 1  ]
assert[factorial[1] == 1  ]
assert[factorial[2] == 2  ]
assert[factorial[3] == 6  ]
assert[factorial[4] == 24 ]
assert[factorial[5] == 120]

#------------------------------------------------------------------------------