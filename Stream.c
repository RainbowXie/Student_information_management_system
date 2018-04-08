# include "Stream.h"
# include "UI.h"

/*
功能：获取选项
返回值：如果获取正确选项，返回选项值，如果失败，返回WRONGOPT.
*/
int GetOption()
{
	int iRet = WRONGOPT;
	fflush(stdin);
	scanf("%1d", &iRet);
	fflush(stdin);
	if (iRet > 7 || iRet < 0)
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
	fflush(stdin);
	scanf("%3d", &iTmpStuID);
	fflush(stdin);
	GetStuInfoUI(STUNAME);
	fflush(stdin);
	scanf("%1023s", szTmpStuName);
	fflush(stdin);
	GetStuInfoUI(STUGENDER);
	fflush(stdin);
	scanf("%7s", szTmpGender);
	fflush(stdin);
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
	fflush(stdin);
	scanf("%5d.%2d.%2d", &iTmpYear, &iTmpMonth, &iTmpDate);
	fflush(stdin);
	/*GetStuInfoUI(STUPHNUB);
	scanf("%31s", szTmpStuPhNub);*/
	GetStuInfoUI(STUCSCOR);
	fflush(stdin);
	scanf("%7f", &fTmpCScore);
	fflush(stdin);

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
	printf("\r\n\n");
	printf("学号：%d\n", pstStuInfo->iStuID);
	printf("名字：");
	for (int i = 0; i < pstStuInfo->iNameLen; i++)
	{
		putchar(pstStuInfo->szStuName[i]);
	}
	printf("\r\n");
	if (MAN == pstStuInfo->sGender)
	{
		printf("性别：男\r\n");
	}
	else if (WOMAN == pstStuInfo->sGender)
	{
		printf("性别：女\r\n");
	}
	else
	{
		printf("性别：其他\r\n");
	}
	printf("生日：%d.%d.%d\r\n", pstStuInfo->stStuBirth.iYear, pstStuInfo->stStuBirth.iMonth, pstStuInfo->stStuBirth.iDate);
	printf("分数：%f\r\n", pstStuInfo->fCScore);
	printf("\r\n\n");
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
	WriteInFile(fp);

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
			pcTmp_ptr = fp->_ptr;
		}
		if (iCount >= iLen)
		{
			i = (int)pcTmp_ptr - (int)fp->_base;
			fseek(fp, i, SEEK_SET);
			break;
		}
		ch = fgetc(fp);
	}
	return;
}

int ModifyStu(FILE *fp, PINFOID pstInfoID, PSTUINFO* pstStuInfo, int iID)
{
	int fd = 0;
	int iRet = NSEARCH;
	int iCh = -1;
	char cTmpBuf = '\0';	//写入值
	int iCount = 0;	//记录写入个数

	rewind(fp);
	while (iRet)
	{
		while (0 == (iCh = fgetc(fp)));
		if (EOF == iCh)
		{
			//iRet = DELERR;
			break;	//OUT
		}
		fseek(fp, -1, SEEK_CUR);	//多读了一位

		fread(pstInfoID, INFOIDSIZ, 1, fp);	//读出学生信息编号大小
		*pstStuInfo = (STUINFO *)malloc(pstInfoID->m_iLen);	//为学生信息分配空间
		fread(*pstStuInfo, pstInfoID->m_iLen, 1, fp);	//从文件中读出学生信息
		if ((*pstStuInfo)->iStuID != iID)
		{
			continue;
		}
		//修改选项//
		while (iRet)
		{
			iRet = 0;
			ModifyUI(0);
			ShowStuInfo(*pstStuInfo, pstInfoID);
			ModifyUI(2);
			iRet = GetOption();
			if (1 == iRet || 0 == iRet)
			{
				break;
			}
			ModifyUI(3);
			system("pause");
			system("cls");
		}
		if (0 == iRet)
		{
			break;
		}

		/**************删除************************/
		
		fseek(fp, -pstInfoID->m_iLen, SEEK_CUR);
		fseek(fp, 0, SEEK_CUR);
		for (int i = 0; i < pstInfoID->m_iLen; i++)
		{
			fputc(0, fp);
		}
		WriteInFile(fp);
		fseek(fp, -8 - pstInfoID->m_iLen, SEEK_CUR);
		for (int i = 0; i < INFOIDSIZ; i++)
		{
			fputc(0, fp);
		}
		WriteInFile(fp);
		//break;
	}
	if (NULL != *pstStuInfo)
	{
		free(*pstStuInfo);
		*pstStuInfo = NULL;
	}
	//********************增加*******************//
	return iRet;
}

