#pragma once
#include "ObserverInterface.h"

class SubjectInterface
{
private:

public:
	SubjectInterface() {};
	virtual ~SubjectInterface() {};

	virtual void attach(ObserverInterface*) = 0;
	virtual void detach(ObserverInterface*) = 0;
	virtual void notify() = 0;
};