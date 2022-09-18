#include "Engine/Renderer/ParticleEmitter.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/XmlUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/ConstantBuffer.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/Particle.hpp"
#include <d3d11.h>

constexpr int GPU_CONSTANT_BUFFER_SLOT = 8;
constexpr bool DO_GPU_SIM = false;

struct GPUSimConstants
{
	Vec3 gravity;							//0-12 bytes
	float deltaSeconds = 0.f;				//12-16 bytes
	float startColor[4] = { 0.f };			//16-32 bytes
	float endColor[4] = { 0.f };			//32-48 bytes
	int gravityScale = 0;					//48-52 bytes
	Vec3 padding;							//52-64 bytes
};

ParticleEmitter::ParticleEmitter(Renderer* renderer, const Vec3& position, const ParticleEmitterData& emitterData)
	:m_position(position), m_renderer(renderer), m_emitterData(emitterData)
{
	m_particleList = new Particle[m_emitterData.m_maxParticles];
	m_particlesCPUMesh.reserve(static_cast<size_t>(m_emitterData.m_maxParticles * 6));

	//TEST the cpu sim by initializing the gpu stuff here and making is gpu true for the emitter
	InitializeGPUSimulationResource();
}

ParticleEmitter::~ParticleEmitter()
{
	delete[] m_particleList;
	m_particleList = nullptr;

	DX_SAFE_RELEASE(m_gpuSimInputBuffer);
	DX_SAFE_RELEASE(m_gpuSimInputBufferSRV);
	DX_SAFE_RELEASE(m_gpuSimOutputBuffer);
	DX_SAFE_RELEASE(m_gpuSimOutputBufferUAV);
	DX_SAFE_RELEASE(m_gpuSimShader);

	delete m_gpuSimConstants;
	m_gpuSimConstants = nullptr;
}

void ParticleEmitter::Update(float deltaSeconds, const Camera& camera)
{
	SpawnParticle(deltaSeconds);
	UpdateParticles(deltaSeconds);
	BuildCPUMesh(camera);
	UpdateDebugData();
	//PrintDebugData();
}

std::vector<Vertex_PCU>& ParticleEmitter::GetCPUMesh()
{
	return m_particlesCPUMesh;
}

void ParticleEmitter::BuildCPUMesh(const Camera& camera)
{
	m_particlesCPUMesh.clear();
	Vec3 camPos;
	EulerAngles camOrientation;
	camera.GetTransform(camPos, camOrientation);
	Vec3 cameraUp = camera.GetUpVector();
	for (int i = 0; i < m_nextParticleIndex; i++)
	{
		AddVertsForParticle(m_particleList[i], camPos, cameraUp);
	}
}

void ParticleEmitter::SpawnParticle(float deltaSeconds)
{
	RandomNumberGenerator rng;

	//calculate num particles to emit
	float particlesToEmitThisFrame = m_emitterData.m_particlesEmittedPerSecond * deltaSeconds;
	int particlesToEmit = RoundDownToInt(particlesToEmitThisFrame);
	float partialParticle = particlesToEmitThisFrame - (float)particlesToEmit;
	m_partialParticle += partialParticle;
	if (m_partialParticle > 1.f)
	{
		particlesToEmit++;
		m_partialParticle--;
	}

	//emit particles and set initial values
	for(int i = 0; i < particlesToEmit; i++)
	{
		//cannot spawn anymore particles as it has reached the max limit
		if (m_nextParticleIndex >= m_emitterData.m_maxParticles)
			break;

		Particle newParticle;
		newParticle.m_position = m_position;
		Vec3 randomDirection = rng.GetRandomDirectionInCone(Vec3(0.f, 0.f, 1.f), 60.f);
		newParticle.m_velocity = rng.GetRandomFloatInRange(m_emitterData.m_startSpeed.m_min, m_emitterData.m_startSpeed.m_max) * randomDirection;
		newParticle.m_rotation = rng.GetRandomFloatInRange(m_emitterData.m_startRotationDegrees.m_min, m_emitterData.m_startRotationDegrees.m_max);
		newParticle.m_lifeTime = rng.GetRandomFloatInRange(m_emitterData.m_particleLifetime.m_min, m_emitterData.m_particleLifetime.m_max);
		//newParticle->m_effectedByGravity = m_emitterData.m_effectedByGravity;

		AddParticleToList(newParticle);
		m_debugData.m_numParticlesSpawned++;
	}
}

