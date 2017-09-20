// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreUObject.h"
#include "tinyxml2.h"
#include "XmlNode.generated.h"
using namespace tinyxml2;

/**
 *
 */
UCLASS(BlueprintType)
class UNREALTINYXML_API UXmlNode : public UObject
{
	GENERATED_BODY()
public:
	TSharedPtr<XMLDocument> XmlFile; // keeps everything in memory
	const XMLNode* Node = NULL;

public:
	bool IsValid();

	static UXmlNode* Create(TSharedPtr<XMLDocument> XmlFile, XMLNode* Node);
	static UXmlNode* Create(TSharedPtr<XMLDocument> XmlFile, const XMLNode* Node);

	/**
	* Gets next (Step) node in a list of nodes, can return NULL.if exceeded,return last one.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Node")
		UXmlNode* GetNextNode(int Step = 1);
	/**
	* Gets previous (Step) node in a list of nodes, can return NULL.if exceeded,return last one.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Node")
		UXmlNode* GetPreviousNode(int Step = 1);

	/**
	* Gets the next node in a list of nodes, can return NULL.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Node")
		UXmlNode* GetParentNode();

	/**
	* Gets the value of the node.
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|Node")
		FString GetNodeValue();

	/**
	* Gets the first child of this node which can be iterated into with GetNextNode, can return NULL.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Node")
		UXmlNode* GetFirstChildNode(const int SearchDeep = 1);

	/**
	* Find first(or "loc") child  node of this node which has the same name as "NameToSearch",Will return Null if can't find.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Node")
		UXmlNode* FindChildNodeByName(const FString& NameToSearch, const int loc = 1);

	/**
	* Gets the name of current node, can return NULL.
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|Node")
		FString GetNodeName();

	/**
	* Given an attribute name,returns the value
		for the attribute of that name, or null if none
		exists
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|Node")
		FString GetAttributeValue(const FString& AttributeName);
};