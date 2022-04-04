# Bubble sorting algorithm
BubbleSort(a)
  size = len(a)
  for (i = 0; i < (size - 1); i += 1)
    for (j = 0; j < (size - 1 - i); j += 1)
      if a(j) > a(j + 1)
        exch = a(j + 1)
        a(j + 1) = a(j)
        a(j) = exch


RandomList(n)
  a = []
  for (i = 0; i < n; i += 1)
    a.Add(random())
  return a


IsAscending(items)
  size = len(items)
  for (i = 0; i < size - 1; i += 1)
    if (items(i) > items(i + 1))
      return false
  return true


PrintList(items)
  for x in items
    print(x)

a = RandomList(1000)
BubbleSort(a)
print(IsAscending(a))