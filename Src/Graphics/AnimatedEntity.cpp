//---------------------------------------------------------------------------
// AnimatedEntity.cpp
//---------------------------------------------------------------------------

/**
@file AnimatedEntity.cpp

Contiene la implementación de la clase que representa una entidad gráfica 
con animaciones.

@see Graphics::CAnimatedEntity
@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#include "AnimatedEntity.h"

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
		
	bool CAnimatedEntity::setAnimation(const std::string &anim, bool loop)
	{
		if(!_entity->getAllAnimationStates()->hasAnimationState(anim))
			return false;

		Ogre::AnimationState* animstate = _entity->getAnimationState(anim);
		TAnim newAnim (anim,animstate);
		animstate->setEnabled(true);
		animstate->setLoop(loop);

		_runningAnimations.insert(newAnim);
		return true;

	} // setAnimation

	//--------------------------------------------------------
		
	bool CAnimatedEntity::stopAnimation(const std::string &anim)
	{
		auto myAnim = _runningAnimations.find(anim);

		if(myAnim == _runningAnimations.end())
			return false;

		myAnim->second->setEnabled(false);
		myAnim->second->setTimePosition(0);

		_runningAnimations.erase(myAnim);

		return true;
	} // stopAnimation

	//--------------------------------------------------------
		
	void CAnimatedEntity::stopAllAnimations()
	{
		if(_runningAnimations.empty())
			return;

		auto anim = _runningAnimations.begin();
		auto end = _runningAnimations.end();

		for(;anim!=end;++anim){
			anim->second->setEnabled(false);
			anim->second->setTimePosition(0);
		}
		_runningAnimations.clear();
	} // stopAllAnimations

	//--------------------------------------------------------
		
	void CAnimatedEntity::tick(float secs)
	{
		/*
		Ogre::Skeleton * skeleton = _entity->getSkeleton();
		Ogre::Bone * bone = skeleton->getBone("Bip01 R Hand");
		bone->setOrientation(_entityNode->getOrientation());
		*/
		
		//si no hay animaciones, no hacemos nada
		if(_runningAnimations.empty())
			return;

		auto anim = _runningAnimations.begin();
		auto end = _runningAnimations.end();

		// recorremos todas las animaciones que se están ejectuando, y
		// les añadimos tiempo para que avancen
		for(;anim!=end;++anim){
			anim->second->addTime(secs);
			// si la animacion ha terminado, avisamos a los observadores 
			// por si tienen alguna tarea pendiente
			if(!_observers.empty() && anim->second->hasEnded()){
				auto obs = _observers.begin();
				auto obsend = _observers.end();

				for(;obs!=obsend;++obs)
					(*obs)->animationFinished(anim->second->getAnimationName());
			}
		}
	} // tick
	//--------------------------------------------------------

	void CAnimatedEntity::attachWeapon(CEntity &arma, unsigned int id){
		
		if(_weapon)
			_scene->getSceneMgr()->destroyEntity(_weapon);
		_weapon = _scene->getSceneMgr()->createEntity(arma.getMesh());
		
		_entity->attachObjectToBone("weapon_bone_2",_weapon);

		_entity->getNumSubEntities();

		std::cout << "material name del mierda heavy! " << _entity->getSubEntity(0)->getMaterialName() << std::endl;
	}

	std::string CAnimatedEntity::getWeaponMaterial(){
		return _weapon->getSubEntity(0)->getMaterialName();
	}

	void CAnimatedEntity::changeMaterialToWeapon(const std::string& materialName){
		_weapon->setMaterialName(materialName);
		//_weapon->getSubEntity(0)->getMaterialName();
		
	}


} // namespace Graphics
