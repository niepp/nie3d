#include "core.h"
#include "xmlutils.h"

void XmlUtils::LoadXmlFromString(XmlDocument* inDoc, const String& inXml)
{
	inDoc->parse<0>(inDoc->allocate_string(inXml.AsCharPtr()));
}

bool XmlUtils::LoadXmlFromFile(XmlDocument* inDoc, const String& inFileName)
{
	Uint32 size(0);
	Int8 *buf = FileUtils::Instance()->ReadTextFile(inFileName.AsCharPtr(), &size);
	if (buf == NULL)
		return false;

	String strXML(buf, size);

	LoadXmlFromString(inDoc, strXML);
	return true;
}

bool XmlUtils::SaveXmlAsFile(XmlDocument* inDoc, const String& inFileName)
{
	String str = XmlUtils::SaveXmlAsString(inDoc);
	if (str.GetLength() == 0)
		return false;

	Bool hr = FileUtils::Instance()->WriteTextFile(inFileName.AsCharPtr(), str.AsCharPtr(), str.GetLength());
	return hr;
}

String XmlUtils::SaveXmlAsString(XmlDocument* inDoc)
{
	std::string strXml;
	rapidxml::print(std::back_inserter(strXml), *inDoc, 0);
	return strXml.c_str();
}

void XmlUtils::ClearXml(XmlDocument* inDoc)
{
	inDoc->clear();
}

XmlNode* XmlUtils::FirstChildNode(XmlNode* inNode, const char* inNodeName)
{
	return inNode->first_node(inNodeName);
}

XmlNode* XmlUtils::LastChildNode(XmlNode* inNode, const char* inNodeName)
{
	return inNode->last_node(inNodeName);
}

XmlNode* XmlUtils::PreSiblingNode(XmlNode* inNode, const char* inNodeName)
{
	return inNode->previous_sibling(inNodeName);
}

XmlNode* XmlUtils::NextSiblingNode(XmlNode* inNode, const char* inNodeName)
{
	return inNode->next_sibling(inNodeName);
}

XmlAttribute* XmlUtils::FirstAttribute(XmlNode* inNode, const char* inName)
{
	XmlAttribute* pAttrNode = inNode->first_attribute(inName);
	return pAttrNode;
}

XmlAttribute* XmlUtils::NextAttribute(XmlAttribute* inAttribute, const char* inName)
{
	return inAttribute->next_attribute(inName);
}

void XmlUtils::SetNodeValue(XmlDocument* inDoc, XmlNode* inNode, const char* inValue)
{
	inNode->value(inDoc->allocate_string(inValue));
}

String XmlUtils::GetNodeValue(XmlNode* inNode)
{
	return inNode->value();
}

void XmlUtils::SetNodeName(XmlDocument* inDoc, XmlNode* inNode, const char* inName)
{
	inNode->name(inDoc->allocate_string(inName));
}

String XmlUtils::GetNodeName(XmlNode* inNode)
{
	return inNode->name();
}

XmlNode* XmlUtils::AllocateElementNode(XmlDocument* inDoc, XmlNode* inParentNode, const char* inName)
{
	XmlNode* pElem = inDoc->allocate_node(rapidxml::node_element, inName);
	inParentNode->append_node(pElem);
	return pElem;
}

XmlNode* XmlUtils::AllocateDeclarationNode(XmlDocument* inDoc, XmlNode* inParentNode, const char* inName)
{
	XmlNode* pElem = inDoc->allocate_node(rapidxml::node_declaration, inName);
	inParentNode->append_node(pElem);
	return pElem;
}

void XmlUtils::InsertNode(XmlNode* inNode, XmlNode* inWhere, XmlNode* inChild)
{
	inNode->insert_node(inWhere, inChild);
}

void XmlUtils::RemoveNode(XmlNode* inNode, XmlNode* inChild)
{
	inNode->remove_node(inChild);
}

void XmlUtils::RemoveAllNode(XmlNode* inNode)
{
	inNode->remove_all_nodes();
}

void XmlUtils::RemoveAllAttribute(XmlNode* inNode)
{
	inNode->remove_all_attributes();
}

XmlNode* XmlUtils::AllocateXmlHeader(XmlDocument* inDoc)
{
	XmlNode* pDeclNode = AllocateDeclarationNode(inDoc, inDoc, "xml");
	XmlUtils::SetAttribute(inDoc, pDeclNode, "version", "1.0");
	XmlUtils::SetAttribute(inDoc, pDeclNode, "encoding", "utf-8");
	return pDeclNode;
}
