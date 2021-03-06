#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <omp.h>
#include "DS_timer.h"

using namespace std;

// 로또 숫자의 범위
#define LOTTO_RANGE 45

// 로또 구매자의 수
#define BUYER (1024 * 1024 * 16)

// 인당 구매한 로또의 수
#define LOTTO_NUM 10

// 추첨된 로또 횟수
#define LOTTO_PLAY_NUM 800

//로또 하나의 정보
struct Lotto {
	int number[6];	// 로또 번호 6자리
	int rank;		// 당첨 등수
};

//한 구매자가 구입한 로또
struct Personal_buy {
	Lotto lotto[LOTTO_NUM];
};

void insertionSort(int* data, int n);
void calcRank(Lotto* data);

// 당첨번호
int win[LOTTO_PLAY_NUM][7];

int main()
{
	Personal_buy *buyer;	// 로또를 구매한 구매자
	int **lottoHist;	// 당첨번호 히스토그램(번호, 값)
	int maxHistCnt = -1;	// 가장 많이 추첨된 횟수

	ofstream winOut("당첨번호.txt")/*, buyerOut("추첨결과.txt")*/;	// 파일출력
	
	DS_timer timer(5);	// 시간 비교용 타이머
	

	timer.initTimers();

	//동적할당
	buyer = (Personal_buy*)malloc(sizeof(Personal_buy) * BUYER);

	lottoHist = (int**)malloc(sizeof(int*) * 2);
	for (int i = 0; i < 2; i++)
		lottoHist[i] = (int*)malloc(sizeof(int) * LOTTO_RANGE);

	for (int i = 0; i < LOTTO_RANGE; i++)
		lottoHist[0][i] = i + 1;
	memset(lottoHist[1], 0, sizeof(int) * LOTTO_RANGE);


	// 구매자 번호 추첨
	for (int i = 0; i < BUYER; i++)
		for (int j = 0; j < LOTTO_NUM; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				// 1 ~ 45 사이의 임의의 번호 부여
				buyer[i].lotto[j].number[k] = rand() % LOTTO_RANGE + 1;

				// 중복된 번호 제거
				for (int l = 0; l < k; l++)
					if (buyer[i].lotto[j].number[l] == buyer[i].lotto[j].number[k])
					{
						k--;
						continue;
					}
			}

			//추첨한 번호 정렬
			insertionSort(buyer[i].lotto[j].number, 6);
		}


	// 당첨번호 추첨
	for (int i = 0; i < LOTTO_PLAY_NUM; i++)
		for (int j = 0; j < 7; j++)
		{
			win[i][j] = rand() % LOTTO_RANGE + 1;

			for (int k = 0; k < j; k++)
				if (win[i][k] == win[i][j])
				{
					j--;
					break;
				}
		}


	//당첨번호 정렬(보너스 번호 제외)
	for (int i = 0; i < LOTTO_PLAY_NUM; i++)
		insertionSort(win[i], 6);


	//당첨번호 출력
	//당첨번호.txt 파일에 저장
	//0 0 0 0 0 0 + 0
	for (int i = 0; i < LOTTO_PLAY_NUM; i++)
	{
		for (int j = 0; j < 6; j++)
			winOut << win[i][j] << " ";
		winOut << "+ " << win[i][6] << endl;
	}


	// 1~45의 번호별 당첨횟수
	for (int i = 0; i < LOTTO_PLAY_NUM; i++)
		for (int j = 0; j < 6; j++)
		{
			lottoHist[1][win[i][j] - 1]++;
		}


	// 히스토그램 출력
	maxHistCnt = lottoHist[1][0];
	for (int i = 0; i < LOTTO_RANGE; i++)
	{
		printf("%2d : ", lottoHist[0][i]);
		for (int j = 0; j < lottoHist[1][i] * 60 / (double)maxHistCnt; j++)
			printf("*");

		printf("    (%d)\n", lottoHist[1][i]);
	}
	printf("\n");


	// 상위 번호 20개를 출력하기 위해 횟수를 저장한 배열 정렬
	int i, j, tmp1, tmp2;
	for (i = 1; i < LOTTO_RANGE; i++)
	{
		tmp1 = lottoHist[0][(j = i)];
		tmp2 = lottoHist[1][j];

		while (--j >= 0 && tmp2 > lottoHist[1][j])
		{
			lottoHist[0][j + 1] = lottoHist[0][j];
			lottoHist[0][j] = tmp1;
			lottoHist[1][j + 1] = lottoHist[1][j];
			lottoHist[1][j] = tmp2;
		}
	}


	// 상위 20개 번호 출력
	for (int i = 0; i < 40; i++)
		printf("%d : %d회\n", lottoHist[0][i], lottoHist[1][i]);

	///////////////////////////////////////////////////////////////////////////////////////
	//등수 확인

	timer.setTimerName(0, (char*)"serial");
	timer.onTimer(0);
	for (int i = 0; i < BUYER; i++)
		for (int j = 0; j < LOTTO_NUM; j++)
			calcRank(&(buyer[i].lotto[j]));
	timer.offTimer(0);

	timer.setTimerName(1, (char*)"parallel");
	timer.onTimer(1);
