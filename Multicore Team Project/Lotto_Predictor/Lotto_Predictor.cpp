#include <stdio.h>
#include <fstream>
#include <time.h>
#include "DS_timer.h"

using namespace std;

#define BUYER 10
#define LOTTO_NUM 10
#define LOTTO_PLAY_NUM 800

typedef struct {
	int number[6];	// 로또 번호 6자리
	int rank;		// 당첨 등수
}Lotto;

typedef struct {
	Lotto lotto[LOTTO_NUM];
}Personal_buy;

void insertionSort(int* data, int n);
void calcRank(Lotto* data);

int win[LOTTO_PLAY_NUM][7];

int main()
{
	Personal_buy buyer[BUYER];

	ofstream out("lotto.txt");

	srand(time(NULL));


	// 구매자 번호 추첨
	for (int i = 0; i < BUYER; i++)
		for (int j = 0; j < LOTTO_NUM; j++)
			for (int k = 0; k < 6; k++)
			{
				// 1 ~ 45 사이의 임의의 번호 부여
				buyer[i].lotto[j].number[k] = rand() % 45 + 1;

				// 중복된 번호 제거
				for (int l = 0; l < k; l++)
					if (buyer[i].lotto[j].number[l] == buyer[i].lotto[j].number[k])
					{
						k--;
						continue;
					}
			}


	//Lotto.number를 정렬
	for (int i = 0; i < BUYER; i++)
		for (int j = 0; j < LOTTO_NUM; j++)
			insertionSort(buyer[i].lotto[j].number, 6);


	// 구매자 번호 출력
	for (int i = 0; i < BUYER; i++)
	{
		printf("[고객 %d]\n", i);

		for (int j = 0; j < LOTTO_NUM; j++)
		{
			printf("\t[%d] ", j);
			for (int k = 0; k < 6; k++)
				printf("%2d ", buyer[i].lotto[j].number[k]);

			printf("\n");
		}
		printf("\n");
	}


	// 당첨번호 추첨
	for (int i = 0; i < LOTTO_PLAY_NUM; i++)
		for (int j = 0; j < 7; j++)
		{
			win[i][j] = rand() % 45 + 1;

			for (int k = 0; k < j; k++)
				if (win[i][j] == win[i][k])
				{
					j--;
					break;
				}
		}


	// 당첨번호 정렬(보너스 번호 제외)
	for (int i = 0; i < LOTTO_PLAY_NUM; i++)
		insertionSort(win[i], 6);


	// 번호 출력
	for (int i = 0; i < LOTTO_PLAY_NUM; i++)
	{
		printf("\n\t<%4d회당첨번호>\n\t", i + 1);

		for (int j = 0; j < 6; j++)
			printf("%d ", win[i][j]);

		printf("\n\t보너스 : %d\n", win[i][6]);
	}


	//
	////등수 확인
	//for (int i = 0; i < BUYER; i++)
	//	for (int j = 0; j < LOTTO_NUM; j++)
	//		calcRank(&(buyer[i].lotto[j]));


	//// 등수 출력
	//for (int i = 0; i < BUYER; i++)
	//{
	//	printf("[고객 %d]\n", i);

	//	for (int j = 0; j < LOTTO_NUM; j++)
	//	{

	//		if (buyer[i].lotto[j].rank)
	//			printf("<<%d등>> ", buyer[i].lotto[j].rank);
	//		else
	//			printf("\t");
	//		printf("[%d] ", j);
	//		for (int k = 0; k < 6; k++)
	//			printf("%2d ", buyer[i].lotto[j].number[k]);

	//		printf("\n");
	//	}
	//	printf("\n");
	//}
	//
	return 0;
}

// 작은 크기의 배열에서 효과적인 삽입정렬 선택
/*
param : 정렬되지 않은 정수 배열, 크기
return : void
삽입정렬을 사용하여 오름차순으로 정렬
*/
void insertionSort(int* data, int n)
{
	int i, j, key;
	for (i = 1; i < n; i++)
	{
		key = data[(j = i)];

		while (--j >= 0 && key < data[j])
		{
			data[j + 1] = data[j];
			data[j] = key;
		}
	}
}


/*
param : 6자리 오름차순 로또 번호 배열
return : void
마지막 회차 당첨번호와 비교해 Lotto.rank에 등수를 저장함
*/
void calcRank(Lotto* data)
{
	int cnt = 0;	// 일치한 당첨번호 개수
	bool bonus = false;	// 보너스 일치 여부

	// 당첨번호 확인
	for (int i = 0, j = 0; i < 6 && j < 6; )
	{
		if (win[LOTTO_PLAY_NUM - 1][i] == data->number[j])
		{
			cnt++;
			i++;
			j++;
		}
		else if (win[LOTTO_PLAY_NUM - 1][i] < data->number[j])
			i++;
		else
			j++;
	}

	// 보너스 확인
	for (int i = 0; i < 6; i++)
		if (win[LOTTO_PLAY_NUM - 1][6] == data->number[i])
		{
			bonus = true;
			break;
		}

	// 일치한 당첨번호의 개수와 보너스 일치 여부를 통해 등수 지정
	switch (cnt)
	{
	case 6:
		data->rank = 1;
		break;
	case 5:
		if (bonus)
			data->rank = 2;
		else
			data->rank = 3;
		break;
	case 4:
		data->rank = 4;
		break;
	case 3:
		data->rank = 5;
		break;
	default:
		data->rank = 0;
		break;
	}
}