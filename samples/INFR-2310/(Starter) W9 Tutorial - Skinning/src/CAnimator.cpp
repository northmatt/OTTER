/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CAnimator.cpp
Simple animator component for demonstrating skeletal animation.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CAnimator.h"
#include "CSkinnedMeshRenderer.h"

namespace nou
{
	CAnimator::CAnimator(Entity& owner, const SkeletalAnim& anim)
	{
		m_owner = &owner;

		CSkinnedMeshRenderer& rend = m_owner->Get<CSkinnedMeshRenderer>();
		m_clip = std::make_unique<SkeletalAnimClip>(anim, rend.GetSkeleton());
	}

	SkeletalAnimClip* CAnimator::GetClip()
	{
		return m_clip.get();
	}

	void CAnimator::Update(float deltaTime)
	{
		CSkinnedMeshRenderer& rend = m_owner->Get<CSkinnedMeshRenderer>();
		Skeleton& skeleton = rend.GetSkeleton();

		m_clip->Update(deltaTime, skeleton);
		m_clip->Apply(skeleton);
		skeleton.DoFK();
		rend.UpdateJointMatrices();
	}
	void CAnimator::resetDuration() {
		m_clip->resetDuration();
	}
	bool CAnimator::GetIsPlaying() {
		return m_clip->GetIsPlaying();
	}
	void CAnimator::SetIsPlaying(bool playing) {
		m_clip->SetIsPlaying(playing);
	}
	bool CAnimator::GetIsLooping() {
		return m_clip->GetIsLooping();
	}
	void CAnimator::SetIsLooping(bool looping) {
		m_clip->SetIsLooping(looping);
	}
	void CAnimator::SetSpeedMultiplier(float speed) {
		m_clip->SetSpeedMultiplier(speed);
	}
}