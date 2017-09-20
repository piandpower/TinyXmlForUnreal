#include "UnrealTinyXmlPrivatePCH.h"
#include "ExcelBasedXml.h"
#include "tinyxml2.h"
#include <string>

using namespace tinyxml2;

//-----------------------------------------------------------------------------------------------------------------

bool UExcelField::AsBool()
{
	return mpStr->ToBool();
}

int32 UExcelField::AsInt()
{
	return FCString::Atoi(**mpStr);
}

float UExcelField::AsFloat()
{
	return FCString::Atof(**mpStr);
}

const FString& UExcelField::AsStr()
{
	return *mpStr;
}

void UExcelField::SplitToStr(const FString& Str, const FString& Delimiter, TArray<FString>& out)
{
	out.Empty();

	FString sRight = Str;

	while (sRight.Len())
	{
		int32 InPos = sRight.Find(Delimiter);
		if (InPos > 0)
		{
			out.Add(sRight.Left(InPos));
			sRight = sRight.Mid(InPos + Delimiter.Len());
		}
		else if (InPos == 0)
		{
			sRight = sRight.Mid(InPos + Delimiter.Len());
		}
		else
		{
			out.Add(sRight);
			break;
		}
	}
}

void UExcelField::SplitToInt(const FString& Str, const FString& Delimiter, TArray<int32>& out)
{
	out.Empty();

	FString sRight = Str;

	while (sRight.Len())
	{
		int32 InPos = sRight.Find(Delimiter);
		if (InPos > 0)
		{
			out.Add(FCString::Atoi(*(sRight.Left(InPos))));
			sRight = sRight.Mid(InPos + Delimiter.Len());
		}
		else if (InPos == 0)
		{
			sRight = sRight.Mid(InPos + Delimiter.Len());
		}
		else
		{
			out.Add(FCString::Atoi(*sRight));
			break;
		}
	}
}

