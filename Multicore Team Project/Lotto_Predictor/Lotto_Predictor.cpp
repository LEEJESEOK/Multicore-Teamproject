#include <stdio.h>
#include <time.h>
#include "DS_timer.h"

int main()
{
	short lotto[6];

	srand(time(NULL));

	for (int i = 0; i < 6; i++)
	{
		lotto[i] = rand() % 45 + 1;

		for (int j = 0; j < i; j++)
			if (lotto[j] == lotto[i])
			{
				i--;
				break;
			}
	}



	for (int i = 0; i < 6; i++)
		printf("%d ", lotto[i]);


	return 0;
}