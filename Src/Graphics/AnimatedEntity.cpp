//---------------------------------------------------------------------------
// AnimatedEntity.cpp
//---------------------------------------------------------------------------

/**
@file AnimatedEntity.cpp

Contiene la implementación de la clase que representa una entidad gráfica 
con animaciones.

@see Graphics::CAnimatedEntity
@see Graphics::CEntity

@author Rubén Mulero
@date June, 2013
*/

#include "AnimatedEntity.h"
#include "Bone.h"

#include <assert.h>
#include "Server.h"
#include "Scene.h"
#include "Entity.h"
#include <OgreEntity.h>
#include <OgreAnimationState.h>
#include <OgreSceneManager.h>
#include <OgreSubEntity.h>
#include <OgreSkeleton.h>

namespace Graphics 
{
		
	bool CAnimatedEntity::setAnimation(const std::string &anim, bool loop, float fadeTime)
	{
		if(!_entity->getAllAnimationStates()->hasAnimationState(anim))
			return false;

		//comprobamos si la animación ya estaba ejecutandose
		auto runningAnim = _runningAnimations.find(anim);

		if(runningAnim != _runningAnimations.end()){
			if(runningAnim->second.state == FADE_OUT){
				runningAnim->second.state = FADE_IN;

				//comprobamos que la animación no estaba justo para 
				//ser sacada
				if(!_deletedAnims.empty()){
					
					auto delAnim = _deletedAnims.begin();
					auto delEnd = _deletedAnims.end();

					for(;delAnim!=delEnd;delAnim++){
						if ((*delAnim) == runningAnim->second.animation->getAnimationName()){
							_deletedAnims.erase(delAnim);
							runningAnim->second.animation->setEnabled(true);
							break;
						}
					}
					
				}
				
			}
			
			return true;
		}

		//cogemos la animacion y la preparamos para ejecutarla
		Ogre::AnimationState* animstate = _entity->getAnimationState(anim);
		animstate->setEnabled(true);
		animstate->setLoop(loop);
		animstate->setWeight(0);

		//seteamos la animación nueva para que haga fade-in, teniendo
		//en cuenta el tiempo de fade que se le ha pasado
		Animation animation;
		animation.animation = animstate;
		animation.state = FADE_IN;
		animation.fadeTime = fadeTime;

		//metemos la animacion en la lista de animaciones ejecutandose
		TAnim newAnim (anim,animation);
		_runningAnimations.insert(newAnim);

		return true;

	} // setAnimation

	//--------------------------------------------------------
		
	bool CAnimatedEntity::stopAnimation(const std::string &anim)
	{
		auto myAnim = _runningAnimations.find(anim);

		if(myAnim == _runningAnimations.end())
			return false;

		if(myAnim->second.animation->hasEnded()){
			myAnim->second.animation->setEnabled(false);
			myAnim->second.animation->setTimePosition(0);
			_runningAnimations.erase(myAnim);
			return true;
		}

		myAnim->second.state = FADE_OUT;

		return true;
	} // stopAnimation

	//--------------------------------------------------------
		
	void CAnimatedEntity::stopAllAnimations()
	{
		if(_runningAnimations.empty())
			return;

		auto anim = _runningAnimations.begin();
		auto end = _runningAnimations.end();

		while(anim!=end){

			if(anim->second.animation->hasEnded()){
				anim->second.animation->setEnabled(false);
				anim->second.animation->setTimePosition(0);
				auto aux = anim;
				aux++;
				_runningAnimations.erase(anim);
				anim = aux;
				continue;
			}

			anim->second.state = FADE_OUT;
			++anim;
		}
	} // stopAllAnimations

	//--------------------------------------------------------
		
	void CAnimatedEntity::tick(float secs)
	{
		/*
		Ogre::Skeleton * skeleton = _entity->getSkeleton();
		Ogre::Bone * bone = skeleton->getBone("Bip01 R Hand");
		bone->setOrientation(_entityNode->getOrientation());
		*/

		//primero hacemos un delete deferred de las animaciones
		//que han terminado
		if(!_deletedAnims.empty()){
			auto delAnim = _deletedAnims.begin();
			auto delEnd = _deletedAnims.end();

			for(;delAnim!=delEnd;delAnim++){
				_runningAnimations.erase(*delAnim);
			}
			_deletedAnims.clear();
		}
		
		//si no hay animaciones, no hacemos nada
		if(_runningAnimations.empty())
			return;

		auto anim = _runningAnimations.begin();
		auto end = _runningAnimations.end();

		// recorremos todas las animaciones que se están ejectuando, y
		// les añadimos tiempo para que avancen
		for(;anim!=end;++anim){
			AnimState state = anim->second.state;

			switch(state){
			case FADE_IN:{
				float weight = anim->second.animation->getWeight();
				weight+=secs/anim->second.fadeTime;
				if( weight > 1.0f ){
					weight = 1.0f;
					anim->second.state = RUNNING;
				}
				anim->second.animation->setWeight(weight);
				anim->second.animation->addTime(secs);
				break;
			}
			case FADE_OUT:{
				float weight = anim->second.animation->getWeight();
				weight-=secs/anim->second.fadeTime;
				if( weight < 0.0f ){
					weight = 0.0f;
					_deletedAnims.push_back(anim->first);
					anim->second.animation->setEnabled(false);
					anim->second.animation->setWeight(0);
					anim->second.animation->setTimePosition(0);
				}else{
					anim->second.animation->setWeight(weight);
					anim->second.animation->addTime(secs);
				}
				break;
			}
			case RUNNING:
				anim->second.animation->addTime(secs);
				break;
			}//switch

			// si la animacion ha terminado, avisamos a los observadores 
			// por si tienen alguna tarea pendiente
			if(!_observers.empty() && anim->second.animation->hasEnded()){
				auto obs = _observers.begin();
				auto obsend = _observers.end();

				for(;obs!=obsend;++obs)
					(*obs)->animationFinished(anim->second.animation->getAnimationName());
			}
		}
	} // tick
	//--------------------------------------------------------

