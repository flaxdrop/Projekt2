#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <iomanip>

std::atomic<bool> pushButton(false);
std::atomic<bool> exit(false);
std::mutex mtx;
std::condition_variable pbutton;
/*enum class TrafficLightColor {GREEN, RED, YELLOW};
void trafficLight(int green, int yellow, int red);
int main (){

    int green = 10
    int red = 5
    int yellow = 10
}*/

void redLight(int redTime)
{
    // mutex std::mutex mtx;
    std::cout << "Traffic Light: Red" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(redTime));
}

void yellowLight(int yellowTime)
{
    mtx.lock();
    std::cout << "Traffic Light: Yellow" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(yellowTime));
    mtx.unlock();
}
// pbutton.wait_for(lock, std::chrono::seconds(yellowTime))

void greenLight(int greenTime)
{
    // mutex std::mutex mtx;
    std::cout << "Traffic Light: Green" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(greenTime));
}

void trafficLightCycle() // Traffic Light cycle, green -> yellow -> red -> yellow
{
}

void pedestrianCycle() // Pedestrian Light cycle
{
}

void pushButton() // Pedestrian pushes the button, ex if traffic light red -> red for longer
{
    // std::unique_lock<std::mutex>lock(mtx);
    // pbutton.wait(lock,[]{ return ready;});
}
void setPushbutton()
{
}

int main()
{

    // config
    int greenTime = 10;
    int redTime = 10;
    int yellowTime = 3;

    std::thread redLightThread(redLight, redTime);
    std::thread yellowLightThread(yellowLight, yellowTime);
    std::thread greenLightThread(greenLight, greenTime);

    redLightThread.join();
    yellowLightThread.join();
    greenLightThread.join();

    return 0;
}

void userInput() {} // exit program, While (!exit) char input; cin >> input; If (input == "q") exit = true.

void logState() {} // Funktion för att se vilken tid dem olika funktionerna utförs i realtid. chrono::system_clock::now()