/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "BoomstraDam.v2.5.h"
#include "malloc.h"
#include <string.h>
#include <pthread.h>

/*	Zobrist hash, for every position on the board for every piece a dedicated key.
	The hash value for the board is calculated by xor-ing all the actual board fields.
	There is a trick to calculate incremental: xor the old position to remove it and
	then xor the new position (2 x xor == nill operation)
*/

long hv[275] = {
826099872 & MASK_HASH,
52486073 & MASK_HASH,
1562489854 & MASK_HASH,
542662890 & MASK_HASH,
3563080865 & MASK_HASH,
415781742 & MASK_HASH,
706986114 & MASK_HASH,
2958038583 & MASK_HASH,
1118311283 & MASK_HASH,
254587328 & MASK_HASH,
0,
1691678043 & MASK_HASH,
2296120532 & MASK_HASH,
768674293 & MASK_HASH,
1729022327 & MASK_HASH,
2169991683 & MASK_HASH,
627363878 & MASK_HASH,
1204486609 & MASK_HASH,
2917902999 & MASK_HASH,
1402368440 & MASK_HASH,
1144264239 & MASK_HASH,
0,
4101685477 & MASK_HASH,
679148885 & MASK_HASH,
3553000852 & MASK_HASH,
3420357793 & MASK_HASH,
1047587215 & MASK_HASH,
3021434154 & MASK_HASH,
1941255771 & MASK_HASH,
276060297 & MASK_HASH,
814654740 & MASK_HASH,
1172570556 & MASK_HASH,
0,
28769688 & MASK_HASH,
1459143336 & MASK_HASH,
609468295 & MASK_HASH,
38476960 & MASK_HASH,
2691909634 & MASK_HASH,
1543763235 & MASK_HASH,
3839804308 & MASK_HASH,
3492704477 & MASK_HASH,
3027886352 & MASK_HASH,
4033736107 & MASK_HASH,
0,
1522863319 & MASK_HASH,
554281998 & MASK_HASH,
2813131750 & MASK_HASH,
2016469404 & MASK_HASH,
2888367760 & MASK_HASH,
4228091905 & MASK_HASH,
4200956281 & MASK_HASH,
758211582 & MASK_HASH,
2241392805 & MASK_HASH,
1863609700 & MASK_HASH,
0,
168569209 & MASK_HASH,
4190782605 & MASK_HASH,
3262707638 & MASK_HASH,
1517057473 & MASK_HASH,
270222079 & MASK_HASH,
2215252842 & MASK_HASH,
2523468590 & MASK_HASH,
3934986301 & MASK_HASH,
3478664766 & MASK_HASH,
1188740278 & MASK_HASH,
0,
2200335080 & MASK_HASH,
3506452584 & MASK_HASH,
1098425709 & MASK_HASH,
2564758935 & MASK_HASH,
2818281433 & MASK_HASH,
716955809 & MASK_HASH,
563122638 & MASK_HASH,
3718575827 & MASK_HASH,
933235486 & MASK_HASH,
1418146271 & MASK_HASH,
0,
1309887773 & MASK_HASH,
416746922 & MASK_HASH,
1873598861 & MASK_HASH,
1482349316 & MASK_HASH,
3301539221 & MASK_HASH,
259168456 & MASK_HASH,
1279041235 & MASK_HASH,
2740231307 & MASK_HASH,
2038582914 & MASK_HASH,
1525556823 & MASK_HASH,
0,
2965154458 & MASK_HASH,
837451363 & MASK_HASH,
566286485 & MASK_HASH,
178474311 & MASK_HASH,
1475181955 & MASK_HASH,
4020187966 & MASK_HASH,
1746471693 & MASK_HASH,
3381482360 & MASK_HASH,
4101625602 & MASK_HASH,
1791117621 & MASK_HASH,
0,
3209185678 & MASK_HASH,
1860949868 & MASK_HASH,
33176629 & MASK_HASH,
3705071422 & MASK_HASH,
2459889480 & MASK_HASH,
2980143214 & MASK_HASH,
1320965834 & MASK_HASH,
3120185811 & MASK_HASH,
1349191065 & MASK_HASH,
3461332275 & MASK_HASH,
0,
1013667601 & MASK_HASH,
2919802030 & MASK_HASH,
2055326975 & MASK_HASH,
2491887813 & MASK_HASH,
2080358 & MASK_HASH,
798019178 & MASK_HASH,
3007191372 & MASK_HASH,
3772396430 & MASK_HASH,
1691461575 & MASK_HASH,
3759358171 & MASK_HASH,
0,
4034908216 & MASK_HASH,
3589574287 & MASK_HASH,
3241951687 & MASK_HASH,
3926924835 & MASK_HASH,
3128869885 & MASK_HASH,
4218714342 & MASK_HASH,
1832712928 & MASK_HASH,
2960189707 & MASK_HASH,
2657041067 & MASK_HASH,
2351543177 & MASK_HASH,
0,
1641413012 & MASK_HASH,
4189754081 & MASK_HASH,
193064009 & MASK_HASH,
1650502218 & MASK_HASH,
1058794680 & MASK_HASH,
4035341372 & MASK_HASH,
2857506028 & MASK_HASH,
3836089978 & MASK_HASH,
2975876463 & MASK_HASH,
3861979047 & MASK_HASH,
0,
4007883898 & MASK_HASH,
2192930749 & MASK_HASH,
3325051885 & MASK_HASH,
3110561124 & MASK_HASH,
2958444608 & MASK_HASH,
769009893 & MASK_HASH,
3183324031 & MASK_HASH,
2628708864 & MASK_HASH,
1015389261 & MASK_HASH,
1886140138 & MASK_HASH,
0,
1121822902 & MASK_HASH,
4198994836 & MASK_HASH,
872391482 & MASK_HASH,
2670945092 & MASK_HASH,
2791415426 & MASK_HASH,
151531934 & MASK_HASH,
1997262106 & MASK_HASH,
3733346630 & MASK_HASH,
4262910252 & MASK_HASH,
1699060740 & MASK_HASH,
0,
1636060924 & MASK_HASH,
2343356398 & MASK_HASH,
279816214 & MASK_HASH,
921916098 & MASK_HASH,
3320701557 & MASK_HASH,
2201904896 & MASK_HASH,
3977146433 & MASK_HASH,
3565134546 & MASK_HASH,
3230658851 & MASK_HASH,
1391588464 & MASK_HASH,
0,
4037017029 & MASK_HASH,
1042296955 & MASK_HASH,
4076711953 & MASK_HASH,
2481553770 & MASK_HASH,
2661081773 & MASK_HASH,
1950584405 & MASK_HASH,
4015610237 & MASK_HASH,
3629228890 & MASK_HASH,
3498559907 & MASK_HASH,
1190580664 & MASK_HASH,
0,
3243844975 & MASK_HASH,
1132456463 & MASK_HASH,
347373776 & MASK_HASH,
3893472540 & MASK_HASH,
3637887151 & MASK_HASH,
1280944591 & MASK_HASH,
681853759 & MASK_HASH,
2844586138 & MASK_HASH,
2657027656 & MASK_HASH,
3157586659 & MASK_HASH,
0,
600246363 & MASK_HASH,
1385207850 & MASK_HASH,
881037863 & MASK_HASH,
1105746220 & MASK_HASH,
254799338 & MASK_HASH,
531823856 & MASK_HASH,
3603865632 & MASK_HASH,
2293874058 & MASK_HASH,
328913454 & MASK_HASH,
799238565 & MASK_HASH,
0,
3516083022 & MASK_HASH,
217352884 & MASK_HASH,
2193379872 & MASK_HASH,
2529941619 & MASK_HASH,
1370992831 & MASK_HASH,
2595022970 & MASK_HASH,
3128340282 & MASK_HASH,
2580637388 & MASK_HASH,
2308905052 & MASK_HASH,
2825464122 & MASK_HASH,
0,
296341154 & MASK_HASH,
773177774 & MASK_HASH,
2638582550 & MASK_HASH,
176092165 & MASK_HASH,
775448834 & MASK_HASH,
1793024526 & MASK_HASH,
3543364573 & MASK_HASH,
1202432003 & MASK_HASH,
2410309978 & MASK_HASH,
505571314 & MASK_HASH,
0,
105962244 & MASK_HASH,
1454264414 & MASK_HASH,
579085442 & MASK_HASH,
2158547312 & MASK_HASH,
1311445631 & MASK_HASH,
710675561 & MASK_HASH,
3569200000 & MASK_HASH,
3768395037 & MASK_HASH,
2126232208 & MASK_HASH,
4261936267 & MASK_HASH,
0,
4270247729 & MASK_HASH,
1717674460 & MASK_HASH,
71614937 & MASK_HASH,
150037740 & MASK_HASH,
310653276 & MASK_HASH,
1580293177 & MASK_HASH,
1056534390 & MASK_HASH,
689393358 & MASK_HASH,
389037889 & MASK_HASH,
863962045 & MASK_HASH,
0,
1093569759 & MASK_HASH,
2968883860 & MASK_HASH,
2721960442 & MASK_HASH,
2124391984 & MASK_HASH,
1546247556 & MASK_HASH,
2456531701 & MASK_HASH,
915089648 & MASK_HASH,
3560725392 & MASK_HASH,
901284894 & MASK_HASH,
2812162839 & MASK_HASH,
0,
2740721228 & MASK_HASH,
2777224628 & MASK_HASH,
3209471049 & MASK_HASH,
1123777420 & MASK_HASH,
3961202151 & MASK_HASH,
3577303823 & MASK_HASH,
2011881682 & MASK_HASH,
2629355473 & MASK_HASH,
356957708 & MASK_HASH,
3711246927 & MASK_HASH,
0 };

