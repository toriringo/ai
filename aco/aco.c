/****************************************/
/*  蟻コロニー最適化法(aco)プログラム   */
/*  acoにより最適値を学習します         */
/*  使い方                              */
/*  \Users\odaka\dl\ch3>aco             */
/****************************************/

/* Visual Studioとの互換性確保 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define NO_A    10    /* 蟻の個体数 */
#define ILIMIT  50    /* 繰り返しの回数 */
#define Q       3     /* フェロモン更新の定数 */
#define RHO     0.8   /* 蒸発の定数 */
#define STEP    10    /* 道のりのステップ数 */
#define EPSILON 0.15  /* 行動選択のランダム性を決定 */
#define SEED    32768 /* 乱数のシード */

void walk(int cost[2][STEP] ,double pheromone[2][STEP] ,int mstep[NO_A][STEP]);    /* 蟻を歩かせる */
void update(int cost[2][STEP] ,double pheromone[2][STEP] ,int mstep[NO_A][STEP]);  /* フェロモンの更新 */
void printmstep(int mstep[NO_A][STEP]);  /* 蟻の経路の表示 */
void printp(double pheromone[2][STEP]);  /* フェロモンの表示 */
double rand1();                          /* 0〜1の実数を返す乱数関数 */
int rand01();                            /* 0又は1を返す乱数関数 */


int main()
{
	int cost[2][STEP] = {               /* 2方向の分岐がSTEP回連続する */
				{1,1,1,1,1,1,1,1,1,1},  /* ステップ毎に距離1と距離5に分岐する */
				{5,5,5,5,5,5,5,5,5,5}};	/* すべてのステップで距離1に分岐すれば最短経路 */
	double pheromone[2][STEP] = {0};    /* 各ステップのフェロモン量 */
	int mstep[NO_A][STEP];              /* 蟻が歩いた経路（cost[0][]を通れば0、cost[1][]を通れば1） */
	int i;  /* 繰り返し回数の制御 */

	/* 乱数の初期化 */
	srand(SEED);

	/* 最適化の本体 */
	for(i = 0; i < ILIMIT; ++i){
		/* フェロモンの状態を表示 */
		printf("%d:\n" ,i);
		printp(pheromone);
		/* 蟻を歩かせる */
		walk(cost, pheromone, mstep);
		/* フェロモンの更新 */
		update(cost, pheromone, mstep);
	}
	/* フェロモンの状態を表示 */
	printf("%d:\n", i);
	printp(pheromone);

	return 0;
}

/**********************/
/*  フェロモンの更新  */
/**********************/
void update(int cost[2][STEP], double pheromone[2][STEP], int mstep[NO_A][STEP] )
{
	int m;              /* 蟻の個体番号 */
	int lm;             /* 蟻の歩いた距離 */
	double sum_lm = 0;  /* 蟻の歩いた合計距離 */
	int i, j;

	/* フェロモンの蒸発 */
	for(i = 0; i < 2; ++i){
		for(j = 0; j < STEP; ++j){
 			pheromone[i][j] *= RHO;
		}
	}

	/* 蟻による上塗り */
	for(m = 0; m < NO_A; ++m){
		/* 個体mの移動距離lmの計算 */
		lm = 0;
		for(i = 0; i < STEP; ++i){
			lm += cost[mstep[m][i]][i];
		}

		/* フェロモンの上塗り */
		for(i = 0; i < STEP; ++i){
			pheromone[mstep[m][i]][i] += Q * (1.0 / lm);
		}
		sum_lm += lm;
	}
	/* 蟻の歩いた平均距離を表示 */
	printf("%lf\n", sum_lm / NO_A);
}

/******************/
/*  蟻を歩かせる  */
/******************/
void walk(int cost[2][STEP], double pheromone[2][STEP], int mstep[NO_A][STEP])
{
	int m;  /* 蟻の個体番号 */
	int s;  /* 蟻の現在ステップ位置 */

	for(m = 0; m < NO_A; ++m){
		for(s = 0; s < STEP; ++s){
			/* ε-greedy法による行動選択 */
			if((rand1() < EPSILON)
				|| (abs(int)(pheromone[0][s] - pheromone[1][s]) < 1e-9))
			{  /* ランダムに行動  */
				mstep[m][s] = rand01();
			}
			else{  /* フェロモン濃度により選択 */
				if(pheromone[0][s] > pheromone[1][s]){
					mstep[m][s] = 0;
				}
				else{
					mstep[m][s] = 1;
				}
			}
		}
	}
	/* 蟻の経路の表示 */
	printmstep(mstep);
}

/**********************/
/*  蟻の経路の表示    */
/**********************/
void printmstep(int mstep[NO_A][STEP])
{
	int i,j ;

	printf("*mstep\n") ;

	for(i = 0; i < NO_A; ++i){
		for(j = 0; j < STEP; ++j){
			printf("%d ", mstep[i][j]);
		}
		printf("\n");
	}
}


/**********************/
/*  フェロモンの表示  */
/**********************/
void printp(double pheromone[2][STEP])
{
	int i, j;

	for(i = 0; i < 2; ++i){
		for(j = 0; j < STEP; ++j){
			printf("%4.2lf ", pheromone[i][j]);
		}
		printf("\n");
	}
}


/******************************/
/*  0〜1の実数を返す乱数関数  */
/******************************/
double rand1()
{
	/* 乱数の計算 */
	return (double)rand() / RAND_MAX;
}

/**************************/
/*  0又は1を返す乱数関数  */
/**************************/
int rand01()
{
	int rnd;

	/* 乱数の最大値を除く */
	while((rnd = rand()) == RAND_MAX);
	/* 乱数の計算 */
	return (int)((double)rnd / RAND_MAX * 2);
}

