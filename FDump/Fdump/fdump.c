#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <crtdbg.h>
#include <stdbool.h>

const int  BYTES_PER_LINE = 16;
const int  LINES_PER_PAGE = 23;

FILE* g_pFile;
int            g_lOffset = 0;
int            g_lfilesize;
unsigned char  g_bySearchData[100];
int            g_iSearchLen;
unsigned char* g_pbyFileData;
int            g_iIndex = 0;

void perr(char* msg)
{
	printf(">");
	puts(msg);
}


//////////////////////////////////////////////////////
// Execute fill command
// Params: command line
// Syntex:  f 1234 'hello world'
//			f 1234 11 22 33 aa bb cc
int fill(char* pszString)
{
	int  temp = 0;
	char token[100];
	int  len = 0;
	len = PopToken(token, sizeof(token), pszString);
	if (len >= 0)
	{
		temp = hexStrToLong(token);
		if (temp == -1)
		{
			perr("Error: invalid number.");
			return;
		}
		else if (temp > g_lfilesize)
		{
			perr("Error: offset overflow");
			return;
		}
		g_lOffset = temp;
	}
	else
	{
		perr("Error: \"f\" require 2 arguments");
		return;
	}

	if (NULL != *pszString) //��ﶵ�ؤ��e������
	{
		len = convertdata(g_bySearchData, sizeof(g_bySearchData), pszString);
		if (-2 == len)
		{
			perr("Error: invalid syntax.");
			return;
		}
		//lseek(g_pFile, g_lOffset, SEEK_SET);
		fseek(g_pFile, g_lOffset, SEEK_SET);
		fwrite(g_bySearchData, len, 1, g_pFile);
		fflush(g_pFile);
	}
	else
	{
		perr("Error: no data");
	}
}

//////////////////////////////////////
//
//
//
//////////////////////////////////////
void dumpPage(int lOffset)
{
	int  iPageLen = BYTES_PER_LINE * LINES_PER_PAGE;
	int  i = 0;
	int  iTextNum = 0;
	char ch[17];
	int  iSpace = 0;
	int  iAddr = 0;
	iAddr = BYTES_PER_LINE * (lOffset / BYTES_PER_LINE);
	iSpace = lOffset % BYTES_PER_LINE;

	for (i = 0; i < iPageLen; i++)
	{
		//�L�X�a�}
		if (0 == i % BYTES_PER_LINE)
		{
			iTextNum = 0;
			printf("%06X  ", (i + iAddr)); //�����X�a�}
		}

		//�L�X�r��줸
		else if (8 == i % BYTES_PER_LINE)
		{
			printf("- ");
		}

		if (i < iSpace)
		{
			printf("   ");
		}
		else if ((lOffset >(g_lfilesize - iPageLen)) && (i > (g_lfilesize - iAddr)))
		{
			printf("   ");
		}
		else
		{
			printf("%02X ", *(g_pbyFileData + (i + iAddr)));//��16�i����XŪ�J���줸�խ�
		}

		//�L�X�r��
		if (i < iSpace)
		{
			ch[iTextNum] = 20; //�L�X�ť�
		}
		else if ((lOffset > (g_lfilesize - iPageLen)) && (i > (g_lfilesize - iAddr)))
		{
			ch[iTextNum] = 46;
		}
		else if ((*(g_pbyFileData + (i + iAddr)) < 32) || *(g_pbyFileData + (i + iAddr)) > 127) //46��ASCII�X�y�I, �N�D�C�L�X�B�z���y�I���
		{
			ch[iTextNum] = 46;
		}
		else
		{
			ch[iTextNum] = *(g_pbyFileData + (i + iAddr));
		}
		iTextNum++;

		if (BYTES_PER_LINE == iTextNum)
		{
			ch[16] = NULL;
			printf("%s\n", ch);
		}
	}
	g_lOffset = iAddr + iPageLen;
	g_iIndex = g_lOffset;
}


