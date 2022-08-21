#include "list.h"
#include <iostream>
#include <algorithm>

// Public functions
List::List() : head(nullptr), tail(nullptr), count(0u)
{
}

List::~List()
{
    this->Clear();
}

void List::PushFront(const std::string& sData)
{
    ListNode *node = new ListNode();
    node->data = sData;
    node->rand = nullptr;

    if (count == 0)
    {
        node->next = node->prev = nullptr;
        head = tail = node;
    }
    else
    {
        head->prev = node;
        node->next = head;
        node->prev = nullptr;
        head = node;
    }

    count++;
}

void List::PushBack(const std::string& sData)
{
    ListNode *node = new ListNode();
    node->data = sData;
    node->rand = nullptr;

    if (count == 0)
    {
        node->next = node->prev = nullptr;
        head = tail = node;
    }
    else
    {
        tail->next = node;
        node->prev = tail;
        node->next = nullptr;
        tail = node;
    }

    count++;
}

void List::PopFront()
{
    if (count == 0) return;

    if (count == 1)
    {
        delete head;
        head = tail = nullptr;
    }
    else
    {
        ListNode *node = head;
        head = head->next;
        head->prev = nullptr;
        delete node;
    }

    count--;
}

void List::PopBack()
{
    if (count == 0) return;

    if (count == 1)
    {
        delete tail;
        head = tail = nullptr;
    }
    else
    {
        ListNode *node = tail;
        tail = tail->prev;
        tail->next = nullptr;
        delete node;
    }

    count--;
}

void List::Insert(const std::string& sData, uint idx)
{
    if (idx < count)
    {
        ListNode *node = GetNodeByIdx(idx);

        ListNode *newNode = new ListNode();
        newNode->data = sData;
        newNode->rand = nullptr;
        newNode->next = node;
        newNode->prev = node->prev;

        if (node->prev)
            node->prev->next = newNode;
        else
            head = newNode;
        node->prev = newNode;

        count++;
    }
    else
    {
        char buffer[64];
        sprintf(buffer, "Failed to access %i index", idx);
        throw std::out_of_range(buffer);
    }
}

void List::Remove(uint idx)
{
    if (idx < count)
    {
        ListNode *node = GetNodeByIdx(idx);

        if (node->prev)
            node->prev->next = node->next;
        else
            head = node->next;

        if (node->next)
            node->next->prev = node->prev;
        else
            tail = node->prev;

        delete node;
        count--;
    }
    else
    {
        char buffer[64];
        sprintf(buffer, "Failed to access %i index", idx);
        throw std::out_of_range(buffer);
    }
}

std::string List::operator[](uint idx) const
{
    if (idx < count)
    {
        ListNode *node = GetNodeByIdx(idx);

        return node->data;
    }
    else
    {
        char buffer[64];
        sprintf(buffer, "Failed to access %i index", idx);
        throw std::out_of_range(buffer);
    }
}

void List::PrintListContent(bool isBackward)
{
    if (count == 0) return;

    ListNode *node = (isBackward) ? tail : head;

    while (node)
    {
        std::cout << node->data << std::endl;
        node = (isBackward) ? node->prev : node->next;
    }
}

void List::Clear()
{
    if (count == 0) return;

    ListNode *node = head;

    while (node)
    {
        ListNode *nodeToRemove = node;
        node = node->next;
        delete nodeToRemove;
    }

    count = 0;
    head = tail = nullptr;
}

void List::PointNodeToNode(uint srcIdx, uint dstIdx)
{
    if ((srcIdx < count) && (dstIdx < count))
    {
        ListNode *srcNode = GetNodeByIdx(srcIdx);

        if (srcIdx == dstIdx)
            srcNode->rand = srcNode;
        else
            srcNode->rand = GetNodeByIdx(dstIdx);
    }
}

void List::PointNodesRandomly() // Special function to randomize links quickly
{
    srand(time(NULL));

    std::vector<ListNode*> addrVec;
    addrVec.reserve(count);

    uint idx = 0;
    for (ListNode *node = head; idx < count; idx++, node = node->next)
        addrVec[idx] = node;

    for (ListNode *node = head; node; node = node->next)
        node->rand = addrVec[rand() % addrVec.capacity()];
}

bool List::IsEqual(const List& other)
{
    if (this->count != other.count)
        return false;

    ListNode *thisNode = this->head, *otherNode = other.head;
    std::unordered_map<ListNode*, uint> thisMap, otherMap;

    for (uint idx = 0; idx < count; idx++)
    {
        if (thisNode->data != otherNode->data)
            return false;

        thisMap.insert(std::pair<ListNode*, uint>(thisNode, idx));
        otherMap.insert(std::pair<ListNode*, uint>(otherNode, idx));

        thisNode = thisNode->next; otherNode = otherNode->next;
    }

    thisNode = this->head, otherNode = other.head;
    for (uint idx = 0; idx < count; idx++)
    {
        if (thisNode->rand || otherNode->rand)
        {
            if (thisMap[thisNode->rand] != otherMap[otherNode->rand])
                return false;
        }

        thisNode = thisNode->next; otherNode = otherNode->next;
    }

    return true;
}

void List::Serialize(FILE * file)
{
    std::unordered_map<ListNode*, uint> idxMap;
    uint idx = 0;

    for (ListNode *node = head; node; node = node->next)
        idxMap.insert(std::pair<ListNode*, uint>(node, idx++));

    fwrite(&count, sizeof(count), 1, file);

    for (ListNode *node = head; node; node = node->next)
    {
        uint strLen = node->data.length();

        fwrite(&strLen, sizeof(uint), 1, file);
        fwrite(node->data.c_str(), sizeof(char), strLen, file);

        // Distinguish between nullptr and zero idx
        uint wrtIdx = (node->rand) ? idxMap[node->rand] + 1 : 0;
        fwrite(&wrtIdx, sizeof(uint), 1, file);
    }
}

void List::Deserialize(FILE * file)
{
    this->Clear();

    std::unordered_map<uint, uint> idxMap;
    std::vector<ListNode*> randAddr;
    uint idx;

    fread(&count, sizeof(count), 1, file);
    randAddr.reserve(count);

    for (idx = 0; idx < count; idx++)
    {
        std::string nodeValue;
        uint strLen, randIdx;

        fread(&strLen, sizeof(uint), 1, file);

        nodeValue.resize(strLen);
        fread(&nodeValue[0], sizeof(char), strLen, file);

        fread(&randIdx, sizeof(uint), 1, file);
        idxMap.insert(std::pair<uint, uint>(idx, randIdx));

        ListNode *node = new ListNode();
        node->data = nodeValue;
        node->rand = nullptr;

        if (idx == 0)
        {
            node->next = node->prev = nullptr;
            head = tail = node;
        }
        else
        {
            tail->next = node;
            node->prev = tail;
            node->next = nullptr;
            tail = node;
        }

        randAddr[idx] = node;
    }

    idx = 0;
    for (ListNode *node = head; node; node = node->next, idx++)
    {
        node->rand = (idxMap[idx]) ? randAddr[idxMap[idx] - 1] : nullptr;
    }
}

// Private functions
ListNode* List::GetNodeByIdx(uint idx) const
{
    ListNode *node = head;
    for (uint i = 0u; i < idx; i++, node = node->next);

    return node;
}
