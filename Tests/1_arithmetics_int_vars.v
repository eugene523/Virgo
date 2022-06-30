##
In this test we are testing basic arithmetic operations for 'int' type:
(+, -, *)
(> , >=, <, <=)
We don't test division '/' and power '^' because they return 'real' type.
##

# Primarily addition

a0 = 0
a1 = 1
a2 = 2
a3 = 3
a4 = 4
a5 = 5
a6 = 6
a120 = 120

a = a1 + a2
assert(a = a3)

a = a1
b = a2
c = a + b
assert(c = a3)

c = (a + b)
assert(c = a3)

assert((a) + b = (b) + a)

c = ((((a) + (b))))
assert(c = a3)

a0 = a0
a1 = a1
a2 = a2
a3 = a3
a4 = a4
a5 = a5
a6 = a6

assert(a2 + a3 = a5)
assert( a1 + a2 + a3   =  a6 )
assert((a1 + a2) + a3  =  a6 )
assert( a1 + -a2      = -a1 )
assert(-(a1 + a2) + a3 =  a0 )
assert(a1 + a2 + a3 = a3 + a2 + a1)
assert(a1 + a2 + a3 + a4 + a5 = +15)
assert((a1) + (a2) + (a3) + (a4) + (a5) = (15))
assert(-a1 + -a2 + -a3 + -a4 + -a5 = -15)
assert((a1) + (a2) + (a3) + (a4) + (a5) = -(-a1 + -a2 + -a3 + -a4 + -a5))
assert(-(-a1) + -(-a2) + -(-a3) + -(-a4) + -(-a5) = -(-15))

assert(a1
       +
      -a2
       +
       a3
       +
      -a4
       +
       a5
       =
       a3)

assert((a1 + (a2 + (a3 + (a4 + (a5))))) = 15)
assert((((((a1) + a2) + a3) + a4) + a5) = 15)
assert((a1 + (a2 + (a3 + (a4 + (a5))))) = (((((a1) + a2) + a3) + a4) + a5))
assert((a1 - (a2 + (a3 - (a4 + (-a5))))) = -a5)
assert((((((a1) - a2) + a3) - a4) + a5) = a3)

assert( 
        (((((a1)))))
             =
        (((((a1)))))
      )

assert(
        a1 + 
       -a2 + 
      --a3 +
     ---a4 +
    ----a5
        =
       (a1)
        +
      (-(a2))
        +
     (-(-(a3)))
        +
    (-(-(-(a4))))
        +
   (-(-(-(-(a5)))))
        )
       
# Primarily subtraction
       
assert(a2 - a3 = -a1)
assert( a1 - a2 - a3   =  -a4 )
assert((a1 - a2) - a3  =  -a4 )
assert( a1 - -a2      = a3 )
assert(-(a1 - a2) - a3 =  -a2 )
assert(a1 - a2 - a3 = -a3 - a2 + a1)
assert(a1 - a2 - a3 - a4 - a5 = -13)
assert(-(a1) - (a2) - (a3) - (a4) - (a5) = -(15))
assert(-a1 - -a2 - -a3 - -a4 - -a5 = 13)
assert((-a1) - (-a2) - (-a3) - (-a4) - (-a5) = (-a1 + a2 + a3 + a4 + a5))
assert(-(-a1) - -(-a2) - -(-a3) - -(-a4) - -(-a5) = -(--13))

assert(a1
       -
      -a2
       -
       a3
       -
      -a4
       -
       a5
       =
      -a1)

assert((a1 - (a2 - (a3 - (a4 - (a5))))) = a3)
assert((((((a1) - a2) - a3) - a4) - a5) = -13)

assert(
        a1 - 
       -a2 - 
      --a3 -
     ---a4 -
    ----a5
        =
       (a1)
        -
      (-(a2))
        -
     (-(-(a3)))
        -
    (-(-(-(a4))))
        -
   (-(-(-(-(a5)))))
        )
        
# Primarily multiplication

assert(a2 * a3 = a6)
assert( a1*a2*a3   =  a6 )
assert((a1 * a2) * a3  =  a6 )
assert( a1 * -a2      = -a2 )
assert(-(a1 * a2) * a3 =  -a6 )
assert(a1 * a2 * a3 * a4 = a4 * a3 * a2 * a1)
assert(a1 * a2 * a3 * a4 * a5 = ----a120)
assert((a1) * (a2) * (a3) * (a4) * (a5) = (a120))
assert(-a1 * -a2 * -a3 * -a4 * -a5 = -a120)
assert((a1) * (a2) * (a3) * (a4) * (a5) = -(-a1 * -a2 * -a3 * -a4 * -a5))
assert(-(-a1) * -(-a2) * -(-a3) * -(-a4) * -(-a5) = -(-a120))

assert(a1
       *
      -a2
       *
       a3
       *
      -a4
       *
       a5
       =
       a120)

assert((a1 * (a2 * (a3 * (a4 * (a5))))) = a120)
assert((((((a1) * a2) * a3) * a4) * a5) = a120)
assert((a1 * (a2 * (a3 * (a4 * (a5))))) = (((((a1) * a2) * a3) * a4) * a5))

assert(
        a1 *
       -a2 * 
      --a3 *
     ---a4 *
    ----a5
        =
       (a1)
        *
      (-(a2))
        *
     (-(-(a3)))
        *
    (-(-(-(a4))))
        *
   (-(-(-(-(a5)))))
        )