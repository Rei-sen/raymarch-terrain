set terminal png size 500, 500
set output 'lerp.png'

lerp(x) = x * x * (3 - 2 * x)

set xrange [0:1]

plot lerp(x) title 'linear interpolation'
