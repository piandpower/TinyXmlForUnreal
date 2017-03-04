
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
	UFUNCTION(BlueprintPure, Category = "CSVFile|Function")
		bool AsBool();
	UFUNCTION(BlueprintPure, Category = "CSVFile|Function")
		int32 AsInt();
	UFUNCTION(BlueprintPure, Category = "CSVFile|Function")
		float AsFloat();
	UFUNCTION(BlueprintPure, Category = "CSVFile|Function")
		const FString& AsStr();
};

UCLASS(BlueprintType)
class UXmlToCsv : public UObject
{
	GENERATED_BODY()

private:
	int32		mnCol;
	int32		mnRow;
	int32		mnPos;
	UCsvField*	mField;
	TMap<FString, int32> mmFieldNames;
	TArray<FString>		 mvDatas;

private:
	UXmlToCsv();
	virtual ~UXmlToCsv();

public:
	UFUNCTION(BlueprintCallable, Category = "CSVFile|Function")
		//must assign to an var, and must call close() after used
		static UXmlToCsv* OpenXmlToCsv(const FString& szfilename);
	UFUNCTION(BlueprintCallable, Category = "CSVFile|Function")
		void Close();
	UFUNCTION(BlueprintPure, Category = "CSVFile|Function")
		UCsvField* FieldByName(const FString& szname);
	UFUNCTION(BlueprintCallable, Category = "CSVFile|Function")
		bool LocationByInt(const FString& szfield, int32 nval);
	UFUNCTION(BlueprintCallable, Category = "CSVFile|Function")
		bool LocationByStr(const FString& szfield, const FString& szval);
	UFUNCTION(BlueprintCallable, Category = "CSVFile|Function")
		bool LocationByTwo(const FString& szfield1, const FString& szfield2, int32 nval1, int32 nval2);
	UFUNCTION(BlueprintPure, Category = "CSVFile|Function")
		bool Eof();
	UFUNCTION(BlueprintPure, Category = "CSVFile|Function")
		int32  RecNum();
	UFUNCTION(BlueprintPure, Category = "CSVFile|Function")
		int32  RecNo();
	UFUNCTION(BlueprintCallable, Category = "CSVFile|Function")
		void SetRecNo(int32 nno);
	UFUNCTION(BlueprintCallable, Category = "CSVFile|Function")
		void First();
	UFUNCTION(BlueprintCallable, Category = "CSVFile|Function")
		void Last();
	UFUNCTION(BlueprintCallable, Category = "CSVFile|Function")
		void Next();
};

