#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

class TimeManager
{
public:
    TimeManager(float cycleDuration);

    void Update(float deltaTime);
    bool IsDay() const;

private:
    float cycleDuration;
    float elapsedTime;
};

#endif // TIMEMANAGER_H
