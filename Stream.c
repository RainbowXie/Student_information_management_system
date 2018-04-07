# include "Stream.h"
# include "UI.h"

int iSID = 1;	//学生结构体编号

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
int GetStuInfo(PSTUINFO* stStuInfo, PINFOID pstInfoID)
{
	char szTmpGender[8] = {0};
	short int sTmpGender = 0;
	int iTmpStuID = 0;
	char szTmpStuName[1024] = {0};
	/*char szTmpStuPhNub[32] = { 0 };*/
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
	/*GetStuInfoUI(STUPHNUB);
	scanf("%31s", szTmpStuPhNub);*/
	GetStuInfoUI(STUCSCOR);
	scanf("%7f", &fTmpCScore);
	
	pstInfoID->m_iID = iSID++;
	pstInfoID->m_iLen = sizeof(STUINFO) + strlen(szTmpStuName) + 1;

	*stStuInfo = (STUINFO *)malloc(pstInfoID->m_iLen);
	(*stStuInfo)->iStuID = iTmpStuID;
	(*stStuInfo)->sGender = sTmpGender;
	(*stStuInfo)->stStuBirth.iYear = iTmpYear;
	(*stStuInfo)->stStuBirth.iMonth = iTmpMonth;
	(*stStuInfo)->stStuBirth.iDate = iTmpDate;
	(*stStuInfo)->fCScore = fTmpCScore;
	(*stStuInfo)->iNameLen = strlen(szTmpStuName) + 1;
	/*(*stStuInfo)->iPhoneLen = strlen(szTmpStuPhNub) + 1;*/
	memmove((*stStuInfo)->szStuName, szTmpStuName, strlen(szTmpStuName) + 1);
	/*memmove((*stStuInfo)->szPhoneNub + (*stStuInfo)->iNameLen - 1, szTmpStuPhNub, strlen(szTmpStuPhNub) + 1);*/

	return iRet;
}

int ShowStuInfo(PSTUINFO pstStuInfo, PINFOID pstInfoID)
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

	return 0;
}
/*
功能：将pstStuInfo和pstInfoID写入文件中，并置为空
参数：pstStuInfo是指向结构体指针的指针；pstInfoID是指向结构体的指针
*/
int WriteToFile(FILE *fp, PSTUINFO* pstStuInfo, PINFOID pstInfoID)
{
	int iRet = 0;
	int fd = 0;
	FindSpace(fp, pstInfoID);
	if (1 != fwrite(pstInfoID, INFOIDSIZ, 1, fp))
	{
		ErrProc(FWRITE);
		iRet = FWRITE;
		goto ERR_PROC;
	}
	if (1 != fwrite(*pstStuInfo, pstInfoID->m_iLen, 1, fp))
	{
		ErrProc(FWRITE);
		iRet = FWRITE;
		goto ERR_PROC;
	}
	fflush(fp);
	fd = _fileno(fp); //获取文件描述符
	_commit(fd); //强制写硬盘

	FlushStu(pstStuInfo, pstInfoID);

ERR_PROC:

	return iRet;
}

/*
功能：在文件中找到足够的连续的空闲空间，并把文件指针指向该空间
参数：fp是指向文件的指针，pstInfoID是指向学生信息长度的结构体的指针
*/
void FindSpace(FILE* fp, PINFOID pstInfoID)
{
	char* pcTmp_ptr = NULL;
	long int i = 0;
	int iCount = 0;
	int iLen = pstInfoID->m_iLen + INFOIDSIZ;
	char ch = '\0';
	rewind(fp);
	ch = fgetc(fp);
	pcTmp_ptr = fp->_base;
	/*pcTmp_ptr = fp->_ptr;*/
	while ( EOF != ch)
	{
		if ('\0' == ch)
		{
			iCount++;
		}
		else
		{
			iCount = 0;
			pcTmp_ptr = fp->_ptr + 1;
		}
		if (iCount >= iLen)
		{
			i = (int)pcTmp_ptr - (int)fp->_base;
			i = fseek(fp, i, SEEK_SET);
			break;
		}
		ch = fgetc(fp);
	}
	return;
}

int Delete(FILE *fp,PINFOID pstInfoID, int iID2)
{
	int fd = 0;
	int iRet = 0;
	char ch = -1;
	char cTmpBuf = '\0';	//写入值
	int iCount = 0;	//记录写入个数

	rewind(fp);
	while (1)
	{
		while ('\0' == (ch = fgetc(fp)));
		fseek(fp, -1, SEEK_CUR);	//多读了一位
		if (EOF == ch)
		{
			iRet = DELERR;
			break;
		}
		
		fread(pstInfoID, INFOIDSIZ, 1, fp);	//读出学生信息编号大小
		if (iID2 != pstInfoID->m_iID)
		{
			fseek(fp, pstInfoID->m_iLen, SEEK_CUR);
			continue;
		}
		else
		{
			//while (pstInfoID->m_iLen == (iCount = fwrite(&cTmpBuf, 1, pstInfoID->m_iLen, fp)))
			//{
			//	fflush(fp);
			//}
			fseek(fp, 0, SEEK_CUR);
			for (int i = 0; i < pstInfoID->m_iLen; i++)
			{
				fputc(0, fp);
			}
			fflush(fp);
			fd = _fileno(fp); //获取文件描述符
			iRet = _commit(fd); //强制写硬盘
			fseek(fp, -8 - pstInfoID->m_iLen, SEEK_CUR);
			for (int i = 0; i < INFOIDSIZ; i++)
			{
				fputc(0, fp);
			}
			fflush(fp);
			fd = _fileno(fp); //获取文件描述符
			_commit(fd); //强制写硬盘
			break;
		}
	}

	return iRet;
}

/*
功能：将pstStuInfo和pstInfoID置为空
*/
int FlushStu(PSTUINFO* pstStuInfo, PINFOID pstInfoID)
{
	pstInfoID->m_iID = 0;
	pstInfoID->m_iLen = 0;
	if (NULL != *pstStuInfo)
	{
		free(*pstStuInfo);
		*pstStuInfo = NULL;
	}
	return 0;
}

void ReadFromFile(FILE* fp, PSTUINFO* stStuInfo, PINFOID pstInfoID, int iID2)
{
	rewind(fp);
	while (1)
	{
		while ('\0' == fgetc(fp));
		
		fread(pstInfoID, INFOIDSIZ, 1, fp);
		if (iID2 != pstInfoID->m_iID)
		{
			fseek(fp, pstInfoID->m_iLen, SEEK_CUR);
			continue;
		}
		else
		{
			*stStuInfo = (STUINFO *)malloc(pstInfoID->m_iLen);
			fread(*stStuInfo, pstInfoID->m_iLen, 1, fp);
			break;
		}
	}

}

/*
初始化文件文件，指针指向文件开头
*/
void InitFile(FILE** fp)
{
	*fp = NULL;
	FILE *fTmpFp = NULL;
	int i = 0;
	if (NULL == (fTmpFp = fopen("data.bin", "rb+")))
	{
		ErrProc(INITERR);
	}
	*fp = fTmpFp;
	//fseek(fTmpFp, 0L, SEEK_END);
	//i = ftell(fTmpFp);
	return;
}

/*
功能：关闭文件
参数：指向文件指针的指针
*/

void CloseFile(FILE** fp)
{

	if (NULL != *fp)
	{
		fclose(*fp);
		*fp = NULL;
	}
}