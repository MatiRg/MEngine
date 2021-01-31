#pragma once
#include "../../Math/Vector3.hpp"
#include "../../Math/Quaternion.hpp"
#include <Bullet/btBulletDynamicsCommon.h>

inline btVector3 ToBulletVector3(const Vector3& Val)
{
    return btVector3( Val.x, Val.y, Val.z );
}

inline Vector3 ToVector3(const btVector3& Val)
{
    return Vector3( Val.x(), Val.y(), Val.z() );
}

inline btQuaternion ToBulletQuaternion(const Quaternion& Val)
{
    return btQuaternion(Val.x, Val.y, Val.z, Val.w);
}

inline Quaternion ToQuaternion(const btQuaternion& Val)
{
    return Quaternion(Val.x(), Val.y(), Val.z(), Val.w());
}