#include "pch.h"

#include "ParticleSystem.h"

#include <Kore/Math/Random.h>

using namespace Kore;

ParticleSystem::ParticleSystem(int maxParticles, const VertexStructure& structure) {
	particlePositions = new vec3[maxParticles];
	particleVelocities = new vec3[maxParticles];
	particleTimesToLive = new float[maxParticles];
	
	numParticles = maxParticles;
	spawnRate = 0.05f;
	nextSpawn = spawnRate;

	position = vec3(0.5f, 1.3f, 0.5f);
	float b = 0.1f;
	emitMin = position + vec3(-b, -b, -b);
	emitMax = position + vec3(b, b, b);
	colorStart = vec4(2.5f, 0, 0, 1);
	colorEnd = vec4(0, 0, 0, 0);
	totalTimeToLive = 3.0f;

	init(structure);
}

void ParticleSystem::init(const VertexStructure& structure) {
	vb = new VertexBuffer(4, structure, 0);
	float* vertices = vb->lock();
	setVertex(vertices, 0, -1, -1, 0, 0, 0);
	setVertex(vertices, 1, -1, 1, 0, 0, 1);
	setVertex(vertices, 2, 1, 1, 0, 1, 1);
	setVertex(vertices, 3, 1, -1, 0, 1, 0);
	vb->unlock();

	// Set index buffer
	ib = new IndexBuffer(6);
	int* indices = ib->lock();
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;
	ib->unlock();
}

void ParticleSystem::setVertex(float* vertices, int index, float x, float y, float z, float u, float v) {
	vertices[index * 8 + 0] = x;
	vertices[index * 8 + 1] = y;
	vertices[index * 8 + 2] = z;
	vertices[index * 8 + 3] = u;
	vertices[index * 8 + 4] = v;
	vertices[index * 8 + 5] = 0.0f;
	vertices[index * 8 + 6] = 0.0f;
	vertices[index * 8 + 7] = -1.0f;
}

void ParticleSystem::update(float deltaTime) {
	// Do we need to spawn a particle?
	nextSpawn -= deltaTime;
	bool spawnParticle = false;
	if (nextSpawn < 0) {
		spawnParticle = true;
		nextSpawn = spawnRate;
	}
		
	for (int i = 0; i < numParticles; i++) {
		if (particleTimesToLive[i] < 0.0f) {
			if (spawnParticle) {
				emitParticle(i);
				spawnParticle = false;
			}
		}

		particleTimesToLive[i] -= deltaTime;

		// Note: We are using no forces or gravity at the moment.
		particlePositions[i] += particleVelocities[i] * deltaTime;
	}
}

void ParticleSystem::render(TextureUnit tex, Texture* image, ConstantLocation mLocation, ConstantLocation tintLocation, mat4 V) {
	Graphics::setBlendingMode(BlendingOperation::SourceAlpha, BlendingOperation::InverseSourceAlpha);
	Graphics::setRenderState(RenderState::DepthWrite, false);
		
	/************************************************************************/
	/* Exercise 7 1.1                                                       */
	/************************************************************************/
	/* Change the matrix V in such a way that the billboards are oriented towards the camera */

	V.Invert();
	V.Set(0, 3, 0.0f);
	V.Set(1, 3, 0.0f);
	V.Set(2, 3, 0.0f);

	/************************************************************************/
	/* Exercise 7 1.2                                                       */
	/************************************************************************/
	/* Animate using at least one new control parameter */		

	for (int i = 0; i < numParticles; i++) {
		// Skip dead particles
		if (particleTimesToLive[i] < 0.0f) continue;

		// Interpolate linearly between the two colors
		float interpolation = particleTimesToLive[i] / totalTimeToLive;
		Graphics::setFloat4(tintLocation, colorStart * interpolation + colorEnd * (1.0f - interpolation));

		mat4 M = mat4::Translation(particlePositions[i].x(), particlePositions[i].y(), particlePositions[i].z()) * mat4::Scale(0.2f, 0.2f, 0.2f);
		Graphics::setMatrix(mLocation, M * V);
		Graphics::setTexture(tex, image);
		Graphics::setVertexBuffer(*vb);
		Graphics::setIndexBuffer(*ib);
		Graphics::drawIndexedVertices();
	}
	Graphics::setRenderState(RenderState::DepthWrite, true);
}

float ParticleSystem::getRandom(float minValue, float maxValue) {
	int randMax = 1000000;
	int randInt = Random::get(0, randMax);
	float r =  (float) randInt / (float) randMax;
	return minValue + r * (maxValue - minValue);
}

void ParticleSystem::emitParticle(int index) {
	// Calculate a random position inside the box
	float x = getRandom(emitMin.x(), emitMax.x());
	float y = getRandom(emitMin.y(), emitMax.y());
	float z = getRandom(emitMin.z(), emitMax.z());

	particlePositions[index] = vec3(x, y, z);
	particleVelocities[index] = vec3(0, 0.3f, 0);
	particleTimesToLive[index] = totalTimeToLive;
}