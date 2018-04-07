# include "Stream.h"
# include "UI.h"

int iSID = 1;	//ѧ���ṹ����

/*
���ܣ���ȡѡ��
����ֵ�������ȡ��ȷѡ�����ѡ��ֵ�����ʧ�ܣ�����WRONGOPT.
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
	scanf("%3d", &iTmpStuID);
	GetStuInfoUI(STUNAME);
	scanf("%1023s", szTmpStuName);

	GetStuInfoUI(STUGENDER);
	scanf("%s", szTmpGender);
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
		printf("��\n");
	}
	else if (WOMAN == pstStuInfo->sGender)
	{
		printf("Ů\n");
	}
	else
	{
		printf("����\n");
	}
	printf("%d.%d.%d\n", pstStuInfo->stStuBirth.iYear, pstStuInfo->stStuBirth.iMonth, pstStuInfo->stStuBirth.iDate);
	printf("%f\n", pstStuInfo->fCScore);

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
	fflush(fp);
	fd = _fileno(fp); //��ȡ�ļ�������
	_commit(fd); //ǿ��дӲ��

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
	char cTmpBuf = '\0';	//д��ֵ
	int iCount = 0;	//��¼д�����

	rewind(fp);
	while (1)
	{
		while ('\0' == (ch = fgetc(fp)));
		fseek(fp, -1, SEEK_CUR);	//�����һλ
		if (EOF == ch)
		{
			iRet = DELERR;
			break;
		}
		
		fread(pstInfoID, INFOIDSIZ, 1, fp);	//����ѧ����Ϣ��Ŵ�С
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
			fd = _fileno(fp); //��ȡ�ļ�������
			iRet = _commit(fd); //ǿ��дӲ��
			fseek(fp, -8 - pstInfoID->m_iLen, SEEK_CUR);
			for (int i = 0; i < INFOIDSIZ; i++)
			{
				fputc(0, fp);
			}
			fflush(fp);
			fd = _fileno(fp); //��ȡ�ļ�������
			_commit(fd); //ǿ��дӲ��
			break;
		}
	}

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