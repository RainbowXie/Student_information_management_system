# include "Stream.h"
# include "UI.h"

/*
���ܣ���ȡѡ��
����ֵ�������ȡ��ȷѡ�����ѡ��ֵ�����ʧ�ܣ�����WRONGOPT.
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
���ܣ���ȡѧ����Ϣ�����浽ѧ���ṹ���ڡ�
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
	if (!strcmp(szTmpGender, "��"))
	{
		sTmpGender = MAN;
	}
	else if (!strcmp(szTmpGender, "Ů"))
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
	printf("ѧ�ţ�%d\n", pstStuInfo->iStuID);
	printf("���֣�");
	for (int i = 0; i < pstStuInfo->iNameLen; i++)
	{
		putchar(pstStuInfo->szStuName[i]);
	}
	printf("\r\n");
	if (MAN == pstStuInfo->sGender)
	{
		printf("�Ա���\r\n");
	}
	else if (WOMAN == pstStuInfo->sGender)
	{
		printf("�Ա�Ů\r\n");
	}
	else
	{
		printf("�Ա�����\r\n");
	}
	printf("���գ�%d.%d.%d\r\n", pstStuInfo->stStuBirth.iYear, pstStuInfo->stStuBirth.iMonth, pstStuInfo->stStuBirth.iDate);
	printf("������%f\r\n", pstStuInfo->fCScore);
	printf("\r\n\n");
	return 0;
}
/*
���ܣ���pstStuInfo��pstInfoIDд���ļ��У�����Ϊ��
������pstStuInfo��ָ��ṹ��ָ���ָ�룻pstInfoID��ָ��ṹ���ָ��
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
���ܣ����ļ����ҵ��㹻�������Ŀ��пռ䣬�����ļ�ָ��ָ��ÿռ�
������fp��ָ���ļ���ָ�룬pstInfoID��ָ��ѧ����Ϣ���ȵĽṹ���ָ��
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
	char cTmpBuf = '\0';	//д��ֵ
	int iCount = 0;	//��¼д�����

	rewind(fp);
	while (iRet)
	{
		while (0 == (iCh = fgetc(fp)));
		if (EOF == iCh)
		{
			//iRet = DELERR;
			break;	//OUT
		}
		fseek(fp, -1, SEEK_CUR);	//�����һλ

		fread(pstInfoID, INFOIDSIZ, 1, fp);	//����ѧ����Ϣ��Ŵ�С
		*pstStuInfo = (STUINFO *)malloc(pstInfoID->m_iLen);	//Ϊѧ����Ϣ����ռ�
		fread(*pstStuInfo, pstInfoID->m_iLen, 1, fp);	//���ļ��ж���ѧ����Ϣ
		if ((*pstStuInfo)->iStuID != iID)
		{
			continue;
		}
		//�޸�ѡ��//
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

		/**************ɾ��************************/
		
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
	//********************����*******************//
	return iRet;
}

