# Testing if-else expressions

#------------------------------------------------

a = 3
if a = 3
  b = 5
assert(b = 5)

#------------------------------------------------

if b = 3
  a = 3
if b = 5
  a = 5
assert(a = 5)

#------------------------------------------------

a = 3
if a > 2
  b = 3
else
  b = 4
assert(b = 3)

if a < 1
  b = 3
else
  b = 4
assert(b = 4)

#------------------------------------------------

a = 5.0
b = 3.0
if (a < 6)
  if (b < 5)
    q = 10
assert(q = 10)

#------------------------------------------------

a = 10
if a < 100
  if a < 90
    if a < 80
      if a < 70
        if a < 60
          if a < 50
            if a < 40
              if a < 30
                if a < 20
                  if a = 10
                    b = true
                  a9 = 9
                a8 = 8
              a7 = 7
            a6 = 6
          a5 = 5
        a4 = 4
      a3 = 3
    a2 = 2
  a1 = 1

assert(b      and
       a1 = 1 and
       a2 = 2 and
       a3 = 3 and
       a4 = 4 and
       a5 = 5 and
       a6 = 6 and
       a7 = 7 and
       a8 = 8 and
       a9 = 9 )

#------------------------------------------------

a = 10
if a < -20
  c = 0
else a < -10
  c = 1
else a < 0
  c = 2
else a < 9
  c = 3
else a < 10
  c = 4
else
  c = 5
assert(c = 5)

#------------------------------------------------