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
		

		// Aqui estoy en el nivel entities
		TiXmlElement* entitiesTag= doc2.FirstChildElement();
		assert(entitiesTag && "No se detecta la etiqueta entities: " );

		std::string nameEntity;
		std::string typeEntity;
		std::string nameAmbit;
		std::string nameComponent;

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

				assert("No se detecta el atributo type de la entidad");

				// esto lo hago en dos pasos para comprobacion de errores
				TiXmlElement *attributesTag = entityTag->FirstChildElement();
				//Compruebo que no sea NULL				
				assert( attributesTag && "No se detecta la etiqueta attributes de la entidad: ");

				
				TiXmlElement *ambitsTag = attributesTag->NextSiblingElement();
				//assert( "No se detecta la etiqueta ambits de la entidad: " && ambitsTag);

				if(ambitsTag != NULL){

					TiXmlElement *ambitTag = ambitsTag->FirstChildElement();
					assert(ambitTag && "No se detecta la etiqueta ambit de la entidad: ");
				
					std::list<std::string> listComponents;

					//itero entre todos los ambios necesarios
					while(ambitTag != NULL){
					
						// Obtengo el nombre, debe de ser 
						nameAmbit = ambitTag->Attribute("name");
						assert( !nameAmbit.empty() && "No se detecta el atributo name en la etiqueta ambit de la entidad: ");
						assert(!((nameAmbit != "Always") && (nameAmbit != "Single") && (nameAmbit != "Client") && (nameAmbit != "Server")) && 
							"El name del ambit debe de ser Always, Single, Server o Client");
					
						if(nameAmbit == "Always" || nameAmbit == ambit){
							if(!ambitTag->NoChildren()){
								TiXmlElement *componentsTag = ambitTag->FirstChildElement();
								assert( componentsTag && "No se detecta la etiqueta components de la entidad: ");
								TiXmlElement *componentTag = componentsTag->FirstChildElement();
								assert( ( componentsTag != NULL) && "No se detecta la etiqueta component de la entidad: ");
								//meto todos los componentes de un ambito especifico
								while(componentTag != NULL){
									nameComponent = componentTag->Attribute("name");
									assert( (!nameComponent.empty()) && "No se detecta el atributo name en la etiqueta component de la entidad: ");
									listComponents.push_back(nameComponent);

									componentTag = componentTag->NextSiblingElement();
								}
							}
						}
						ambitTag = ambitTag->NextSiblingElement();
					}// fin while ambitTag

					// Ya que tengo todos los componentes los pongo en blueprintElement
					blueprintElement.components = listComponents;
				
					// miro si ya existe, y si es assi 
					assert( "Ya existe en una entidad con ese tipo ");

					// Añadimos a la tabla
					
				}
			}// fin if entity type
			entityTag = entityTag->NextSiblingElement();
		}


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
