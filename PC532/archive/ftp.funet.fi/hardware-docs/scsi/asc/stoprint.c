

#include <dos.h>
#include <stdio.h>
#include <constant.h>
#include <ascstruc.lib>
#include <asccom.lib>

char try_again;

main()
{
	try_again=TRUE;
	while (try_again)
	{
		flush();
		check();
	}
}

#include <check.lib>
