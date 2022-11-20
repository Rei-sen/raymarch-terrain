set terminal latex
set output './main-gnuplottex-fig1.tex'
  f(x, y) = sin(x) + cos(y)
  set xrange [0:10]
  set yrange [0:10]

  splot f(x, y)
