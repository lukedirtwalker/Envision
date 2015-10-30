#example: script "<swapAttribute thenBranch elseBranch>"

from RefactorSupport import RefactorSupport

if len(args) >= 2:    
    RefactorSupport.swapAttributesByName(target, args[0], args[1])
else:
    print('Need to provide attribute names as argument')
