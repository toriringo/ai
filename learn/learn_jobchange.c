// 帰納的学習プログラム
// 学習データと教師データを読み込んで、教師データが1になる、最適な学習データのパターンを出力する
// learn_jobchange < ldata.txt

// Visual Studioとの互換性確保
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define OK      1
#define NG      0
#define SETSIZE 15    // 学習データ数
#define CNO     3     // パラメータの数
#define GENMAX  10000 // 解候補数
#define SEED    32767 // 乱数のシード

void readdata(int data[SETSIZE][CNO], int teacher[SETSIZE]);
int rand012();
int calcscore(int data[SETSIZE][CNO], int teacher[SETSIZE], int answer[CNO]);


int main()
{
	int score = 0;          // スコア（0〜SETSIZE点）
	int answer[CNO];        // 解候補
	int data[SETSIZE][CNO]; // 学習データセット
	int teacher[SETSIZE];   // 教師データ
	int bestscore = 0;      // スコアの最良値
	int bestanswer[CNO];    // 探索途中での最良解

	srand(SEED); // 乱数の初期化

	// 学習データセットの読み込み
	readdata(data, teacher);

	// 解候補生成と検査
	for (int i = 0; i < GENMAX; ++i){
		// 解候補生成
		for (int j = 0; j < CNO; ++j){
			answer[j] = rand012();
		}

		// 検査
		score = calcscore(data, teacher, answer);

		// 最良スコアの更新
		if (score > bestscore){  // これまでの最良値なら更新
			for (int j = 0; j < CNO; ++j){
				bestanswer[j] = answer[j];
			}
			bestscore = score;
			for (int j = 0; j < CNO; ++j){
				printf("%1d ", bestanswer[j]);
			}
			printf(":score=%d\n", bestscore);
		}
	}

	// 最良解の出力
	printf("\n最良解\n");
	for (int j = 0; j < CNO; ++j){
		printf("%1d ", bestanswer[j]);
	}
	printf(":score=%d\n", bestscore);

	return 0;
}

// 解候補パターンのスコア（0〜SETSIZE点）の計算
int calcscore(int data[SETSIZE][CNO],
              int teacher[SETSIZE],
              int answer[CNO])
{
	int score = 0; // スコア（0〜SETSIZE点）
	int point;     // 一致した桁数(0〜CNO)

	for (int i = 0; i < SETSIZE; ++i) {
		// 一致度計算
		point = 0;
		for (int j = 0; j < CNO; ++j) {
			if (answer[j] == 2) ++point;  // ワイルドカード
			else if (answer[j] == data[i][j]) ++point;  // 一致
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


// 学習データセットの読み込み
void readdata(int data[SETSIZE][CNO], int teacher[SETSIZE])
{
	for (int i = 0; i < SETSIZE; ++i) {
		for (int j = 0; j < CNO; ++j) {
			scanf("%d", &data[i][j]);
            #ifdef _DEBUG
            printf("%d ", data[i][j]);
            #endif
		}
		scanf("%d", &teacher[i]);
        #ifdef _DEBUG
        printf("%d \n", teacher[i]);
        #endif
	}
    #ifdef _DEBUG
    printf("\n\n");
    #endif
}


// 0，1又は2を返す乱数関数
int rand012()
{
	int rnd;

	// 乱数の最大値を除く
	while((rnd = rand()) == RAND_MAX);
	// 乱数の計算
	return (double)rnd / RAND_MAX * 3;
}

