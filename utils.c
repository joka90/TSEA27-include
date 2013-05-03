#include "utils.h"


//turn off optimization 
#pragma GCC push_options
#pragma GCC optimize ("O0")
uint16_t filterSampleArrayMeanPlusPlus(volatile uint16_t  *samples, uint8_t numOfSamples, uint8_t threshold)
{
	uint8_t currentNumInRow=0;
	uint16_t currentSum=0;
	for(uint8_t i = 0; i < numOfSamples-1; i++)
	{
		if(absThreshold(samples[i], samples[i+1]) < threshold)
		{
			currentNumInRow++;
			currentSum+=samples[i];//summera
		}
	}
	if(absThreshold(samples[0], samples[numOfSamples-1]) < threshold)
	{
		currentNumInRow++;
		currentSum+=samples[numOfSamples-1];//summera
	}
	if(currentNumInRow!=0)
	{
		uint16_t ttt = (uint16_t)(currentSum/currentNumInRow);//return result
		return ttt;//return result
	}
	else
	{
		return median(samples,numOfSamples);
	}
}
#pragma GCC pop_options
//end turn off optimization 

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
void insertionSort(volatile uint16_t *a, uint8_t array_size)
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
uint16_t median(volatile uint16_t *a, uint8_t size)
{
	insertionSort(a,size);
	return a[size>>1];
}
int16_t int8to16(int8_t inParm)
{
	if(inParm > 128)
	{
		inParm |= 0xFF00;
	}
	return inParm;
}