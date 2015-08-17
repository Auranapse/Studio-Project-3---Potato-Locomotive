#include "Strategy.h"


Strategy::Strategy(void)
{
}


Strategy::~Strategy(void)
{
}

Strategy::Strategy(E_AI_TYPE e_type)
{
	switch(e_type)
	{
	case AI_SECURITY:
		break;

	case AI_SCIENTIST:
		break;
	}
}

void Strategy::Update(double dt)
{
}
