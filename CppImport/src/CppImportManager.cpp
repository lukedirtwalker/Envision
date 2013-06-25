/***********************************************************************************************************************
 **
 ** Copyright (c) 2011, 2013 ETH Zurich
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

#include "CppImportManager.h"

#include "ClangFrontendActionFactory.h"

namespace CppImport {

CppImportManager::~CppImportManager()
{
	SAFE_DELETE(compilationDB_);
	SAFE_DELETE(myTool_);
}

bool CppImportManager::setSrcPath(QString& srcpath, QString& dbpath)
{
	// set a filter to only get files which are c++ sources
	QStringList cppFilter;
	cppFilter << "*.cpp" << "*.cc" << "*.cxx";
	QDirIterator dirIterator(srcpath, cppFilter, QDir::Files, QDirIterator::Subdirectories);
	while(dirIterator.hasNext())
		sources_.push_back(dirIterator.next().toStdString());
	return setCompilationDbPath(dbpath);
}

Model::Model*CppImportManager::createModel()
{
	myTool_ = new clang::tooling::ClangTool(*compilationDB_,sources_);
	OOModel::Project* project = new OOModel::Project("CppImport");
	ClangFrontendActionFactory* frontendActionFactory = new ClangFrontendActionFactory(project);
	myTool_->run(frontendActionFactory);
	frontendActionFactory->outputStatistics();
	SAFE_DELETE(frontendActionFactory);
	return new Model::Model("CppImport", project);
}

bool CppImportManager::setCompilationDbPath(QString& path)
{
	std::string Error;
	compilationDB_ = nullptr;
	compilationDB_ = clang::tooling::CompilationDatabase::loadFromDirectory(path.toAscii().data(),Error);

	if(!compilationDB_)
	{
		std::cout << "Error could not load compilation database :  " << Error << std::endl;
		return false;
	}
	return true;
}

}
