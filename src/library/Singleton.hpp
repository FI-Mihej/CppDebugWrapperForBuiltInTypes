#pragma once

template <class X>
X& Singleton()
{
	static X x;
	return x;
}

