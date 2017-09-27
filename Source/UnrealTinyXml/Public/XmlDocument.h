// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreUObject.h"
#include "tinyxml2.h"
#include "XmlDocument.generated.h"
using namespace tinyxml2;

/**
 * 
 */
UCLASS(BlueprintType)
class UNREALTINYXML_API UXmlDocument : public UObject
{
	GENERATED_BODY()
	
	
public:
	XMLDocument* XmlFilePtr; // 指向Xml文档对象的指针
	bool IsValid();

	/**
	* Open a xml file from disk.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Xml")
		static UXmlDocument* OpenXml(const FString& XmlPath);

	/**
	* Open a xml file from buffers.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Xml")
		static UXmlDocument* OpenXmlFromBuffers(const TArray<uint8>& Buffers);

	/**
	* Get the root node of current loaded xml file, returns NULL if it can't be parsed.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Xml")
		UXmlNode* GetRootNode();

	/**
	* Clear the document that has this node, resetting it to the initial state(All nodes of this document will become null!).
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Xml")
		void Clear();

};
