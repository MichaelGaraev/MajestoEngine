#pragma once

class GameTimer
{
public:
    GameTimer();

    float TotalTime() const; // in seconds
    float DeltaTime() const; // in seconds

    void Reset(); // Call before message loop.
    void Start(); // Call when unpaused.
    void Stop(); // Call when paused.
    void Tick(); // Call every frame.

private:
    double mSecondsPerCount;

    // Counting Delta time
    __int64 mPrevTime;
    __int64 mCurrTime;
    double mDeltaTime;

    // Implement Total time
    __int64 mBaseTime;
    __int64 mPausedTime;
    __int64 mStopTime;

    bool mStopped;
};