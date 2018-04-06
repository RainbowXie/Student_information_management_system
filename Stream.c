# include "Stream.h"
# include "UI.h"



/*
功能：获取选项
返回值：如果获取正确选项，返回选项值，如果失败，返回WRONGOPT.
*/
int GetOption()
{
	int iRet = WRONGOPT;
	scanf("%1d", &iRet);
	if (iRet > 6 || iRet < 0)
	{
		iRet = WRONGOPT;
	}

	return iRet;
}

/*
功能：获取学生信息，保存到学生结构体内。
*/
int GetStuInfo(PSTUINFO* stStuInfo)
{
	char szTmpGender[8] = {0};
	short int sTmpGender = 0;
	int iTmpStuID = 0;
	char szTmpStuName[1024] = {0};
	char szTmpStuPhNub[32] = { 0 };
	int iTmpYear = 0;
	int iTmpMonth = 0;
	int iTmpDate = 0;
	float fTmpCScore = 0;
	int iRet = WRONGSTUINFO;

	GetStuInfoUI(STUID);
	scanf("%3d", &iTmpStuID);
	GetStuInfoUI(STUNAME);
	scanf("%1023s", szTmpStuName);

	GetStuInfoUI(STUGENDER);
	scanf("%s", szTmpGender);
	if (!strcmp(szTmpGender, "男"))
	{
		sTmpGender = MAN;
	}
	else if (!strcmp(szTmpGender, "女"))
	{
		sTmpGender = WOMAN;
	}
	else
	{
		sTmpGender = UNKNOWN;
	}

	GetStuInfoUI(STUBIRTH);
	scanf("%d-%d-%d", &iTmpYear, &iTmpMonth, &iTmpDate);
	GetStuInfoUI(STUPHNUB);
	scanf("%31s", szTmpStuPhNub);
	GetStuInfoUI(STUCSCOR);
	scanf("%7f", &fTmpCScore);
	
	*stStuInfo = (STUINFO *)malloc(sizeof(STUINFO) + strlen(szTmpStuPhNub) + 1 + strlen(szTmpStuName) + 1);
	(*stStuInfo)->iStuID = iTmpStuID;
	(*stStuInfo)->sGender = sTmpGender;
	(*stStuInfo)->stStuBirth.iYear = iTmpYear;
	(*stStuInfo)->stStuBirth.iMonth = iTmpMonth;
	(*stStuInfo)->stStuBirth.iDate = iTmpDate;
	(*stStuInfo)->fCScore = fTmpCScore;
	(*stStuInfo)->iNameLen = strlen(szTmpStuName) + 1;
	(*stStuInfo)->iPhoneLen = strlen(szTmpStuPhNub) + 1;
	memmove((*stStuInfo)->szStuName, szTmpStuName, strlen(szTmpStuName) + 1);
	memmove((*stStuInfo)->szPhoneNub + (*stStuInfo)->iNameLen - 1, szTmpStuPhNub, strlen(szTmpStuPhNub) + 1);

	return iRet;
}

int ShowStuInfo(PSTUINFO pstStuInfo)
{
	printf("%d\n", pstStuInfo->iStuID);
	for (int i = 0; i < pstStuInfo->iNameLen; i++)
	{
		putchar(pstStuInfo->szStuName[i]);
	}
	if (MAN == pstStuInfo->sGender)
	{
		printf("男\n");
	}
	else if (WOMAN == pstStuInfo->sGender)
	{
		printf("女\n");
	}
	else
	{
		printf("其他\n");
	}
	printf("%d.%d.%d\n", pstStuInfo->stStuBirth.iYear, pstStuInfo->stStuBirth.iMonth, pstStuInfo->stStuBirth.iDate);
	printf("%f\n", pstStuInfo->fCScore);

}
int WriteToFile()
{

}