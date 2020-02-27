#include <gtest/gtest.h>

#include <Maths/Vector2.hpp>

TEST(Vector2, operators) {
	acid::Vector2f a(3.0f, -7.2f);
	acid::Vector2f b(-1.74f, 15.4f);
	acid::Vector2ui c(5, 2);
	
	ASSERT_EQ(a + 20.0f, acid::Vector2f(23, 12.8));
	ASSERT_EQ(-1.11f * a, acid::Vector2f(-3.33, 7.992));
	ASSERT_EQ(a + b, acid::Vector2f(1.26, 8.2));
	ASSERT_EQ(a - b, acid::Vector2f(4.74, -22.6));
	ASSERT_EQ(a * b, acid::Vector2f(-5.22, -110.88));
	ASSERT_EQ(a / b, acid::Vector2f(-1.72414, -0.467532));
	ASSERT_EQ(a.Angle(b), 2.85931);
	ASSERT_EQ(a.Dot(b), -116.1);
	ASSERT_EQ(a.Lerp(b, 0.5f), acid::Vector2f(0.63, 4.1));
	ASSERT_EQ(a.Scale(10.0f), acid::Vector2f(30, -72));
	ASSERT_EQ(a.Rotate(1.5708f), acid::Vector2f(7.19999, 3.00003));
	ASSERT_EQ(-a, acid::Vector2f(-3, 7.2));
	ASSERT_EQ(a.Normalize(), acid::Vector2f(0.384615, -0.923077));
	ASSERT_EQ(a.Length(), 7.8);
	ASSERT_EQ(a.Distance(b), 23.0917);;
}
