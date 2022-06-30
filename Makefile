all:
	gcc caverna.c caverna.h game_state.c game_state.h main-automatica.c t3-x-y-z.h  t3-x-y-x.c -o saida -O2

clean:
	rm -f saida;
