#pragma once

class iaitask
{
public:
	iaitask();

	virtual ~iaitask();

	virtual void update() = 0;

	virtual bool isComplete() = 0;
};