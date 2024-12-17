#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <iomanip>
#include <queue>
enum class TrafficLightColor
{
    GREEN,
    RED,
    YELLOW
};
std::atomic<TrafficLightColor> currentState(TrafficLightColor::RED);
std::queue<bool> pushbutton;
std::atomic<bool> exitprogram(false);
std::mutex mtx;
std::condition_variable cv;
void trafficLightController(int greenTime, int redTime, int yellowTime, int extendedRedTime);
void userInput();
void logState(const std::string &event);

int main()
{

    int greenTime = 5;
    int redTime = 5;
    int yellowTime = 3;
    int extendedRedTime = 10;

    std::thread userInputThread(userInput);
    std::thread trafficLightControllerThread(trafficLightController, greenTime, redTime, yellowTime, extendedRedTime);

    userInputThread.join();
    trafficLightControllerThread.join();

    return 0;
}

void userInput()
{
    while (!exitprogram)
    {
        char input;
        std::cin >> input;
        if (input == 'q')
        {
            exitprogram = true;
            cv.notify_all();
        }
        else if (input == 'p')
        {
            {
                std::unique_lock<std::mutex> lock(mtx);
                pushbutton.push(true);
                logState("Pedestrian Button Pushed");
            }
            cv.notify_one();
        }
        else
        {
            std::cout << "Invalid input" << std::endl;
        }
    }
}

void logState(const std::string &event)
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << "[" << std::put_time(localtime(&now_c), "%T") << "] " << event << std::endl;
}

void trafficLightController(int greenTime, int redTime, int yellowTime, int extendedRedTime)
{
    while (!exitprogram)
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (!pushbutton.empty())

        {
            pushbutton.pop();
            if (currentState != TrafficLightColor::RED)

            {
                currentState = TrafficLightColor::YELLOW;
                logState("Traffic Light: Yellow");
                cv.wait_for(lock, std::chrono::seconds(yellowTime));

                currentState = TrafficLightColor::RED;
                logState("Traffic Light: Red");
                cv.wait_for(lock, std::chrono::seconds(redTime));
            }
            logState("Traffic Light: Extended red light for pedestrian");
            cv.wait_for(lock, std::chrono::seconds(extendedRedTime));
            continue;
        }

        currentState = TrafficLightColor::YELLOW;
        logState("Traffic Light: Yellow");
        cv.wait_for(lock, std::chrono::seconds(yellowTime));

        currentState = TrafficLightColor::GREEN;
        logState("Traffic Light: Green");
        cv.wait_for(lock, std::chrono::seconds(greenTime));

        currentState = TrafficLightColor::YELLOW;
        logState("Traffic Light: Yellow");
        cv.wait_for(lock, std::chrono::seconds(yellowTime));

        currentState = TrafficLightColor::RED;
        logState("Traffic Light: Red");
        cv.wait_for(lock, std::chrono::seconds(redTime));
    }
}
