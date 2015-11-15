# This query can negate the result of a single query
# example: methods|not ast -s=of -n=supports

inputCopy = Query.input
queryName = Query.args.pop(0)
queryFunction = getattr(Query, queryName)
queryResult = queryFunction(Query.args, [Query.input])[0]

inputCopy.remove(queryResult)
Query.result = inputCopy