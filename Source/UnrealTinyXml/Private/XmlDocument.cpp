// Fill out your copyright notice in the Description page of Project Settings.

#include "XmlDocument.h"
#include "XmlNode.h"
#include "Paths.h"
bool UXmlDocument::IsValid()
{
	return this->XmlFilePtr != nullptr;
}


UXmlDocument* UXmlDocument::CreateXml()
{
	UXmlDocument* Instance = NewObject<UXmlDocument>();
	Instance->XmlFilePtr = new tinyxml2::XMLDocument();
	return Instance;
}

UXmlDocument* UXmlDocument::OpenXml(const FString& XmlPath)
{
	FString Path = FPaths::ProjectContentDir() + XmlPath;

	if (!(FPaths::FileExists(Path)))
	{
		return nullptr;
	}

	UXmlDocument* Instance = CreateXml();

	Instance->XmlFilePtr->LoadFile(TCHAR_TO_UTF8(*Path));
	if (Instance->XmlFilePtr->Error())
	{
		if (Instance->IsValidLowLevel())
		{
			Instance->ConditionalBeginDestroy();
		}

		return nullptr;
	}
	return Instance;
}

UXmlDocument* UXmlDocument::OpenXmlFromBuffers(const TArray<uint8>& Buffers)
{
	if (Buffers.Num()<=0)
	{
		return nullptr;
	}

	//����Node
	UXmlDocument* Instance = CreateXml();

	//��ȡ�ļ�
	Instance->XmlFilePtr->Parse((char*)&Buffers[0], Buffers.Num());
	//���Xml�ļ������Ƿ��������
	if (Instance->XmlFilePtr->Error())
	{
		if (Instance->IsValidLowLevel())
		{
			Instance->ConditionalBeginDestroy();
		}

		return nullptr;
	}
	return Instance;

}


bool UXmlDocument::SaveXmlFie(const FString& File)
{
	this->XmlFilePtr->SaveFile(TCHAR_TO_UTF8(*File));
	if (XmlFilePtr->Error())
	{
		return false;
	}
	return true;
}

UXmlNode* UXmlDocument::CreateRootNode(const FString& RootName)
{
	if (!IsValid())
	{
		return nullptr;
	}

	//Create�����
	tinyxml2::XMLNode* RootNode = XmlFilePtr->NewElement(TCHAR_TO_UTF8(*RootName));

	//������
	if (RootNode == nullptr)
	{
		return nullptr;
	}

	TSharedPtr<XMLDocument> XmlFile = MakeShareable(XmlFilePtr);
	return UXmlNode::Create(XmlFile, RootNode);
}

UXmlNode* UXmlDocument::GetRootNode()
{
	if (!IsValid())
	{
		return nullptr;
	}
	//��ȡ�����
	tinyxml2::XMLNode* RootNode = XmlFilePtr->RootElement();

	//������
	if (RootNode == nullptr)
	{
		return nullptr;
	}

	TSharedPtr<XMLDocument> XmlFile = MakeShareable(XmlFilePtr);
	return UXmlNode::Create(XmlFile, RootNode);
}

void UXmlDocument::Clear()
{
	this->XmlFilePtr->Clear();
	this->XmlFilePtr = nullptr;
	if (this->IsValidLowLevel())
	{
		this->ConditionalBeginDestroy();
	}
	return;
}
