#pragma once
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

typedef rapidxml::xml_document<char> XmlDocument;
typedef rapidxml::xml_node<char> XmlNode;
typedef rapidxml::xml_attribute<char> XmlAttribute;

class CORE_API XmlUtils
{
public:
	static void LoadXmlFromString(XmlDocument* inDoc, const String& inXml);

	static bool LoadXmlFromFile(XmlDocument* inDoc, const String& inFileName);

	static bool SaveXmlAsFile(XmlDocument* inDoc, const String& inFileName);

	static String SaveXmlAsString(XmlDocument* inDoc);

	static void ClearXml(XmlDocument* inDoc);

	static XmlNode* FirstChildNode(XmlNode* inNode, const char* inNodeName = NULL);

	static XmlNode* LastChildNode(XmlNode* inNode, const char* inNodeName = NULL);

	static XmlNode* PreSiblingNode(XmlNode* inNode, const char* inNodeName = NULL);

	static XmlNode* NextSiblingNode(XmlNode* inNode, const char* inNodeName = NULL);

	static bool HasAttributeValue(XmlNode* inNode, const char* inName);

	static XmlAttribute* FirstAttribute(XmlNode* inNode, const char* inName = NULL);

	static XmlAttribute* NextAttribute(XmlAttribute* inAttribute, const char* inName = NULL);

	static void SetNodeValue(XmlDocument* inDoc, XmlNode* inNode, const char* inValue);

	static String GetNodeValue(XmlNode* inNode);

	static void SetNodeName(XmlDocument* inDoc, XmlNode* inNode, const char* inName);

	static String GetNodeName(XmlNode* inNode);

	static XmlNode* AllocateElementNode(XmlDocument* inDoc, XmlNode* inParentNode, const char* inName);

	static XmlNode* AllocateDeclarationNode(XmlDocument* inDoc, XmlNode* inParentNode, const char* inName);

	static void InsertNode(XmlNode* inNode, XmlNode* inWhere, XmlNode* inChild);

	static void RemoveNode(XmlNode* inNode, XmlNode* inChild);

	static void RemoveAllNode(XmlNode* inNode);

	static void RemoveAllAttribute(XmlNode* inNode);

	static XmlNode* AllocateXmlHeader(XmlDocument* inDoc);

	template<typename T>
	static bool GetAttribute(XmlNode* inNode, const char* inName, T& outVal);

	template<typename T>
	static void SetAttribute(XmlDocument* inDoc, XmlNode* inNode, const char* inName, const T& inVal);

};

inline bool XmlUtils::HasAttributeValue(XmlNode* inNode, const char* inName)
{
	XmlAttribute* pAttrNode = inNode->first_attribute(inName);
	return pAttrNode != NULL && pAttrNode->value() != NULL;
}

template<typename T>
bool XmlUtils::GetAttribute(XmlNode* inNode, const char* inName, T& outVal)
{
	if (XmlUtils::HasAttributeValue(inNode, inName) == false) {
		return false;
	}

	String str("");
	XmlAttribute* pAttrNode = inNode->first_attribute(inName);
	if (pAttrNode != NULL) {
		str = pAttrNode->value();
	}

	if (str.GetLength() == 0) {
		return false;
	}

	return StringTo(str, outVal);

}

template<typename T>
void XmlUtils::SetAttribute(XmlDocument* inDoc, XmlNode* inNode, const char* inName, const T& inVal)
{
	String str = ToString(inVal);
	XmlAttribute* pAttrNode = inNode->first_attribute(inName);
	if (pAttrNode != NULL) {
		pAttrNode->value(inDoc->allocate_string(str.AsCharPtr()));
	}
	else {
		pAttrNode = inDoc->allocate_attribute(inName, inDoc->allocate_string(str.AsCharPtr()));
		inNode->append_attribute(pAttrNode);
	}

}
