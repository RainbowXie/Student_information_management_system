# include "UI.h"
# include "Stream.h"

/*
���ܣ����˵�����
*/
int MainInterface(void)
{
	printf("       ��ӭ��½ѧ����Ϣ����ϵͳ��\n");
	printf("===================================\n");
	printf("      1. ����ѧ����Ϣ\n");
	printf("      2. �޸�ѧ����Ϣ\n");
	printf("      3. ��ѯѧ����Ϣ\n");
	printf("      4. ɾ��ѧ����Ϣ\n");
	printf("      5. ͳ��C���Գɼ�\n");
	printf("      6. �洢��Ϣ\n");
	printf("      0. �˳�\n");
	printf("===================================\n");
	printf("��ѡ��");

	return 0;
}

/*
���ܣ�������
*/
void ErrProc(int ErrNub)
{
	switch (ErrNub)
	{
	case WRONGOPT:
	{
		printf("��...ѡ������( ��, �� )��\n");
		break;
	}
	
	}
	return;
}

void GetStuInfoUI(enum emStuInfo emStuType)
{
	switch (emStuType)
	{
	case STUID:
	{
		printf("������ѧ�ţ�");
		break;
	}
	case STUNAME:
	{
		printf("������ѧ��������");
		break;
	}
	case STUPHNUB:
	{
		printf("������ѧ���ĵ绰��");
		break;
	}
	case STUCSCOR:
	{
		printf("������ѧ����C���Գɼ���");
		break;
	}
	case STUBIRTH:
	{
		printf("������ѧ�����գ���ʽΪxxxx-xx-xx��");
		break;
	}
	case STUGENDER:
	{
		printf("������ѧ���Ա�");
	}
	}
}