#define _CRT_SECURE_NO_WARNINGS

#include <io.h>
# include <stdio.h>
# include <string.h>
# define EXIT 0
# define WRONGOPT -2
# define INITERR -5
# define DELERR -3
# ifndef WRONGSTUINFO
# define WRONGSTUINFO -1
# endif // !STUINFO

# ifndef FWRITE
# define FWRITE -4
# endif // !FWRITE

# define MAN 1
# define WOMAN 2
# define UNKNOWN 3

enum emStuInfo
{
	STUID,
	STUNAME,
	STUBIRTH,
	/*STUPHNUB,*/
	STUCSCOR,
	STUGENDER
}emStuInfo;


/*

*/
typedef struct tagSTUINFO
{
	int iStuID;
	float fCScore;
	short int sGender;
	struct
	{
		int iYear;
		int iMonth;
		int iDate;
	}stStuBirth;
	int iNameLen;
	/*int iPhoneLen;*/
	char szStuName[1];
	//char szPhoneNub[1];
}STUINFO, *PSTUINFO;
# define INFOIDSIZ 8
/*
存放学生信息结构体的长度和编号
结构体大小：8
*/
typedef struct tagINFOID
{
	int m_iID;
	int m_iLen;
}INFOID, *PINFOID;

int GetOption();
int GetStuInfo(PSTUINFO* stStuInfo, PINFOID pstInfoID);
int ShowStuInfo(PSTUINFO pstStuInfo, PINFOID pstInfoID);
void InitFile(FILE** fp);

int WriteToFile(FILE *fp, PSTUINFO* pstStuInfo, PINFOID pstInfoID);
int FlushStu(PSTUINFO* pstStuInfo, PINFOID pstInfoID);
void ReadFromFile(FILE* fp, PSTUINFO* stStuInfo, PINFOID pstInfoID, int iID2);
void FindSpace(FILE* fp, PINFOID pstInfoID);
void CloseFile(FILE** fp);

int Delete(FILE *fp, PINFOID pstInfoID, int iID2);