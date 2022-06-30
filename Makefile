all:
	gcc caverna.c caverna.h game_state.c game_state.h main-automatica.c t3-x-y-z.h  t3_v1.c -o saida1 -O2
	gcc caverna.c caverna.h game_state.c game_state.h main-automatica.c t3-x-y-z.h  t3_v2.c -o saida2 -O2

clean:
	rm -f saida;