long * white_man_hash = &hv[0];
long * black_man_hash = &hv[55];
long * white_king_hash = &hv[110];
long * black_king_hash = &hv[165];
long * empty_field_hash = &hv[220];

struct hash_node * hash = NULL;
struct hash_node * prev_hash = NULL;

extern void	Initialize_endgame(void);


void * partial_init_hash_depth(void * Ptr) {
	long node_nr;
	InitPnt init_rec = (InitPnt) Ptr;
	for (node_nr = init_rec->low ; node_nr < init_rec->high; node_nr++)	{hash[node_nr].search_depth = -1;}; // reset depth to -1 for each new move
}

void parallel_init_hash_depth(void){
   pthread_t thread_id[THREAD_MAX];
   struct init_boundaries init_rc[THREAD_MAX];
   int i, j;

   long part_size = HASH_ARRAY_SIZE/THREAD_MAX;

   for(i=0; i < THREAD_MAX-1; i++)
   {
	  init_rc[i].low = i*part_size;
	  init_rc[i].high = (i+1) * part_size;
	  //printf("init hash: %d : %d\n", init_rc[i].low, init_rc[i].high);
	  pthread_create( &thread_id[i], NULL, partial_init_hash_depth, (void *) &init_rc[i] );
   }

   // and the latest one for the last part
   init_rc[THREAD_MAX-1].low = (THREAD_MAX-1)*part_size;
   init_rc[THREAD_MAX-1].high = HASH_ARRAY_SIZE;
   //printf("init hash: %d : %d\n", init_rc[THREAD_MAX-1].low, init_rc[THREAD_MAX-1].high);
   pthread_create( &thread_id[i], NULL, partial_init_hash_depth, (void *) &init_rc[THREAD_MAX-1] );

   for(j=0; j < THREAD_MAX; j++)
   {
	  pthread_join( thread_id[j], NULL);
   }

   //printf("init_hash_parallel done\n");

}

