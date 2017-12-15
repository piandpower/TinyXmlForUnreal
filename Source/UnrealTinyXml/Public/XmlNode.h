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
	XMLNode* Node = NULL;

public:
	bool IsValid();

	static UXmlNode* Create(TSharedPtr<XMLDocument> XmlFile, XMLNode* Node);

	/**
	* Add child node to current node at last.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Node")
		UXmlNode* AddChildNode(const FString& ChildName);

	/**
	* Clear all child node.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Node")
		void RomoveAllChild();

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
	* By index to location child node
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Node")
		UXmlNode* GetChildNodeByIndex(int32 Index);

	/**
	* Gets the name of current node, can return NULL.
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|Node")
		FString GetNodeName();

	/**
	* Set a bool value for attribute name
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Node")
		void SetAttributeBool(const FString& AttributeName, bool AttributeValue);

	/**
	* Set a int value for attribute name
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Node")
		void SetAttributeInt(const FString& AttributeName, int32 AttributeValue);

	/**
	* Set a float value for attribute name
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Node")
		void SetAttributeFloat(const FString& AttributeName, float AttributeValue);

	/**
	* Set a string value for attribute name
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|Node")
		void SetAttributeString(const FString& AttributeName, const FString& AttributeValue);

	/**
	* Given an attribute name,returns bool value or null
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|Node")
		bool GetAttributeBool(const FString& AttributeName);

	/**
	* Given an attribute name,returns int value or null
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|Node")
		int32 GetAttributeInt(const FString& AttributeName);

	/**
	* Given an attribute name,returns float value or null
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|Node")
		float GetAttributeFloat(const FString& AttributeName);

	/**
	* Given an attribute name,returns the value
		for the attribute of that name, or null if none
		exists
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|Node")
		FString GetAttributeString(const FString& AttributeName);
};
