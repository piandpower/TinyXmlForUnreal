// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTinyXmlPrivatePCH.h"
#include "XmlNode.h"




bool UXmlNode::IsValid()
{
	return (XmlFile.IsValid()) && (Node != nullptr);
}

UXmlNode* UXmlNode::Create(TSharedPtr<XMLDocument> XmlFile, XMLNode* Node)
{
	//创建Node
	UXmlNode* Instance = NewObject<UXmlNode>();
	Instance->XmlFile = XmlFile;
	Instance->Node = Node;
	return Instance;
}

UXmlNode* UXmlNode::Create(TSharedPtr<XMLDocument> XmlFile, const XMLNode* Node)
{
	//创建Node
	UXmlNode* Instance = NewObject<UXmlNode>();
	Instance->XmlFile = XmlFile;
	Instance->Node = Node;
	return Instance;
}

UXmlNode* UXmlNode::GetNextNode(int Step /*= 1*/)
{
	//检查错误
	if (!IsValid() || Step <= 0)
	{
		return NULL;
	}

	//找到下一个
	const XMLNode* NextNode = Node->NextSibling();

	if (NextNode == nullptr)
		return NULL;

	//如果要找不止一个,则继续找
	if (Step > 1)
	{
		for (int i = 2; i <= Step; i++)
		{
			if (NextNode->NextSibling() == nullptr)
				break;
			NextNode = NextNode->NextSibling();
		}
		return UXmlNode::Create(XmlFile, NextNode);
	}
	else
		return UXmlNode::Create(XmlFile, NextNode);
}


UXmlNode* UXmlNode::GetPreviousNode(int Step /*= 1*/)
{
	//检查错误
	if (!IsValid() || Step <= 0)
	{
		return NULL;
	}

	//找到下一个
	const XMLNode* NextNode = Node->PreviousSibling();

	if (NextNode == nullptr)
		return NULL;
	//如果要找不止一个,则继续找
	if (Step > 1)
	{
		for (int i = 2; i <= Step; i++)
		{
			if (NextNode->PreviousSibling() == nullptr)
				break;
			NextNode = NextNode->PreviousSibling();
		}
		return UXmlNode::Create(XmlFile, NextNode);
	}
	else
		return UXmlNode::Create(XmlFile, NextNode);
}

UXmlNode* UXmlNode::GetParentNode()
{
	if (!IsValid())
	{
		return NULL;
	}
	const XMLNode* ParentNode = Node->Parent();
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
	const XMLNode* FirstChild = Node->FirstChild();
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
	const XMLNode* TempNode = Node->FirstChild();
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

FString UXmlNode::GetNodeName()
{
	if (!IsValid())
	{
		return "";
	}
	return UTF8_TO_TCHAR(Node->ToElement()->Name());
}

FString UXmlNode::GetAttributeValue(const FString& AttributeName)
{
	if (!IsValid())
	{
		return TEXT("");
	}
	return UTF8_TO_TCHAR(Node->ToElement()->Attribute(TCHAR_TO_UTF8(*AttributeName)));
}


