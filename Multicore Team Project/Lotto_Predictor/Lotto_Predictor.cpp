#include <stdio.h>
#include <time.h>
#include "DS_timer.h"

#define BUYER 10
#define LOTTO_NUM 10

typedef struct {
	int number[6];	// 로또 번호 6자리
	int rank;		// 당첨 등수
}Lotto;

typedef struct {
	Lotto lotto[LOTTO_NUM];
}Personal_buy;

void insertionSort(int* data, int n);
void calcRank(Lotto* data);

int win[7];

int main()
{
	Personal_buy buyer[BUYER];

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


	/*
	Lotto.number를 정렬
	*/
	for (int i = 0; i < BUYER; i++)
		for (int j = 0; j < LOTTO_NUM; j++)
			insertionSort(buyer[i].lotto[j].number, 6);


	// 구매자 번호 출력
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


	// 당첨번호 추첨
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


	// 번호 출력
	printf("\n\t<당첨번호>\n\t");
	for (int i = 0; i < 6; i++)
		printf("%d ", win[i]);
	printf("\n\t보너스 : %d\n\n", win[6]);


	/*
	등수 확인
	*/
	for (int i = 0; i < BUYER; i++)
		for (int j = 0; j < LOTTO_NUM; j++)
			calcRank(&(buyer[i].lotto[j]));


	// 등수 출력
	for (int i = 0; i < BUYER; i++)
	{
		printf("[%d 고객]\n", i);

		for (int j = 0; j < LOTTO_NUM; j++)
		{
			printf("\t[%d]", j);
			for (int k = 0; k < 6; k++)
				printf("%d ", buyer[i].lotto[j].number[k]);

			if (buyer[i].lotto[j].rank)
				printf(" <<%d등>>", buyer[i].lotto[j].rank);
			printf("\n");
		}
		printf("\n");
	}

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
Lotto.rank에 등수를 저장함
*/
void calcRank(Lotto* data)
{
	int cnt = 0;	// 일치한 번호 개수
	bool bonus = false;	// 보너스 번호 일치 여부

	for (int i = 0, j = 0; i < 6 && j < 6; )
	{
		if (win[i] == data->number[j])
		{
			cnt++;
			i++;
			j++;
		}
		else if (win[i] < data->number[j])
			i++;
		else
			j++;
	}

	for (int i = 0; i < 6; i++)
		if (win[6] == data->number[i])
		{
			bonus = true;
			break;
		}

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