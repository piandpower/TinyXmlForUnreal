// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTinyXmlPrivatePCH.h"
#include "UnrealTinyXmlDocument.h"



bool UUnrealTinyXmlDocument::IsValid()
{
	return this->XmlFilePtr != nullptr;
}

UUnrealTinyXmlDocument* UUnrealTinyXmlDocument::NewXmlDocument()
{
	//创建Node
	UUnrealTinyXmlDocument* Instance = NewObject<UUnrealTinyXmlDocument>();
	Instance->XmlFilePtr = new tinyxml2::XMLDocument();
	return Instance;
}

bool UUnrealTinyXmlDocument::LoadXmlFie(const FString& File)
{
	//获取Xml文件路径
	FString XmlPath = FPaths::GameContentDir() + File;

	//检查文件是否存在
	if (!(FPaths::FileExists(XmlPath)))
	{
		return false;
	}

	//读取文件
	this->XmlFilePtr->LoadFile(TCHAR_TO_UTF8(*XmlPath));
	//检测Xml文件解析是否包含错误
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

	//获取根结点
	tinyxml2::XMLNode* RootNode = XmlFilePtr->RootElement();

	//检查错误
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
