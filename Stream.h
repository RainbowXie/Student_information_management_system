# include <stdio.h>
# include <string.h>
# define EXIT 0
# define WRONGOPT -2

# ifndef WRONGSTUINFO
# define WRONGSTUINFO -1
# endif // !STUINFO

# define MAN 1
# define WOMAN 2
# define UNKNOWN 3

enum emStuInfo
{
	STUID,
	STUNAME,
	STUBIRTH,
	STUPHNUB,
	STUCSCOR,
	STUGENDER
}emStuInfo;


/*
szPhoneNub[1]µÄÎ»ÖÃÎªszPhoneNub[1] +iNameLen - 1£»
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
	int iPhoneLen;
	char szStuName[1];
	char szPhoneNub[1];
}STUINFO, *PSTUINFO;

int GetOption();
int ShowStuInfo(PSTUINFO pstStuInfo);