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

		//________________________________________________________________________

		~CBone() {
			// Nada que hacer
		}

		//________________________________________________________________________

		std::string getName() {
			return _bone->getName();
		}

		//________________________________________________________________________

		void setManuallyControlled(bool setToManual) {
			_bone->setManuallyControlled(setToManual);
		}

		//________________________________________________________________________

		bool isManuallyControlled() {
			return _bone->isManuallyControlled();
		}

		//________________________________________________________________________

		void getGlobaPose(Vector3& worldPosition, Quaternion& worldOrientation) {
			Quaternion parentQuat = _parentNode->_getDerivedOrientation();
			Vector3 parentPos = _parentNode->_getDerivedPosition();
			Quaternion boneQuat = _bone->_getDerivedOrientation();
			Vector3 bonePos = _bone->_getDerivedPosition();

			worldOrientation = parentQuat * boneQuat;
			worldPosition = parentQuat * bonePos + parentPos;
		}

		//________________________________________________________________________

		void setGlobalPose(const Vector3& worldPosition, Quaternion& worldOrientation) {
			_bone->setManuallyControlled(true);

			_bone->_setDerivedOrientation( _parentNode->_getDerivedOrientation().Inverse() * worldOrientation );
			_bone->_setDerivedPosition(  _parentNode->_getDerivedOrientation().Inverse() * (worldPosition - _parentNode->_getDerivedPosition()) );
		}

		//________________________________________________________________________

		void yaw(float radians) {
			_bone->yaw( Ogre::Radian(radians) );
		}

		//________________________________________________________________________

		void pitch(float radians) {
			_bone->pitch( Ogre::Radian(radians) );
		}

		//________________________________________________________________________

		void roll(float radians) {
			_bone->roll( Ogre::Radian(radians) );
		}

	private:
		Ogre::SceneNode* _parentNode;
		Ogre::Bone* _bone;
	};

}

#endif