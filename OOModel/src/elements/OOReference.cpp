/***********************************************************************************************************************
 **
 ** Copyright (c) 2011, 2012 ETH Zurich
 ** All rights reserved.
 **
 ** Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 ** following conditions are met:
 **
 **    * Redistributions of source code must retain the above copyright notice, this list of conditions and the
 **      following disclaimer.
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
 **********************************************************************************************************************/

/*
 * OOReference.cpp
 *
 *  Created on: Mar 29, 2012
 *      Author: Dimitar Asenov
 */

#include "OOReference.h"

#include "src/expressions/ReferenceExpression.h"
#include "src/expressions/VariableAccess.h"
#include "src/expressions/MethodCallExpression.h"

namespace OOModel {

NODE_DEFINE_EMPTY_CONSTRUCTORS(OOReference, Model::Reference)
NODE_DEFINE_TYPE_REGISTRATION_METHODS(OOReference)

bool OOReference::resolve()
{
	// TODO Implement resolution routines
	return false;

//	Method* MethodCallExpression::methodDefinition()
//	{
//		Method* met = nullptr;
//
//		if (prefix())
//		{
//			Class* classNode = prefix()->classDefinition();
//
//			if (classNode)	met = dynamic_cast<Method*> (classNode->navigateTo(classNode, ref()->path()));
//		}
//		else
//			met = dynamic_cast<Method*> (ref()->get());
//
//		return met;
//	}

//	Class* ReferenceExpression::classDefinition()
//	{
//		ReferenceExpression* exp = this;
//		QString path;
//		while (exp)
//		{
//			if (!path.isEmpty()) path.prepend(',');
//			path.prepend(exp->ref()->path());
//
//			exp = dynamic_cast<ReferenceExpression*> (exp->prefix());
//		}
//
//		return dynamic_cast<Class*> (navigateTo(this, path));
//	}

//	Class* VariableAccess::classDefinition()
//	{
//		QString path = ref()->path();
//
//		ReferenceExpression* exp = dynamic_cast<ReferenceExpression*> (prefix());
//		while (exp)
//		{
//			if (!path.isEmpty()) path.prepend(',');
//			path.prepend(exp->ref()->path());
//
//			exp = dynamic_cast<ReferenceExpression*> (exp->prefix());
//		}
//
//		Model::Node* var = navigateTo(this, path);
//
//		Field* f = dynamic_cast<Field*> (var);
//		if (f) return f->type()->classDefinition();
//
//		VariableDeclaration* vd = dynamic_cast<VariableDeclaration*> (var);
//		if (vd) return vd->type()->classDefinition();
//
//		return nullptr;
//	}
}

} /* namespace OOModel */