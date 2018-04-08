# include <stdio.h>
# include <stdlib.h>
# include "UI.h"
# include "Stream.h"

int iSID = 1;	//学生结构体编号
int main()
{
	int iOption = WRONGOPT;
	FILE *fp = NULL;
	int iID = 0;
	InitFile(&fp);
	
	PSTUINFO stStuInfo = NULL;
	INFOID stInfoID = { 0 };

	//iSID = fgetc(fp);	//从文件中读出序列号
	//if (0 == iSID)
	//{
	//	iSID = 1;
	//}

	while (EXIT != iOption)
	{
		MainInterface();

		iOption = GetOption();
		system("cls");
		switch (iOption)
		{
		case 1:
		{
			iOption = WRONGOPT;
			GetStuInfo(&stStuInfo, &stInfoID);
			ShowStuInfo(stStuInfo, &stInfoID);
			WriteToFile(fp, &stStuInfo, &stInfoID);
			system("pause");
			system("cls");
			break;
		}
		case 2:
		{
			iOption = WRONGOPT;
			iID = 0;
			ModifyUI(1);
			fflush(stdin);
			scanf("%d", &iID);
			fflush(stdin);

			if (ModifyStu(fp, &stInfoID, &stStuInfo, iID))
			{
				GetStuInfo(&stStuInfo, &stInfoID);
				ShowStuInfo(stStuInfo, &stInfoID);
				WriteToFile(fp, &stStuInfo, &stInfoID);
				ModifyUI(4);
			}
			else
			{
				ModifyUI(5);
			}
			system("pause");
			system("cls");
			break;
		}
		case 3:
		{
			iOption = WRONGOPT;
			SearchUI(iOption);
			iOption = GetOption();
			GetStuInfoUI(iOption);
			switch (iOption)
			{
			case STUID:
			{
				int iStuID = 0;
				fflush(stdin);
				scanf("%4d", &iStuID);
				fflush(stdin);
				SearchByStr(&iStuID, fp, &stInfoID, &stStuInfo, iOption);
				//ShowStuInfo(stStuInfo, &stInfoID);
				break;
			}
			case STUNAME:
			{
				char szTmpStuName[1024] = { 0 };
				fflush(stdin);
				scanf("%1023s", szTmpStuName);
				fflush(stdin);
				SearchByStr(szTmpStuName, fp, &stInfoID, &stStuInfo, iOption);
				break;
			}
			case STUBIRTH:
			{
				struct tagSTUBIRTH StuBirth = {0};
				fflush(stdin);
				scanf("%5d.%2d.%2d", &StuBirth.iYear, &StuBirth.iMonth, &StuBirth.iDate);
				fflush(stdin);
				SearchByStr(&StuBirth, fp, &stInfoID, &stStuInfo, iOption);
				break;
			}
			case STUCSCOR:
			{
				float fTmpCScore = 0;
				fflush(stdin);
				scanf("%7f", &fTmpCScore);
				fflush(stdin);
				SearchByStr(&fTmpCScore, fp, &stInfoID, &stStuInfo, iOption);
				break;
			}
			case STUGENDER:
			{
				char szTmpGender[8] = { 0 };
				short int sTmpGender = 0;
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
				SearchByStr(&sTmpGender, fp, &stInfoID, &stStuInfo, iOption);
				break;
			}
			}/*switch (iOption) end*/
			system("pause");
			system("cls");
			break;
		}
		case 4:
		{
			iOption = WRONGOPT;
			iID = 0;
			DeleteUI(1);
			fflush(stdin);
			scanf("%d", &iID);
			fflush(stdin);

			if (Delete(fp, &stInfoID, &stStuInfo, iID))
			{
				DeleteUI(4);
			}
			else
			{
				DeleteUI(5);
			}
			system("pause");
			system("cls");
			break;
		}
		case 5:
		{
			iOption = WRONGOPT;
			Statistics(fp, &stInfoID, &stStuInfo);
			system("pause");
			system("cls");
			break;
		}
		case 6:
		{
			iOption = WRONGOPT;
			Storage(fp, &stInfoID, &stStuInfo);
			system("pause");
			system("cls");
			break;
		}
		case 7:
		{
			ShowAllInfo(fp, &stInfoID, &stStuInfo);
			system("pause");
			system("cls");
			break;
		}
		default:
		{
			ErrProc(iOption);
			break;
		}
		}/*switch (iOption) end*/
	}

	//rewind(fp);
	//fputc(iSID, fp);
	//WriteInFile(fp);
	CloseFile(&fp);
	return 0;
}