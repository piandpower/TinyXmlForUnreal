// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTinyXmlPrivatePCH.h"
#include "UnrealTinyXmlDocument.h"




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
	//UE_LOG(LogTemp, Warning, TEXT("�Ƿ���Ч:%d"), XmlFile.IsValid());
	//UE_LOG(LogTemp, Warning, TEXT("��������:%d"), XmlFile.GetSharedReferenceCount());
	//UE_LOG(LogTemp, Warning, TEXT("�Ƿ�Ψһ:%d"), XmlFile.IsUnique());
	////Node = nullptr;
	//XMLDocument* doc = XmlFile.Get();
	//doc->Clear();
	//doc = nullptr;
	//XmlFile.Reset();
	//UE_LOG(LogTemp, Warning, TEXT("�Ƿ���Ч:%d"), XmlFile.IsValid());
	//UE_LOG(LogTemp, Warning, TEXT("ɾ������������:%d"), XmlFile.GetSharedReferenceCount());
	//UE_LOG(LogTemp, Warning, TEXT("�Ƿ�Ψһ:%d"), XmlFile.IsUnique());
	return;
}
