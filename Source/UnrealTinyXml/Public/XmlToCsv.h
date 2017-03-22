
#pragma once

#include "XmlToCsv.generated.h"

UCLASS()
class UCsvField : public UObject
{
	GENERATED_BODY()
private:
	const FString* mpStr;

public:
	void Init(const FString* str) { mpStr = str; }
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|CSV")
		bool AsBool();
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|CSV")
		int32 AsInt();
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|CSV")
		float AsFloat();
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|CSV")
		const FString& AsStr();
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|CSV")
		void SplitToStr(const FString& Delimiter, TArray<FString>& out);
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|CSV")
		void SplitToInt(const FString& Delimiter, TArray<int32>& out);
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|CSV")
		void SplitToFloat(const FString& Delimiter, TArray<float>& out);
};

UCLASS(BlueprintType)
class UXmlToCsv : public UObject
{
	GENERATED_BODY()

private:
	int32		mnCol;//列
	int32		mnRow;//行
	int32		mnCurrentRow;//当前行
	TMap<FString, int32> mmFieldNames;//保存列名列表
	TArray<FString>		 mvDatas;//存储实际数据
	static UCsvField*	mField;//字段
	static TMap<FString, UXmlToCsv*> mmXmlToCsvs;

private:
	UXmlToCsv();
	virtual ~UXmlToCsv();

public:

	/**
	* Open a Excel based xml file, must assign to an var, and must call close() after used.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|CSV")
		static UXmlToCsv* OpenXmlToCsv(const FString& szfilename);
	/**
	* Clear opened xml file.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|CSV")
		static void CloseXmlToCsv(const FString& szfilename);
	/**
	* Get the value of given field name of current row.
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|CSV")
		UCsvField* FieldByName(const FString& szname);
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|CSV")
		bool LocationByInt(const FString& szfield, int32 nval);
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|CSV")
		bool LocationByStr(const FString& szfield, const FString& szval);
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|CSV")
		bool LocationByTwo(const FString& szfield1, const FString& szfield2, int32 nval1, int32 nval2);
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|CSV")
		bool Eof();
	/**
	* Get total row.
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|CSV")
		int32  GetTotalRow();
	/**
	* Get current row.
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|CSV")
		int32  GetCurrentRow();
	/**
	* Set current row.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|CSV")
		void SetCurrentRow(int32 nno);
	/**
	* Go to first row.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|CSV")
		void First();
	/**
	* Go to last row.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|CSV")
		void Last();
	/**
	* Go to next row.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|CSV")
		void Next();
};

