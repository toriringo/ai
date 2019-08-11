/*********************************************/
/* 単純な帰納的学習プログラム                */
/*                                           */
/* パターン学習器                            */
/*   100個の学習データを読み込んで、         */
/*   適合する10桁の2進数パターンを答えます   */
/*                                           */
/* 使い方                                    */
/* :\Users\odaka\dl\ch2>learnstock<ldata.txt */
/*********************************************/

/* Visual Studioとの互換性確保 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define OK      1 
#define NG      0
#define SETSIZE 100   /* 学習データ数 */
#define CNO     10    /* 社数 */
#define GENMAX  10000 /* 解候補数 */
#define SEED    32767 /* 乱数のシード */


/* 学習データセットの読み込み */
void readdata(int data[SETSIZE][CNO], int teacher[SETSIZE]);
/* 0，1又は2を返す乱数関数 */
int rand012();  
/* 解候補パターンのスコア（0〜SETSIZE点）の計算 */
int calcscore(int data[SETSIZE][CNO], int teacher[SETSIZE], int answer[CNO]);


/****************/
/*  main()関数  */
/****************/
int main()
{
	int i, j;
	int score = 0;          /* スコア（0〜SETSIZE点） */
	int answer[CNO];        /* 解候補 */
	int data[SETSIZE][CNO]; /* 学習データセット */
	int teacher[SETSIZE];   /* 教師データ */
	int bestscore = 0;      /* スコアの最良値 */
	int bestanswer[CNO];    /* 探索途中での最良解 */
	
	srand(SEED); /* 乱数の初期化 */
	
	/* 学習データセットの読み込み */
	readdata(data, teacher);
	
	/* 解候補生成と検査 */
	for (i = 0; i < GENMAX; ++i){
		/* 解候補生成 */
		for (j = 0; j < CNO; ++j){
			answer[j] = rand012();
		}
		
		/* 検査 */
		score = calcscore(data, teacher, answer);
		
		/* 最良スコアの更新 */
		if (score > bestscore) {  /* これまでの最良値なら更新 */
			for (j = 0; j < CNO; ++j){
				bestanswer[j] = answer[j];
			}
			bestscore = score;
			for (j = 0; j < CNO; ++j){
				printf("%1d ", bestanswer[j]);
			}
			printf(":score=%d\n", bestscore);
		}
	}

	/* 最良解の出力 */
	printf("\n最良解\n");
	for (j = 0; j < CNO; ++j){
		printf("%1d ", bestanswer[j]);
	}
	printf(":score=%d\n", bestscore);

	return 0;
}

/**********************************************/
/*解候補パターンのスコア（0〜SETSIZE点）の計算*/
/**********************************************/
int calcscore(int data[SETSIZE][CNO],
              int teacher[SETSIZE],
              int answer[CNO])
{
	int score = 0; /* スコア（0〜SETSIZE点） */
	int point;     /* 一致した桁数(0〜CNO)   */
	int i, j;
 
	for (i = 0; i < SETSIZE; ++i) {
		/* 一致度計算 */
		point = 0;
		for (j = 0; j < CNO; ++j) {
			if (answer[j] == 2) ++point;  /* ワイルドカード */
			else if (answer[j] == data[i][j]) ++point;  /* 一致 */
		}
		
		if ((point == CNO) && (teacher[i] == 1)) {
			++score;
		}
		else if ((point != CNO) && (teacher[i] == 0)) {
			++score;
		}
	}
	return score ;
}


/******************************/
/* 学習データセットの読み込み */
/******************************/
void readdata(int data[SETSIZE][CNO], int teacher[SETSIZE])
{
	int i,j;

	for (i = 0; i < SETSIZE; ++i) {
		for (j = 0; j < CNO; ++j) {
			scanf("%d", &data[i][j]);
		}
		scanf("%d", &teacher[i]);
	}
}


/***************************/
/* 0，1又は2を返す乱数関数 */
/***************************/
int rand012()
{
	int rnd;
 
	/* 乱数の最大値を除く */
	while((rnd = rand()) == RAND_MAX);
	/* 乱数の計算 */ 
	return (double)rnd / RAND_MAX * 3;
}