///////////////////////////////////////////////////
// Convert HEX string to long value
// Params:	pszToken	string to be converted.
// Return:	long value
long hexStrToLong(char* pszToken)
{
	int   iDecimal = 0;
	char* endptr;
	iDecimal = strtol(pszToken, &endptr, 16);

	if (*endptr == '\0')
	{
		return iDecimal; //�ഫ���\
	}
	else
	{
		return -1; //�ഫ����
	}
}

/////////////////////////////////////////////////////
// Execute dump command
// Params:  command line
// Syntex:
//			d 1234 
//			d  
int dump(char* pszString)
{
	int  temp = 0;
	int  len = 0;
	char token[100];
	if (0 != strlen(pszString))
	{
		len = PopToken(token, sizeof(token), pszString);
	}

	if (len > 0)
	{
		temp = hexStrToLong(token);
		if (-1 == temp)
		{
			perr("Error: invalid number"); //���~�ƭ�(�L�k��16�i��)
			return;
		}
		else if (temp > g_lfilesize)
		{
			perr("Error: offset overflow"); //�W�X���d��
			return;
		}
		g_lOffset = temp;
	}
	else if (-1 == len)
	{
		perr("Error: invalid number");
		return;
	}
	dumpPage(g_lOffset);
}


/////////////////////////////////////////
// Search patten data from current offset
// Params:  pbyKey		key data
//			keylen	key data lenth
// 
int searchPattern(unsigned char* pbyKey, int iKeylen)
{
	int iFind = 1;

	if (NULL == *pbyKey) //�M�䶵�ؤ��e����
	{
		perr("Error: no search pattern.");
		return;
	}
	while ((iFind != 0) && (g_iIndex < (g_lfilesize-strlen(pbyKey))))
	{
		iFind = memcmp(g_pbyFileData + g_iIndex, pbyKey, iKeylen);
		if (0 == iFind)
		{
			printf("Found at offset %02X\n", g_iIndex);
			g_iIndex++;
		}
		else
		{
			g_iIndex++;
		}
	}
	if (g_iIndex == (g_lfilesize - strlen(pbyKey)))
	{
		printf("Not found!\n");
	}
	g_lOffset = g_iIndex - 1; //�]��g_iIndex�|�h�[�@���ҥH���^�ӡA�bd�\����ܤ~���|��m����
}


//////////////////////////////////////////////////////
// Execute search command
// Params: command line
// Syntex:  s 'hello world'
//			s 11 22 33 aa bb cc
//			s
int search(char* pszString)
{
	unsigned char Data[100];

	int len = strlen(pszString);
	if (len > 0)
	{
		len = convertdata(Data, sizeof(Data), pszString);
		if (len == -1)
		{
			perr("Error: data error.");
			return;
		}
		if (len == -2)
		{
			perr("Error: invalid syntax.");
			return;
		}
		if (len == -3)
		{
			perr("Error: data overflow.");
		}
		memcpy(g_bySearchData, Data, len);
		g_iSearchLen = len;
	}
	searchPattern(g_bySearchData, g_iSearchLen);
}

///////////////////////////////////////////////////
// Convert a string to data
//		pszString: 'hello world'
//		pszString�G11 23 FA 4C 77
// return:	length of <pbyData>
int convertdata(unsigned char* pbyData, int iDataBufLen, char* pszString)
{
	int   len = 0;
	char* endptr;
	char  szTemp[100];
	int	  iLen = 0;

	if (strlen(pszString) >= 2)
	{
		if (('\'' == pszString[0]) && ('\'' == pszString[strlen(pszString) - 1])) //(0 != (strlen(pszString) - 1))�קK��s �u'�v �����p
		{
			memcpy(pbyData, pszString + 1, strlen(pszString) - 2); //�h�Y���޸��O�d������r
			return strlen(pszString) - 2;
		}
		else
		{
			while (0 != strlen(pszString))
			{
				iLen = PopToken(szTemp, sizeof(szTemp), pszString);
				if (iLen < 0)
				{
					return -1;
				}
				pbyData[len] = hexStrToLong(szTemp);
				len++;
				//
				if (len > iDataBufLen)
				{
					return -3;
				}
			}
		}
		return len;
	}
	else
	{
		return -2;
	}
}


