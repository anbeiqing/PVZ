#pragma once
#include"coreMinimal.h"
#include<functional>
#include<chrono>

using std::chrono::milliseconds;


class Timer final
{
public:
    Timer() {lastTime=std::chrono::steady_clock::now();};
    ~Timer() {mainWorld.Timers.erase(this);};

    template<typename T>
    void Bind(double del, T* obj, void(T::* func)(),bool repeat = false,double firstDelay=-1) {
        callback = std::bind(func, obj);
        delay = std::chrono::duration<double>(del);
        lastTime = std::chrono::steady_clock::now();
        if (firstDelay >= 0)lastTime -= milliseconds(int(1000 * (del- firstDelay)));
        bPersistent = repeat;
        mainWorld.Timers.insert(this);

    }
    void Bind(double del, std::function<void()>func, bool repeat = false, double firstDelay = -1) {
        callback = func;
        delay= std::chrono::duration<double>(del);
        lastTime = std::chrono::steady_clock::now();
        if (firstDelay >= 0)lastTime -= milliseconds(int(1000 * (del - firstDelay)));
        bPersistent = repeat;
        mainWorld.Timers.insert(this);

    }
    void Execute() {
        if (delay.count()>0&&getDelay().count() >= delay.count()&&bRunning) {
            callback();
            if (bPersistent)lastTime = std::chrono::steady_clock::now();
            else delay=std::chrono::duration<double>(0);
        }
    }
	double GetDelay() {
		return getDelay().count();
	}
    void SetDelay(double time) {
        delay = std::chrono::duration<double>(time);
    }
    void Reset() {
        lastTime = std::chrono::steady_clock::now();
    }
    void Stop() {
        bRunning = false;
    }
    void Continue() {
        bRunning = true;
    }
private:

    bool bPersistent;
    bool bRunning=true;
    std::function<void()> callback;
    std::chrono::duration<double> delay;
    std::chrono::time_point<std::chrono::steady_clock> lastTime;
    std::chrono::duration<double> getDelay() {
        return std::chrono::steady_clock::now() - lastTime;
    }
};
    