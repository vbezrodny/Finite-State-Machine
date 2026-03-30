#include <stdio.h>

#define LEN 30

#define ERROR " ERROR"
#define INT " int"
#define LONG " long"
#define UNSIGNED " unsigned"

typedef enum States { Normal, Slash, Comment, Asterisk, DoubleSlash, 
					  StringConsts, ExceptionStringConsts, CharLiteral, ExceptionCharLiteral,
					  Natural, EightSixteen, Eight, Sixteen, PostSixteen,
					  Unsigned, Long, LongLong,
					  PostUnsignedL, PostUnsignedLL, PostLong, PostLongLong,
					  Blocked } States;

char numberBuffer[256];
int bufferPos = 0;

void addCharToNumber(int c);
void flushNumber(FILE *fo, const char *type);
void flushError(FILE *fo, int errorChar);

int isBlocked(int c); // a-Z, _ , 0-9
int isErrorNumber(int c); // a-Z, _ , .
int isErrorSuffix(int c); // a-Z, _ , . , 0-9

int isNatural(int c); // 0-9
int isZero(int c); // 0
int isEight(int c); // 0-7
int isX(int c); // x X
int isSixteen(int c); // 0-F
int isUnsigned(int c); // u U
int isLong(int c); // l L

int main(int argc, char **argv)
{
	FILE *fi, *fo;
	States State = Normal;
	int c;

	if (argc != 3) {
		return 3;
	}
	fi = fopen(argv[1], "rb");
	if (!fi) {
		fprintf(stderr, "Input file \"%s\" open error.\n", argv[1]);
		return 1;
	}
	fo = fopen(argv[2], "wb");
	if (!fo) {
		fclose(fi);
		fprintf(stderr, "Output file \"%s\" open error.\n", argv[2]);
		return 2;
	}

	while ((c=fgetc(fi)) != EOF) {
		switch (State) {
			case Normal:
				if (c == '/')
					State = Slash;
				else if (c == '"')
					State = StringConsts;
				else if (c == '\'')
					State = CharLiteral;
				else if (isZero(c)) {
					addCharToNumber(c);
					State = EightSixteen;
				}
				else if (isNatural(c)) {
					addCharToNumber(c);
					State = Natural;
				}
				else if (isBlocked(c) && !(c >= '0' && c <= '9'))
					State = Blocked;
				else
					State = Normal;
			break;
			
			case Slash:
				if (c == '*')
					State = Comment;
				else if (c == '/')
					State = DoubleSlash;
				else if (c == '"')
					State = StringConsts;
				else if (c == '\'')
					State = CharLiteral;
				else
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
			
			case DoubleSlash:
				if (c == '\r' || c == '\n')
					State = Normal;
				else
					State = DoubleSlash;
			break;	
			
			case StringConsts:
				if (c == '"')
					State = Normal;
				else if (c == '\\')
					State = ExceptionStringConsts;
				else
					State = StringConsts;
			break;	
			
			case ExceptionStringConsts:
				State = StringConsts;
			break;

			case CharLiteral:
				if (c == '\'')
					State = Normal;
				else if (c == '\\')
					State = ExceptionCharLiteral;
				else
					State = CharLiteral;
			break;

			case ExceptionCharLiteral:
				State = CharLiteral;
			break;

			case EightSixteen:
				if (isEight(c)) {
					addCharToNumber(c);
					State = Eight;
				}
				else if (isX(c)) {
					addCharToNumber(c);
					State = Sixteen;
				}
				else if (isLong(c)) {
					addCharToNumber(c);
					State = Long;
				}
				else if (isUnsigned(c)) {
					addCharToNumber(c);
					State = Unsigned;
				}
				else if (isErrorNumber(c) || (!isEight(c) && isSixteen(c))) {
					addCharToNumber(c);
					flushError(fo, c);
					State = Blocked;
				}
				else if (c == '"') {
					flushNumber(fo, INT);
					State = StringConsts;
				}
				else if (c == '\'') {
					flushNumber(fo, INT);
					State = CharLiteral;
				}
				else if (c == '/') {
					flushNumber(fo, INT);
					State = Slash;
				}
				else {
					flushNumber(fo, INT);
					State = Normal;
				}
			break;

			case Eight:
				if (isEight(c)) {
					addCharToNumber(c);
					State = Eight;
				}
				else if (isLong(c)) {
					addCharToNumber(c);
					State = Long;
				}
				else if (isUnsigned(c)) {
					addCharToNumber(c);
					State = Unsigned;
				}
				else if (isErrorNumber(c) || (!isEight(c) && isSixteen(c))) {
					addCharToNumber(c);
					flushError(fo, c);
					State = Blocked;
				}
				else if (c == '"') {
					flushNumber(fo, INT);
					State = StringConsts;
				}
				else if (c == '\'') {
					flushNumber(fo, INT);
					State = CharLiteral;
				}
				else if (c == '/') {
					flushNumber(fo, INT);
					State = Slash;
				}
				else {
					flushNumber(fo, INT);
					State = Normal;
				}
			break;

			case Sixteen:
				if (isSixteen(c)) {
					addCharToNumber(c);
					State = PostSixteen;
				}
				else if (c == '"') {
					addCharToNumber(c);
					flushError(fo, c);
					State = StringConsts;
				}
				else if (c == '\'') {
					addCharToNumber(c);
					flushError(fo, c);
					State = CharLiteral;
				}
				else if (c == '/') {
					addCharToNumber(c);
					flushError(fo, c);
					State = Slash;
				}
				else {
					if (c != '\n') addCharToNumber(c);
					flushError(fo, c);
					State = Normal;
				}
			break;

			case PostSixteen:
				if (isSixteen(c)) {
					addCharToNumber(c);
					State = PostSixteen;
				}
				else if (isLong(c)) {
					addCharToNumber(c);
					State = Long;
				}
				else if (isUnsigned(c)) {
					addCharToNumber(c);
					State = Unsigned;
				}
				else if (isErrorNumber(c)) {
					addCharToNumber(c);
					flushError(fo, c);
					State = Blocked;
				}
				else if (c == '"') {
					flushNumber(fo, INT);
					State = StringConsts;
				}
				else if (c == '\'') {
					flushNumber(fo, INT);
					State = CharLiteral;
				}
				else if (c == '/') {
					flushNumber(fo, INT);
					State = Slash;
				}
				else {
					flushNumber(fo, INT);
					State = Normal;
				}
			break;

			case Natural:
				if (isNatural(c)) {
					addCharToNumber(c);
					State = Natural;
				}
				else if (isLong(c)) {
					addCharToNumber(c);
					State = Long;
				}
				else if (isUnsigned(c)) {
					addCharToNumber(c);
					State = Unsigned;
				}
				else if (isErrorNumber(c)) {
					addCharToNumber(c);
					flushError(fo, c);
					State = Blocked;
				}
				else if (c == '"') {
					flushNumber(fo, INT);
					State = StringConsts;
				}
				else if (c == '\'') {
					flushNumber(fo, INT);
					State = CharLiteral;
				}
				else if (c == '/') {
					flushNumber(fo, INT);
					State = Slash;
				}
				else {
					flushNumber(fo, INT);
					State = Normal;
				}
			break;

			case Long:
				if (isLong(c)) {
					addCharToNumber(c);
					State = LongLong;
				}
				else if (isUnsigned(c)) {
					addCharToNumber(c);
					State = PostUnsignedL;
				}
				else if (isErrorSuffix(c)) {
					addCharToNumber(c);
					flushError(fo, c);
					State = Blocked;
				}
				else if (c == '"') {
					flushNumber(fo, LONG INT);
					State = StringConsts;
				}
				else if (c == '\'') {
					flushNumber(fo, LONG INT);
					State = CharLiteral;
				}
				else if (c == '/') {
					flushNumber(fo, LONG INT);
					State = Slash;
				}
				else {
					flushNumber(fo, LONG INT);
					State = Normal;
				}
			break;

			case LongLong:
				if (isUnsigned(c)) {
					addCharToNumber(c);
					State = PostUnsignedLL;
				}
				else if (isErrorSuffix(c)) {
					addCharToNumber(c);
					flushError(fo, c);
					State = Blocked;
				}
				else if (c == '"') {
					flushNumber(fo, LONG LONG INT);
					State = StringConsts;
				}
				else if (c == '\'') {
					flushNumber(fo, LONG LONG INT);
					State = CharLiteral;
				}
				else if (c == '/') {
					flushNumber(fo, LONG LONG INT);
					State = Slash;
				}
				else {
					flushNumber(fo, LONG LONG INT);
					State = Normal;
				}
			break;

			case PostUnsignedL:
				if (isErrorSuffix(c)) {
					addCharToNumber(c);
					flushError(fo, c);
					State = Blocked;
				}
				else if (c == '"') {
					flushNumber(fo, UNSIGNED LONG INT);
					State = StringConsts;
				}
				else if (c == '\'') {
					flushNumber(fo, UNSIGNED LONG INT);
					State = CharLiteral;
				}
				else if (c == '/') {
					flushNumber(fo, UNSIGNED LONG INT);
					State = Slash;
				}
				else {
					flushNumber(fo, UNSIGNED LONG INT);
					State = Normal;
				}
			break;

			case PostUnsignedLL:
				if (isErrorSuffix(c)) {
					addCharToNumber(c);
					flushError(fo, c);
					State = Blocked;
				}
				else if (c == '"') {
					flushNumber(fo, UNSIGNED LONG LONG INT);
					State = StringConsts;
				}
				else if (c == '\'') {
					flushNumber(fo, UNSIGNED LONG LONG INT);
					State = CharLiteral;
				}
				else if (c == '/') {
					flushNumber(fo, UNSIGNED LONG LONG INT);
					State = Slash;
				}
				else {
					flushNumber(fo, UNSIGNED LONG LONG INT);
					State = Normal;
				}
			break;

			case Unsigned:
				if (isLong(c)) {
					addCharToNumber(c);
					State = PostLong;
				}
				else if (isErrorSuffix(c)) {
					addCharToNumber(c);
					flushError(fo, c);
					State = Blocked;
				}
				else if (c == '"') {
					flushNumber(fo, UNSIGNED INT);
					State = StringConsts;
				}
				else if (c == '\'') {
					flushNumber(fo, UNSIGNED INT);
					State = CharLiteral;
				}
				else if (c == '/') {
					flushNumber(fo, UNSIGNED INT);
					State = Slash;
				}
				else {
					flushNumber(fo, UNSIGNED INT);
					State = Normal;
				}
			break;

			case PostLong:
				if (isLong(c)) {
					addCharToNumber(c);
					State = PostLongLong;
				}
				else if (isErrorSuffix(c)) {
					addCharToNumber(c);
					flushError(fo, c);
					State = Blocked;
				}
				else if (c == '"') {
					flushNumber(fo, UNSIGNED LONG INT);
					State = StringConsts;
				}
				else if (c == '\'') {
					flushNumber(fo, UNSIGNED LONG INT);
					State = CharLiteral;
				}
				else if (c == '/') {
					flushNumber(fo, UNSIGNED LONG INT);
					State = Slash;
				}
				else {
					flushNumber(fo, UNSIGNED LONG INT);
					State = Normal;
				}
			break;

			case PostLongLong:
				if (isErrorSuffix(c)) {
					addCharToNumber(c);
					flushError(fo, c);
					State = Blocked;
				}
				else if (c == '"') {
					flushNumber(fo, UNSIGNED LONG LONG INT);
					State = StringConsts;
				}
				else if (c == '\'') {
					flushNumber(fo, UNSIGNED LONG LONG INT);
					State = CharLiteral;
				}
				else if (c == '/') {
					flushNumber(fo, UNSIGNED LONG LONG INT);
					State = Slash;
				}
				else {
					flushNumber(fo, UNSIGNED LONG LONG INT);
					State = Normal;
				}
			break;

			case Blocked:
				if (isBlocked(c))
					State = Blocked;
				else if (c == '"')
					State = StringConsts;
				else if (c == '\'')
					State = CharLiteral;
				else if (c == '/')
					State = Slash;
				else
					State = Normal;
			break;
		}
	}
	
	fclose(fi);
	fclose(fo);
	return 0;
}

