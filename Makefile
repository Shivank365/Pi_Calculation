all:
	gcc src/main.c lib/function.c -Iinclude -lpthread -lm -o obb/swl2

run:
	obb/swl2 $(thread_count)	$(n)

plot:
	gnuplot -p plot.p

clean:
	rm -rf o obb/swl2