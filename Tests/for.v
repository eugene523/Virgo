# Testing for/while loops

#-----------------------------------------------------------------------------#
# for as while                                                                #
#-----------------------------------------------------------------------------#

x = 0
for x < 10
  x += 1
assert(x = 10)

s = 0
for (i = 0; i <= 10; i += 1)
  if i % 2 = 0
    s += i
assert(s = 30)

#------------------------------------------------------------------------------

s = 0
i = 0
for i <= 10
  if i % 2 = 0
    s += i
  i += 1
assert(s = 30)

#------------------------------------------------------------------------------

sum_even = 0
sum_odd = 0
i = 0
for i <= 10
  if i % 2 = 0
    sum_even += i
  else
    sum_odd += i
  i += 1

assert(sum_even = 30)
assert(sum_odd = 25)

#------------------------------------------------------------------------------

i = 0
s = 0
for i <= 10
  i += 1
  if i < 5
    skip
  s += i
assert(s = 56)

#------------------------------------------------------------------------------

i = 0
for i < 10
  if i > 5
    break
  i += 1
assert(i = 6)

#------------------------------------------------------------------------------

i = 0
for i < 10
  if i > 5
    break
  i += 1
assert(i = 6)

#------------------------------------------------------------------------------

i = 0
for i < 10
  j = 0
  breakOutLoop = false
  for j < 10
    if j = 3
      breakOutLoop = true
      break
    j += 1
  if breakOutLoop
    break
  assert(j = 3)
  i += 1

#-----------------------------------------------------------------------------#
# for (c styled)                                                              #
#-----------------------------------------------------------------------------#

s = 0
for (i = 0; i <= 10; i += 1)
  if i % 2 = 0
    s += i
assert(s = 30)

#------------------------------------------------------------------------------

sum_even = 0
sum_odd = 0

for (i = 0; i <= 10; i += 1)
  if i % 2 = 0
    sum_even += i
  else
    sum_odd += i

assert(sum_even = 30)
assert(sum_odd = 25)

#------------------------------------------------------------------------------

s = 0
for (i = 0; i <= 10; i += 1)
  if i < 5
    skip
  s += i
assert(s = 45)

#------------------------------------------------------------------------------

for (i = 0; i < 10; i += 1)
  if i > 5
    break
assert(i = 6)

#------------------------------------------------------------------------------

for (i = 0; i < 10; i += 1)
  if i > 5
    break
assert(i = 6)

#------------------------------------------------------------------------------

for (i = 0; i < 10; i += 1)
  for (j = 0; j < 10; j += 1)
    if j = 3
      break
  assert(j = 3)