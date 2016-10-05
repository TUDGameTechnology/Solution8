#pragma once

#include <Kore/Graphics/Graphics.h>

// A simple particle implementation
class Particle {
public:
	Kore::mat4 M;
	
	// The current position
	Kore::vec3 position;
	
	// The current velocity
	Kore::vec3 velocity;

	// The remaining time to live
	float timeToLive;

	// The total time time to live
	float totalTimeToLive;

	// Is the particle dead (= ready to be re-spawned?)
	bool dead;

	/************************************************************************/
	/* Solution - Simulate fire by interpolating colors over lifetime       */
	/************************************************************************/
	// The beginning color
	Kore::vec4 colorStart;

	// The end color
	Kore::vec4 colorEnd;
	
	Particle();
	
	void emit(Kore::vec3 pos, Kore::vec3 velocity, float timeToLive, Kore::vec4 colorStart, Kore::vec4 colorEnd);
	void integrate(float deltaTime);
};

class ParticleSystem {
public:
	Kore::VertexBuffer* vb;
	Kore::IndexBuffer* ib;

	// The center of the particle system
	Kore::vec3 position;

	// The minimum coordinates of the emitter box
	Kore::vec3 emitMin;

	// The maximal coordinates of the emitter box
	Kore::vec3 emitMax;
	
	// The list of particles
	Particle* particles;

	// The number of particles
	int numParticles;

	// The spawn rate
	float spawnRate;
	
	// When should the next particle be spawned?
	float nextSpawn;

	ParticleSystem(int maxParticles, const Kore::VertexStructure& structure );

	void update(float deltaTime);
	void render(Kore::TextureUnit tex, Kore::Texture* image, Kore::ConstantLocation mLocation, Kore::ConstantLocation tintLocation, Kore::mat4 V);
private:
	void init(const Kore::VertexStructure& structure);
	void setVertex(float* vertices, int index, float x, float y, float z, float u, float v);
	float getRandom(float minValue, float maxValue);
	void emitParticle(int index);
};