/*
功能：按学号删除学生信息
参数：fp是文件指针；pstInfoID是指向编号结构体的指针；pstStuInfo是指向学生信息结构体的指针的指针。iID是学生学号
*/
int Delete(FILE *fp, PINFOID pstInfoID, PSTUINFO* pstStuInfo, int iID)
{
	int fd = 0;
	int iRet = NSEARCH;
	int iCh = -1;
	char cTmpBuf = '\0';	//写入值
	int iCount = 0;	//记录写入个数

	rewind(fp);
	while (iRet)
	{
		while (0 == (iCh = fgetc(fp)));
		if (EOF == iCh)
		{
			//iRet = DELERR;
			break;	//OUT
		}
		fseek(fp, -1, SEEK_CUR);	//多读了一位

		fread(pstInfoID, INFOIDSIZ, 1, fp);	//读出学生信息编号大小
		*pstStuInfo = (STUINFO *)malloc(pstInfoID->m_iLen);	//为学生信息分配空间
		fread(*pstStuInfo, pstInfoID->m_iLen, 1, fp);	//从文件中读出学生信息
		if ((*pstStuInfo)->iStuID != iID)
		{
			continue;
		}
		//删除选项//
		while (iRet)
		{ 
			iRet = 0;
			DeleteUI(0);
			ShowStuInfo(*pstStuInfo, pstInfoID);
			DeleteUI(2);
			iRet = GetOption();
			if (1 == iRet || 0 == iRet)
			{
				break;
			}
			DeleteUI(3);
			system("pause");
			system("cls");
		}
		if (0 == iRet)
		{
			break;
		}

		/*删除*/
		/***************************************/
		fseek(fp, -pstInfoID->m_iLen, SEEK_CUR);
		fseek(fp, 0, SEEK_CUR);
		for (int i = 0; i < pstInfoID->m_iLen; i++)
		{
			fputc(0, fp);
		}
		WriteInFile(fp);
		fseek(fp, -8 - pstInfoID->m_iLen, SEEK_CUR);
		for (int i = 0; i < INFOIDSIZ; i++)
		{
			fputc(0, fp);
		}
		WriteInFile(fp);
		//break;
		if (NULL != *pstStuInfo)
		{
			free(*pstStuInfo);
			*pstStuInfo = NULL;
		}
	}/*while (iRet) end*/

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
		char ch = '\0';
		int iRet = 0;
		/*while ('\0' == fgetc(fp));*/
		while ('\0' == (ch = fgetc(fp)));
		fseek(fp, -1, SEEK_CUR);	//多读了一位

		if (EOF == ch)
		{
			iRet = DELERR;
			break;
		}
		fread(pstInfoID, INFOIDSIZ, 1, fp);
		if (iID2 != pstInfoID->m_iID)	//如果编号不对，就往后跳学生信息结构体个长度
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

/*
功能：显示所有学生信息
参数：fp是文件指针；pstInfoID是指向编号结构体的指针；pstStuInfo是指向学生信息结构体的指针的指针。
*/
int ShowAllInfo(FILE *fp, PINFOID pstInfoID, PSTUINFO *pstStuInfo)
{
	int fd = 0;
	int iRet = NSEARCH;
	int iCh = -1;
	char cTmpBuf = '\0';	//写入值
	int iCount = 0;	//记录写入个数

	rewind(fp);
	while (iRet)
	{
			while (0 == (iCh = fgetc(fp)));
			if (EOF == iCh)
			{
				//iRet = DELERR;
				break;
			}
			fseek(fp, -1, SEEK_CUR);	//多读了一位
	
			fread(pstInfoID, INFOIDSIZ, 1, fp);	//读出学生信息编号大小
			*pstStuInfo = (STUINFO *)malloc(pstInfoID->m_iLen);
			fread(*pstStuInfo, pstInfoID->m_iLen, 1, fp);
			ShowStuInfo(*pstStuInfo, pstInfoID);
			
			if (NULL != *pstStuInfo)
			{
				free(*pstStuInfo);
				*pstStuInfo = NULL;
			}
	}

	return 0;
}


/*
功能：根据学生信息搜索学生，并显示。
参数：strInfo是无符号指针，接收所有输入的搜索关键字，pstInfoID是指向编号结构体的指针；pstStuInfo是指向学生信息结构体的指针的指针，iOption是选项
返回值：如果找到，返回找到的相同信息的数量，如果没找到，返回NSEARCH。
*/
int SearchByStr(void* strInfo, FILE *fp, PINFOID pstInfoID, PSTUINFO *pstStuInfo, int iOption)
{
	int fd = 0;
	int iRet = NSEARCH;
	int iCh = -1;
	char cTmpBuf = '\0';	//写入值
	int iCount = 0;	//记录写入个数

	rewind(fp);
	while (iRet)
	{
		while (0 == (iCh = fgetc(fp)));
		if (EOF == iCh)
		{
			//iRet = DELERR;
			break;
		}
		fseek(fp, -1, SEEK_CUR);	//多读了一位
		//if (EOF == ch)
		//{
		//	//iRet = DELERR;
		//	break;
		//}

		fread(pstInfoID, INFOIDSIZ, 1, fp);	//读出学生信息编号大小
		*pstStuInfo = (STUINFO *)malloc(pstInfoID->m_iLen);
		fread(*pstStuInfo, pstInfoID->m_iLen, 1, fp);
		switch (iOption)
		{
		case STUID:
		{
			
			if (*(int*)strInfo == (*pstStuInfo)->iStuID)
			{
				iRet++;
				ShowStuInfo(*pstStuInfo, pstInfoID);
				//iRet = 0;
			}

			break;
		}
		case STUNAME:
		{
			if (strstr(((*pstStuInfo)->szStuName), ((char*)strInfo)))
			{
				iRet++;
				ShowStuInfo(*pstStuInfo, pstInfoID);
			}
			//(*pstStuInfo)->szStuName)[i]串，strInfo关键字
			//for (int i = 0; ((*pstStuInfo)->szStuName)[i] != '\0'; i++)
			//{
			//	int k = 0;
			//	for (int j = i, k = 0; ((/*char*/short int*)strInfo)[k] != '\0' && ((*pstStuInfo)->szStuName)[i] == ((short int/*char*/*)strInfo)[k]; j++, k++);

			//	if (k > 0 && ((/*char*/short int*)strInfo)[k] != '\0')
			//	{
			//		ShowStuInfo(*pstStuInfo, pstInfoID);
			//		break;
			//	}
			//}
			//if (0 != iRet)
			//{
			//	free(*pstStuInfo);
			//	*pstStuInfo = NULL;
			//}
			break;
		}
		case STUBIRTH:
		{
			if (((struct tagSTUBIRTH *)strInfo)->iYear == (*pstStuInfo)->stStuBirth.iYear &&
				((struct tagSTUBIRTH *)strInfo)->iMonth == (*pstStuInfo)->stStuBirth.iMonth &&
				((struct tagSTUBIRTH *)strInfo)->iDate == (*pstStuInfo)->stStuBirth.iDate)
			{
				iRet++;
				ShowStuInfo(*pstStuInfo, pstInfoID);
			}
			
			break;
		}
		case STUCSCOR:
		{
			if (*(float*)strInfo == (*pstStuInfo)->fCScore)
			{
				iRet++;
				ShowStuInfo(*pstStuInfo, pstInfoID);
				//iRet = 0;
			}

			break;
		}
		case STUGENDER:
		{
			if (*(short int*)strInfo == (*pstStuInfo)->sGender)
			{
				iRet++;
				ShowStuInfo(*pstStuInfo, pstInfoID);
				//iRet = 0;
			}
			
			break;
		}
		}/*switch(iOption) end*/

		//if (iID2 != pstInfoID->m_iID)
		//{
		//	fseek(fp, pstInfoID->m_iLen, SEEK_CUR);
		//	continue;
		//}
		//else
		//{
		//	while (pstInfoID->m_iLen == (iCount = fwrite(&cTmpBuf, 1, pstInfoID->m_iLen, fp)))
		//	{
		//		fflush(fp);
		//	}
		//	fseek(fp, 0, SEEK_CUR);
		//	for (int i = 0; i < pstInfoID->m_iLen; i++)
		//	{
		//		fputc(0, fp);
		//	}


		//	fflush(fp);
		//	fd = _fileno(fp); //获取文件描述符
		//	iRet = _commit(fd); //强制写硬盘
		//	fseek(fp, -8 - pstInfoID->m_iLen, SEEK_CUR);
			//for (int i = 0; i < INFOIDSIZ; i++)
			//{
			//	fputc(0, fp);
			//}
			//fflush(fp);
			//fd = _fileno(fp); //获取文件描述符
			//_commit(fd); //强制写硬盘
		//	break;
		if (NULL != *pstStuInfo)
		{
			free(*pstStuInfo);
			*pstStuInfo = NULL;
		}
	}/*while(1) end*/
	if (NSEARCH == iRet)
	{
		ErrProc(iRet);
	}

	return iRet;
}
 
/*
强制将缓冲区文件写入硬盘
*/
void WriteInFile(FILE* fp)
{
	int fd = 0;
	fflush(fp);
	fd = _fileno(fp); //获取文件描述符
	_commit(fd); //强制写硬盘
}
/*
功能：统计最高分、最低分、总成绩、平均成绩
*/
void Statistics(FILE *fp, PINFOID pstInfoID, PSTUINFO *pstStuInfo)
{
	float iSum = 0;
	float iHighest = 0;
	float iLowest = 0;
	int iStuCount = 0;
	int iRet = NSEARCH;
	int iCh = -1;
	int iCount = 0;	//记录写入个数
	rewind(fp);
	while (iRet)
	{
		while (0 == (iCh = fgetc(fp)));
		if (EOF == iCh)
		{
			//iRet = DELERR;
			break;
		}
		fseek(fp, -1, SEEK_CUR);	//多读了一位

		fread(pstInfoID, INFOIDSIZ, 1, fp);	//读出学生信息编号大小
		*pstStuInfo = (STUINFO *)malloc(pstInfoID->m_iLen);
		fread(*pstStuInfo, pstInfoID->m_iLen, 1, fp);	//读出学生信息
		//ShowStuInfo(*pstStuInfo, pstInfoID);

		//****************Statistics******************//
		iSum += (*pstStuInfo)->fCScore;
		iCount++;
		if (iHighest < (*pstStuInfo)->fCScore)
		{
			iHighest = (*pstStuInfo)->fCScore;
		}
		
		if (1 == iCount)
		{
			iLowest = (*pstStuInfo)->fCScore;
		}
		else if (iLowest > (*pstStuInfo)->fCScore)
		{
			iLowest = (*pstStuInfo)->fCScore;
		}

		if (NULL != *pstStuInfo)
		{
			free(*pstStuInfo);
			*pstStuInfo = NULL;
		}
	}/*while (iRet) end*/
	printf("本班总分为：%.3f\r\n", iSum);
	printf("本班平均分为：%.3f\r\n", iSum / iCount);
	printf("本班最高分为：%.3f\r\n", iHighest);
	printf("本班最低分为：%.3f\r\n", iLowest);

}

void Storage(FILE *fp, PINFOID pstInfoID, PSTUINFO *pstStuInfo)
{
	int ich = 0;
	int iCount = 0;
	int iOption = 0;
	rewind(fp);
	while (EOF != (ich = fgetc(fp)))
	{
		if (ich == 0)
		{
			putc('-', stdout);
			iCount++;
		}
		else if (0 != ich)
		{
			putc('+', stdout);
			iCount = 0;
		}
		if (200 <= iCount)
		{
			printf("\r\n\n");
			break;
		}
	}
	/**********************碎片整理******************/
	while (1)
	{
		StorageUI(0);
		iOption = GetOption();
		if (1 == iOption || 0 == iOption)
		{
			break;
		}
		ModifyUI(3);
		system("pause");
		system("cls");
	}
	if (0 == iOption)
	{
		return;
	}
	{
		int fd = 0;
		int iRet = NSEARCH;
		int iCh = -1;
		char cTmpBuf = '\0';	//写入值
		int iCount = 0;	//记录写入个数

		rewind(fp);
		while (iRet)
		{
			while (0 == (iCh = fgetc(fp)));
			if (EOF == iCh)
			{
				//iRet = DELERR;
				break;	//OUT
			}
			fseek(fp, -1, SEEK_CUR);	//多读了一位

			fread(pstInfoID, INFOIDSIZ, 1, fp);	//读出学生信息编号大小
			*pstStuInfo = (STUINFO *)malloc(pstInfoID->m_iLen);	//为学生信息分配空间
			fread(*pstStuInfo, pstInfoID->m_iLen, 1, fp);	//从文件中读出学生信息
			
			/**************删除************************/

			fseek(fp, -pstInfoID->m_iLen, SEEK_CUR);
			fseek(fp, 0, SEEK_CUR);
			for (int i = 0; i < pstInfoID->m_iLen; i++)
			{
				fputc(0, fp);
			}
			WriteInFile(fp);
			fseek(fp, -8 - pstInfoID->m_iLen, SEEK_CUR);
			for (int i = 0; i < INFOIDSIZ; i++)
			{
				fputc(0, fp);
			}
			WriteInFile(fp);
			//break;
			WriteToFile(fp, pstStuInfo, pstInfoID);
		}


		/*********************释放************************/
		if (NULL != *pstStuInfo)
		{
			free(*pstStuInfo);
			*pstStuInfo = NULL;
		}
	}
}