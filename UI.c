# include "UI.h"
# include "Stream.h"

/*
功能：主菜单界面
*/
int MainInterface(void)
{
	printf("       欢迎登陆学生信息管理系统！\n");
	printf("===================================\n");
	printf("      1. 增加学生信息\n");
	printf("      2. 修改学生信息\n");
	printf("      3. 查询学生信息\n");
	printf("      4. 删除学生信息\n");
	printf("      5. 统计C语言成绩\n");
	printf("      6. 存储信息\n");
	printf("      0. 退出\n");
	printf("===================================\n");
	printf("请选择：");

	return 0;
}

/*
功能：错误处理
*/
void ErrProc(int ErrNub)
{
	switch (ErrNub)
	{
	case WRONGOPT:
	{
		printf("呃...选错啦ㄟ( ▔, ▔ )ㄏ\n");
		break;
	}
	case INITERR:
	{
		printf("System wrong! Database doesn't exist！");
		exit(-1);
		break;
	}
	case FWRITE:
	{
		printf("System wrong! Database doesn't exist！");
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
		printf("请输入学号：");
		break;
	}
	case STUNAME:
	{
		printf("请输入学生姓名：");
		break;
	}
	/*case STUPHNUB:
	{
		printf("请输入学生的电话：");
		break;
	}*/
	case STUCSCOR:
	{
		printf("请输入学生的C语言成绩：");
		break;
	}
	case STUBIRTH:
	{
		printf("请输入学生生日，格式为xxxx-xx-xx：");
		break;
	}
	case STUGENDER:
	{
		printf("请输入学生性别：");
		break;
	}
	}
}
