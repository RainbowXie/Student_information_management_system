# include <stdio.h>
# include <stdlib.h>
# ifndef EXIT
# define EXIT 0
# endif // !EXIT

# ifndef INITERR 
# define INITERR -5
# endif //!INITERR

# ifndef FWRITE
# define FWRITE -4
# endif // !FWRITE

# ifndef DELERR
# define DELERR -3
# endif // !DELERR

# ifndef WRONGOPT
# define WRONGOPT -2
# endif // !WRONGOPT

# ifndef WRONGSTUINFO
# define WRONGSTUINFO -1
# endif // !STUINFO

int MainInterface(void);
void ErrProc(int ErrNub);
void GetStuInfoUI(enum emStuInfo emStuType);
void MyErrProc();

