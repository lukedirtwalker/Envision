/***********************************************************************************************************************
 * Field.cpp
 *
 *  Created on: Feb 1, 2011
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#include "top_level/Field.h"

namespace OOModel {

EXTENDABLENODE_DEFINE_EMPTY_CONSTRUCTORS(Field, Model::ExtendableNode)
EXTENDABLENODE_DEFINE_TYPE_REGISTRATION_METHODS(Field, Model::ExtendableNode)

REGISTER_ATTRIBUTE(Field, name, Text, false, false, true)
REGISTER_ATTRIBUTE(Field, type, Type, false, false, true)
REGISTER_ATTRIBUTE(Field, visibility, Visibility, false, false, true)
REGISTER_ATTRIBUTE(Field, storageSpecifier, StorageSpecifier, false, false, true)

bool Field::definesSymbol() const
{
	return true;
}

const QString& Field::symbolName() const
{
	return name();
}

}
