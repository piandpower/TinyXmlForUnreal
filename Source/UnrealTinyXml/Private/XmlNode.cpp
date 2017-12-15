// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTinyXmlPrivatePCH.h"
#include "XmlNode.h"




bool UXmlNode::IsValid()
{
	return (XmlFile.IsValid()) && (Node != nullptr);
}

UXmlNode* UXmlNode::Create(TSharedPtr<XMLDocument> XmlFile, XMLNode* Node)
{
	//´´½¨Node
	UXmlNode* Instance = NewObject<UXmlNode>();
	Instance->XmlFile = XmlFile;
	Instance->Node = Node;
	return Instance;
}


UXmlNode* UXmlNode::AddChildNode(const FString& ChildName)
{
	if (!IsValid())
	{
		return nullptr;
	}

	XMLElement* RetNode = Node->GetDocument()->NewElement(TCHAR_TO_UTF8(*ChildName));
	Node->InsertEndChild(RetNode);

	if (RetNode == nullptr)
		return nullptr;

	return UXmlNode::Create(XmlFile, RetNode);
}

void UXmlNode::RomoveAllChild()
{
	if (!IsValid())
	{
		return;
	}

	Node->DeleteChildren();
}

UXmlNode* UXmlNode::GetNextNode(int Step /*= 1*/)
{
	if (!IsValid() || Step <= 0)
	{
		return NULL;
	}

	XMLNode* NextNode = Node->NextSibling();

	if (NextNode == nullptr)
		return NULL;

	if (Step > 1)
	{
		for (int i = 2; i <= Step; i++)
		{
			if (NextNode->NextSibling() == nullptr)
				break;
			NextNode = NextNode->NextSibling();
		}
	}

	return UXmlNode::Create(XmlFile, NextNode);
}


UXmlNode* UXmlNode::GetPreviousNode(int Step /*= 1*/)
{
	if (!IsValid() || Step <= 0)
	{
		return NULL;
	}

	XMLNode* NextNode = Node->PreviousSibling();

	if (NextNode == nullptr)
		return NULL;

	if (Step > 1)
	{
		for (int i = 2; i <= Step; i++)
		{
			if (NextNode->PreviousSibling() == nullptr)
				break;
			NextNode = NextNode->PreviousSibling();
		}
	}
	return UXmlNode::Create(XmlFile, NextNode);
}

UXmlNode* UXmlNode::GetParentNode()
{
	if (!IsValid())
	{
		return NULL;
	}
	XMLNode* ParentNode = Node->Parent();
	if (ParentNode == nullptr)
	{
		return NULL;
	}

	return UXmlNode::Create(XmlFile, ParentNode);
}

FString UXmlNode::GetNodeValue()
{
	if (!IsValid())
	{
		return TEXT("");
	}
	return UTF8_TO_TCHAR(Node->ToElement()->GetText());
}

UXmlNode* UXmlNode::GetFirstChildNode(const int SearchDeep)
{
	if (!IsValid())
	{
		return NULL;
	}
	XMLNode* FirstChild = Node->FirstChild();
	int counter = 1;
	while (counter < SearchDeep)
	{
		FirstChild = FirstChild->FirstChild();
		counter++;
	}

	if (FirstChild == nullptr)
	{
		return NULL;
	}
	return UXmlNode::Create(XmlFile, FirstChild);
}

UXmlNode* UXmlNode::FindChildNodeByName(const FString& NameToSearch, const int loc)
{
	if (!IsValid())
	{
		return NULL;
	}
	XMLNode* TempNode = Node->FirstChild();
	if (TempNode == nullptr)
		return nullptr;

	int counter = 0;

	while (TempNode != nullptr)
	{
		if (strcmp(TempNode->ToElement()->Name(), TCHAR_TO_UTF8(*NameToSearch))  == 0 )
		{
			counter++;
			if (counter >= loc)
				return UXmlNode::Create(XmlFile, TempNode);
		}
		TempNode = TempNode->NextSibling();
	}

	return NULL;

}

UXmlNode* UXmlNode::GetChildNodeByIndex(int32 Index)
{
	if (!IsValid())
	{
		return nullptr;
	}

	int counter = 0;

	for (XMLNode* TempNode = Node->FirstChild(); TempNode != nullptr; TempNode = TempNode->NextSibling(), counter++)
	{
		if (counter >= Index)
			return UXmlNode::Create(XmlFile, TempNode);
	}

	return nullptr;
}

FString UXmlNode::GetNodeName()
{
	if (!IsValid())
	{
		return "";
	}
	return UTF8_TO_TCHAR(Node->ToElement()->Name());
}

void UXmlNode::SetAttributeBool(const FString& AttributeName, bool AttributeValue)
{
	if (!IsValid())
	{
		return;
	}
	Node->ToElement()->SetAttribute(TCHAR_TO_UTF8(*AttributeName), AttributeValue);
}

void UXmlNode::SetAttributeInt(const FString& AttributeName, int32 AttributeValue)
{
	if (!IsValid())
	{
		return;
	}

	Node->ToElement()->SetAttribute(TCHAR_TO_UTF8(*AttributeName), AttributeValue);
}

void UXmlNode::SetAttributeFloat(const FString& AttributeName, float AttributeValue)
{
	if (!IsValid())
	{
		return;
	}

	Node->ToElement()->SetAttribute(TCHAR_TO_UTF8(*AttributeName), AttributeValue);
}

void UXmlNode::SetAttributeString(const FString& AttributeName, const FString& AttributeValue)
{
	if (!IsValid())
	{
		return;
	}

	Node->ToElement()->SetAttribute(TCHAR_TO_UTF8(*AttributeName), TCHAR_TO_UTF8(*AttributeValue));
}

bool UXmlNode::GetAttributeBool(const FString& AttributeName)
{
	if (!IsValid())
	{
		return false;
	}

	return Node->ToElement()->BoolAttribute(TCHAR_TO_UTF8(*AttributeName));
}

int32 UXmlNode::GetAttributeInt(const FString& AttributeName)
{
	if (!IsValid())
	{
		return 0;
	}

	return Node->ToElement()->IntAttribute(TCHAR_TO_UTF8(*AttributeName));
}

float UXmlNode::GetAttributeFloat(const FString& AttributeName)
{
	if (!IsValid())
	{
		return 0.0f;
	}

	return Node->ToElement()->FloatAttribute(TCHAR_TO_UTF8(*AttributeName));
}

FString UXmlNode::GetAttributeString(const FString& AttributeName)
{
	if (!IsValid())
	{
		return TEXT("");
	}
	return UTF8_TO_TCHAR(Node->ToElement()->Attribute(TCHAR_TO_UTF8(*AttributeName)));
}


