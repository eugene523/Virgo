##
In this test we are testing basic logic operations:
and, or, not, =, !=
##

# And
assert(true and false = false)
assert((1 < 2) and (2 < 3))

# Or
assert(true or false = true)
assert((1 < 2) or (2 > 5))

# Not
assert(not false)
assert(not not false = false)
assert(not not not false)