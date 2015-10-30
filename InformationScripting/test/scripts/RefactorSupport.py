from AstApi import Node

class RefactorSupport(object):
    @staticmethod
    def swapAttributesByName(node, nameA, nameB):
        attributeA = node.get(nameA)
        attributeB = node.get(nameB)
        
        swapAttributes(node, attributeA, attributeB)
        
    @staticmethod
    def swapAttributes(node, attributeA, attributeB):
        swapAttributesFromDifferentNodes(node, node, attributeA, node, attributeB)

    @staticmethod
    def swapAttributesFromDifferentNodes(commonParent, nodeA, attributeA, nodeB, attributeB):
        if attributeA == None or attributeB == None:
            raise ValueError("Attributes a and b must not be None")
        commonParent.beginModification("Swap attributes")
        attributeType = "None"
        if attributeA != None:
            attributeType = attributeA.typeName()
        else:
            attributeType = attributeB.typeName()
        
        temp = Node.createNewNode(attributeType, None)
        nodeA.replaceChild(attributeA, temp)
        nodeB.replaceChild(attributeB, attributeA)
        nodeA.replaceChild(temp, attributeB)
        commonParent.endModification()