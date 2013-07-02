#ifndef __Graphics_Bone_H
#define __Graphics_Bone_H

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

			worldOrientation = parentQuat * boneQuat;
			worldPosition = parentQuat * bonePos + parentPos;
		}

		void setGlobalPose(const Vector3& worldPosition, Quaternion& worldOrientation) {
			_bone->setManuallyControlled(true);
			_bone->_setDerivedPosition(_parentNode->_getDerivedOrientation().Inverse() * (worldPosition - _parentNode->_getDerivedPosition()));
		}

	private:
		Ogre::SceneNode* _parentNode;
		Ogre::Bone* _bone;
	};

}

#endif