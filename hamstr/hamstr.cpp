#include <stdio.h>
#include <stdint.h>
#include <cstdlib>
#include <math.h>
#include <ctime>

int calculateMaxHamsterUsage(int dailyRate, int avarice, int hamsterCount);

// quick select
int getPivotIndex(int left, int right);
int partition(unsigned long long* arr, int left, int right);
int quickSelect(unsigned long long* arr, int kOrder, int left, int right);

void fisherYatesShuffle(unsigned long long* arr, int N );

int main(int ac, char** av)
{
    const char* inputFileName = NULL;
    const char* outputFileName = NULL;

    // read input arguments
    if(ac > 1)
    {
        // file name had been passed via command line
        inputFileName = av[1];
        outputFileName = av[2];
    }
    else
    {
        // use default file name
        inputFileName = "hamstr.in";
        outputFileName = "hamstr.out";
    }

    FILE *inputFile = fopen(inputFileName, "r");
    unsigned int dailyLimit, hamsterCount = 0;
    unsigned int *dailyRate = NULL;
    unsigned int *avarice = NULL;
    unsigned long long* maxHamsterUsage = NULL;

    if (inputFile)
    {
        fscanf(inputFile, "%u", &dailyLimit);
        fscanf(inputFile, "%u", &hamsterCount);

        dailyRate = (unsigned int*) malloc(sizeof(int) * hamsterCount);
        avarice = (unsigned int*) malloc(sizeof(int) * hamsterCount);
        maxHamsterUsage = (unsigned long long*) malloc(sizeof(int64_t) * hamsterCount);
    }

    // read food usage and avarice values
    for (int i = 0; i < hamsterCount; ++i)
        fscanf(inputFile, "%u %u", dailyRate + i, avarice + i);

    int maxCount = 0;
    int chosenHamsterCount = round(hamsterCount / 2.0);
    int first = 0;
    int last = hamsterCount;

    while(true)
    {
        // calcualte max food usage for each hamster
        for (int j = 0; j < hamsterCount; ++j)
            maxHamsterUsage[j] = calculateMaxHamsterUsage(dailyRate[j], avarice[j], chosenHamsterCount - 1);

        fisherYatesShuffle(maxHamsterUsage, hamsterCount);

        unsigned long long sumResult = 0;
        quickSelect(maxHamsterUsage, chosenHamsterCount - 1, 0, hamsterCount - 1);
        for (int i = 0; i < chosenHamsterCount && sumResult <= dailyLimit; ++i)
            sumResult += maxHamsterUsage[i];

        if (sumResult <= dailyLimit)
        {
            maxCount = chosenHamsterCount;
            if (chosenHamsterCount == hamsterCount) break;
            if (first == last) break;

            first = chosenHamsterCount;
            chosenHamsterCount = round((first + last) / 2.0);
        }
        else
        {
            if (last - first == 1) break;
            last = chosenHamsterCount;
            chosenHamsterCount = round((first + last) / 2.0);
        }
    }

    // save result to file
    FILE *outputFile = fopen(outputFileName, "w");
    if(outputFile)
    {
        fprintf (outputFile, "%d", maxCount);

        fclose(outputFile);
        outputFile = NULL;
    }

    return 0;
}

int calculateMaxHamsterUsage(int dailyRate, int avarice, int hamsterCount)
{
    return dailyRate + hamsterCount * avarice;
}

int getPivotIndex(int left, int right)
{
    return left + (right - left) / 2.0;
}

int partition(unsigned long long* arr, int left, int right)
{
    int pivotIndex = getPivotIndex(left, right);
    int pivotValue = arr[pivotIndex];
    int tmpLeft = left;
    int tmpRight = right;

    do
    {
        // looking for element in left part
        while(tmpLeft <= right && arr[tmpLeft] < pivotValue) tmpLeft++;

        // looking for element int right part
        while(tmpRight > 0 && arr[tmpRight] > pivotValue) tmpRight--;

        if(tmpLeft <= tmpRight)
        {
            int tmp = arr[tmpLeft];
            arr[tmpLeft] = arr[tmpRight];
            arr[tmpRight] = tmp;

            // we must track pivotIndex
            if(tmpLeft == pivotIndex)
                pivotIndex = tmpRight;
            else if(tmpRight == pivotIndex)
                pivotIndex = tmpLeft;

            tmpLeft++;
            tmpRight--;
        }

    } while(tmpLeft <= tmpRight);

    // swap if pivot not on the latest pos
    if(pivotIndex != tmpLeft)
    {
        int tmp = arr[pivotIndex];
        arr[pivotIndex] = arr[tmpLeft-1];
        arr[tmpLeft-1] = tmp;
        pivotIndex = tmpLeft - 1;
    }

    return pivotIndex;
}

int quickSelect(unsigned long long* arr, int kOrder, int left, int right)
{

    int pivotPos = partition(arr, left, right);

    if (pivotPos == kOrder)
        return arr[pivotPos];

    else if (pivotPos < kOrder)
        return quickSelect(arr, kOrder, pivotPos + 1, right);
    else
        return quickSelect(arr, kOrder, left, pivotPos - 1);
}

void fisherYatesShuffle(unsigned long long* arr, int N )
{
 int j, tmp;
 for(int i = N-1; i >= 0; i--)
 {
  j = rand() % (i + 1);

  tmp = arr[j];
  arr[j] = arr[i];
  arr[i] = tmp;
 }
}

