/***********************************************************************************************************************
 **
 ** Copyright (c) 2011, 2016 ETH Zurich
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

#include "UnionType.h"

namespace OOModel {

UnionType::UnionType() : Type{true}
{}

UnionType::UnionType(const UnionType& other)
	: Type{other}
{
	for (auto& type : other.unifiedTypes_)
		unifiedTypes_.emplace_back(std::unique_ptr<Type>{type->clone()});
}

bool UnionType::equals(const Type* other) const
{
	if (auto otherUnion = dynamic_cast<const UnionType*> (other))
	{
		return std::equal(unifiedTypes_.begin(), unifiedTypes_.end(),
								otherUnion->unifiedTypes_.begin(), otherUnion->unifiedTypes_.end(),
								[](const std::unique_ptr<Type>& a, const std::unique_ptr<Type>& b) {
			return a->equals(b.get());
		});
	}

	return false;
}

UnionType* UnionType::clone() const
{
	return new UnionType{*this};
}

void UnionType::addType(std::unique_ptr<Type> type)
{
	unifiedTypes_.emplace_back(std::move(type));
}

QList<const Type*> UnionType::unifiedTypes() const
{
	QList<const Type*> result;
	for (const auto& type : unifiedTypes_)
		result.append(type.get());
	return result;
}

}
