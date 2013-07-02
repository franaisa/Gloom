#ifndef __Graphics_Bone_H
#define __Graphics_Bone_H

#include <Basesubsystems/math.h>
#include <OgreBone.h>
#include <OgreSceneNode.h>

/**

@author Francisco Aisa García.
*/

namespace Graphics {
	
	//typedef Ogre::Bone CBone;

	class CBone {
	public:
		CBone(Ogre::SceneNode* parentNode, Ogre::Bone* ogreBone) : _parentNode(parentNode), _bone(ogreBone) {
			// Nada que hacer
		}

		~CBone() {
			// Nada que hacer
		}

		void setManuallyControlled(bool setToManual) {
			_bone->setManuallyControlled(setToManual);
		}

		bool isManuallyControlled() {
			return _bone->isManuallyControlled();
		}

		void getGlobaPose(Vector3& worldPosition, Quaternion& worldOrientation) {
			Ogre::Quaternion parentQuat = _parentNode->_getDerivedOrientation();
			Ogre::Vector3 parentPos = _parentNode->_getDerivedPosition();
			Ogre::Quaternion boneQuat = _bone->_getDerivedOrientation();
			Ogre::Vector3 bonePos = _bone->_getDerivedPosition();

			worldOrientation = boneQuat * parentQuat;
			worldPosition = parentQuat * bonePos + parentPos;
		}

		void setGlobalPose(const Vector3& worldPosition, Quaternion& worldOrientation) {
			_bone->setManuallyControlled(true);
			_bone->_setDerivedPosition(_parentNode->_getDerivedOrientation().Inverse() * (worldPosition - _parentNode->_getDerivedPosition()));

			/*Ogre::Vector3 parentPos = _parentNode->_getDerivedPosition(); // node local pos
			Ogre::Vector3 parentQuatXbonePos = worldPosition - parentPos;
			Ogre::Quaternion parentQuat = _parentNode->_getDerivedOrientation(); // node local ori
			Ogre::Vector3 bonePos = parentQuat.Inverse() * parentQuatXbonePos;
			Ogre::Vector3 inverseParentScale = 1.0 / _parentNode->_getDerivedScale();
			_bone->setManuallyControlled(true);
			_bone->_setDerivedPosition(bonePos * inverseParentScale);
			Ogre::Quaternion worldQuat(worldOrientation); // desired orientation in world terms
			Ogre::Quaternion boneQuat = worldQuat.Inverse() * parentQuat; // equiv to ("boneQuat = worldQuat / parentQuat")
			_bone->_setDerivedOrientation(boneQuat);*/
		}

	private:
		Ogre::SceneNode* _parentNode;
		Ogre::Bone* _bone;
	};

}

#endif