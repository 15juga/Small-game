#include "CollisionTest.h"

CollisionTesting::CollisionTesting()
{
}

CollisionTesting::~CollisionTesting()
{
}



bool CollisionTesting::SphereSphere(BoundingSphere sph1, BoundingSphere sph2)
{
    return sph1.Intersects(sph2);
}

bool CollisionTesting::SphereAABB(BoundingSphere sph, BoundingBox box)
{
    return sph.Intersects(box);
}

bool CollisionTesting::SphereOBB(BoundingSphere sph, BoundingOrientedBox box)
{
    return sph.Intersects(box);
}

bool CollisionTesting::BoxInBox(BoundingBox box1, BoundingBox box2)
{
    return box1.Intersects(box2);
}

bool CollisionTesting::SphereInFrustum(BoundingSphere sph, BoundingFrustum frustum)
{
    if (sph.ContainedBy(plane, plane1, plane2, plane3, plane4, plane5) == (ContainmentType::CONTAINS | ContainmentType::INTERSECTS))
    {
        return true;
    }
    else
        return false;

}

bool CollisionTesting::AABBInFrustum(BoundingBox box, BoundingFrustum frustum)
{
    return false;
}

bool CollisionTesting::OBBInFrustum(BoundingOrientedBox box, BoundingFrustum frustum)
{
    return false;
}
