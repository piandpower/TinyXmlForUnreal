// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTinyXmlPrivatePCH.h"
#include "UnrealTinyXmlDocument.h"



bool UUnrealTinyXmlDocument::IsValid()
{
	return this->XmlFilePtr != nullptr;
}

bool UUnrealTinyXmlDocument::LoadXmlFie(const FString& File)
{
	//��ȡXml�ļ�·��
	FString XmlPath = FPaths::GameContentDir() + "XmlConfig/" + File;

	//����ļ��Ƿ����
	if (!(FPaths::FileExists(XmlPath)))
	{
		UE_LOG(LogTemp, Error, TEXT("Ҫ������Xml�ļ������ڣ�ȷ��·����ȷ"));
		return false;
	}

	//��ȡ�ļ�
	this->XmlFilePtr->LoadFile(TCHAR_TO_UTF8(*XmlPath));
	//���Xml�ļ������Ƿ��������
	if (XmlFilePtr->Error())
	{
		UE_LOG(LogTemp, Error, TEXT("Xml�ļ����ش���,��������:%s"), UTF8_TO_TCHAR(XmlFilePtr->ErrorName()));
		return false;
	}
	return true;


}

UUnrealTinyXmlNode* UUnrealTinyXmlDocument::GetRootNode()
{
	if (!IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Xml�ĵ�����ɹ�����һ��xml�ļ��󣬲��ܻ�ȡ�ڵ�"));
	}

	//��ȡ�����
	tinyxml2::XMLNode* RootNode = XmlFilePtr->RootElement();

	//������
	if (RootNode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("��ȡ����Xml�ļ������ʧ��!"));
		return nullptr;
	}

	TSharedPtr<XMLDocument> XmlFile = MakeShareable(XmlFilePtr);
	return UUnrealTinyXmlNode::Create(XmlFile, RootNode);
}

void UUnrealTinyXmlDocument::Clear()
{
	this->XmlFilePtr->Clear();
	UE_LOG(LogTemp, Warning, TEXT("�������"));
	return;
}