void ParticleEmitter::AddParticleToList(Particle& particle)
{
	m_particleList[m_nextParticleIndex] = particle;
	m_nextParticleIndex++;
}

void ParticleEmitter::RemoveParticleFromList(int particleIndex)
{
	//overwrite the dead particle with the last alive particle in the list
	m_particleList[particleIndex] = m_particleList[m_nextParticleIndex - 1];
	//reduce the next particle index by 1
	m_nextParticleIndex--;
	m_debugData.m_deadParticles++;
}

void ParticleEmitter::ProcessDeadParticles(float deltaSeconds)
{
	UNUSED(deltaSeconds);
	for (int i = 0; i < m_nextParticleIndex; i++)
	{
		if (!m_particleList[i].IsAlive())
		{
			RemoveParticleFromList(i);
		}
	}
}

void ParticleEmitter::UpdateParticles(float deltaSeconds)
{
	//if (m_emitterData.m_isGPUEmitter)
	if(DO_GPU_SIM)
		UpdateOnGPU(deltaSeconds);
	else
		UpdateOnCPU(deltaSeconds);

	ProcessDeadParticles(deltaSeconds);
}

void ParticleEmitter::UpdateOnCPU(float deltaSeconds)
{
	for (int i = 0; i < m_nextParticleIndex; i++)
	{
		if (m_particleList[i].IsAlive())
		{
			m_particleList[i].Update(deltaSeconds, m_emitterData);
		}
	}
}

void ParticleEmitter::UpdateOnGPU(float deltaSeconds)
{
	//update constant buffer
	UpdateAndBindGPUSimConstants(deltaSeconds);
	//bind resources
	m_renderer->BindComputeShaderAndComputeResources(m_gpuSimShader, m_gpuSimInputBufferSRV, m_gpuSimOutputBufferUAV);
	//copy cpu data to gpu
	m_renderer->CopyCPUToGPU(m_particleList, sizeof(Particle) * m_nextParticleIndex, m_gpuSimInputBuffer);
	//kickoff compute simulation
	m_renderer->DispatchComputeShader(m_emitterData.m_maxParticles, 1, 1);
	//get back data from gpu
	m_renderer->CopyGPUToCPU(m_particleList, sizeof(Particle) * m_nextParticleIndex, m_gpuSimOutputBuffer);
	//copy output data to input in prep of next iteration
	m_renderer->CopyOverResourcesInGPU(m_gpuSimInputBuffer, m_gpuSimOutputBuffer);
}

void ParticleEmitter::AddVertsForParticle(const Particle& particle, const Vec3& cameraPos, const Vec3& cameraUp)
{
	Vec3 particleToCamera = (cameraPos - particle.m_position).GetNormalized();
	float particleQuadHalfSize = PARTICLE_QUAD_SIZE / 2.f;
	Vec3 rightVector = CrossProduct3D(cameraUp, particleToCamera);

	/*Mat44 rotationMatrix = Mat44::CreateXRotationDegrees(particle.m_rotation);
	upVector = rotationMatrix.TransformVectorQuantity3D(upVector);
	rightVector = rotationMatrix.TransformVectorQuantity3D(rightVector);*/

	//draw the quad using up and right vector which have been gotten for the purpose of camera facing billboarding
	Vec3 topLeft = particle.m_position + cameraUp * particleQuadHalfSize + (-rightVector) * particleQuadHalfSize;
	Vec3 botLeft = particle.m_position + (-cameraUp) * particleQuadHalfSize + (-rightVector) * particleQuadHalfSize;
	Vec3 botRight = particle.m_position + (-cameraUp) * particleQuadHalfSize + rightVector * particleQuadHalfSize;
	Vec3 topRight = particle.m_position + cameraUp * particleQuadHalfSize + rightVector * particleQuadHalfSize;

	Rgba8 particleColor;
	particleColor.SetFromFloats(particle.m_color);
	AddVertsForQuad3D(m_particlesCPUMesh, topLeft, botLeft, botRight, topRight, particleColor);
	//AddVertsForQuad3D(verts, topLeft, botLeft, botRight, topRight, Rgba8::WHITE);
}

