#include <stdio.h>
#include <time.h>
#include "DS_timer.h"

#define BUYER 10
#define LOTTO_NUM 10

typedef struct {
	int number[6];
	int rank;
}Lotto;

typedef struct {
	Lotto lotto[LOTTO_NUM];
}Personal_buy;

void insertionSort(int* data, int n);

int main()
{
	Personal_buy buyer[BUYER];
	int win[7];

	srand(time(NULL));

	// 구매자 번호
	for (int i = 0; i < BUYER; i++)
	{
		for (int j = 0; j < LOTTO_NUM; j++)
			for (int k = 0; k < 6; k++)
			{
				buyer[i].lotto[j].number[k] = rand() % 45 + 1;
				for (int l = 0; l < k; l++)
				{
					if (buyer[i].lotto[j].number[l] == buyer[i].lotto[j].number[k])
					{
						k--;
						continue;
					}
				}
			}
	}

	/*
	Lotto.number를 정렬
	*/

	for (int i = 0; i < BUYER; i++)
		for (int j = 0; j < LOTTO_NUM; j++)
			insertionSort(buyer[i].lotto[j].number, 6);

	for (int i = 0; i < BUYER; i++)
	{
		printf("[%d 고객]\n", i);
		for (int j = 0; j < LOTTO_NUM; j++)
		{
			printf("\t[%d]", j);
			for (int k = 0; k < 6; k++)
				printf("%d ", buyer[i].lotto[j].number[k]);
			printf("\n");
		}
		printf("\n");
	}


	// 당첨자 번호
	for (int i = 0; i < 7; i++)
	{
		win[i] = rand() % 45 + 1;

		for (int j = 0; j < i; j++)
			if (win[j] == win[i])
			{
				i--;
				break;
			}
	}

	/*
	당첨번호 벙렬(보너스 번호 제외)
	*/
	insertionSort(win, 6);

	printf("\n\t<당첨자 번호>\n\t");
	for (int i = 0; i < 6; i++)
		printf("%d ", win[i]);
	printf("\n\t보너스번호 : %d\n", win[6]);



	/*
	당첨번호 확인
	*/



	return 0;
}

// 작은 크기의 배열에서 효과적인 삽입정렬 선택
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

void findWinner(Lotto data)
{

}