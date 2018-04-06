# include <stdio.h>
# include <stdlib.h>
# include "UI.h"
# include "Stream.h"

int main()
{
	int iOption = WRONGOPT;
	FILE *fp = NULL;
	PSTUINFO stStuInfo = NULL;

	while (EXIT != iOption)
	{
		MainInterface();

		iOption = GetOption();

		switch (iOption)
		{
		case 1:
		{
			iOption = WRONGOPT;
			GetStuInfo(&stStuInfo);
			ShowStuInfo(stStuInfo);
			WriteToFile(&fp);

			if (NULL != stStuInfo)
			{
				free(stStuInfo);
				stStuInfo = NULL;
			}
			
			break;
		}
		case 2:
		{
			iOption = WRONGOPT;
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

	return 0;
}