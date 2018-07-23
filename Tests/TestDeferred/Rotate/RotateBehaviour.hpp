#pragma once

#include <Objects/IBehaviour.hpp>
#include <Objects/GameObject.hpp>

using namespace fl;

namespace test
{
	class RotateBehaviour :
		public IBehaviour
	{
	private:
		Vector3 m_rate;
	public:
		RotateBehaviour(const Vector3 &rate = Vector3::ZERO);

		~RotateBehaviour();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "RotateBehaviour"; };
	};
}
