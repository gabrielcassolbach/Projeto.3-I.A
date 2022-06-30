all:
	gcc caverna.c caverna.h game_state.c game_state.h main-automatica.c t3_v1.h  t3_v1.c -o saida -O2

clean:
	rm -f saida;