	void CAnimatedEntity::attachWeapon(CEntity &arma, unsigned int id){
		
		if(_weapon)
			_scene->getSceneMgr()->destroyEntity(_weapon);
		_weapon = _scene->getSceneMgr()->createEntity(arma.getMesh());
		
		_entity->attachObjectToBone("weapon_bone_2",_weapon);

	}

	std::string CAnimatedEntity::getWeaponMaterial(){
		return _weapon->getSubEntity(0)->getMaterialName();
	}

	void CAnimatedEntity::changeMaterialToWeapon(const std::string& materialName){
		_weapon->setMaterialName(materialName);
		//_weapon->getSubEntity(0)->getMaterialName();
		
	}

	void CAnimatedEntity::freeBoneOrientation(const std::string &bone){
		_entity->getSkeleton()->getBone(bone)->setManuallyControlled(true);
	}

	void CAnimatedEntity::lockBoneOrientation(const std::string &bone){
		_entity->getSkeleton()->getBone(bone)->setManuallyControlled(false);
	}

	void CAnimatedEntity::moveBone(const std::string &bone, float pitch){
		Ogre::Bone * entityBone = _entity->getSkeleton()->getBone(bone);
		entityBone->reset();

		Ogre::Skeleton * skel = _entity->getSkeleton();

		//entityBone->setManuallyControlled(true);

		unsigned short boneHandle = entityBone->getHandle();
		Ogre::AnimationStateIterator animStateIt = _entity->getAllAnimationStates()->getAnimationStateIterator();

		while( animStateIt.hasMoreElements() )
		{
			Ogre::AnimationState *pAnimState = animStateIt.getNext();
			// ignore disabled animations
			skel->getAnimation(pAnimState->getAnimationName())->destroyNodeTrack(boneHandle);
		}

		entityBone->pitch(Ogre::Radian(pitch));
	}

	bool CAnimatedEntity::load() {
		bool success = CEntity::load();
		_skeleton = _entity->getSkeleton();

		return success;
	}

	Quaternion CAnimatedEntity::getBoneOrientation(const std::string& boneName) {
		Ogre::Quaternion parentQuat = _entityNode->_getDerivedOrientation();
		Ogre::Quaternion boneQuat = _skeleton->getBone(boneName)->_getDerivedOrientation();

		return parentQuat * boneQuat;
	}

	Vector3 CAnimatedEntity::getBonePosition(const std::string& boneName) {
		Ogre::Quaternion parentQuat = _entityNode->_getDerivedOrientation();
		Ogre::Vector3 parentPos = _entityNode->_getDerivedPosition();
		Ogre::Vector3 bonePos = _skeleton->getBone(boneName)->_getDerivedPosition();

		return parentQuat * bonePos + parentPos;
	}

	void CAnimatedEntity::getBonePose(const std::string& boneName, Vector3& position, Quaternion& orientation) {
		Ogre::Bone* bone = _skeleton->getBone(boneName);

		Ogre::Quaternion parentQuat = _entityNode->_getDerivedOrientation();
		Ogre::Vector3 parentPos = _entityNode->_getDerivedPosition();
		Ogre::Quaternion boneQuat = bone->_getDerivedOrientation();
		Ogre::Vector3 bonePos = bone->_getDerivedPosition();

		orientation = parentQuat * boneQuat;
		position = parentQuat * bonePos + parentPos;
	}

	void CAnimatedEntity::setBoneOrientation(const std::string& boneName, const Quaternion& orientation) {
		Ogre::Quaternion parentQuat = _entityNode->_getDerivedOrientation(); // node local ori
		Ogre::Bone* bone = _skeleton->getBone(boneName);
		bone->setManuallyControlled(true);
		Ogre::Quaternion worldQuat(orientation); // desired orientation in world terms
		Ogre::Quaternion boneQuat = worldQuat.Inverse() * parentQuat; // equiv to ("boneQuat = worldQuat / parentQuat")
		bone->_setDerivedOrientation(boneQuat);
	}

	void CAnimatedEntity::setBonePosition(const std::string& boneName, const Vector3& position) {
		Ogre::Vector3 parentPos = _entityNode->_getDerivedPosition(); // node local pos
		Ogre::Vector3 parentQuatXbonePos = position - parentPos;
		Ogre::Quaternion parentQuat = _entityNode->_getDerivedOrientation(); // node local ori
		Ogre::Vector3 bonePos = parentQuat.Inverse() * parentQuatXbonePos;
		Ogre::Vector3 inverseParentScale = 1.0 / _entityNode->_getDerivedScale();
		Ogre::Bone* bone = _skeleton->getBone(boneName);
		bone->setManuallyControlled(true);
		bone->_setDerivedPosition(bonePos * inverseParentScale);
	}

	void CAnimatedEntity::setBonePose(const std::string& boneName, const Vector3& position, const Quaternion& orientation) {
		Ogre::Bone* bone = _skeleton->getBone(boneName);
		bone->setManuallyControlled(true);
		bone->_setDerivedPosition(_entityNode->_getDerivedOrientation().Inverse() * (position - _entityNode->_getDerivedPosition()));
	}

	Graphics::CBone CAnimatedEntity::getBone(const std::string& boneName) const {
		return Graphics::CBone( _entityNode, _skeleton->getBone(boneName) );
	}

} // namespace Graphics
