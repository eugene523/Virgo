MatMult(mat, x)
  y = []
  y.Add(mat(0)(0) * x(0) + mat(0)(1) * x(1))
  y.Add(mat(1)(0) * x(0) + mat(1)(1) * x(1))
  return y

PrintVect(x)
  print("[", x(0), " ", x(1), "]")

mat = [[0.8, 0.3],
       [0.2, 0.7]]

x = [0, 1]

for i in seg(0, 100)
  PrintVect(x)
  x = MatMult(mat, x)

