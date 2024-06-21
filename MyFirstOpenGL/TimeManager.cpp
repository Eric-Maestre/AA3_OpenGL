#include "TimeManager.h"

TimeManager::TimeManager(float cycleDuration)
	:cycleDuration(cycleDuration), elapsedTime(0.f)
{
}

void TimeManager::Update(float dt)
{
    elapsedTime += dt;
    if (elapsedTime >= cycleDuration)
    {
        elapsedTime -= cycleDuration;
    }
}

bool TimeManager::IsDay() const
{
    return elapsedTime < cycleDuration / 2.0f;
}
