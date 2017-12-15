// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealTinyXmlPrivatePCH.h"
#include "XmlDocument.h"



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
	FString Path = FPaths::GameContentDir() + XmlPath;

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

	//创建Node
	UXmlDocument* Instance = CreateXml();

	//读取文件
	Instance->XmlFilePtr->Parse((char*)&Buffers[0], Buffers.Num());
	//检测Xml文件解析是否包含错误
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

	//Create根结点
	tinyxml2::XMLNode* RootNode = XmlFilePtr->NewElement(TCHAR_TO_UTF8(*RootName));

	//检查错误
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
	//获取根结点
	tinyxml2::XMLNode* RootNode = XmlFilePtr->RootElement();

	//检查错误
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
