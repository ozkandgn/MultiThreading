#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

double firstTimer = 0;

void *FCFS(void *arg)
{
	firstTimer = clock();
	printf("Thread %d ise basladi...\n", (long)arg);
	int gc[10][2];
	for (int i = 0; i<10; i++)
	{
		for (int j = 0; j<2; j++)
		{
			gc[i][j] = 0;
		}
	}
	int counter = -1;
	FILE *file = fopen("data.txt", "r");
	int process, arrivalTime, burstTime = 0, priority;
	while (!feof(file))
	{
		if (burstTime == 0)
		{
			fscanf(file, "%d %d %d %d", &process, &arrivalTime, &burstTime, &priority);
			counter++;
		}
		else
		{
			burstTime--;
			gc[counter][0] = process;
			gc[counter][1]++;
		}
		if (feof(file))
		{
			for (int i = 0; i<burstTime; i++)
			{
				gc[counter][0] = process;
				gc[counter][1]++;
			}
		}
	}
	int total = 0;
	printf("**FCFS**\nP BT\n");
	for (int i = 0; i<10; i++)
	{
		printf("%d %d\t", gc[i][0], gc[i][1]);
		total += gc[i][1];
		printf("Toplam BurstTime=%d\n", total);
	}
	printf("**END FCFS**\n");
	printf("*** Geçen Zaman=%.3f Nanosaniye ***\n\n\n", (clock() - firstTimer));
}

void *SCFP(void *arg)
{
	firstTimer = clock();
	printf("Thread %d ise basladi...\n", (long)arg);
	FILE *file = fopen("a.txt", "r");
	int gc[10][2];
	for (int i = 0; i<10; i++)
	{
		for (int j = 0; j<2; j++)
		{
			gc[i][j] = 0;
		}
	}
	int process[10], arrivalTime[10], burstTime[10], priority[10];
	int counter = -1;
	int time = 0;
	int gcCounter = 0;
	int minBurstCounter = 0;
	int maxBurstCounter = 0;
	int oldMinBurstCounter = 0;
	bool firstBurst = true;
	int burstCounter = 0;
	int totalBurst = 0;
	int totalTime = 0;
	int backBurst = 0;
	printf("**SCFP**\nP BT \n");
	while (!feof(file))
	{
		counter++;
		fscanf(file, "%d %d %d %d", &process[counter], &arrivalTime[counter], &burstTime[counter], &priority[counter]);
		totalTime += burstTime[counter];
	}
	while (totalTime != time)
	{
		oldMinBurstCounter = minBurstCounter;
		bool isChanged = false;
		for (int i = 0; i<counter + 1; i++)
		{
			if (time >= arrivalTime[i])
			{
				if ((burstTime[minBurstCounter]>burstTime[i] && burstTime[i]>0))
				{
					minBurstCounter = process[i];
					isChanged = true;
				}
				if (burstTime[maxBurstCounter]<burstTime[i])
				{
					maxBurstCounter = process[i];
				}
			}
		}
		burstTime[minBurstCounter]--;
		if (burstTime[minBurstCounter] == 0)
		{
			backBurst = burstCounter + 1 - burstTime[maxBurstCounter];
			burstTime[minBurstCounter] = burstTime[maxBurstCounter];
		}
		if (isChanged&&!firstBurst)
		{
			printf("%d %d\t", process[oldMinBurstCounter], burstCounter);
			printf("Toplam BurstTime=%d\n", totalBurst);
			burstCounter = 0;
		}
		else
		{
			firstBurst = false;
		}
		burstCounter++;
		totalBurst++;
		time++;

	}
	printf("%d %d\t", process[minBurstCounter], backBurst);
	printf("Toplam BurstTime=%d\n", totalBurst - burstTime[maxBurstCounter]);
	printf("%d %d\t", process[maxBurstCounter], burstTime[maxBurstCounter]);
	printf("Toplam BurstTime=%d\n", totalBurst);
	printf("**END SCFP**\n");
	printf("*** Geçen Zaman=%.3f Nanosaniye ***\n\n\n", (clock() - firstTimer));
}

