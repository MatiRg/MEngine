#pragma once

// Use Public Inheritance
// Do not use as polymorphic type 
// https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-copyable_Mixin
class NonCopyable
{
private:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
protected:
    NonCopyable() = default;
    virtual ~NonCopyable() = default;
};

class NonMovable
{
private:
    NonMovable(NonMovable&&) = delete;
    NonMovable& operator=(NonMovable&&) = delete;
protected:
    NonMovable() = default;
    virtual ~NonMovable() = default;
};

// Use Public Inheritance
// https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-copyable_Mixin
class NonCopyableMovable
{
private:
    NonCopyableMovable(NonCopyableMovable&&) = delete;
    NonCopyableMovable(const NonCopyableMovable&) = delete;
    NonCopyableMovable& operator=(NonCopyableMovable&&) = delete;
    NonCopyableMovable& operator=(const NonCopyableMovable&) = delete;
protected:
    NonCopyableMovable() = default;
    virtual ~NonCopyableMovable() = default;
};