/////////////////////////////////////////////////
// Pop a token from <buf>
// Params:	pszToken	 Token string of output
//          iTokenBufLen Token buffer len
//			pszData		string buffer as stack
// Return:	lenth of output string
int PopToken(char* pszToken, int iTokenBufLen, char* pszData)
{
	char* pszTokenCut;
	char* pszNext;

	pszTokenCut = strtok_s(pszData, " ", &pszNext);
	if (NULL == pszTokenCut)
	{
		//pszToken = " ";
		//pszToken[0] = 0x00;
		return -1;
	}
	else
	{
		strcpy_s(pszToken, iTokenBufLen, pszTokenCut);
		memmove(pszData, pszNext, strlen(pszNext) + 1); //�]���q�r���ҥH+1
		return strlen(pszToken);
	}
}


//////////////////////////////////////////////////////////////////
// Prompt '>' character and input a command line
// pop command character and return other in <str> buffer
// Return: command character
// Syntex:  d
//			d 1234
//			f 1234 'hello world'
//			f 1234 12 34 ab ff ee
//			s 'hello world'
//			s 
int GetCommand(char* pszString, int szBufLen)
{
	char token[100];
	int  len = 0;
	char ch;
	printf(">");
	
	//gets_s(pszString, szBufLen);
	fgets(pszString, szBufLen, stdin);
	pszString[strlen(pszString) - 1] = NULL; //�N����r���h��

	len = strlen(pszString);
	if (len >= 0)
	{
		len = PopToken(token, sizeof(token), pszString);
		if (len == 1)
		{
			ch = tolower(token[0]);
			
			if (ch == 'd' || ch == 'f' || ch == 's')
			{
				return ch;
			}
		}
	}
	//perr("invalid command.");
}

////////////////////////////////////////////////////
// Open dump file
// Exit program if file open error.
// Syntex: fdump testfile.dat
//
bool OpenFile(int argc, char* argv[])
{
	errno_t iRet;

	if (2 != argc)
	{
		return false;
	}

	iRet = fopen_s(&g_pFile, argv[1], "rb+");

	if (0 == iRet)
	{
		fseek(g_pFile, 0, SEEK_END);
		g_lfilesize = ftell(g_pFile);
		g_pbyFileData = (char*)malloc(g_lfilesize);
		fseek(g_pFile, 0, SEEK_SET);
		fread(g_pbyFileData, sizeof(unsigned char), g_lfilesize, g_pFile);  //fread(����, �C�@�Ӹ�ƶ��ؤj�p, ��ƶ����`��, �ɮ�)
		
		return true;
	}
	else
	{
		return false;
	}
}

////////////////////////////////////////////////////////
void main(int argc, char* argv[]) {
	int  iMore = 1;
	char szBuf[100];
	int  iCommand;
	bool bOK;

	bOK = OpenFile(argc, argv);
	if (false == bOK)
	{
		perr("Open File Error !");
		return;
	}
	printf("File Name: %s     Size = %d [%X]\n", argv[1], g_lfilesize, g_lfilesize);

	while (1)
	{
		iCommand = GetCommand(szBuf, sizeof(szBuf));
		switch (iCommand)
		{
		case 'd':
			dump(szBuf);
			break;
		case 'f':
			fill(szBuf);
			fclose(g_pFile);
			bOK = OpenFile(argc, argv);
			if (false == bOK)
			{
				perr("Open File Error !");
				return;
			}
			break;
		case 's':
			search(szBuf);
			break;

		default:
			perr("Error: invalid command.");
			break;
		}
	}
}