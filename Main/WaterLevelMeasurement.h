const bool enabled = false;

const int pingPin = 9;
const int echoPin = 10;

// tank height in cm
const float tankHeight = 60;
// tank volume in l
const float tankVolume = 60;
// tank diameter in cm
const float tankDiameter = 35.6;

float getWaterLevelInLiter();
float convertLiterToPercent(float volume);
bool isWaterLevelLow(float waterlevelPercentage);
bool isWaterLevelCritical(float waterlevelPercentage);
bool isWaterLevelDisabled();
float calculateVolume(float distance, float height, float volume);
float calculateDiameter(float height, float volume);

void clear();
void ping();