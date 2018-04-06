# include <stdio.h>
# include <stdlib.h>
# ifndef EXIT
# define EXIT 0
# endif // !EXIT

# ifndef WRONGOPT
# define WRONGOPT -2
# endif // !WRONGOPT

# ifndef WRONGSTUINFO
# define WRONGSTUINFO -1
# endif // !STUINFO

int MainInterface(void);
void ErrProc(int ErrNub);
void GetStuInfoUI(enum emStuInfo emStuType);

