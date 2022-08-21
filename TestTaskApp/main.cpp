#include <iostream>
using namespace std;

#include <list.h>

#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <chrono>

template <typename T>
uint MeasureRunTime(std::function< void(void) > lambda)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    lambda();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto delta_ms = std::chrono::duration_cast<T>(t2 - t1);

    return delta_ms.count();
}

/* Task 1:
 * Напишите функцию, которая принимает на вход знаковое целое число и печатает его двоичное
 * представление, не используя библиотечных классов или функций.
*/
template <typename T>
void PrintIntAsBin(T number)
{
    constexpr auto msbIdx = sizeof(T) * 8 - 1;

    for (int idx = msbIdx; idx >= 0; idx--)
        cout << ((number >> idx) & 0x1u);

    cout << endl;
}

void Task1()
{
    // Task 1 run
    srand(time(NULL));
    int number = rand();
    cout << "Number: " << number << endl;
    PrintIntAsBin(number);
}

/* Task 2:
 * Напишите функцию, удаляющую последовательно дублирующиеся символы в строке
*/
void RemoveDups(char* str)
{
    if ((str == nullptr) || (*str == '\0'))
        return;

    char* nextChar = str;
    const auto MAX_STR_LEN = 255; // For extreme case when str isn't null-terminated

    for (auto i = 0; (*nextChar) && (i < MAX_STR_LEN); i++)
    {
        if (*++nextChar != *str)
            *++str = *nextChar;
    }
}

void Task2()
{
    // Task 2 run
    char data[] = "AAA BBB AAA";
    RemoveDups(data);
    printf("%s\n", data);
}

void Task3(const uint elementsCount)
{
    FILE * pFile;
    List lst1, lst2;

    for (uint i = 0; i < elementsCount; i++)
        lst1.PushBack(std::to_string(i));
    lst1.PointNodesRandomly();

    cout << "List with size: " << elementsCount << endl;

    pFile = fopen("myFile.txt", "wb");
    uint serialDurationMs = MeasureRunTime<std::chrono::milliseconds>(std::function<void(void)>([&]()
    {
        lst1.Serialize(pFile);
    }));
    fclose(pFile);

    pFile = fopen("myFile.txt", "rb");
    uint deserialDurationMs = MeasureRunTime<std::chrono::milliseconds>(std::function<void(void)>([&]()
    {
        lst2.Deserialize(pFile);
    }));
    fclose(pFile);

    cout << "Are lists equal: " << lst1.IsEqual(lst2) << endl;
    cout << "Serialization time: " << serialDurationMs << " ms\n";
    cout << "Deserialization time: " << deserialDurationMs << " ms\n";
    cout << endl << endl;
}


int main()
{
    Task1();
    Task2();

    // Task 3 run
    cout << "\n\nTask 3 started\n";
    cout << std::boolalpha;

    // Let's measure complexity of serialization and deserialization algo
    constexpr uint LIST_SIZE = 10000u;
    vector<uint>factArr = { 1u, 2u, 3u, 4u, 5u, 10u, 20u, 50u, 100u, 1000u };

    for (auto& factor : factArr)
        Task3(factor * LIST_SIZE);

    remove("myFile.txt");
    cout << "Task 3 finished\n";

    return 0;
}
