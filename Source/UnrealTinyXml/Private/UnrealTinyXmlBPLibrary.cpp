// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UnrealTinyXmlPrivatePCH.h"
#include "UnrealTinyXmlBPLibrary.h"

UUnrealTinyXmlBPLibrary::UUnrealTinyXmlBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UUnrealTinyXmlBPLibrary::UnrealTinyXmlSampleFunction(float Param)
{
	return -1;
}

int UUnrealTinyXmlBPLibrary::TestXML(const FString& file)
{
	FString XmlPath = FPaths::GameContentDir() + "XmlConfig/" + file;

	if (!(FPaths::FileExists(XmlPath)))
		return 1;
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();


	doc->LoadFile(TCHAR_TO_UTF8(*XmlPath));

	if (doc->Error())
	{
		UE_LOG(LogTemp, Error, TEXT("Xml�ļ����ش���,��������:%s"), UTF8_TO_TCHAR(doc->ErrorName()));
		return 1;
	}
	tinyxml2::XMLNode* RootElement = doc->RootElement();
	const char* ourchar = RootElement->FirstChild()->ToElement()->Attribute("category");
	TCHAR* text = UTF8_TO_TCHAR(ourchar);
	UE_LOG(LogTemp, Warning, TEXT("%s"),text);
	return 0;
}

UUnrealTinyXmlNode* UUnrealTinyXmlBPLibrary::ParseXmlFile(const FString& File)
{
	//��ȡXml�ļ�·��
	FString XmlPath = FPaths::GameContentDir() + "XmlConfig/" + File;

	//����ļ��Ƿ����
	if (!(FPaths::FileExists(XmlPath)))
	{
		UE_LOG(LogTemp, Error, TEXT("Ҫ������Xml�ļ������ڣ�ȷ��·����ȷ"));
		return nullptr;
	}

	//����xml�ĵ�����
	tinyxml2::XMLDocument* XmlFilePtr = new tinyxml2::XMLDocument();
	//��ȡ�ļ�
	XmlFilePtr->LoadFile(TCHAR_TO_UTF8(*XmlPath));

	//���Xml�ļ������Ƿ��������
	if (XmlFilePtr->Error())
	{
		UE_LOG(LogTemp, Error, TEXT("Xml�ļ����ش���,��������:%s"), UTF8_TO_TCHAR(XmlFilePtr->ErrorName()));
		return nullptr;
	}

	//��ȡ��һ���ֽڵ�
	tinyxml2::XMLNode* RootNode = XmlFilePtr->RootElement();

	//������
	if (RootNode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("��ȡ����Xml�ļ������ʧ��"));
		return nullptr;
	}

	TSharedPtr<XMLDocument> XmlFile = MakeShareable(XmlFilePtr);
	
	return UUnrealTinyXmlNode::Create(XmlFile, RootNode);
}

