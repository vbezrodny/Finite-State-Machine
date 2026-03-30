 

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
				if (c == '*' || c == '\*')
					State = Comment;
				else if (c == '/')
					
					fputc('/', fo);
					State = Slash;
				else
					
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


if (i 
int a;
int b;
pritntf(a b);


std:String str = "some text // here /*does that mean comment?*/ for \" \\ exapmle";
std:cout << " GeeksforGeeks is best" 
            " platform to learn \n It" 
            " is used by students to" 
            " gain knowledge \n It is" 
            " really helpful";

fprintf(stderr, /"Input //file \"%s\" open error.\n", argv[1]);

 

 