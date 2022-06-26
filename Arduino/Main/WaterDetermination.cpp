#include "WaterDetermination.h"

bool isCurrentlyRaining(String weatherData[])
{
  if(weatherData[0].equals("ok"))
  {
    return weatherData[3].equals("rain");
  }
  return false;
}

bool isTodaysAndTomorrowsRainfallSufficient(String weatherData[])
{
  if(weatherData[0].equals("ok"))
  {
    const String rainfallToday = weatherData[5];
    const int indexToday = rainfallToday.indexOf(' ');
    const float rainfallTodayValue = rainfallToday.substring(0, indexToday).toFloat();

    const String rainfallTomorrow = weatherData[5];
    const int indexTomorrow = rainfallTomorrow.indexOf(' ');
    const float rainfallTomorrowValue = rainfallTomorrow.substring(0, indexTomorrow).toFloat();

    if(rainfallTodayValue + rainfallTomorrowValue >= 25){
      return true;
    }
  }
  return false;
}