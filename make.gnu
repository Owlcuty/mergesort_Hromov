set logscale x
set term png
set output "plot.png"
plot "test.dat" using 1:2 with lines title "Фактическое число случаев", \
	 "test.dat" using 1:3 with lines title "Теоретическое число случаев"
