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
	printf("      7. ��ʾȫ��ѧ����Ϣ\n");
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
	case NSEARCH:
	{
		printf("û�ҵ�(�ѩn��)\n");
		break;
	}
	case WRONGOPT:
	{
		printf("��...ѡ������( ��, �� )��\n");
		break;
	}
	case INITERR:
	{
		printf("System wrong! Database doesn't exist��");
		exit(-1);
		break;
	}
	case FWRITE:
	{
		printf("System wrong! Database doesn't exist��");
		exit(-1);
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
	/*case STUPHNUB:
	{
		printf("������ѧ���ĵ绰��");
		break;
	}*/
	case STUCSCOR:
	{
		printf("������ѧ����C���Գɼ���");
		break;
	}
	case STUBIRTH:
	{
		printf("������ѧ�����գ���ʽΪxxxx.xx.xx��");
		break;
	}
	case STUGENDER:
	{
		printf("������ѧ���Ա�(��/Ů/����)��");
		break;
	}
	}
}
void SearchUI(int iOption)
{
	printf("1.��ѧ�Ų���\n");
	printf("2.����������\n");
	printf("3.���������ڲ���\n");
	printf("4.����������\n");
	printf("5.���Ա����\n");
	printf("6.����������\n");
	printf("===================================\n");
	printf("��ѡ��");
}

void DeleteUI(int iOption)
{
	if (1 == iOption)
	{
		printf("������ѧ��ѧ�ţ�\n");
	}
	else if (0 == iOption)
	{
		printf("��Ҫɾ����ѧ���ǣ�\n");
	}
	else if (2 == iOption)
	{
		printf("�Ƿ�ɾ����\n");
		printf("1.ɾ��\n");
		printf("0.����\n");
		printf("��ѡ��");
	}
	else if (3 == iOption)
	{
		printf("����ѡ�������ѡ��\n");
	}
	else if (4 == iOption)
	{
		printf("ɾ���ɹ���\n");
	}
	else if (5 == iOption)
	{
		printf("ɾ��ʧ�ܣ�\n");
	}
	return;
}

void ModifyUI(int iOption)
{
	if (1 == iOption)
	{
		printf("������ѧ��ѧ�ţ�\n");
	}
	else if (0 == iOption)
	{
		printf("��Ҫ�޸ĵ�ѧ���ǣ�\n");
	}
	else if (2 == iOption)
	{
		printf("�Ƿ��޸ģ�\n");
		printf("1.�޸�\n");
		printf("0.����\n");
		printf("��ѡ��");
	}
	else if (3 == iOption)
	{
		printf("����ѡ�������ѡ��\n");
	}
	else if (4 == iOption)
	{
		printf("�޸ĳɹ���\n");
	}
	else if (5 == iOption)
	{
		printf("�޸�ʧ�ܣ�\n");
	}
	return;
}

void StorageUI(int iOption)
{
	switch (iOption)
	{
	case 0:
	{
		printf("���Խ�����Ƭ�����Ƿ�����\r\n");
		printf("1.��\r\n");
		printf("0.����\r\n");
		printf("��ѡ��\r\n");
		break;
	}
	case 3:
	{
		printf("����ѡ�������ѡ��\n");
		break;
	}
	}
}