void UExcelField::SplitToFloat(const FString& Str, const FString& Delimiter, TArray<float>& out)
{
	out.Empty();

	FString sRight = Str;

	while (sRight.Len())
	{
		int32 InPos = sRight.Find(Delimiter);
		if (InPos > 0)
		{
			out.Add(FCString::Atof(*(sRight.Left(InPos))));
			sRight = sRight.Mid(InPos + Delimiter.Len());
		}
		else if (InPos == 0)
		{
			sRight = sRight.Mid(InPos + Delimiter.Len());
		}
		else
		{
			out.Add(FCString::Atof(*sRight));
			break;
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------

UExcelField*	UExcelBasedXml::mField = nullptr;
TMap<FString, UExcelBasedXml*> UExcelBasedXml::mmTables;
FString UExcelBasedXml::xmlRootDir = FPaths::GameContentDir();

UExcelBasedXml::UExcelBasedXml()
	: mnCurrentRow(0), mnCol(0), mnRow(0)
{
}

UExcelBasedXml::~UExcelBasedXml()
{
}


void UExcelBasedXml::InitProperties(const FString& rootDir)
{
	UExcelBasedXml::xmlRootDir = rootDir;
}

UExcelBasedXml* UExcelBasedXml::OpenXmlTable(const FString& szfilename)
{
	UExcelBasedXml* pRetXmlToCsv = nullptr;

	UExcelBasedXml** ppFind = mmTables.Find(szfilename);
	if (ppFind)
		pRetXmlToCsv = *ppFind;
	else
	{
		XMLDocument doc;
		FString path = FPaths::GameContentDir() + szfilename;


		
		doc.LoadFile(TCHAR_TO_UTF8(*path));
		if (doc.Error()) return pRetXmlToCsv;

		const char* WORKSHEET = "Worksheet";
		const char* TABLE = "Table";
		const char* ROW = "Row";
		const char* CELL = "Cell";
		const char* DATA = "Data";

		XMLElement* pTable = doc.RootElement()->FirstChildElement(WORKSHEET)->FirstChildElement(TABLE);

		int nRow = 0;

		for (XMLElement* pRow = pTable->FirstChildElement(ROW); pRow; pRow = pRow->NextSiblingElement(ROW))
		{
			nRow++;
		}


		if (nRow > 1)
		{
			XMLElement* pRow = pTable->FirstChildElement(ROW);

			int nCol = 0;

			for (XMLElement* pCell = pRow->FirstChildElement(CELL); pCell; pCell = pCell->NextSiblingElement(CELL))
			{
				nCol++;
			}

			pRetXmlToCsv = NewObject<UExcelBasedXml>();
			//Add to root to prevent grab collection.
			pRetXmlToCsv->AddToRoot();
			pRetXmlToCsv->mnCol = nCol;
			//Fill rows(exclude first row which is field row.)
			pRetXmlToCsv->mnRow = nRow - 1;
			//Init data array.
			pRetXmlToCsv->mvDatas.Empty(nCol * nRow);
			pRetXmlToCsv->mvDatas.AddDefaulted(nCol * nRow);

			int ntmpRow = 0;
			for (; pRow; pRow = pRow->NextSiblingElement(ROW))
			{
				int ntmpPos = 0;
				for (XMLElement* pCell = pRow->FirstChildElement(CELL); pCell; pCell = pCell->NextSiblingElement(CELL))
				{
					if (int newPos = pCell->IntAttribute("ss:Index"))
					{
						ntmpPos += newPos - ntmpPos - 1;
					}
					
					XMLElement* pData = pCell->FirstChildElement(DATA);
					if (pData)
						pRetXmlToCsv->mvDatas[ntmpRow*nCol + ntmpPos] = UTF8_TO_TCHAR(pData->GetText());
					ntmpPos++;
					if (ntmpPos > nCol) break;
				}
				ntmpRow++;
				if (ntmpRow > nRow) break;
			}

			for (int i = 0; i < pRetXmlToCsv->mnCol; i++)
			{
				pRetXmlToCsv->mmFieldNames.Add(pRetXmlToCsv->mvDatas[i], i);
			}

			pRetXmlToCsv->mnCurrentRow = 1;
		}

		doc.Clear();

		mmTables.Add(szfilename, pRetXmlToCsv);

	}
	
	return pRetXmlToCsv;
}

void UExcelBasedXml::CloseXmlTable(const FString& szfilename)
{
	UExcelBasedXml** ppFind = mmTables.Find(szfilename);
	if (ppFind)
	{
		mmTables.Remove(szfilename);
		UExcelBasedXml* pFind = *ppFind;
		pFind->mmFieldNames.Empty();
		pFind->mvDatas.Empty();

		pFind->RemoveFromRoot();

		if (pFind->IsValidLowLevel())
		{
			pFind->ConditionalBeginDestroy();
		}
	}
}

UExcelField* UExcelBasedXml::GetFieldByName(const FString& szname)
{
	int32* pnfind = mmFieldNames.Find(szname);
	if (pnfind == nullptr) return NULL;

	if (mField == nullptr)
	{
		mField = NewObject<UExcelField>();
		mField->AddToRoot();
	}
	
	mField->Init(&(mvDatas[mnCurrentRow*mnCol + *pnfind]));
	return mField;
}

bool UExcelBasedXml::LocateByInt(const FString& szfield, int32 nval)
{
	FString sztmp1 = FString::FromInt(nval);
	return LocateByStr(szfield, sztmp1);
}

bool UExcelBasedXml::LocateByStr(const FString& szfield, const FString& szval)
{
	int32* pnfind = mmFieldNames.Find(szfield);
	if (pnfind != nullptr)
	{
		for (int npos = mnCurrentRow; npos <= mnRow; npos++)
		{
			if (mvDatas[npos*mnCol + *pnfind] == szval)
			{
				mnCurrentRow = npos;
				return true;
			}
		}
		for (int npos = 1; npos < mnCurrentRow; npos++)
		{
			if (mvDatas[npos*mnCol + *pnfind] == szval)
			{
				mnCurrentRow = npos;
				return true;
			}
		}
		mnCurrentRow = mnRow + 1;
	}
	return false;
}

bool UExcelBasedXml::LocationByTwo(const FString& szfield1, const FString& szfield2, int32 nval1, int32 nval2)
{
	FString sztmp1 = FString::FromInt(nval1);
	FString sztmp2 = FString::FromInt(nval2);

	int32* pnfind1 = mmFieldNames.Find(szfield1);
	int32* pnfind2 = mmFieldNames.Find(szfield2);

	if (pnfind1 && pnfind2)
	{
		for (int npos = mnCurrentRow; npos <= mnRow; npos++)
		{
			if (mvDatas[npos*mnCol + *pnfind1] == sztmp1
				&& mvDatas[npos*mnCol + *pnfind2] == sztmp2)
			{
				mnCurrentRow = npos;
				return true;
			}
		}
		for (int npos = 1; npos < mnCurrentRow; npos++)
		{
			if (mvDatas[npos*mnCol + *pnfind1] == sztmp1
				&& mvDatas[npos*mnCol + *pnfind2] == sztmp2)
			{
				mnCurrentRow = npos;
				return true;
			}
		}
		mnCurrentRow = mnRow + 1;
	}

	return false;
}

bool UExcelBasedXml::Eof() { return mnCurrentRow > mnRow; }
int32  UExcelBasedXml::GetTotalRow() { return mnRow; }
int32  UExcelBasedXml::GetCurrentRow() { return mnCurrentRow; }
void UExcelBasedXml::SetCurrentRow(int32 nno) 
{	
	if (nno > 0 && nno <= this->mnRow)
	{
		mnCurrentRow = nno;
	}
	else
	{
		mnCurrentRow = 1;
	}
}
void UExcelBasedXml::First() { mnCurrentRow = 1; }
void UExcelBasedXml::Last() { if (mnCurrentRow <= mnRow) mnCurrentRow = mnRow; }
void UExcelBasedXml::Next() { if (mnCurrentRow < mnRow) mnCurrentRow++; }