void addCharToNumber(int c)
{
	if (bufferPos < 255) {
		numberBuffer[bufferPos++] = c;
	}
}

void flushNumber(FILE *fo, const char *type)
{
	if (bufferPos > 0) {
		numberBuffer[bufferPos] = '\0';
		fprintf(fo, "%-*s%s\n", LEN, numberBuffer, type);
		bufferPos = 0;
	}
}

void flushError(FILE* fo, int errorChar)
{
	if (bufferPos > 0) {
		numberBuffer[bufferPos] = '\0';
		fprintf(fo, "%-*s%c%s\n", LEN, numberBuffer, errorChar, ERROR);
		bufferPos = 0;
	}
}

int isBlocked(int c)
{
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == '_')
		return 1;
	return 0;
}

int isErrorNumber(int c)
{
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c == '.')
		return 1;
	return 0;
}

int isErrorSuffix(int c)
{
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == '_')
		return 1;
	return 0;
}

int isNatural(int c)
{
	if (c >= '0' && c <= '9')
		return 1;
	return 0;
}

int isZero(int c)
{
	if (c == '0') 
		return 1;
	return 0;
}

int isEight(int c)
{
	if (c >= '0' && c <= '7')
		return 1;
	return 0;
}

int isX(int c)
{
	if (c == 'x' || c == 'X')
		return 1;
	return 0;
}

int isSixteen(int c)
{
	if (c >= '0' && c <= '9' || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F')
		return 1;
	return 0;
}

int isUnsigned(int c)
{
	if (c == 'u' || c == 'U')
		return 1;
	return 0;
}

int isLong(int c)
{
	if (c == 'l' || c == 'L')
		return 1;
	return 0;
}