void *SCFNP(void *arg)
{
	firstTimer = clock();
	printf("Thread %d ise basladi...\n", (long)arg);
	FILE *file = fopen("a.txt", "r");
	int gc[10][2];
	for (int i = 0; i<10; i++)
	{
		for (int j = 0; j<2; j++)
		{
			gc[i][j] = 0;
		}
	}
	int process[10], arrivalTime[10], burstTime[10], priority[10];
	int counter = -1;
	int time = 0;
	int gcCounter = 0;
	int minBurstCounter = 0;
	int maxBurstCounter = 0;
	int oldMinBurstCounter = 0;
	bool firstBurst = true;
	int burstCounter = 0;
	int totalBurst = 0;
	int totalTime = 0;
	int backBurst = 0;
	bool isReady = true;
	printf("**SCFNP**\nP BT \n");
	while (!feof(file))
	{
		counter++;
		fscanf(file, "%d %d %d %d", &process[counter], &arrivalTime[counter], &burstTime[counter], &priority[counter]);
		totalTime += burstTime[counter];
	}
	while (totalTime != time)
	{
		oldMinBurstCounter = minBurstCounter;
		bool isChanged = false;
		for (int i = 0; i<counter + 1; i++)
		{
			if (time >= arrivalTime[i] && isReady)
			{
				if ((burstTime[minBurstCounter]>burstTime[i] && burstTime[i]>0))
				{
					minBurstCounter = process[i];
					isChanged = true;
				}
				if (burstTime[maxBurstCounter]<burstTime[i])
				{
					maxBurstCounter = process[i];
				}
			}
		}
		isReady = false;
		burstTime[minBurstCounter]--;
		if (burstTime[minBurstCounter] == 0)
		{
			isReady = true;
			backBurst = burstCounter + 1 - burstTime[maxBurstCounter];
			burstTime[minBurstCounter] = burstTime[maxBurstCounter];
		}
		if (isChanged&&!firstBurst)
		{
			printf("%d %d\t", process[oldMinBurstCounter], burstCounter);
			printf("Toplam BurstTime=%d\n", totalBurst);
			burstCounter = 0;
		}
		else
		{
			firstBurst = false;
		}
		burstCounter++;
		totalBurst++;
		time++;

	}
	printf("%d %d\t", process[minBurstCounter], backBurst);
	printf("Toplam BurstTime=%d\n", totalBurst - burstTime[maxBurstCounter]);
	printf("%d %d\t", process[maxBurstCounter], burstTime[maxBurstCounter]);
	printf("Toplam BurstTime=%d\n", totalBurst);
	printf("**END SCFNP**\n");
	printf("*** Geçen Zaman=%.3f Nanosaniye ***\n\n\n", (clock() - firstTimer));
}

void *Priority(void *arg)
{
	firstTimer = clock();
	printf("Thread %d ise basladi...\n", (long)arg);
	FILE *file = fopen("a.txt", "r");
	int gc[10][2];
	for (int i = 0; i<10; i++)
	{
		for (int j = 0; j<2; j++)
		{
			gc[i][j] = 0;
		}
	}
	int process[10], arrivalTime[10], burstTime[10], priority[10];
	int counter = -1;
	int time = 0;
	int gcCounter = 0;
	int maxPriorityCounter = 0;
	int oldMaxPriorityCounter = 0;
	int minPriorityCounter = 0;
	bool firstBurst = true;
	int burstCounter = 0;
	int totalBurst = 0;
	int totalTime = 0;
	printf("**Priority**\nP BT \n");
	while (!feof(file))
	{
		counter++;
		fscanf(file, "%d %d %d %d", &process[counter], &arrivalTime[counter], &burstTime[counter], &priority[counter]);
		totalTime += burstTime[counter];
	}
	while (totalTime != time)
	{
		oldMaxPriorityCounter = maxPriorityCounter;
		bool isChanged = false;
		for (int i = 0; i<counter + 1; i++)
		{
			for (int j = 0; j<counter + 1; j++)
			{
				if (time >= arrivalTime[i])
				{
					if (priority[maxPriorityCounter]<priority[i])
					{
						maxPriorityCounter = i;
						isChanged = true;
					}
					if (priority[maxPriorityCounter]>priority[i])
					{
						minPriorityCounter = i;
					}
				}
			}
		}
		burstTime[maxPriorityCounter]--;
		if (burstTime[maxPriorityCounter] == 0)
		{
			priority[maxPriorityCounter] = -1;

		}
		if (isChanged&&!firstBurst)
		{

			printf("%d %d \t", process[oldMaxPriorityCounter], burstCounter);
			printf("Toplam BurstTime=%d\n", totalBurst);
			burstCounter = 0;
		}
		else
		{
			firstBurst = false;
		}
		burstCounter++;
		totalBurst++;
		time++;

	}
	printf("%d %d\t", process[maxPriorityCounter], burstCounter);
	printf("Toplam BurstTime=%d\n", totalBurst);
	printf("**END Priority**\n");
	printf("*** Geçen Zaman=%.3f Nanosaniye ***\n\n\n", (clock() - firstTimer));
}

