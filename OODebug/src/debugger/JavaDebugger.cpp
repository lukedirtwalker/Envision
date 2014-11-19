/***********************************************************************************************************************
**
** Copyright (c) 2011, 2014 ETH Zurich
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
** following conditions are met:
**
**    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
**      disclaimer.
**    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
**      following disclaimer in the documentation and/or other materials provided with the distribution.
**    * Neither the name of the ETH Zurich nor the names of its contributors may be used to endorse or promote products
**      derived from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
** INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
** WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
***********************************************************************************************************************/

#include "JavaDebugger.h"

#include "../run_support/java/JavaRunner.h"
#include "jdwp/DebugConnector.h"

#include "OOModel/src/declarations/Method.h"
#include "OOModel/src/declarations/Class.h"
#include "OOModel/src/declarations/Module.h"

namespace OODebug {


void JavaDebugger::debugTree(Model::TreeManager* manager, const QString& pathToProjectContainerDirectory)
{
	Model::Node* mainContainer = JavaRunner::runTree(manager, pathToProjectContainerDirectory, true);
	// Find the class name where the main method is in.
	OOModel::Class* mainClass = nullptr;
	while (!mainClass)
	{
		auto parent = mainContainer->parent();
		mainClass = DCast<OOModel::Class>(parent);
		mainContainer = parent;
		if (!mainContainer) break;
	}
	OOModel::Module* module = nullptr;
	while (!module)
	{
		auto parent = mainContainer->parent();
		module = DCast<OOModel::Module>(parent);
		mainContainer = parent;
		if (!mainContainer) break;
	}

	Q_ASSERT(mainClass);
	QString mainClassName = mainClass->name();
	// TODO properly support nested packages
	if (module)
		mainClassName.prepend(".").prepend(module->name());
	debugConnector().connect(mainClassName);
}

DebugConnector& JavaDebugger::debugConnector()
{
	static DebugConnector debugConnector;
	return debugConnector;
}

} /* namespace OODebug */
