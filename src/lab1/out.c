

#include <stdio.h>

 
typedef enum States { Normal, Slash, Comment, Asterisk } States;

int main(int argc, char **argv)
{
FILE *fi, *fo;				
States State = Normal;		
int c;						


	if (argc != 3)
	{
		
		return 3;
	}
	fi = fopen(argv[1], "rb");
	if (!fi)
	{
		fprintf(stderr, "Input file \"%s\" open error.\n", argv[1]);
		return 1;
	}
	fo = fopen(argv[2], "wb");
	if (!fo)
	{
		fclose(fi);
		fprintf(stderr, "Output file \"%s\" open error.\n", argv[2]);
		return 2;
	}

	while ((c=fgetc(fi)) != EOF)		
	{
		switch (State)	
		{
			case Normal:		
				if (c == '/')			
					State = Slash;		
				else		
					State = c;
			break;
			
			case Slash:			
				if (c == '*')
					State = Comment;
				else if (c == '/')
					// Дописать добавление символа в файл
					fputc('/', fo);
					State = Slash;
				else
					// Дописать добавление символа в файл
					fputc('/', fo);
					fputc(c, fo);
					State = Normal;
			break;
			
			case Comment:
				if (c == '*')
					State = Asterisk;
				else
					State = Comment;
			break;
			
			case Asterisk:
				if (c == '/')
					State = Normal;
				else if (c == '*')
					State = Asterisk;
				else
					State = Comment;	
			break;
			
					
		}
	}
  
	
	
	
	
	fclose(fi);    
	fclose(fo);    
	return 0;
}

// Здесь представлен абстрактный случай многострочного комментария непосредственно после знака деления (допустимо для языка C)
if (i / 2 == 10) printf("some text here.");



