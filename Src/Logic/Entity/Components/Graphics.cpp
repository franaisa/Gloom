/**
@file Graphics.cpp

Contiene la implementación del componente que controla la representación
gráfica de la entidad.
 
@see Logic::CGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "Graphics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/StaticEntity.h"

#include "Logic/Messages/MessageActivate.h"
#include "Logic/Messages/MessageChangeMaterial.h"
#include "Logic/Messages/MessageDecal.h"


#include "Graphics/OgreDecal.h"

namespace Logic 
{
	IMP_FACTORY(CGraphics);
	
	//---------------------------------------------------------

	CGraphics::~CGraphics() 
	{
		if(_graphicsEntity)
		{
			_scene->removeEntity(_graphicsEntity);
			_graphicsEntity = 0;
		}

	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CGraphics::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		_scene = _entity->getMap()->getScene();

		if(entityInfo->hasAttribute("model"))
			_model = entityInfo->getStringAttribute("model");

		
		_graphicsEntity = createGraphicsEntity(entityInfo);
		if(!_graphicsEntity)
			return false;

		if(entityInfo->hasAttribute("scale")){

			//con el escale puedo escalar lo que quiera cuanto quiera. El mapa ya sale al tamaño que queramos ^^
			_graphicsEntity->setScale(entityInfo->getVector3Attribute("scale"));
			// con esto puedo girar lo que quiera cuanto quiera. Lo he probado con el mapa
		}
		
		//¿queremos material custom?
		if(entityInfo->hasAttribute("materialName") && _model == "heavy.mesh"){
			//material type
			std::string materialName = entityInfo->getStringAttribute("materialName");
			_material.push_back(materialName+"HeadBlue");
			_material.push_back(materialName+"EyeLBlue");
			_material.push_back(materialName+"EyeRBlue");
			_material.push_back(materialName+"BodyBlue");
			_material.push_back(materialName+"HandsBlue");

			_graphicsEntity->changeMaterial(_material);
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	Graphics::CEntity* CGraphics::createGraphicsEntity(const Map::CEntity *entityInfo) {
		bool isStatic = false;
		if(entityInfo->hasAttribute("static"))
			isStatic = entityInfo->getBoolAttribute("static");

		if(isStatic) {
			_graphicsEntity = new Graphics::CStaticEntity(_entity->getName(),_model);
			if(!_scene->addStaticEntity((Graphics::CStaticEntity*)_graphicsEntity))
				return 0;
		}
		else {
			_graphicsEntity = new Graphics::CEntity(_entity->getName(),_model);
			if(!_scene->addEntity(_graphicsEntity))
				return 0;
		}

		
		_graphicsEntity->setTransform(_entity->getPosition(),_entity->getOrientation());
		
		return _graphicsEntity;

	} // createGraphicsEntity

	//---------------------------------------------------------
	
	void CGraphics::setTransform(const Vector3 &position, const Ogre::Quaternion &orientation) {
		_graphicsEntity->setTransform(position,orientation);
	}

	//---------------------------------------------------------

	bool CGraphics::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::ACTIVATE			|| 
			   msgType == Message::CHANGE_MATERIAL	||
			   msgType == Message::DECAL			;
	} // accept
	
	//---------------------------------------------------------

	void CGraphics::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::ACTIVATE: {
				setVisible(std::static_pointer_cast<CMessageActivate>(message)->getActivated());
				break;
			}
			case Message::CHANGE_MATERIAL: {
				std::shared_ptr<CMessageChangeMaterial> chgMatMsg = std::static_pointer_cast<CMessageChangeMaterial>(message);
				changeMaterial( chgMatMsg->getMaterialName() );
				break;
			}
			case Message::DECAL: {
				std::cout << "RECIBIDOOOOOO" << std::endl;
				std::shared_ptr<CMessageDecal> msgDecal = std::static_pointer_cast<CMessageDecal>(message);
				drawDecal(msgDecal->getPosition(), msgDecal->getTexture());
				break;
			}
		}

	} // process

	//---------------------------------------------------------

	void CGraphics::setVisible(bool renderGraphicEntity) {
		_graphicsEntity->setVisible(renderGraphicEntity);
	}

	//---------------------------------------------------------

	void CGraphics::changeMaterial(const std::string& materialName) {
		if(materialName != "original")
			_graphicsEntity->changeMaterial(_material);
		else
			_graphicsEntity->changeMaterial(materialName);
	}

	//---------------------------------------------------------

	void CGraphics::changeMaterial(const std::list<std::string>& materialList) {
		_graphicsEntity->changeMaterial(materialList);
	}

	//---------------------------------------------------------

	void CGraphics::onActivate() {
		setVisible(true);
		_graphicsEntity->setTransform(_entity->getPosition(),_entity->getOrientation());

	}//---------------------------------------------------------
	//onActivate


	void CGraphics::onTick(unsigned int msecs){
		_graphicsEntity->setTransform(_entity->getPosition(),_entity->getOrientation());
	}//---------------------------------------------------------
	//onTick


	void CGraphics::changeScale(float newScale){
		_graphicsEntity->setScale(newScale);		
	}//---------------------------------------------------------
	//changeScale

	void CGraphics::onDeactivate() {
		setVisible(false);
	}
	//---------------------------------------------------------

	void CGraphics::setPosition(Vector3 vPos){
		_graphicsEntity->setPosition(vPos);
	}
	//---------------------------------------------------------

	void CGraphics::drawDecal(Vector3 vPos, std::string vTexture) {
		OgreDecal::OgreMesh worldMesh;
 
		/// This method will extract all of the triangles from the mesh to be used later. Only should be called once.
		/// If you scale your mesh at all, pass it in here.

		worldMesh.initialize( this->getOgreMesh()->getMesh(), Vector3(1,1,1));
 
		/// Get the DecalGenerator singleton and initialize it
		OgreDecal::DecalGenerator& generator = OgreDecal::DecalGenerator::getSingleton();
		//generator.initialize( sceneMgr );
		generator.initialize(this->getSceneManager());
 
		/// Set Decal parameters:
		Ogre::Vector3 pos = vPos; /// Send a ray into the mesh
		float width = 1.0f;
		float height = 1.0f;
		//std::string textureName = "gunshotwall"; /// Make sure your texture has a depth_bias greater than 1 to avoid z-fighting
		//std::string textureName = "nicholasCage"; /// Make sure your texture has a depth_bias greater than 1 to avoid z-fighting

		/// We have everything ready to go. Now it's time to actually generate the decal:
		if (_primerDecal)
		{
			vListaDecals.push_back(this->getSceneManager()->createManualObject());
			//decalObject[_iContadorDecals] = this->getSceneManager()->createManualObject();
		}

		_iContadorDecals++;
		if (_iContadorDecals > _iMaxDecals)
		{
			_iContadorDecals = 1;//Lo reseteo a 1 porque luego voy a restar uno para su acceso en el índice
			_primerDecal = false;
		}

		//OgreDecal::Decal decal = generator.createDecal( &worldMesh, pos, width, height, textureName, true, decalObject[_iContadorDecals] );
		Ogre::ManualObject* manObject =  vListaDecals[_iContadorDecals-1];
		OgreDecal::Decal decal = generator.createDecal( &worldMesh, pos, width, height, vTexture, false, manObject );		
		std::cout << "ha creado el decal" << std::endl;
		/// Render the decal object. Always verify the returned object - it will be NULL if no decal could be created.
		if ((decal.object) && (_primerDecal)) {
			//sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject( decal.object );
			this->getSceneManager()->getRootSceneNode()->createChildSceneNode()->attachObject( decal.object );
			std::cout << "entraaaaaa" << std::endl;
		}
		std::cout << "pinta" << std::endl;
	} // decals

} // namespace Logic

