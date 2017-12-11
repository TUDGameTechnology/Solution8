#pragma once

#include "pch.h"

using namespace Kore;


// A plane is defined as the plane's normal and the distance of the plane to the origin
class PlaneCollider {
public:
	float d;
	vec3 normal;
};

// A sphere is defined by a radius and a center.
class SphereCollider {
public:
	vec3 center;
	float radius;

	/************************************************************************/
	/* Exercise 7 1.4                                                       */
	/************************************************************************/
	/* Implement the collision functions below */

	// Return true iff there is an intersection with the other sphere
	bool IntersectsWith(const SphereCollider& other) {
		return PenetrationDepth(other) > 0.0f;
	}

	// Collision normal is the normal vector pointing towards the other sphere
	vec3 GetCollisionNormal(const SphereCollider& other) {
		vec3 n = other.center - center;
		n = n.normalize();
		return n;
	}

	// The penetration depth
	float PenetrationDepth(const SphereCollider& other) {
		return other.radius + radius - (other.center - center).getLength();
	}


	bool IntersectsWith(const PlaneCollider& other) {
		return other.normal.dot(center) + other.d <= radius;
	}

	vec3 GetCollisionNormal(const PlaneCollider& other) {
		return other.normal;
	}

	float PenetrationDepth(const PlaneCollider &other) {
		return other.normal.dot(center) + other.d - radius;
	}


};
