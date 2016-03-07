from xml.etree import ElementTree
from xml.etree.ElementTree import Element, SubElement
from xml.dom import minidom

def complexType(name):
    result = Element('xs:complexType')
    result.set('name', name)
    return result

def schema():
    result = Element('xs:schema')
    result.set('xmlns:xs', 'http://www.w3.org/2001/XMLSchema')
    return result

def toElement(classNode):
    ct = complexType(classNode.name)
    for field in classNode.fields:
        print(field.name)
    return ct

if not type(Query.target) is Class:
    raise Exception('Needs to be executed on a class')

top = schema()
top.append(toElement(Query.target))


xmlString = ElementTree.tostring(top, 'utf-8')
prettyXml = minidom.parseString(xmlString).toprettyxml()
print(prettyXml)





