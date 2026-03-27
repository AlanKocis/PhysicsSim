#ifndef CONTACT_RESOLVER_H
#define CONTACT_RESOLVER_H

#include <h/CollisionManager.h>
#include <h/RigidBody.h>

void ResolveContact(RigidBody& A, RigidBody& B, const Contact& c);

#endif