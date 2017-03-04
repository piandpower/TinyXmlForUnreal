

#include "UnrealTinyXmlPrivatePCH.h"
#include "XmlToCsv.h"
#include "tinyxml2.h"

using namespace tinyxml2;

//-----------------------------------------------------------------------------------------------------------------

bool UCsvField::AsBool()
{
	return mpStr->ToBool();
}

int32 UCsvField::AsInt()
{
	return FCString::Atoi(**mpStr);
}

float UCsvField::AsFloat()
{
	return FCString::Atof(**mpStr);
}

const FString& UCsvField::AsStr()
{
	return *mpStr;
}

//-----------------------------------------------------------------------------------------------------------------

UXmlToCsv::UXmlToCsv()
	: mnPos(0), mnCol(0), mnRow(0), mField(nullptr)
{
}

UXmlToCsv::~UXmlToCsv()
{
}

UXmlToCsv* UXmlToCsv::OpenXmlToCsv(const FString& szfilename)
{
	UXmlToCsv* pRetXmlToCsv = nullptr;
	XMLDocument doc;
	FString path = FPaths::GameDir() + szfilename;
	doc.LoadFile(TCHAR_TO_ANSI(*path));
	if (doc.Error()) return pRetXmlToCsv;
	const char* WORKSHEET	= "Worksheet";
	const char* TABLE		= "Table";
	const char* ROW			= "Row";
	const char* CELL		= "Cell";
	const char* DATA		= "Data";

	XMLElement* pTable = doc.RootElement()->FirstChildElement(WORKSHEET)->FirstChildElement(TABLE);
	
	int nRow = 0;

	//get excel table row
	for (XMLElement* pRow = pTable->FirstChildElement(ROW); pRow; pRow = pRow->NextSiblingElement(ROW))
	{
		nRow++;
	}

	nRow--;

	if (nRow > 1)
	{
		XMLElement* pRow = pTable->FirstChildElement(ROW);
		pRow = pRow->NextSiblingElement(ROW);

		int nCol = 0;

		for (XMLElement* pCell = pRow->FirstChildElement(CELL); pCell; pCell = pCell->NextSiblingElement(CELL))
		{
			nCol++;
		}

		pRetXmlToCsv = NewObject<UXmlToCsv>();
		pRetXmlToCsv->mnCol = nCol;
		pRetXmlToCsv->mnRow = nRow;

		pRetXmlToCsv->mvDatas.Empty(nCol * nRow);
		pRetXmlToCsv->mvDatas.AddDefaulted(nCol * nRow);

		int ntmpRow = 0;

		for (; pRow; pRow = pRow->NextSiblingElement(ROW))
		{
			int ntmpPos = 0;
			for (XMLElement* pCell = pRow->FirstChildElement(CELL); pCell; pCell = pCell->NextSiblingElement(CELL))
			{
				XMLElement* pData = pCell->FirstChildElement(DATA);
				if (pData) pRetXmlToCsv->mvDatas[ntmpRow*nCol + ntmpPos] = UTF8_TO_TCHAR(pData->GetText());
				ntmpPos++;
				if (ntmpPos > nCol) break;
			}
			ntmpRow++;
			if (ntmpRow > nRow) break;
		}

		for (int i=0; i<pRetXmlToCsv->mnCol; i++)
		{
			pRetXmlToCsv->mmFieldNames.Add(pRetXmlToCsv->mvDatas[i], i);
		}

		pRetXmlToCsv->mnPos = 1;
		pRetXmlToCsv->mField = NewObject<UCsvField>();
	}

	doc.Clear();

	return pRetXmlToCsv;
}

void UXmlToCsv::Close()
{
	mmFieldNames.Empty();
	mvDatas.Empty();

	if (mField->IsValidLowLevel())
	{
		mField->ConditionalBeginDestroy();
		mField = nullptr;
	}

	if (this->IsValidLowLevel())
	{
		this->ConditionalBeginDestroy();
	}
}

UCsvField* UXmlToCsv::FieldByName(const FString& szname)
{
	int32* pnfind = mmFieldNames.Find(szname);
	if (pnfind == nullptr || mField == nullptr) return NULL;

	mField->Init(&(mvDatas[mnPos*mnCol + *pnfind]));
	return mField;
}

bool UXmlToCsv::LocationByInt(const FString& szfield, int32 nval)
{
	FString sztmp1 = FString::FromInt(nval);
	return LocationByStr(szfield, sztmp1);
}

bool UXmlToCsv::LocationByStr(const FString& szfield, const FString& szval)
{
	int32* pnfind = mmFieldNames.Find(szfield);
	if (pnfind != nullptr)
	{
		for (int npos = mnPos; npos <= mnRow; npos++)
		{
			if (mvDatas[npos*mnCol + *pnfind] == szval)
			{
				mnPos = npos;
				return true;
			}
		}
		for (int npos = 1; npos < mnPos; npos++)
		{
			if (mvDatas[npos*mnCol + *pnfind] == szval)
			{
				mnPos = npos;
				return true;
			}
		}
		mnPos = mnRow + 1;
	}
	return false;
}

bool UXmlToCsv::LocationByTwo(const FString& szfield1, const FString& szfield2, int32 nval1, int32 nval2)
{
	FString sztmp1 = FString::FromInt(nval1);
	FString sztmp2 = FString::FromInt(nval2);

	int32* pnfind1 = mmFieldNames.Find(szfield1);
	int32* pnfind2 = mmFieldNames.Find(szfield2);

	if (pnfind1 && pnfind2)
	{
		for (int npos = mnPos; npos <= mnRow; npos++)
		{
			if (mvDatas[npos*mnCol + *pnfind1] == sztmp1
				&& mvDatas[npos*mnCol + *pnfind2] == sztmp2)
			{
				mnPos = npos;
				return true;
			}
		}
		for (int npos = 1; npos < mnPos; npos++)
		{
			if (mvDatas[npos*mnCol + *pnfind1] == sztmp1
				&& mvDatas[npos*mnCol + *pnfind1] == sztmp2)
			{
				mnPos = npos;
				return true;
			}
		}
		mnPos = mnRow + 1;
	}

	return false;
}

bool UXmlToCsv::Eof() { return mnPos > mnRow; }
int32  UXmlToCsv::RecNum() { return mnRow; }
int32  UXmlToCsv::RecNo() { return mnPos; }
void UXmlToCsv::SetRecNo(int32 nno) { mnPos = nno; }
void UXmlToCsv::First() { mnPos = 1; }
void UXmlToCsv::Last() { if (mnPos < mnRow) mnPos = mnRow; }
void UXmlToCsv::Next() { if (mnPos <= mnRow) mnPos++; }
