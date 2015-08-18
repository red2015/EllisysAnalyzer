

std::string WStringToString (const std::wstring& wstr)
{
   return (std::string(wstr.begin(), wstr.end()));
}

std::wstring StringToWString(const std::string& str)
{
	return (std::wstring(str.begin(),str.end()));
}