/*
���ܣ���ѧ��ɾ��ѧ����Ϣ
������fp���ļ�ָ�룻pstInfoID��ָ���Žṹ���ָ�룻pstStuInfo��ָ��ѧ����Ϣ�ṹ���ָ���ָ�롣iID��ѧ��ѧ��
*/
int Delete(FILE *fp, PINFOID pstInfoID, PSTUINFO* pstStuInfo, int iID)
{
	int fd = 0;
	int iRet = NSEARCH;
	int iCh = -1;
	char cTmpBuf = '\0';	//д��ֵ
	int iCount = 0;	//��¼д�����

	rewind(fp);
	while (iRet)
	{
		while (0 == (iCh = fgetc(fp)));
		if (EOF == iCh)
		{
			//iRet = DELERR;
			break;	//OUT
		}
		fseek(fp, -1, SEEK_CUR);	//�����һλ

		fread(pstInfoID, INFOIDSIZ, 1, fp);	//����ѧ����Ϣ��Ŵ�С
		*pstStuInfo = (STUINFO *)malloc(pstInfoID->m_iLen);	//Ϊѧ����Ϣ����ռ�
		fread(*pstStuInfo, pstInfoID->m_iLen, 1, fp);	//���ļ��ж���ѧ����Ϣ
		if ((*pstStuInfo)->iStuID != iID)
		{
			continue;
		}
		//ɾ��ѡ��//
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

		/*ɾ��*/
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
���ܣ���pstStuInfo��pstInfoID��Ϊ��
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
		fseek(fp, -1, SEEK_CUR);	//�����һλ

		if (EOF == ch)
		{
			iRet = DELERR;
			break;
		}
		fread(pstInfoID, INFOIDSIZ, 1, fp);
		if (iID2 != pstInfoID->m_iID)	//�����Ų��ԣ���������ѧ����Ϣ�ṹ�������
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
��ʼ���ļ��ļ���ָ��ָ���ļ���ͷ
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
���ܣ��ر��ļ�
������ָ���ļ�ָ���ָ��
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
���ܣ���ʾ����ѧ����Ϣ
������fp���ļ�ָ�룻pstInfoID��ָ���Žṹ���ָ�룻pstStuInfo��ָ��ѧ����Ϣ�ṹ���ָ���ָ�롣
*/
int ShowAllInfo(FILE *fp, PINFOID pstInfoID, PSTUINFO *pstStuInfo)
{
	int fd = 0;
	int iRet = NSEARCH;
	int iCh = -1;
	char cTmpBuf = '\0';	//д��ֵ
	int iCount = 0;	//��¼д�����

	rewind(fp);
	while (iRet)
	{
			while (0 == (iCh = fgetc(fp)));
			if (EOF == iCh)
			{
				//iRet = DELERR;
				break;
			}
			fseek(fp, -1, SEEK_CUR);	//�����һλ
	
			fread(pstInfoID, INFOIDSIZ, 1, fp);	//����ѧ����Ϣ��Ŵ�С
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
���ܣ�����ѧ����Ϣ����ѧ��������ʾ��
������strInfo���޷���ָ�룬������������������ؼ��֣�pstInfoID��ָ���Žṹ���ָ�룻pstStuInfo��ָ��ѧ����Ϣ�ṹ���ָ���ָ�룬iOption��ѡ��
����ֵ������ҵ��������ҵ�����ͬ��Ϣ�����������û�ҵ�������NSEARCH��
*/
int SearchByStr(void* strInfo, FILE *fp, PINFOID pstInfoID, PSTUINFO *pstStuInfo, int iOption)
{
	int fd = 0;
	int iRet = NSEARCH;
	int iCh = -1;
	char cTmpBuf = '\0';	//д��ֵ
	int iCount = 0;	//��¼д�����

	rewind(fp);
	while (iRet)
	{
		while (0 == (iCh = fgetc(fp)));
		if (EOF == iCh)
		{
			//iRet = DELERR;
			break;
		}
		fseek(fp, -1, SEEK_CUR);	//�����һλ
		//if (EOF == ch)
		//{
		//	//iRet = DELERR;
		//	break;
		//}

		fread(pstInfoID, INFOIDSIZ, 1, fp);	//����ѧ����Ϣ��Ŵ�С
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
			//(*pstStuInfo)->szStuName)[i]����strInfo�ؼ���
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
		//	fd = _fileno(fp); //��ȡ�ļ�������
		//	iRet = _commit(fd); //ǿ��дӲ��
		//	fseek(fp, -8 - pstInfoID->m_iLen, SEEK_CUR);
			//for (int i = 0; i < INFOIDSIZ; i++)
			//{
			//	fputc(0, fp);
			//}
			//fflush(fp);
			//fd = _fileno(fp); //��ȡ�ļ�������
			//_commit(fd); //ǿ��дӲ��
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
ǿ�ƽ��������ļ�д��Ӳ��
*/
void WriteInFile(FILE* fp)
{
	int fd = 0;
	fflush(fp);
	fd = _fileno(fp); //��ȡ�ļ�������
	_commit(fd); //ǿ��дӲ��
}
/*
���ܣ�ͳ����߷֡���ͷ֡��ܳɼ���ƽ���ɼ�
*/
void Statistics(FILE *fp, PINFOID pstInfoID, PSTUINFO *pstStuInfo)
{
	float iSum = 0;
	float iHighest = 0;
	float iLowest = 0;
	int iStuCount = 0;
	int iRet = NSEARCH;
	int iCh = -1;
	int iCount = 0;	//��¼д�����
	rewind(fp);
	while (iRet)
	{
		while (0 == (iCh = fgetc(fp)));
		if (EOF == iCh)
		{
			//iRet = DELERR;
			break;
		}
		fseek(fp, -1, SEEK_CUR);	//�����һλ

		fread(pstInfoID, INFOIDSIZ, 1, fp);	//����ѧ����Ϣ��Ŵ�С
		*pstStuInfo = (STUINFO *)malloc(pstInfoID->m_iLen);
		fread(*pstStuInfo, pstInfoID->m_iLen, 1, fp);	//����ѧ����Ϣ
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
	printf("�����ܷ�Ϊ��%.3f\r\n", iSum);
	printf("����ƽ����Ϊ��%.3f\r\n", iSum / iCount);
	printf("������߷�Ϊ��%.3f\r\n", iHighest);
	printf("������ͷ�Ϊ��%.3f\r\n", iLowest);

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
	/**********************��Ƭ����******************/
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
		char cTmpBuf = '\0';	//д��ֵ
		int iCount = 0;	//��¼д�����

		rewind(fp);
		while (iRet)
		{
			while (0 == (iCh = fgetc(fp)));
			if (EOF == iCh)
			{
				//iRet = DELERR;
				break;	//OUT
			}
			fseek(fp, -1, SEEK_CUR);	//�����һλ

			fread(pstInfoID, INFOIDSIZ, 1, fp);	//����ѧ����Ϣ��Ŵ�С
			*pstStuInfo = (STUINFO *)malloc(pstInfoID->m_iLen);	//Ϊѧ����Ϣ����ռ�
			fread(*pstStuInfo, pstInfoID->m_iLen, 1, fp);	//���ļ��ж���ѧ����Ϣ
			
			/**************ɾ��************************/

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


		/*********************�ͷ�************************/
		if (NULL != *pstStuInfo)
		{
			free(*pstStuInfo);
			*pstStuInfo = NULL;
		}
	}
}