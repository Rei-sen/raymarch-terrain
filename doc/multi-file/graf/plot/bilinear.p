set terminal png size 500, 500
set output 'bilinear.png'

a = 0.5
b = 0.25
c = 0.75
d = 0.5

t(p) = p

# f(x, y) = a + (b - a) * x + (c - a) * y + ( a - b - c + d) * x * y;

f(x, y) = a + (b - a) * t(x) + (c - a) * t(y) + (a - b - c + d) * t(x) * t(y);

set xrange [0:1]
set yrange [0:1]

splot f(x, y) title 'interpolacja biliniowa'
# plot t(x)
