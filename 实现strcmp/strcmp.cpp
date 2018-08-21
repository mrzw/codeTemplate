int strcmp(const char* str1, const char* str2)
{
	while ((*str1) && (*str2) && (*str1 == *str2))
	{
		str1++;
		str2++;
	}
 
	if (*(unsigned char*)str1 > *(unsigned char*)str2)
	{
		return 1;
	}
	else if (*(unsigned char*)str1 < *(unsigned char*)str2)
	{
		return -1;
	}
	else
	{
		return 0;
	}  
}
