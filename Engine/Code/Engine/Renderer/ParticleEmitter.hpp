#pragma once
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Renderer/ParticleEmitterData.hpp"
#include <vector>

constexpr float PARTICLE_QUAD_SIZE = 0.2f;
//constexpr unsigned int MAX_PARTICLES = 100000;

class Camera;
class Renderer;
class ConstantBuffer;

struct Particle;
struct ParticleEmitterData;
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;
struct ID3D11ComputeShader;

struct ParticleEmitterDebugData
{
	int m_numParticlesSpawned = 0;
	int m_particleListSize = 0;
	int m_aliveParticles = 0;
	int m_deadParticles = 0;
	bool m_isGPUSim = false;
};

class ParticleEmitter
{
public:
	ParticleEmitter(Renderer* renderer, const Vec3& position, const ParticleEmitterData& emitterData);
	~ParticleEmitter();

	void Update(float deltaSeconds, const Camera& camera);
	void UpdateEmitterData(const ParticleEmitterData& updatedData);

	std::vector<Vertex_PCU>& GetCPUMesh();
	ParticleEmitterDebugData GetDebugData() const;
	ParticleEmitterData GetEmitterData() const;

private:
	void BuildCPUMesh(const Camera& camera);
	void SpawnParticle(float deltaSeconds);
	void UpdateParticles(float deltaSeconds);
	void UpdateOnCPU(float deltaSeconds);
	void UpdateOnGPU(float deltaSeconds);
	void AddVertsForParticle(const Particle& particle, const Vec3& cameraPos, const Vec3& cameraUp);
	void InitializeGPUSimulationResource();
	void UpdateAndBindGPUSimConstants(float deltaSeconds);
	void PrintDebugData();
	void DebugPrintParticleData(const Particle& particle);
	void UpdateDebugData();
	void AddParticleToList(Particle& particle);
	void RemoveParticleFromList(int particleIndex);
	void ProcessDeadParticles(float deltaSeconds);

private:
	Renderer* m_renderer = nullptr;
	Vec3 m_position;
	ParticleEmitterData m_emitterData;
	//std::vector<Particle> m_particleList = {};
	Particle* m_particleList = nullptr;
	std::vector<Vertex_PCU> m_particlesCPUMesh;
	ParticleEmitterDebugData m_debugData;
	float m_partialParticle = 0.f;
	int m_nextParticleIndex = 0;	//index where the next new particle will be stored at

	//gpu simulation variables
	ID3D11Buffer* m_gpuSimInputBuffer = nullptr;
	ID3D11ShaderResourceView* m_gpuSimInputBufferSRV = nullptr;
	ID3D11Buffer* m_gpuSimOutputBuffer = nullptr;
	ID3D11UnorderedAccessView* m_gpuSimOutputBufferUAV = nullptr;
	ID3D11ComputeShader* m_gpuSimShader = nullptr;
	ConstantBuffer* m_gpuSimConstants = nullptr;
};
