#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <crtdbg.h>
#define BYTES_PER_LINE 16 
#define LINES_PER_PAGE 23

const int len = LINES_PER_PAGE * BYTES_PER_LINE;//�C����X����23��A�C��16�줸��
FILE* pFile = NULL;

void main(int argc, char* argv[])
{
	const int iPageLen = LINES_PER_PAGE * BYTES_PER_LINE;//�C����X����23��A�C��16�줸��
	char* szTextapostrophe[100];
	errno_t iRet;
	iRet = fopen_s(&pFile, "D:\\VS_Project\\HW\\FDump\\test.dat", "rb+");

	// get file length
	fseek(pFile, 0, SEEK_END);
	int iBufferSize = ftell(pFile);
	unsigned char* pszData = (char*)malloc(iBufferSize);

	//�p���ɮפj�p��feek�^���Y
	fseek(pFile, 0, SEEK_SET);
	int iReadLen = fread(pszData, sizeof(unsigned char), iBufferSize, pFile);  //fread(����, �C�@�Ӹ�ƶ��ؤj�p, ��ƶ����`��, �ɮ�)

	char szInput[100];
	char* szCommandKey;
	char* szFindKey;
	char szFindKeyTwo[100] = { NULL };
	char* pszNext;
	char* pszNextTwo;
	int iIndex = 0;
	int iNum = 0;//16to10
	int iAddr = 0;//������m
	int iAddr2 = 0;//����d�\��ϥզr��
	int iTextNum = 0;//�����r��s�r�����ƨ�16
	int iFind; //memcp��ﵲ�G0(��)�Ψ�L��(�S��)
	unsigned char c;
	char ch[17];
	unsigned char Hex[100] = { NULL };
	char* pszHex;
	char* pszD = NULL;
	int i = 0;
	int iHexLen = 0; //�p��s���μƥ�
	int iStatus_S = 0;
	int iStatus_F = 0;
	int iStatus_D = 0;
	int szInputLen = 0;

	if (NULL != pFile)
	{
		fseek(pFile, 0, SEEK_SET);
		while (1)
		{
			iFind = 1;
			iAddr = 0;
			iNum = 0;
			iStatus_D = 0;
			printf(">");
			gets_s(szInput, sizeof(szInput));

			printf("%d \n", sizeof(szInput));

			szInputLen = (strlen(szInput));
			if ((0 != szInputLen) && (' ' != szInput[0]))
			{
				if ((' ' == szInput[1]) || (NULL == szInput[1]))
				{
					if (1 != szInputLen)
					{
						if (0 != strstr(szInput, "\'"))
						{
							iStatus_S = 0;
							iStatus_F = 0;
						}
						else
						{
							iStatus_S = 1;
							iStatus_F = 1;
						}
					}

					szCommandKey = strtok_s(szInput, " '", &szFindKey);//���ΩR�O�r��s d f
					/*if (NULL == szCommandKey)
					{
						printf("Error:  invalid command.\n");
					}
					else */if ((0 == iStatus_S) && ('s' == *szCommandKey))
					{
						szFindKey = strtok_s(szFindKey, "'", &pszNext);
					}
					else
					{
						szFindKey = strtok_s(szFindKey, " '", &pszNext);//���Τ޸������e
					}

					if ((NULL != szFindKey) && (0 == strcmp(szCommandKey, "s"))) //�i��r��ƻs�H�K����u��J�@�Ӧrs����O��W���M���
					{
						memcpy(szFindKeyTwo, szFindKey, strlen(szFindKey));
						szFindKeyTwo[strlen(szFindKey)] = NULL;
					}

					if ((0 != strlen(pszNext)) && (('f' == *szCommandKey)))
					{
						strcpy_s(szFindKeyTwo, sizeof(szFindKeyTwo), pszNext);

						if (1 == iStatus_F)
						{
							int j = 0;

							for (j = 0; j < strlen(szFindKeyTwo); j++)
							{
								if ((szFindKeyTwo[j] >= 'G') && (szFindKeyTwo[j] <= 'Z'))
								{
									iStatus_F = 2;
								}
								else if ((szFindKeyTwo[j] >= 'g') && (szFindKeyTwo[j] <= 'z'))
								{
									iStatus_F = 2;
								}
							}
							if (2 != iStatus_F)
							{
								i = 0;
								//�о�
								pszHex = strtok_s(szFindKeyTwo, " ", &pszNextTwo);
								while (NULL != pszHex)
								{
									Hex[i] = (unsigned char)strtol(pszHex, &pszD, 16);
									i++;
									pszHex = strtok_s(NULL, " ", &pszNextTwo);
								}
								//
								iHexLen = i;
							}
						}
					}
					else
					{
						if ((1 == iStatus_S) && ((1 != szInputLen)))
						{
							strcat_s(szFindKeyTwo, sizeof(szFindKeyTwo), " ");
							strcat_s(szFindKeyTwo, sizeof(szFindKeyTwo), pszNext);//�r��X�֦����M��Hex

							int j = 0;

							for (j = 0; j < strlen(szFindKeyTwo); j++)
							{
								if ((szFindKeyTwo[j] >= 'G') && (szFindKeyTwo[j] <= 'Z'))
								{
									iStatus_S = 2;
								}
								else if ((szFindKeyTwo[j] >= 'g') && (szFindKeyTwo[j] <= 'z'))
								{
									iStatus_S = 2;
								}
							}

							if (2 != iStatus_S)
							{
								i = 0;
								strcpy_s(Hex, sizeof(Hex), "");
								//�о�
								pszHex = strtok_s(szFindKeyTwo, " ", &pszNextTwo);
								while (NULL != pszHex)
								{
									Hex[i] = (unsigned char)strtol(pszHex, &pszD, 16);
									i++;
									pszHex = strtok_s(NULL, " ", &pszNextTwo);
								}
								//
								iHexLen = i;
							}
						}
					}

					//�U���O����
					if ('d' == *szCommandKey)
					{
						if (szFindKey != NULL)
						{
							for (i = 0; i < strlen(szFindKey); i++)
							{
								if ((szFindKey[i] >= '0') && (szFindKey[i] <= '9'))
								{
									iNum = iNum * 16 + szFindKey[i] - '0';
								}
								else if ((szFindKey[i] >= 'A') && (szFindKey[i] <= 'F'))
								{
									iNum = iNum * 16 + szFindKey[i] - 'A' + 10;
								}
								else if ((szFindKey[i] >= 'a') && (szFindKey[i] <= 'f'))
								{
									iNum = iNum * 16 + szFindKey[i] - 'a' + 10;
								}
								else
								{
									iStatus_D = 1;
								}
							}
							if (1 != iStatus_D)
							{
								iIndex = iNum;
								iAddr = iNum;
								iAddr2 = iAddr % 16; //������
								iAddr = BYTES_PER_LINE * (iAddr / BYTES_PER_LINE); //�����
								fseek(pFile, iAddr, SEEK_SET); //fseek(�ɮ�, �̾ڰ_�I���ʦ�m, �_�I)
							}
						}

						if (1 != iStatus_D)
						{
							for (i = 0; i < iPageLen; i++)
							{
								fread(&c, sizeof(unsigned char), 1, pFile);
								if (0 == i % BYTES_PER_LINE)
								{
									iTextNum = 0;
									printf("%06X  ", ftell(pFile) - 1); //�����X�a�}
								}

								else if (8 == i % BYTES_PER_LINE)
								{
									printf("- ");
								}
								if (i < iAddr2)
								{
									printf("   ");
								}
								else
								{
									printf("%02X ", c);//��16�i����XŪ�J���줸�խ�
								}

								c = (c < 32 ) ? 46 : c;//46��ASCII�X�y�I,�N�D�C�L�X�B�z���y�I���
								c = (c > 127) ? 46 : c; //�o�˴N����ܺ~�r,����46

								ch[iTextNum] = c;
								iTextNum++;
								if (BYTES_PER_LINE == iTextNum)
								{
									ch[16] = NULL;
									printf("%s\n", ch);
								}
							}
						}
						else
						{
							printf("Error: invalid number\n");
						}
					}

					else if (('s' == *szCommandKey) && (szFindKeyTwo[0] != NULL))
					{
						if (2 != iStatus_S)
						{
							while ((iFind != 0) && (iIndex != iBufferSize))
							{
								if (iIndex < iBufferSize)
								{
									if (1 == iStatus_S)
									{
										iFind = memcmp(pszData + iIndex, Hex, iHexLen);
										iStatus_S = 1;
									}
									else
									{
										iFind = memcmp(pszData + iIndex, szFindKeyTwo, strlen(szFindKeyTwo));
										iStatus_S = 0;
									}


									if (0 == iFind)
									{
										fseek(pFile, iIndex, SEEK_SET);
										printf("Found at offset %02X\n", ftell(pFile));
										iIndex++;
									}
									else
									{
										iIndex++;
									}
								}
							}
							if (iIndex == iBufferSize)
							{
								printf("Not found!\n");
							}
						}
						else
						{
							printf("Error: invalid syntax\n");
						}
					}
					else if ('f' == *szCommandKey)
					{
						if (2 != iStatus_F)
						{
							pszNext = strtok_s(pszNext, "'", &pszNextTwo);//���Τ޸������e
							for (i = 0; i < strlen(szFindKey); i++)
							{
								if ((szFindKey[i] >= '0') && (szFindKey[i] <= '9'))
								{
									iNum = iNum * 16 + szFindKey[i] - '0';
								}
								else if ((szFindKey[i] >= 'A') && (szFindKey[i] <= 'F'))
								{
									iNum = iNum * 16 + szFindKey[i] - 'A' + 10;
								}
								else if ((szFindKey[i] >= 'a') && (szFindKey[i] <= 'f'))
								{
									iNum = iNum * 16 + szFindKey[i] - 'a' + 10;
								}
								else
								{
									iStatus_D = 1;
								}
							}
							if (1 != iStatus_D)
							{
								iIndex = iNum;
								iAddr = iNum;
								iAddr2 = iAddr % 16;
								iAddr = BYTES_PER_LINE * (iAddr / BYTES_PER_LINE);
								fseek(pFile, iAddr + iAddr2, SEEK_SET);
								if (0 == iStatus_F)
								{
									fwrite(pszNext, strlen(pszNext), 1, pFile);
								}
								else
								{
									fwrite(Hex, sizeof(unsigned char), iHexLen, pFile);
								}
							}
							else
							{
								printf("Error: invalid number\n");
							}


							fseek(pFile, SEEK_SET, SEEK_SET);//�N��Ц^�ɮװ_�I
							iReadLen = fread(pszData, sizeof(unsigned char), iBufferSize, pFile);  //��Ū�ɮק�s�ק��
							fseek(pFile, iAddr, SEEK_SET);//�N��Ц^16��ƭ��}�Y��m
						}
						else
						{
							printf("Error: invalid syntax\n");
						}
					}
					else
					{
						printf("Error:  invalid command.\n");
					}
					//printf("iStatus_S = %d, iNum = %d, iIndex = %d\n", iStatus_S, iNum, iIndex);
					//printf("szFindKeyTwo = %s\n", szFindKeyTwo);
				}
				else
				{
					printf("Error:  invalid command.\n");
				}

			}
			else
			{
				printf("Error:  invalid command.\n");
			}
		}

		fclose(pFile);
	}
	else
	{
		printf("�ɮ׶}�_����!\n");
	}


	//char szCMD[100] = { 0 };

	//sprintf_s(szCMD, sizeof(szCMD), "A B C");

	//char* pszKey;
	//char* pszNext;
	//char* delim = " \n";
	//pszKey = strtok_s(szCMD, delim, &pszNext);
	//pszKey = strtok_s(NULL, delim, &pszNext);
	//pszKey = strtok_s(NULL, delim, &pszNext);
	return 0;
}