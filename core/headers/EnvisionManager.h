/***********************************************************************************************************************
 * EnvisionManager.h
 *
 *  Created on: Oct 29, 2010
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#ifndef ENVISIONMANAGER_H_
#define ENVISIONMANAGER_H_

#include <QString>
#include <QtGui/QWidget>
#include "PluginInfo.h"

namespace Envision {

class EnvisionManager
{
	public:
		virtual QList<PluginInfo> getAllLoadedPluginsInfo() = 0;
		virtual QWidget* getMainWindow() = 0;
};

}

#endif /* ENVISIONMANAGER_H_ */
