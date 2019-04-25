#include <iostream>

#include "Helpers/Delegate.hpp"

using namespace acid;

class Object1 :
	public Observer
{
public:
	explicit Object1(std::string message, const float &scalar, Delegate<void(float, uint32_t)> & delegate) :
		m_message(std::move(message)),
		m_scalar(scalar)
	{
		printf("Created %p => %f\n", static_cast<void*>(this), m_scalar);

		std::function<void(float, uint32_t)> function = [this](float a, uint32_t i)
		{
			m_scalar += a;
			printf("%s, %f => %p\n", m_message.c_str(), m_scalar * static_cast<float>(i), static_cast<void*>(this));
		};
		delegate.Add(std::move(function), std::ref(*this));
	}

	~Object1()
	{
		printf("Deleted %p => %f\n", static_cast<void*>(this), m_scalar);
	}

	std::string m_message;
	float m_scalar;
};

int main(int argc, char **argv)
{
	Delegate<void(float, uint32_t)> delegate;
	{
		Object1 obj1("Hello world", 1.5f, delegate);
	}
	Object1 obj2("Second object", 2.0f, delegate);
	delegate(1.0f, 20);

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return EXIT_SUCCESS;
}
