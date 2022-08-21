#ifndef LIST_H
#define LIST_H

#include <string>

using uint = unsigned int;

/* Task 3:
 * Реализуйте функции сериализации и десериализации двусвязного списка в бинарном формате в файл.
 * Алгоритмическая сложность решения должна быть меньше квадратичной.
*/

// структуру ListNode модифицировать нельзя
struct ListNode {
    ListNode * prev;
    ListNode * next;
    ListNode * rand; // указатель на произвольный элемент данного списка, либо NULL
    std::string data;
};

class List {
public:
    List();
    ~List();
    List(const List& copy) = delete;
    List(const List&& copy) = delete;
    List& operator=(const List& op) = delete;
    List& operator=(const List&& op) = delete;

    void PushFront(const std::string& sData);
    void PushBack(const std::string& sData);
    void PopFront();
    void PopBack();
    void Insert(const std::string& sData, uint idx);
    void Remove(uint idx);

    std::string operator[](uint idx) const;

    void PrintListContent(bool isBackward = false);
    void Clear();
    int Count() const { return count; }
    void PointNodeToNode(uint srcIdx, uint dstIdx);
    void PointNodesRandomly();
    bool IsEqual(const List& other);

    void Serialize(FILE * file); // сохранение в файл (файл открыт с помощью fopen(path, "wb"))
    void Deserialize(FILE * file); // загрузка из файла (файл открыт с помощью fopen(path, "rb"))

protected:
    ListNode* GetNodeByIdx(uint idx) const;

    ListNode * head;
    ListNode * tail;
    uint count;
};


#endif // LIST_H
