#pragma once

enum class EPrimitiveType
{
    Triangles = 0,
    Lines,
    Points
};

enum class EBlendMode
{
    None,
    Alpha,
    Add,
    Mod
};

enum class EPolygonMode
{
    Point,
    Line,
    Fill
};

enum class EDepthMode
{
    Never,
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual,
    Always
};

enum class ECullMode
{
    Front,
    Back,
    FrontBack
};

enum class EFrontFace
{
    CW, // Clockwise
    CCW // Counterclockwise
};
