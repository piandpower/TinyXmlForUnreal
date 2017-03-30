

#include "UnrealTinyXmlPrivatePCH.h"
#include "XmlToCsv.h"
#include "tinyxml2.h"
#include <string>

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

void UCsvField::SplitToStr(const FString& Delimiter, TArray<FString>& out)
{
	out.Empty();

	FString sRight = *mpStr;

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

void UCsvField::SplitToInt(const FString& Delimiter, TArray<int32>& out)
{
	out.Empty();

	FString sRight = *mpStr;

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

void UCsvField::SplitToFloat(const FString& Delimiter, TArray<float>& out)
{
	out.Empty();

	FString sRight = *mpStr;

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

UCsvField*	UXmlToCsv::mField = nullptr;
TMap<FString, UXmlToCsv*> UXmlToCsv::mmXmlToCsvs;

UXmlToCsv::UXmlToCsv()
	: mnCurrentRow(0), mnCol(0), mnRow(0)
{
}

UXmlToCsv::~UXmlToCsv()
{
}

void TCharToChar(const FString& inStr, std::string& outStr)
{
#ifdef _UNICODE
	const int BuffMax = 1024;
	char convertTemp[BuffMax];
	int nLen = WideCharToMultiByte(CP_ACP, 0, *inStr, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, *inStr, -1, convertTemp, nLen, 0, 0);
	outStr = convertTemp;
#else   
	outStr = *inStr;
#endif  
}

UXmlToCsv* UXmlToCsv::OpenXmlToCsv(const FString& szfilename)
{
	UXmlToCsv* pRetXmlToCsv = nullptr;

	UXmlToCsv** ppFind = mmXmlToCsvs.Find(szfilename);
	if (ppFind)
		pRetXmlToCsv = *ppFind;
	else
	{
		XMLDocument doc;
		FString path = FPaths::GameContentDir() + szfilename;

		std::string cstr;
		TCharToChar(path, cstr);

		//读取Xml.
		doc.LoadFile(cstr.c_str());
		if (doc.Error()) return pRetXmlToCsv;

		//常量
		const char* WORKSHEET = "Worksheet";
		const char* TABLE = "Table";
		const char* ROW = "Row";
		const char* CELL = "Cell";
		const char* DATA = "Data";

		//获取表元素
		XMLElement* pTable = doc.RootElement()->FirstChildElement(WORKSHEET)->FirstChildElement(TABLE);

		//获取Excel行数
		int nRow = 0;

		for (XMLElement* pRow = pTable->FirstChildElement(ROW); pRow; pRow = pRow->NextSiblingElement(ROW))
		{
			nRow++;
		}

		//总数-1，忽略第一行中文字段名
		nRow--;
		if (nRow > 1)
		{
			//跳到Excel表第二行(英文字段名)
			XMLElement* pRow = pTable->FirstChildElement(ROW);
			pRow = pRow->NextSiblingElement(ROW);//为第二行

												 //获取列数
			int nCol = 0;

			for (XMLElement* pCell = pRow->FirstChildElement(CELL); pCell; pCell = pCell->NextSiblingElement(CELL))
			{
				nCol++;
			}

			//保存行列数
			pRetXmlToCsv = NewObject<UXmlToCsv>();
			pRetXmlToCsv->AddToRoot();
			pRetXmlToCsv->mnCol = nCol;
			pRetXmlToCsv->mnRow = nRow - 1;
			//初始化数据数组
			pRetXmlToCsv->mvDatas.Empty(nCol * nRow);
			pRetXmlToCsv->mvDatas.AddDefaulted(nCol * nRow);

			int ntmpRow = 0;
			//从第二行开始遍历所有行,将每一行的所有列的数据存入mvDatas里
			for (; pRow; pRow = pRow->NextSiblingElement(ROW))
			{
				int ntmpPos = 0;
				for (XMLElement* pCell = pRow->FirstChildElement(CELL); pCell; pCell = pCell->NextSiblingElement(CELL))
				{
					XMLElement* pData = pCell->FirstChildElement(DATA);
					if (pData)
						pRetXmlToCsv->mvDatas[ntmpRow*nCol + ntmpPos] = UTF8_TO_TCHAR(pData->GetText());
					ntmpPos++;
					if (ntmpPos > nCol) break;
				}
				ntmpRow++;
				if (ntmpRow > nRow) break;
			}

			//Excel表第二行每一列加入字段列表
			for (int i = 0; i < pRetXmlToCsv->mnCol; i++)
			{
				pRetXmlToCsv->mmFieldNames.Add(pRetXmlToCsv->mvDatas[i], i);
			}

			pRetXmlToCsv->mnCurrentRow = 1;
		}

		doc.Clear();

		mmXmlToCsvs.Add(szfilename, pRetXmlToCsv);

		UE_LOG(LogTemp, Warning, TEXT("行:%d"), pRetXmlToCsv->mnRow);
		UE_LOG(LogTemp, Warning, TEXT("列:%d"), pRetXmlToCsv->mnCol);
	}
	
	return pRetXmlToCsv;
}

void UXmlToCsv::CloseXmlToCsv(const FString& szfilename)
{
	UXmlToCsv** ppFind = mmXmlToCsvs.Find(szfilename);
	if (ppFind)
	{
		mmXmlToCsvs.Remove(szfilename);
		UXmlToCsv* pFind = *ppFind;
		pFind->mmFieldNames.Empty();
		pFind->mvDatas.Empty();

		pFind->RemoveFromRoot();

		if (pFind->IsValidLowLevel())
		{
			pFind->ConditionalBeginDestroy();
		}
	}
}

UCsvField* UXmlToCsv::FieldByName(const FString& szname)
{
	int32* pnfind = mmFieldNames.Find(szname);
	if (pnfind == nullptr) return NULL;

	if (mField == nullptr)
	{
		mField = NewObject<UCsvField>();
		mField->AddToRoot();
	}
	
	mField->Init(&(mvDatas[mnCurrentRow*mnCol + *pnfind]));
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

bool UXmlToCsv::LocationByTwo(const FString& szfield1, const FString& szfield2, int32 nval1, int32 nval2)
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
				&& mvDatas[npos*mnCol + *pnfind1] == sztmp2)
			{
				mnCurrentRow = npos;
				return true;
			}
		}
		mnCurrentRow = mnRow + 1;
	}

	return false;
}

bool UXmlToCsv::Eof() { return mnCurrentRow > mnRow; }
int32  UXmlToCsv::GetTotalRow() { return mnRow; }
int32  UXmlToCsv::GetCurrentRow() { return mnCurrentRow; }
void UXmlToCsv::SetCurrentRow(int32 nno) 
{	
	if (nno > 0 && nno <= this->mnRow)
	{
		mnCurrentRow = nno;
	}
	else
	{
		mnCurrentRow = 1;
		UE_LOG(LogTemp, Error, TEXT("设置的Xml当前行不合法,所以回到第一行."));
	}
}
void UXmlToCsv::First() { mnCurrentRow = 1; }
void UXmlToCsv::Last() { if (mnCurrentRow <= mnRow) mnCurrentRow = mnRow; }
void UXmlToCsv::Next() { if (mnCurrentRow < mnRow) mnCurrentRow++; }