void ParticleEmitter::InitializeGPUSimulationResource()
{
	//create input buffer
	m_renderer->CreateD3DShaderResource(&m_gpuSimInputBuffer, sizeof(Particle) * m_emitterData.m_maxParticles, sizeof(Particle), m_particleList);
	//create input buffer view
	m_renderer->CreateShaderResourceView(&m_gpuSimInputBufferSRV, m_gpuSimInputBuffer);
	//create output buffer
	m_renderer->CreateD3DUnorderedAccessBuffer(&m_gpuSimOutputBuffer, sizeof(Particle) * m_emitterData.m_maxParticles, sizeof(Particle), nullptr);
	//create output buffer view
	m_renderer->CreateUnorderedAccessView(&m_gpuSimOutputBufferUAV, m_gpuSimOutputBuffer);
	//create compute shader and bind it
	m_renderer->CreateAndCompileComputeShader("Data/Shaders/EmitterSimulateCS.hlsl", &m_gpuSimShader);
	//bind compute shader and resources
	m_renderer->BindComputeShaderAndComputeResources(m_gpuSimShader, m_gpuSimInputBufferSRV, m_gpuSimOutputBufferUAV);
	//create constant buffer
	m_gpuSimConstants = m_renderer->CreateConstantBuffer(sizeof(GPUSimConstants));
}

void ParticleEmitter::UpdateAndBindGPUSimConstants(float deltaSeconds)
{
	GPUSimConstants constants;
	constants.gravity = Vec3(0.f, 0.f, -10.f);
	constants.deltaSeconds = deltaSeconds;
	m_emitterData.m_startColor.GetAsFloats(constants.startColor);
	m_emitterData.m_endColor.GetAsFloats(constants.endColor);
	constants.gravityScale = m_emitterData.m_gravityScale;

	m_renderer->CopyCPUToGPU(&constants, sizeof(GPUSimConstants), m_gpuSimConstants);
	m_renderer->BindConstantBufferToComputeShader(GPU_CONSTANT_BUFFER_SLOT, m_gpuSimConstants);
}

void ParticleEmitter::PrintDebugData()
{
	DebuggerPrintf("|-------------| \n");
	for (int i = 0; i < 10; i++)
	{
		if (m_nextParticleIndex > i)
		{
			DebugPrintParticleData(m_particleList[i]);
		}
	}
}

void ParticleEmitter::DebugPrintParticleData(const Particle& particle)
{
	DebuggerPrintf(Stringf("Particle: Pos = (%.2f, %.2f, %.2f) Vel = (%.2f, %.2f, %.2f)\n", particle.m_position.x, particle.m_position.y, particle.m_position.z,
		particle.m_velocity.x, particle.m_velocity.y, particle.m_velocity.z).c_str());
}

void ParticleEmitter::UpdateDebugData()
{
	m_debugData.m_aliveParticles = m_nextParticleIndex;
	m_debugData.m_particleListSize = m_emitterData.m_maxParticles;
	m_debugData.m_isGPUSim = DO_GPU_SIM;
}

ParticleEmitterDebugData ParticleEmitter::GetDebugData() const
{
	return m_debugData;
}

ParticleEmitterData ParticleEmitter::GetEmitterData() const
{
	return m_emitterData;
}

void ParticleEmitter::UpdateEmitterData(const ParticleEmitterData& updatedData)
{
	m_emitterData = updatedData;
}
