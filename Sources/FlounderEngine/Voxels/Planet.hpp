#pragma once

#include "../Objects/Component.hpp"
#include "Chunk.hpp"

namespace Flounder
{
	enum PlanetSide
	{
		SideFront = 0,
		SideBack = 1,
		SideUp = 2,
		SideDown = 3,
		SideLeft = 4,
		SideRight = 5
	};

	class F_EXPORT Planet :
		public Component
	{
	private:
		std::vector<Chunk *> *m_chunks;
		int m_sideLength;
		bool m_generate;
	public:
		Planet(const int &sideLength = 4);

		~Planet();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		std::string GetName() const override { return "Planet"; };

		static Vector3 GetSideDirection(const PlanetSide &side);

		static PlanetSide GetSide(const Vector3 &position, const int &sideLength);
	private:
		void Generate();
	};
}
