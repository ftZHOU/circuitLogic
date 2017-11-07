set term jpeg
set output 'chrono.jpeg'
unset ytics
set title 'chronogram'
set yrange [0:8]
plot 'example.data' using 1:2 with lines title 'a', \
     'example.data' using 1:3 with lines title 'b', \
     'example.data' using 1:4 with lines title 'c', \
     'example.data' using 1:5 with lines title 'd' \
     