void * partial_init_hash_zero(void * Ptr) {
	long node_nr;
	InitPnt init_rec = (InitPnt) Ptr;
	for (node_nr = init_rec->low ; node_nr < init_rec->high; node_nr++)	{hash[node_nr].occ_empty = 0;}; // clear entry
}

void parallel_init_hash_zero(void){
   pthread_t thread_id[THREAD_MAX];
   struct init_boundaries init_rc[THREAD_MAX];
   int i, j;

   long part_size = HASH_ARRAY_SIZE/THREAD_MAX;

   for(i=0; i < THREAD_MAX-1; i++)
   {
	  init_rc[i].low = i*part_size;
	  init_rc[i].high = (i+1) * part_size;
	  //printf("init hash: %d : %d\n", init_rc[i].low, init_rc[i].high);
	  pthread_create( &thread_id[i], NULL, partial_init_hash_zero, (void *) &init_rc[i] );
   }

   // and the latest one for the last part
   init_rc[THREAD_MAX-1].low = (THREAD_MAX-1)*part_size;
   init_rc[THREAD_MAX-1].high = HASH_ARRAY_SIZE;
   //printf("init hash: %d : %d\n", init_rc[THREAD_MAX-1].low, init_rc[THREAD_MAX-1].high);
   pthread_create( &thread_id[i], NULL, partial_init_hash_zero, (void *) &init_rc[THREAD_MAX-1] );

   for(j=0; j < THREAD_MAX; j++)
   {
	  pthread_join( thread_id[j], NULL);
   }

   //printf("init_hash_parallel done\n");

}

