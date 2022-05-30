#include <gtest/gtest.h>

#include <Maths/Vector2.hpp>

#define EXPECT_FLOAT_APPROX_EQ(epsilon, expected, actual) EXPECT_LE((expected) - (actual), epsilon)
#define EXPECT_VECTOR_APPROX_EQ(epsilon, expected, actual) EXPECT_LE(((expected) - (actual)).Length(), epsilon)

TEST(Vector2, operators) {
	acid::Vector2f a(3.0f, -7.2f);
	acid::Vector2f b(-1.74f, 15.4f);
	acid::Vector2ui c(5, 2);

	EXPECT_VECTOR_APPROX_EQ(1e-6f, a + 20.0f, acid::Vector2f(23.0f, 12.8f));
	EXPECT_VECTOR_APPROX_EQ(1e-6f, -1.11f * a, acid::Vector2f(-3.33f, 7.992f));
	EXPECT_VECTOR_APPROX_EQ(1e-6f, a + b, acid::Vector2f(1.26f, 8.2f));
	EXPECT_VECTOR_APPROX_EQ(1e-5f, a - b, acid::Vector2f(4.74f, -22.6f));
	EXPECT_VECTOR_APPROX_EQ(1e-6f, a * b, acid::Vector2f(-5.22f, -110.88f));
	EXPECT_VECTOR_APPROX_EQ(1e-5f, a / b, acid::Vector2f(-1.72414f, -0.467532f));
	EXPECT_FLOAT_APPROX_EQ (1e-5f, a.Angle(b), 2.85931f);
	EXPECT_FLOAT_APPROX_EQ (1e-6f, a.Dot(b), -116.1f);
	EXPECT_VECTOR_APPROX_EQ(1e-6f, a.Lerp(b, 0.5f), acid::Vector2f(0.63f, 4.1f));
	EXPECT_VECTOR_APPROX_EQ(1e-6f, a.Scale(10.0f), acid::Vector2f(30.0f, -72.0f));
	EXPECT_VECTOR_APPROX_EQ(1e-5f, a.Rotate(1.5708f), acid::Vector2f(7.19999f, 3.00003f));
	EXPECT_VECTOR_APPROX_EQ(1e-6f, -a, acid::Vector2f(-3.0f, 7.2f));
	EXPECT_VECTOR_APPROX_EQ(1e-6f, a.Normalize(), acid::Vector2f(0.384615f, -0.923077f));
	EXPECT_FLOAT_APPROX_EQ (1e-6f, a.Length(), 7.8f);
	EXPECT_FLOAT_APPROX_EQ (1e-4f, a.Distance(b), 23.0917f);;
}
