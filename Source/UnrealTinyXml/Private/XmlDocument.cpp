// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTinyXmlPrivatePCH.h"
#include "XmlDocument.h"



bool UXmlDocument::IsValid()
{
	return this->XmlFilePtr != nullptr;
}

UXmlDocument* UXmlDocument::NewXmlDocument()
{
	//����Node
	UXmlDocument* Instance = NewObject<UXmlDocument>();
	Instance->XmlFilePtr = new tinyxml2::XMLDocument();
	return Instance;
}

bool UXmlDocument::LoadXmlFie(const FString& File)
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