void init_hash()
{	long  node;
	if (hash == NULL)
	{	hash = (struct hash_node *)malloc(HASH_ARRAY_SIZE*sizeof(struct hash_node));
		parallel_init_hash_zero();
		//for (node = 0 ; node < HASH_ARRAY_SIZE; node++)	{hash[node].occ_empty = 0; }; // preset with empty boards
		//printf("hash array claimed\n");
		// init opening books
		fcloseall(); // make sure all previous opened (log) files are closed
		//Initialize_WhiteBook();
		//Initialize_BlackBook();
		Initialize_endgame();
		//init_self_play_book();
		//Initialize_frequency_tables();
		init_hash_locks();
	}
	else {
		// hash allready claimed, but we make (for debug purposes) a copy of the hash
		//prev_hash = (struct hash_node *)malloc(HASH_ARRAY_SIZE*sizeof(struct hash_node));
		//memcpy((void *)prev_hash, (void *)hash, HASH_ARRAY_SIZE*sizeof(struct hash_node));
	}
	if (hash == NULL)
	{	printf("No space for hash\n");
		return;
	}
	//for (node = 0 ; node < HASH_ARRAY_SIZE; node++)	{hash[node].search_depth = -1;}; // reset depth to -1 for each new move
	//parallel_init_hash_depth();
	Idle = false;
	wake_sleeping_threads();
	return;
}
long calc_initial_hash_key(NodePnt node)
{	register long temp;
	register BitArray cur_bit;
	int i,j;

	if (node->status & IS_WHITE_ON_MOVE) temp = WHITE_HASH;
	else temp = 0;

	for (j=0; j<50; j++)
	{	i = ext_to_int[j+1];
		cur_bit = bits[i];
		if (node->occ_empty & cur_bit)				/* true means occupied */
		{	if (node->man_king& cur_bit)			/* true means a man */
			{	if (node->white_black & cur_bit)	/* true means white */
					temp = temp ^ white_man_hash[i];
				else
					temp = temp ^ black_man_hash[i];
			}
			else
			{	if (node->white_black & cur_bit)	/* true means white */
					temp = temp ^ white_king_hash[i];
				else
					temp = temp ^ black_king_hash[i];
			}
		}
	}
	return temp;
}
