// stdafx.cpp : source file that includes just the standard includes
// Server.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

int HandleDistributor::GetNewHandle()
{
	if (vacancies.empty())
	{
		return ++this->max;
	}
	else
	{
		auto temp = *vacancies.begin();
		vacancies.erase(vacancies.begin());
		return temp;
	}
}

BOOL HandleDistributor::ReleaseHandle(int handle)
{
	if (
		handle < 0 ||
		handle > this->max ||
		vacancies.find(handle) != vacancies.end()
		)
	{
		return FALSE;
	}
	if (handle == this->max)
	{
		while (vacancies.empty() ? --max, 0 : --max == *vacancies.rbegin())
		{
			vacancies.erase(*vacancies.rbegin());
		}
	}
	else
	{
		vacancies.insert(handle);
	}
	return TRUE;
}
