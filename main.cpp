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

void redLight(int redTime)
{
    std::unique_lock<std::mutex> lock(mtx);
    // mutex std::mutex mtx;
    logState("Traffic Light: Red");
    std::this_thread::sleep_for(std::chrono::seconds(redTime));
    cv.wait_for(lock, std::chrono::seconds(redTime));
}

void yellowLight(int yellowTime)
{
    mtx.lock();
    logState("Traffic Light: Yellow");
    std::this_thread::sleep_for(std::chrono::seconds(yellowTime));
    mtx.unlock();
}

void greenLight(int greenTime)
{
    // mutex std::mutex mtx;

    logState("Traffic Light: Green");
    std::this_thread::sleep_for(std::chrono::seconds(greenTime));
}

void trafficLightCycle(int greenTime, int redTime, int yellowTime) // Traffic Light cycle, green -> yellow -> red -> yellow
{
    TrafficLightColor color = TrafficLightColor::RED;
    while (!exitprogram)
    {
        if (color == TrafficLightColor::GREEN)
        {
            greenLight(greenTime);
            color = TrafficLightColor::YELLOW;
        }
        else if (color == TrafficLightColor::YELLOW)
        {
            yellowLight(yellowTime);
            color = TrafficLightColor::RED;
        }
        else if (color == TrafficLightColor::RED)
        {
        }
    }
}

// void pedestrianCycle() // Pedestrian Light cycle
//{
// }

// void pushButton() // Pedestrian pushes the button, ex if traffic light red -> red for longer
//{

// std::unique_lock<std::mutex>lock(mtx);
// pbutton.wait(lock,[]{ return ready;});
//}
void setPushbutton()
{
}

int main()
{

    // config
    int greenTime = 10;
    int redTime = 10;
    int yellowTime = 3;
    int extendedRedTime = 15;

    // std::thread redLightThread(redLight, redTime);
    // std::thread yellowLightThread(yellowLight, yellowTime);
    // std::thread greenLightThread(greenLight, greenTime);
    std::thread userInputThread(userInput);
    std::thread trafficLightControllerThread(trafficLightController, greenTime, redTime, yellowTime, extendedRedTime);

    userInputThread.join();
    trafficLightControllerThread.join();

    // redLightThread.join();
    // yellowLightThread.join();
    /// greenLightThread.join();

    /*while (!exitprogram)
    {
        if (pushbutton)
        {
            pedestrianCycle();
            pushbutton = false;
        }
        else
        {
            trafficLightCycle();
        }
    }*/

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
        }
        else if (input == 'p')
        {
            {
                std::unique_lock<std::mutex> lock(mtx);
                pushbutton.push(true);
            }
            cv.notify_all();
        }
        else
        {
            std::cout << "Invalid input" << std::endl;
        }
    }

} // exit program, While (!exit) char input; cin >> input; If (input == "q") exit = true.

void logState(const std::string &event)
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << "[" << std::put_time(localtime(&now_c), "%T") << "] " << event << std::endl;
} // Funktion för att se vilken tid dem olika funktionerna utförs i realtid. chrono::system_clock::now()

void trafficLightController(int greenTime, int redTime, int yellowTime, int extendedRedTime)
{
    while (!exitprogram)
    {
        std::unique_lock<std::mutex> lock(mtx);
        /*std::lock_guard<std::mutex> lock(mtx);*/
        if (!pushbutton.empty())

        {
            pushbutton.pop();
            logState("Pedestrian Button Pushed");

            if (currentState != TrafficLightColor::RED)

            {
                currentState = TrafficLightColor::YELLOW;
                logState("Traffic Light: Yellow");
                cv.wait_for(lock, std::chrono::seconds(yellowTime));
                // std::this_thread::sleep_for(std::chrono::seconds(yellowTime));
                //  currentState = TrafficLightColor::RED;
                //  logState("Traffic Light: Red");
            }
            currentState = TrafficLightColor::RED;
            logState("Traffic Light: Extended Red");
            cv.wait_for(lock, std::chrono::seconds(extendedRedTime));
            // std::this_thread::sleep_for(std::chrono::seconds(extendedRedTime));
            continue;
        }

        currentState = TrafficLightColor::YELLOW;
        logState("Traffic Light: Yellow");
        cv.wait_for(lock, std::chrono::seconds(yellowTime));
        // std::this_thread::sleep_for(std::chrono::seconds(yellowTime));

        currentState = TrafficLightColor::GREEN;
        logState("Traffic Light: Green");
        cv.wait_for(lock, std::chrono::seconds(greenTime));
        // std::this_thread::sleep_for(std::chrono::seconds(greenTime));

        currentState = TrafficLightColor::YELLOW;
        logState("Traffic Light: Yellow");
        cv.wait_for(lock, std::chrono::seconds(yellowTime));
        // std::this_thread::sleep_for(std::chrono::seconds(yellowTime));

        currentState = TrafficLightColor::RED;
        logState("Traffic Light: Red");
        cv.wait_for(lock, std::chrono::seconds(redTime));
        // std::this_thread::sleep_for(std::chrono::seconds(redTime));
    }
}
