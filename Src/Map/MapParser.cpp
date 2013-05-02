/**
@file MapParser.cpp

Contiene la implementación de la clase que encapsula el parseo de mapas.

@see Map::CMapParser

@author David Llansó García
@date Agosto, 2010
*/

#include <fstream>
#include <sstream>
#include <cassert>

#include "MapEntity.h"
#include "MapParser.h"
#include "scanner.h"

#include "tinyxml.h"

namespace Map {

	CMapParser* CMapParser::_instance = 0;

	//--------------------------------------------------------

	CMapParser::CMapParser(): _traceScanning(false),
							  _traceParsing(false),
							  _entityInProgress(0)
	{
		_instance = this;

	} // CMapParser

	//--------------------------------------------------------

	CMapParser::~CMapParser()
	{
		releaseEntityList();
		_instance = 0;

	} // ~CMapParser
	
	//--------------------------------------------------------

	bool CMapParser::Init()
	{
		assert(!_instance && "Segunda inicialización de Map::CMapParser no permitida!");

		new CMapParser();
		return true;

	} // Init

	//--------------------------------------------------------

	void CMapParser::Release()
	{
		assert(_instance && "Map::CMapParser no está inicializado!");
		if(_instance)
		{
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CMapParser::parseStream(std::istream& in, const std::string& name)
	{
		_streamname = name;

		CScanner scanner(&in);
		scanner.set_debug(_traceScanning);
		_lexer = &scanner;

		releaseEntityList();
		CParser parser(*this);
		parser.set_debug_level(_traceParsing);
		return (parser.parse() == 0);
	} // parseStream

	//--------------------------------------------------------

	bool CMapParser::parseFile(const std::string &filename, const std::string &ambit)
	{
		/*
		std::ifstream in(filename.c_str());
		if (!in.good()) return false;
		return parseStream(in, filename);
		/*/
		
		
		char * aux =(char*)filename.c_str();
		TiXmlDocument doc2(aux);

		
		
		assert("Error al leer el XML: " && doc2.LoadFile());
		if (!doc2.LoadFile()){ 
			
			return false;
		}
		
		/*
		TiXmlElement* componentsPriorityTag= doc2.FirstChildElement();
		assert(componentsPriorityTag && "No se detecta la etiqueta entities: " );

		TiXmlElement* entitiesTag= componentsPriorityTag->NextSiblingElement();
		
		// Si es nulo, significa que no tiene etiqueta componentsPriority, por lo que la leida en componentsPriority es realmente la de entity
		if(entitiesTag != NULL ){
			
		}else{
			entitiesTag = componentsPriorityTag;
		}
		*/


		TiXmlElement* archetypesTag = doc2.FirstChildElement();
		TiXmlElement* entitiesTag;

		if( std::strcmp(archetypesTag->ValueTStr().c_str(),"archetypes") == 0){
			TiXmlElement* componentsPriorityTag= archetypesTag->FirstChild()->ToElement();
			assert(componentsPriorityTag && "No se detecta la etiqueta componentsPriority: " );

			// Aqui estoy en el nivel entities
			//TiXmlElement* entitiesTag= archetypesTag->NextSiblingElement();
			entitiesTag= archetypesTag->LastChild()->ToElement();
			assert(entitiesTag && "No se detecta la etiqueta entities: " );
		}else{
			entitiesTag = archetypesTag;
		}

		std::string nameEntity;
		std::string typeEntity;
		std::string nameAttribute;
		std::string valueAttribute;
		// Ahora voy a recorrer todas las entity dentro de entities

		TiXmlElement* entityTag= entitiesTag->FirstChildElement();
		assert(entityTag &&  "No se detecta la etiqueta entity de la entidad: ");
		
		while (entityTag != NULL){

			if (entityTag->Attribute("type") != NULL){
		
				nameEntity = entityTag->Attribute("name");
				assert(!nameEntity.empty() && "No se detecta el atributo name de la entidad");
				typeEntity = entityTag->Attribute("type");
				assert(!typeEntity.empty() && "No se detecta el atributo type de la entidad");

				Map::CEntity *entidad = new Map::CEntity(nameEntity);
				entidad->setType(typeEntity);
				//pongo el atributo necesario name
				entidad->setAttribute("name", nameEntity);

				assert("No se detecta el atributo type de la entidad");

				// esto lo hago en dos pasos para comprobacion de errores
				TiXmlElement *attributesTag = entityTag->FirstChildElement();
				// esta etiqueta puede ser null, lo puede ser cuando queremos cargar algo en el mapa que es directamente el arquetipo
				
				//assert( attributesTag && "No se detecta la etiqueta attributes de la entidad: ");

				if(attributesTag != NULL){

					TiXmlElement *attribute = attributesTag->FirstChildElement();
					//assert(attribute && "No se detecta la etiqueta attribute de la entidad");

					//itero entre todos los ambios necesarios
					while(attribute != NULL){
					
						// Obtengo el nombre, debe de ser 
						nameAttribute = attribute->Attribute("name");
						assert( !nameAttribute.empty() && "No se detecta el atributo name en la etiqueta attribute de la entidad: ");
					
						valueAttribute = attribute->Attribute("value");
						assert( !valueAttribute.empty() && "No se detecta el atributo value en la etiqueta attribute de la entidad: ");

						entidad->setAttribute(nameAttribute, valueAttribute);

						attribute = attribute->NextSiblingElement();
					}// fin while ambitTag
					
				}// fin if attributesTag
				//Guardo la entidad en la lista de entidades
				_entityList.push_back(entidad);
			}// fin if entity type
			entityTag = entityTag->NextSiblingElement();
		}

		return true;
		/* */
	} // parseFile

	//--------------------------------------------------------

	bool CMapParser::parseString(const std::string &input, const std::string& name)
	{
		std::istringstream iss(input);
		return parseStream(iss, name);
	} // parseString

	//--------------------------------------------------------

	void CMapParser::error(const class location& l,
			   const std::string& m)
	{
		std::cerr << l << ": " << m << std::endl;
	} // error

	//--------------------------------------------------------

	void CMapParser::error(const std::string& m)
	{
		std::cerr << m << std::endl;
	} // error

	//--------------------------------------------------------
		
	void CMapParser::releaseEntityList()
	{
		while(!_entityList.empty())
		{
			_entityInProgress = _entityList.back();
			_entityList.pop_back();
			delete _entityInProgress;
		}
	}

} // namespace Map
