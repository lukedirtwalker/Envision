# methods -scope=g|evaluation/isRecursive|toClass -a=Node

def isRecursive(methodTuple):
    calls = Query.ast(["-t=MethodCallExpression", "-scope=of"], [TupleSet([methodTuple])])[0]
    for callTuple in calls.tuples("ast"):
        call = callTuple.ast
        methodDef = call.methodDefinition()
        if methodDef != None and methodDef.name == methodTuple.ast.name:
            return True
    return False

Query.result = Query.input

for astTuple in Query.result.tuples("ast"):
    if not isRecursive(astTuple):
        Query.result.remove(astTuple)
