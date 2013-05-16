/**
 * TSEA27 Elektronikprojekt
 *
 * IDENTIFIERING
 *
 * Modul: Alla
 * Filnamn: utils.c
 * Skriven av: J. Källström
 * Datum: 2013-05-15
 * Version: 1.0
 *
 * BESKRIVNING
 *
 * Allmänna hjälpfunktioner
 */

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
			currentSum+=samples[i]; // summera
		}
	}
	if(absThreshold(samples[0], samples[numOfSamples-1]) < threshold)
	{
		currentNumInRow++;
		currentSum+=samples[numOfSamples-1]; // summera
	}
	if(currentNumInRow!=0)
	{
		uint16_t ttt = (uint16_t)(currentSum/currentNumInRow);//return result
		return ttt;
	}
	else
	{
		return median(samples,numOfSamples);
	}
}
#pragma GCC pop_options
//end turn off optimization 

//turn off optimization 
#pragma GCC push_options
#pragma GCC optimize ("O0")
uint16_t filterMeanTimeAware(volatile uint16_t  *samples, uint8_t numOfSamples, uint8_t threshold, uint8_t start)
{
	//räkna ut medelvärde
	//ta bort för långa värden som uppkommer pga avbrott
	//sluta läsa av arrayen om roboten står still
	uint8_t currentNumInRow=0;
	uint16_t currentSum=0;
	uint8_t i = 0;
	uint8_t n = start;
	uint8_t spike = 0;
	while(i < numOfSamples-1)
	{
		if(absThreshold(samples[n], samples[n-1]) < (samples[n]>>threshold))
		{
			currentNumInRow++;
			currentSum+=samples[n];//summera
		}
		else
		{
			spike = 1;
			break; //vi bildar inte medelvärde om det är utanför gränsen. t.ex står stilla.
		}
		n--;
		i++;
		if(n<0)
		{
			n=numOfSamples-1;
		}
	}
	if((spike==0)&&(absThreshold(samples[start], samples[start+1]) < (samples[start]>>threshold)))//testa sista om vi inte har fått en spike
	{
		currentNumInRow++;
		currentSum+=samples[start+1];//summera
	}
	if(currentNumInRow!=0)
	{
		uint16_t ttt = (uint16_t)(currentSum/currentNumInRow);//return result
		return ttt;//return result
	}
	else
	{
		return samples[start];//om vi har en -x^2 dvs derivatan runt den ser ish ut som en triangel.
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
uint16_t median(volatile uint16_t *b, uint8_t size)
{
	uint16_t a[size];
	for(uint8_t n = 0; n < size; n++)//copy array before sort.
	{
		a[n]=b[n];
	}
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
