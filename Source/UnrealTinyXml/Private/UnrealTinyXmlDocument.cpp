// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTinyXmlPrivatePCH.h"
#include "UnrealTinyXmlDocument.h"



bool UUnrealTinyXmlDocument::IsValid()
{
	return this->XmlFilePtr != nullptr;
}

UUnrealTinyXmlDocument* UUnrealTinyXmlDocument::NewXmlDocument()
{
	//����Node
	UUnrealTinyXmlDocument* Instance = NewObject<UUnrealTinyXmlDocument>();
	Instance->XmlFilePtr = new tinyxml2::XMLDocument();
	return Instance;
}

bool UUnrealTinyXmlDocument::LoadXmlFie(const FString& File)
{
	//��ȡXml�ļ�·��
	FString XmlPath = FPaths::GameContentDir() + File;

	//����ļ��Ƿ����
	if (!(FPaths::FileExists(XmlPath)))
	{
		return false;
	}

	//��ȡ�ļ�
	this->XmlFilePtr->LoadFile(TCHAR_TO_UTF8(*XmlPath));
	//���Xml�ļ������Ƿ��������
	if (XmlFilePtr->Error())
	{
		return false;
	}
	return true;


}

UUnrealTinyXmlNode* UUnrealTinyXmlDocument::GetRootNode()
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
	return UUnrealTinyXmlNode::Create(XmlFile, RootNode);
}

void UUnrealTinyXmlDocument::Clear()
{
	this->XmlFilePtr->Clear();
	this->XmlFilePtr = nullptr;
	if (this->IsValidLowLevel())
	{
		this->ConditionalBeginDestroy();
	}
	return;
}
