#include "Particles.hpp"

#include "Scenes/Scenes.hpp"

namespace acid {
Particles::Particles() {
}

void Particles::Update() {
	if (Scenes::Get()->GetScene()->IsPaused()) return;

	for (auto it = particles.begin(); it != particles.end();) {
		for (auto it1 = (*it).second.begin(); it1 != (*it).second.end();) {
			(*it1).Update();

			if (!(*it1).IsAlive()) {
				it1 = (*it).second.erase(it1);
				continue;
			}

			++it1;
		}

		if (it->second.empty()) {
			it = particles.erase(it);
			continue;
		}

		std::sort((*it).second.begin(), (*it).second.end());
		(*it).first->Update((*it).second);
		++it;
	}
}

void Particles::AddParticle(Particle &&particle) {
	auto it = particles.find(particle.GetParticleType());

	if (it == particles.end()) {
		particles.emplace(particle.GetParticleType(), std::vector<Particle>());
		it = particles.find(particle.GetParticleType());
	}

	(*it).second.emplace_back(std::move(particle));
}

/*void Particles::RemoveParticle(const Particle &particle) {
	auto it = particles.find(particle.GetParticleType());

	if (it != particles.end()) {
		it->second.erase(std::remove_if(it->second.begin(), it->second.end(), [particle](Particle &p) {
			return p == particle;
		}), it->second.end());
	}
}*/

void Particles::Clear() {
	particles.clear();
}
}