#pragma omp parallel for
	for (int i = 0; i < BUYER; i++)
		for (int j = 0; j < LOTTO_NUM; j++)
			calcRank(&(buyer[i].lotto[j]));
	timer.offTimer(1);

	timer.setTimerName(2, (char*)"static");
	timer.onTimer(2);
#pragma omp parallel for schedule(static, 1) 
	for (int i = 0; i < BUYER; i++)
		for (int j = 0; j < LOTTO_NUM; j++)
			calcRank(&(buyer[i].lotto[j]));
	timer.offTimer(2);

	timer.setTimerName(3, (char*)"dynamic");
	timer.onTimer(3);
#pragma omp parallel for schedule(dynamic, 1) 
	for (int i = 0; i < BUYER; i++)
		for (int j = 0; j < LOTTO_NUM; j++)
			calcRank(&(buyer[i].lotto[j]));
	timer.offTimer(3);

	timer.setTimerName(4, (char*)"guided");
	timer.onTimer(4);
#pragma omp parallel for schedule(guided, 1) 
	for (int i = 0; i < BUYER; i++)
		for (int j = 0; j < LOTTO_NUM; j++)
			calcRank(&(buyer[i].lotto[j]));
	timer.offTimer(4);

	///////////////////////////////////////////////////////////////////////////////////////

	/*
	참여자가 많을때에 생성되는 파일의 크기가 커서 사용불가
	1M명까지 동작 확인함
	*/
	//// 등수 출력
	//for (int i = 0; i < BUYER; i++)
	//{
	//	for (int j = 0; j < LOTTO_NUM; j++)
	//	{
	//		if (buyer[i].lotto[j].rank)
	//		{
	//			buyerOut << "[고객 " << i << "]" << endl;
	//			buyerOut << "<<" << buyer[i].lotto[j].rank << "등>> [" << j << "] ";
	//			for (int k = 0; k < 6; k++)
	//				buyerOut << buyer[i].lotto[j].number[k] << " ";
	//			buyerOut << endl;
	//		}
	//	}
	//}

	// 타이머 결과 출력
	timer.printTimer();

	winOut.close();
	// buyerOut.close();

	return 0;
}

// 작은 크기의 배열에서 효과적인 삽입정렬 선택
/*
param : 정렬되지 않은 정수 배열, 크기
return : void
삽입정렬을 사용하여 오름차순으로 정렬
*/
void insertionSort(int* data, int size)
{
	int i, j, key;
	for (i = 1; i < size; i++)
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

	for (int i = 0; i < 6; i++)
		if (win[LOTTO_PLAY_NUM - 1][6] == data->number[i])
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
