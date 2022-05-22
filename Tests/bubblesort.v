# Bubble sorting algorithm
BubbleSort(a)
  size = len(a)
  for i in seg(0, size - 1)
    for j in seg(0, size - 1 - i)
      if a(j) > a(j + 1)
        exch = a(j + 1)
        a(j + 1) = a(j)
        a(j) = exch

RandomList(n)
  a = []
  for i in seg(0, n)
    a.Add(random())
  return a

IsAscending(items)
  size = len(items)
  for i in seg(0, size - 1)
    if (items(i) > items(i + 1))
      return false
  return true

PrintList(items)
  for x in items
    print(x)

a = RandomList(100)
BubbleSort(a)
print(IsAscending(a))