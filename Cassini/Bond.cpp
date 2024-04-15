#include "Bond.h"

const string Bond::entityID = "Bond";
const string Bond::GetEntityID() { return entityID; }

Bond::Bond(Graphics& gfx, ResourceManager& manager, const string& systemID, Particle* bondingPair[2], UINT indexA, UINT indexB)
	:Cylinder(gfx, manager, systemID, entityID)
{
	pair[0] = bondingPair[0];
	pair[1] = bondingPair[1];
	indices[0] = indexA;
	indices[1] = indexB;
	XMFLOAT3 posA = bondingPair[0]->GetPosition();
	XMFLOAT3 posB = bondingPair[1]->GetPosition();
	length = XMVectorGetX(XMVector3Length(XMLoadFloat3(&posA) - XMLoadFloat3(&posB)));
}

void Bond::Update(Graphics& gfx, ResourceManager& manager)
{

}

UINT Bond::GetIndexA() const
{
	return indices[0];
}
UINT Bond::GetIndexB() const
{
	return indices[1];
}

float Bond::GetEnergy() const
{
	return energy;
}
float Bond::GetLength() const
{
	return length;
}

Particle* Bond::GetParticle(UINT index) const
{
	return pair[index];
}

XMFLOAT3 Bond::GetStretch() const
{
	const auto& ptcla = GetParticle(0);
	const auto& ptclb = GetParticle(1);
	XMFLOAT3 posA = ptcla->GetPosition();
	XMFLOAT3 posB = ptclb->GetPosition();
	float dist = XMVectorGetX(XMVector3Length(XMLoadFloat3(&posB) - XMLoadFloat3(&posA)));
	float x = dist / length;
	return { 60.0f, 60.0f, dist / length * length };
}

XMMATRIX Bond::GetUpdatedTransform() const
{
	XMFLOAT3 bondPos = GetPosition();
	XMFLOAT3 stretch = GetStretch();
	const auto& ptcla = GetParticle(0);
	const auto& ptclb = GetParticle(1);
	XMFLOAT3 posA = ptcla->GetPosition();
	XMFLOAT3 posB = ptclb->GetPosition();
	XMVECTOR orig = { 0.0f, 0.0f, -1.0f };
	XMVECTOR dir = XMVector3Normalize(XMLoadFloat3(&posB) - XMLoadFloat3(&posA));
	float angle = XMVectorGetX(XMVector3AngleBetweenVectors(orig, dir));
	XMVECTOR axis = XMVector3Cross(orig, dir);
	XMMatrixRotationAxis(axis, angle);
	return
		XMMatrixScaling(stretch.x, stretch.y, stretch.z) *
		XMMatrixRotationAxis(axis, angle) *
		XMMatrixTranslation(posA.x, posA.y, posA.z);
}

XMFLOAT3 Bond::GetMove() const
{
	XMFLOAT3 midpoint;
	const auto& ptcla = GetParticle(0);
	const auto& ptclb = GetParticle(1);
	XMFLOAT3 posA = ptcla->GetPosition();
	XMFLOAT3 posB = ptclb->GetPosition();
	XMStoreFloat3(&midpoint, (XMLoadFloat3(&posB) + XMLoadFloat3(&posA)) / 2);
	return midpoint;
}
