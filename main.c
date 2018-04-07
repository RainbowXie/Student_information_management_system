# include <stdio.h>
# include <stdlib.h>
# include "UI.h"
# include "Stream.h"

int main()
{
	int iOption = WRONGOPT;
	FILE *fp = NULL;
	int iID = 0;
	InitFile(&fp);

	PSTUINFO stStuInfo = NULL;
	INFOID stInfoID = { 0 };

	while (EXIT != iOption)
	{
		MainInterface();

		iOption = GetOption();

		switch (iOption)
		{
		case 1:
		{
			iOption = WRONGOPT;
			GetStuInfo(&stStuInfo, &stInfoID);
			ShowStuInfo(stStuInfo, &stInfoID);
			WriteToFile(fp, &stStuInfo, &stInfoID);

			
			break;
		}
		case 2:
		{
			iOption = WRONGOPT;

			scanf("%d", &iID);

			ReadFromFile(fp, &stStuInfo, &stInfoID, iID);
			break;
		}
		case 3:
		{
			iOption = WRONGOPT;
			break;
		}
		case 4:
		{
			iOption = WRONGOPT;
			break;
		}
		case 5:
		{
			iOption = WRONGOPT;
			iID = 0;
			scanf("%d", &iID);
			Delete(fp, &stInfoID, iID);
			break;
		}
		case 6:
		{
			iOption = WRONGOPT;
			break;
		}
		default:
		{
			ErrProc(iOption);
			break;
		}
		}/*switch (iOption) end*/
	}
	CloseFile(&fp);
	return 0;
}