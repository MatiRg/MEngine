#pragma once

struct EBeginFrame
{
};

struct EUpdate
{
    EUpdate(const float aDT):
        DT(aDT)
    {
    }

    float DT;
};

struct ELateUpdate
{
    ELateUpdate(const float aDT):
        DT(aDT)
    {
    }

    float DT;
};

struct EGUI
{
};

struct ERender
{
};

struct EEndFrame
{
};