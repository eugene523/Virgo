##
In this test we are testing basic logic operations:
and, or, xor, not, ==, !=
##

# And
assert(true and false = false)
assert((1 < 2) and (2 < 3))

# Or
assert(true or false = true)
assert((1 < 2) or (2 > 5))

# Xor
assert(true xor false)
assert(true xor true = false)
assert(false xor false = false)

# Not
assert(not false)
assert(not not false = false)
assert(not not not false)