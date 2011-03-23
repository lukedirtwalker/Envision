/***********************************************************************************************************************
 * Project.cpp
 *
 *  Created on: Jan 27, 2011
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#include "top_level/Project.h"

namespace OOModel {

EXTENDABLENODE_DEFINE_EMPTY_CONSTRUCTORS(Project, Model::ExtendableNode)
EXTENDABLENODE_DEFINE_TYPE_REGISTRATION_METHODS(Project, Model::ExtendableNode)

REGISTER_ATTRIBUTE(Project, name, Text, false, false, true)
REGISTER_ATTRIBUTE(Project, projects, TypedListOfProject, false, false, true)
REGISTER_ATTRIBUTE(Project, libraries, TypedListOfLibrary, false, false, true)
REGISTER_ATTRIBUTE(Project, modules, TypedListOfModule, false, false, true)
REGISTER_ATTRIBUTE(Project, classes, TypedListOfClass, false, false, true)

bool Project::definesSymbol() const
{
	return true;
}

const QString& Project::symbolName() const
{
	return name();
}

}
