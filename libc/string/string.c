
char upperCase(const char c)
{
	return ((c >= 'a') && (c <= 'z'))? (c & 0xdf) : c;
}

char lowerCase(const char c)
{
	return ((c >= 'A') && (c <= 'Z'))? (c | 0x20) : c;
}
