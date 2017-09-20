
#pragma once

#include "ExcelBasedXml.generated.h"

UCLASS()
class UExcelField : public UObject
{
	GENERATED_BODY()
private:
	const FString* mpStr;

public:
	void Init(const FString* str) { mpStr = str; }
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|ExcelBasedXml")
		bool AsBool();
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|ExcelBasedXml")
		int32 AsInt();
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|ExcelBasedXml")
		float AsFloat();
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|ExcelBasedXml")
		const FString& AsStr();
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|ExcelBasedXml")
		static void SplitToStr(const FString& Str, const FString& Delimiter, TArray<FString>& out);
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|ExcelBasedXml")
		static void SplitToInt(const FString& Str, const FString& Delimiter, TArray<int32>& out);
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|ExcelBasedXml")
		static void SplitToFloat(const FString& Str, const FString& Delimiter, TArray<float>& out);
};

UCLASS(BlueprintType)
class UExcelBasedXml : public UObject
{
	GENERATED_BODY()

private:
	int32		mnCol;
	int32		mnRow;
	int32		mnCurrentRow;
	TMap<FString, int32> mmFieldNames;
	TArray<FString>		 mvDatas;
	static UExcelField*	mField;
	static FString	xmlRootDir;

	static TMap<FString, UExcelBasedXml*> mmTables;

private:
	UExcelBasedXml();
	virtual ~UExcelBasedXml();

public:

	/**
	* Init xml table basic properties, such as xml file directory(default is content directory.)..
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|ExcelBasedXml")
		static void InitProperties(const FString& xmlRootDir);

	/**
	* Open a Excel based xml file, must assign to an var, and must call close() after used.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|ExcelBasedXml")
		static UExcelBasedXml* OpenXmlTable(const FString& szfilename);
	/**
	* Clear opened xml file.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|ExcelBasedXml")
		static void CloseXmlTable(const FString& szfilename);
	/**
	* Get the value of given field name of current row.
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|ExcelBasedXml")
		UExcelField* GetFieldByName(const FString& szname);
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|ExcelBasedXml")
		bool LocateByInt(const FString& szfield, int32 nval);
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|ExcelBasedXml")
		bool LocateByStr(const FString& szfield, const FString& szval);
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|ExcelBasedXml")
		bool LocationByTwo(const FString& szfield1, const FString& szfield2, int32 nval1, int32 nval2);
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|ExcelBasedXml")
		bool Eof();
	/**
	* Get total row.
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|ExcelBasedXml")
		int32  GetTotalRow();
	/**
	* Get current row.
	*/
	UFUNCTION(BlueprintPure, Category = "UnrealTinyXml|ExcelBasedXml")
		int32  GetCurrentRow();
	/**
	* Set current row.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|ExcelBasedXml")
		void SetCurrentRow(int32 nno);
	/**
	* Go to first row.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|ExcelBasedXml")
		void First();
	/**
	* Go to last row.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|ExcelBasedXml")
		void Last();
	/**
	* Go to next row.
	*/
	UFUNCTION(BlueprintCallable, Category = "UnrealTinyXml|ExcelBasedXml")
		void Next();
};

