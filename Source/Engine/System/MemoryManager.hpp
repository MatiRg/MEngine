#pragma once
#include "../Core/NonCopyable.hpp"
#include <vector>
#include <string>

// For debug purposes
class CMemoryManager final: public NonCopyableMovable
{
    struct SRecord
    {
        void* Ptr = nullptr;
        std::string File;
        int Line = 0;

        SRecord(void* aPtr, const std::string& aFile, const int aLine):
            Ptr(aPtr),
            File(aFile),
            Line(aLine)
        {
        }
    };
public:
    ~CMemoryManager();

    static CMemoryManager& GetInstance();

    // Ptr, File, Line
    void AddRecord(void*, const std::string&, const int);
    // Ptr
    void EraseRecord(void*);

    void Report();
private:
    CMemoryManager();
private:
    std::vector<SRecord> Records;
};

inline CMemoryManager& GetMemoryManager()
{
    return CMemoryManager::GetInstance();
}

#define ADD_MEMORY_RECORD(Ptr) GetMemoryManager().AddRecord( Ptr , __FILE__, __LINE__ )

#define ERASE_MEMORY_RECORD(Ptr) GetMemoryManager().EraseRecord( Ptr )