int quantumTime;
void *RR(void *arg)
{
	firstTimer = clock();
	printf("Thread %d ise basladi...\n", (long)arg);
	FILE *file = fopen("a.txt", "r");
	int process[10], arrivalTime[10], burstTime[10], priority[10];
	int counter = -1;
	int totalTime = 0;
	int totalBurst = 0;
	int time = 0;
	int quantumCounter = 0;
	printf("Quantum Time=%d\n", quantumTime);
	printf("**RR**\nP BT \n");
	while (!feof(file) || totalTime != time)
	{
		if (counter != 9)
		{
			counter++;
			fscanf(file, "%d %d %d %d", &process[counter], &arrivalTime[counter], &burstTime[counter], &priority[counter]);
			totalTime += burstTime[counter];
		}
		if (arrivalTime[quantumCounter] <= time&&burstTime[quantumCounter]>0)
		{
			if (burstTime[quantumCounter] <= quantumTime)
			{
				totalBurst += burstTime[quantumCounter];
				burstTime[quantumCounter] = 0;
				printf("%d %d\t", process[quantumCounter], 0);
				printf("Toplam BurstTime=%d\n", totalBurst);
			}
			else
			{
				totalBurst += quantumTime;
				burstTime[quantumCounter] -= quantumTime;
				printf("%d %d\t", process[quantumCounter], burstTime[quantumCounter]);
				printf("Toplam BurstTime=%d\n", totalBurst);
			}
			quantumCounter++;
			if (quantumCounter == 10)
			{
				quantumCounter = 0;
			}
		}
		time++;
	}
	bool turnHead = false;
	for (int i = quantumCounter; i<counter + 1; i++)
	{
		if (burstTime[i]>0)
		{
			if (burstTime[i] <= quantumTime)
			{
				totalBurst += burstTime[i];
				burstTime[i] = 0;
				printf("%d %d\t", process[i], 0);
				printf("Toplam BurstTime=%d\n", totalBurst);
			}
			else
			{
				totalBurst += quantumTime;
				burstTime[i] -= quantumTime;
				printf("%d %d\t", process[i], burstTime[i]);
				printf("Toplam BurstTime=%d\n", totalBurst);
			}
			turnHead = true;
		}
		if (i == counter&&turnHead)
		{
			i = 0;
			turnHead = false;
		}
	}
	printf("**END RR**\n");
	printf("*** Geçen Zaman=%.3f Nanosaniye ***\n\n\n", (clock() - firstTimer));
}

int main()
{

	pthread_t tid[7];
	pthread_create(&tid[0], NULL, FCFS, (void*)tid[0]);
	pthread_join(tid[0], NULL);
	pthread_create(&tid[1], NULL, SCFP, (void*)tid[1]);
	pthread_join(tid[1], NULL);
	pthread_create(&tid[2], NULL, SCFNP, (void*)tid[2]);
	pthread_join(tid[2], NULL);
	pthread_create(&tid[6], NULL, Priority, (void*)tid[6]);
	pthread_join(tid[6], NULL);
	quantumTime = 3;
	pthread_create(&tid[3], NULL, RR, (void*)tid[3]);
	pthread_join(tid[3], NULL);
	quantumTime = 4;
	pthread_create(&tid[4], NULL, RR, (void*)tid[4]);
	pthread_join(tid[4], NULL);
	quantumTime = 8;
	pthread_create(&tid[5], NULL, RR, (void*)tid[5]);
	pthread_join(tid[5], NULL);
}