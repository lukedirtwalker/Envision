/***********************************************************************************************************************
**
** Copyright (c) 2011, 2015 ETH Zurich
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

#include "AstQuery.h"

#include "OOModel/src/declarations/Class.h"

#include "../graph/Graph.h"
#include "../graph/InformationNode.h"

#include "../visitors/AllNodesOfType.h"

namespace InformationScripting {

AstQuery::AstQuery(QueryType type, Model::Node* target, QStringList args)
	: target_{target}, type_{type}
{
	if (args.size() && args[0] == "g") scope_ = Scope::Global;
}

Graph* AstQuery::execute(QList<Graph*> input)
{
	switch (type_)
	{
		case QueryType::Methods: return methodsQuery(input);
		case QueryType::BaseClasses: return baseClassesQuery(input);
	}
}

Graph* AstQuery::methodsQuery(QList<Graph*>)
{
	// TODO handle input
	auto g = new Graph();
	if (scope_ == Scope::Local)
	{
		auto parentClass = target_->firstAncestorOfType<OOModel::Class>();
		for (auto method : *parentClass->methods())
		{
			auto node = new InformationNode({{"ast", method}});
			g->add(node);
		}
	}
	else if (scope_ == Scope::Global)
	{
		auto root = target_->manager()->root();
		AllNodesOfType<OOModel::Method> visitor;
		visitor.visit(root);
		auto allMethods =  visitor.results();
		for (auto method : allMethods)
			g->add(new InformationNode({{"ast", method}}));

	}
	return g;
}

Graph* AstQuery::baseClassesQuery(QList<Graph*>)
{
	// TODO handle input
	auto g = new Graph();
	if (scope_ == Scope::Local)
	{
		OOModel::Class* parentClass = DCast<OOModel::Class>(target_);
		if (!parentClass) parentClass = target_->firstAncestorOfType<OOModel::Class>();

		auto classNode = new InformationNode({{"ast", parentClass}});
		g->add(classNode);

		addBaseEdgesFor(parentClass, classNode, g);
	}
	else if (scope_ == Scope::Global)
	{
		// TODO
	}
	return g;
}

void AstQuery::addBaseEdgesFor(OOModel::Class* childClass, InformationNode* classNode, Graph* g)
{
	auto bases = childClass->directBaseClasses();
	for (auto base : bases)
	{
		auto baseNode = new InformationNode({{"ast", base}});
		g->add(baseNode);
		g->addDirectedEdge(classNode, baseNode, "base class");
		addBaseEdgesFor(base, baseNode, g);
	}
}

} /* namespace InformationScripting */
