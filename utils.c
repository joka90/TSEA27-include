#include "utils.h"

uint16_t filterSampleArrayMeanPlusPlus(volatile uint16_t  *samples, uint8_t numOfSamples, uint8_t threshold)
{
	uint8_t maxNumInRow=0;
	uint8_t currentNumInRow=0;
	uint16_t currentSum=0;
	uint16_t bestSum=0;
	for(uint8_t i = 0; i < numOfSamples-1; i++)
	{
		if(absThreshold(samples[i], samples[i+1]) < threshold)
		{
			currentNumInRow++;
			currentSum+=samples[i];//summera
		}
		else
		{
			if(maxNumInRow<=currentNumInRow)
			{
				bestSum=currentSum+samples[i];
				maxNumInRow=currentNumInRow;
			}
			currentNumInRow=0;
		}
		if(maxNumInRow<=currentNumInRow)
		{
			if(!(i < numOfSamples-2))//sista
			{
				bestSum=currentSum+samples[i+1];
			}
			else
			{
				bestSum=currentSum;
			}
			maxNumInRow=currentNumInRow;
		}
	}
	return (uint16_t)(bestSum/(maxNumInRow+1));//return result
}

uint16_t absThreshold(uint16_t a1, uint16_t a2)
{
  if(a1 < a2)
  {
    return a2-a1;
  }
  else
  {
    return a1-a2;
  }
}
void insertionSort(uint16_t *a, uint8_t array_size)
{
     uint8_t i, j;
	 uint16_t index;
     for (i = 1; i < array_size; ++i)
     {
          index = a[i];
          for (j = i; j > 0 && a[j-1] > index; j--)
               a[j] = a[j-1];

          a[j] = index;
     }
}
uint16_t median(uint16_t *a, uint8_t size)
{
	insertionSort(a,size);
	return a[size>>1];
}