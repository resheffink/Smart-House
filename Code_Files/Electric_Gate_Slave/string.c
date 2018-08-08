/*
	Project: String Library for C
	Version: 1.0.0
	Website: <https://sourceforge.net/projects/stringlibforc/>

	Copyright 2012 Fikri Fadzil

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char **split(char *str, char delimiter)
{
	char *tempWords		= NULL;
	char **tempStoreWord = NULL;

	int i, length = strlen(str);
	int a, b, flag = 0;

	a = b = 1;
	for(i = 0; i < length; i++)
	{
		if(str[i] == delimiter)
		{
			if(flag)
			{
				tempWords[a - 1] = '\0';

				tempStoreWord = (char **) realloc(tempStoreWord, b * sizeof(char *));
				tempStoreWord[b++ - 1] = tempWords;

				a = 1; flag = 0;
			}
		}
		else
		{
			if(flag)
			{
				tempWords = (char *) realloc(tempWords, (a + 1) * sizeof(char));
				tempWords[a++ - 1] = str[i];
			}
			else
			{
				tempWords = (char *) calloc(1, sizeof(char));
				tempWords[a++ -1] = str[i];

				flag = 1;
			}
		}
	}

	if(flag)
	{
		tempWords[a] = '\0';

		tempStoreWord = (char **) realloc(tempStoreWord, b * sizeof(char *));
		tempStoreWord[b - 1] = tempWords;

		flag = 0;
	}

	return tempStoreWord;
}

char *toLowerCase(char *str)
{
	int i, length = strlen(str);

	for(i = 0; i < length; i++)
	{
		if(isalpha(str[i]))
		{
			if(64 < str[i] && str[i] < 91)
				str[i] += 32;
		}
	}

	return str;
}

char *toUpperCase(char *str)
{
	int i, length = strlen(str);

	for(i = 0; i < length; i++)
	{
		if(isalpha(str[i]))
		{
			if(96 < str[i] && str[i] < 123)
				str[i] -= 32;
		}
	}

	return str;
}

int equals(char *str1, char *str2)
{
	return (strcmp(str1, str2) == 0)? 1: 0;
}

int equalsIgnoreCase(char *str1, char *str2)
{
	return equals(toUpperCase(str1), toUpperCase(str2));
}

char *reverse(char *str)
{
	int a = 0, b = strlen(str);
	char *temp = (char *) calloc(b, sizeof(char));

	for(; a < strlen(str); a++, b--)
	{
		temp[a] = str[b - 1];
	}
	temp[a] = '\0';

	return temp;
}

int indexOf(char *str, char *pattern)
{
	char *temp;

	if(strlen(str) >= strlen(pattern))
	{
		temp = strstr(str, pattern);
		if(temp != NULL)
			return (temp - str);
	}

	return -1;
}

int endsWith(char *str, char *pattern)
{
	int result = 0;

	if(strlen(str) >= strlen(pattern))
	{
		str += (strlen(str) - strlen(pattern));
		result = strcmp(str, pattern);

		return (result == 0)? 1: 0;
	}

	return 0;
}

int startsWith(char *str, char *pattern)
{
	int result = strncmp(str, pattern, strlen(pattern));
	return (result == 0)? 1: 0;
}

int length(char *str)
{
	return strlen(str);
}

char *getString()
{
    int i = 0;    // counter
    char temp, *str = NULL;

    while((temp = getchar()) != '\n')
    {
        str = (char *) realloc(str, ++i * sizeof(char));
        str[i - 1] = temp;
    }

    str = (char *) realloc(str, ++i * sizeof(char));
    str[i - 1] = '\0';

    return str;
}