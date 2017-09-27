// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTinyXmlPrivatePCH.h"
#include "XmlDocument.h"



bool UXmlDocument::IsValid()
{
	return this->XmlFilePtr != nullptr;
}


UXmlDocument* UXmlDocument::OpenXml(const FString& XmlPath)
{
	//��ȡXml�ļ�·��
	FString Path = FPaths::GameContentDir() + XmlPath;

	//����ļ��Ƿ����
	if (!(FPaths::FileExists(Path)))
	{
		return nullptr;
	}

	//����Node
	UXmlDocument* Instance = NewObject<UXmlDocument>();
	Instance->XmlFilePtr = new tinyxml2::XMLDocument();

	//��ȡ�ļ�
	Instance->XmlFilePtr->LoadFile(TCHAR_TO_UTF8(*Path));
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

UXmlDocument* UXmlDocument::OpenXmlFromBuffers(const TArray<uint8>& Buffers)
{
	if (Buffers.Num()<=0)
	{
		return nullptr;
	}

	//����Node
	UXmlDocument* Instance = NewObject<UXmlDocument>();
	Instance->XmlFilePtr = new tinyxml2::XMLDocument();

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
