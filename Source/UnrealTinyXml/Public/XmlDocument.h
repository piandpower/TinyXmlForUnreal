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
	XMLDocument* XmlFilePtr; // ָ��Xml�ĵ������ָ��
	bool IsValid();

	/**
	* Create a instance of TinyXmlDocument which can load xml,or close xml.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Xml", Meta = (DisplayName = "Create New XmlDocument"))
		static UXmlDocument* NewXmlDocument();
	/**
	* Load xml file from disk, return ture if load successful, or it will be false.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Xml")
		bool LoadXmlFie(const FString& File);

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