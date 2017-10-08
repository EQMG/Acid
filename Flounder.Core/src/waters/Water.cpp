#include "Water.hpp"

#include "../camera/Camera.hpp"
#include "../devices/Display.hpp"
#include "../maths/Maths.hpp"
#include "../textures/Texture.hpp"
#include "Waters.hpp"
#include "ShaderWaters.hpp"

namespace Flounder
{
	const float Water::WAVE_SPEED = 20.0f;
	const float Water::WAVE_LENGTH = 50.0f;
	const float Water::AMPLITUDE = 0.6f;

	const float Water::SQUARE_SIZE = 1.4f;
	const int Water::VERTEX_COUNT = 176;

	const Colour Water::WATER_COLOUR = Colour("#366996");

	const float Water::SHINE_DAMPER = 1.0f;
	const float Water::REFLECTIVITY = 0.0f;

	Water::Water(const Vector3 &position, const Vector3 &rotation) :
		m_uniformObject(new UniformBuffer(sizeof(ShaderWaters::UboObject))),
		m_model(nullptr),
		m_colour(new Colour(WATER_COLOUR)),
		m_position(new Vector3(position)),
		m_rotation(new Vector3(rotation)),
		m_offset(new Vector3()),
		m_moved(true),
		m_modelMatrix(new Matrix4()),
		m_aabb(new Aabb())
	{
		GenerateMesh();
	}

	Water::~Water()
	{
		delete m_model;

		delete m_colour;

		delete m_position;
		delete m_rotation;
		delete m_offset;

		delete m_modelMatrix;
		delete m_aabb;
	}

	void Water::Update()
	{
		if (m_moved)
		{
			Matrix4::TransformationMatrix(*m_position, *m_rotation, 1.0f, m_modelMatrix);
			m_moved = false;
		}

		m_offset->Set(
			2.0f * SQUARE_SIZE * round(Camera::Get()->GetCamera()->GetPosition()->m_x / (2.0f * SQUARE_SIZE)),
			0.0f,
			2.0f * SQUARE_SIZE * round(Camera::Get()->GetCamera()->GetPosition()->m_z / (2.0f * SQUARE_SIZE))
		);
	}

	void Water::CmdRender(const VkCommandBuffer & commandBuffer, const Pipeline & pipeline, const UniformBuffer & uniformScene)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ uniformScene.GetWriteDescriptor(0, descriptorSet), m_uniformObject->GetWriteDescriptor(1, descriptorSet) }; // TODO: Modulaize this! , m_texture->GetWriteDescriptor(2, descriptorSet)

		ShaderWaters::UboObject uboObject = {};
		uboObject.transform = Matrix4(*m_modelMatrix);
		uboObject.waterOffset = Vector3(*m_offset);
		uboObject.waveTime = Engine::Get()->GetTime() / WAVE_SPEED;
		uboObject.waveLength = WAVE_LENGTH;
		uboObject.amplitude = AMPLITUDE;
		uboObject.squareSize = static_cast<float>(SQUARE_SIZE);
		uboObject.diffuseColour = Colour(m_colour->m_r, m_colour->m_g, m_colour->m_b,
			Waters::Get()->GetEnableReflections() ? Waters::Get()->GetColourIntensity() : 1.0f);
		uboObject.shineDamper = SHINE_DAMPER;
		uboObject.reflectivity = REFLECTIVITY;
		uboObject.ignoreReflections = !Waters::Get()->GetEnableReflections();
		m_uniformObject->Update(&uboObject);

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		VkBuffer vertexBuffers[] = { m_model->GetVertexBuffer()->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		VkDescriptorSet descriptors[] = { pipeline.GetDescriptorSet() };
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdDraw(commandBuffer, m_model->GetVertexBuffer()->GetVertexCount(), 1, 0, 0);
	}

	void Water::GenerateMesh()
	{
		std::vector<Vertex> vertices = std::vector<Vertex>();

		for (int col = 0; col < VERTEX_COUNT - 1; col++)
		{
			for (int row = 0; row < VERTEX_COUNT - 1; row++)
			{
				int topLeft = (row * VERTEX_COUNT) + col;
				int topRight = topLeft + 1;
				int bottomLeft = ((row + 1) * VERTEX_COUNT) + col;
				int bottomRight = bottomLeft + 1;

				if (row % 2 == 0)
				{
					StoreQuad1(vertices, topLeft, topRight, bottomLeft, bottomRight, col % 2 == 0);
				}
				else
				{
					StoreQuad2(vertices, topLeft, topRight, bottomLeft, bottomRight, col % 2 == 0);
				}
			}
		}

		m_model = new Model(vertices);

		m_position->m_x -= m_aabb->m_maxExtents->m_x / 2.0f;
		m_position->m_z -= m_aabb->m_maxExtents->m_z / 2.0f;
		m_aabb->Update(*m_position, *m_rotation, 1.0f, m_aabb);
	}

	void Water::StoreQuad1(std::vector<Vertex> &vertices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed) const
	{
		StoreVertex(vertices, topLeft, Vector2(0.0f, 1.0f), mixed ? Vector2(1.0f, 0.0f) : Vector2(1.0f, 1.0f));
		StoreVertex(vertices, bottomLeft, mixed ? Vector2(1.0f, -1.0f) : Vector2(1.0f, 0.0f), Vector2(0.0f, -1.0f));

		if (mixed)
		{
			StoreVertex(vertices, topRight, Vector2(-1.0f, 0.0f), Vector2(-1.0f, 1.0f));
		}
		else
		{
			StoreVertex(vertices, bottomRight, Vector2(-1.0f, -1.0f), Vector2(-1.0f, 0.0f));
		}

		StoreVertex(vertices, bottomRight, Vector2(0.0f, -1.0f), mixed ? Vector2(-1.0f, 0.0f) : Vector2(-1.0f, -1.0f));
		StoreVertex(vertices, topRight, mixed ? Vector2(-1.0f, 1.0f) : Vector2(-1.0f, 0.0f), Vector2(0.0f, 1.0f));

		if (mixed)
		{
			StoreVertex(vertices, bottomLeft, Vector2(1.0f, 0.0f), Vector2(1.0f, -1.0f));
		}
		else
		{
			StoreVertex(vertices, topLeft, Vector2(1.0f, 1.0f), Vector2(1.0f, 0.0f));
		}
	}

	void Water::StoreQuad2(std::vector<Vertex> &vertices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed) const
	{
		StoreVertex(vertices, topRight, Vector2(-1.0f, 0.0f), mixed ? Vector2(0.0f, 1.0f) : Vector2(-1.0f, 1.0f));
		StoreVertex(vertices, topLeft, mixed ? Vector2(1.0f, 1.0f) : Vector2(0.0f, 1.0f), Vector2(1.0f, 0.0f));

		if (mixed)
		{
			StoreVertex(vertices, bottomRight, Vector2(0.0f, -1.0f), Vector2(-1.0f, -1.0f));
		}
		else
		{
			StoreVertex(vertices, bottomLeft, Vector2(1.0f, -1.0f), Vector2(0.0f, -1.0f));
		}

		StoreVertex(vertices, bottomLeft, Vector2(1.0f, 0.0f), mixed ? Vector2(0.0f, -1.0f) : Vector2(1.0f, -1.0f));
		StoreVertex(vertices, bottomRight, mixed ? Vector2(-1.0f, -1.0f) : Vector2(0.0f, -1.0f), Vector2(-1.0f, 0.0f));

		if (mixed)
		{
			StoreVertex(vertices, topLeft, Vector2(0.0f, 1.0f), Vector2(1.0f, 1.0f));
		}
		else
		{
			StoreVertex(vertices, topRight, Vector2(-1.0f, 1.0f), Vector2(0.0f, 1.0f));
		}
	}

	void Water::StoreVertex(std::vector<Vertex> &vertices, const int &index, const Vector2 &otherPoint1, const Vector2 &otherPoint2) const
	{
		const int gridX = index % VERTEX_COUNT;
		const int gridZ = index / VERTEX_COUNT;
		const float x = static_cast<float>(gridX * SQUARE_SIZE);
		const float z = static_cast<float>(gridZ * SQUARE_SIZE);
		const float encoded = Encode(otherPoint1.m_x, otherPoint1.m_y, otherPoint2.m_x, otherPoint2.m_y);

		if (x > m_aabb->m_maxExtents->m_x)
		{
			m_aabb->m_maxExtents->m_x = x;
		}
		else if (x < m_aabb->m_minExtents->m_x)
		{
			m_aabb->m_minExtents->m_x = x;
		}

		if (z > m_aabb->m_maxExtents->m_z)
		{
			m_aabb->m_maxExtents->m_z = z;
		}
		else if (z < m_aabb->m_minExtents->m_z)
		{
			m_aabb->m_minExtents->m_z = z;
		}

		Vertex vertex = {};
		vertex.position = Vector3(x, z, encoded);
		vertices.push_back(vertex);
	}

	float Water::Encode(const float &x, const float &z, const float &x2, const float &z2) const
	{
		const float p3 = (x + 1.0f) * 27.0f;
		const float p2 = (z + 1.0f) * 9.0f;
		const float p1 = (x2 + 1.0f) * 3.0f;
		const float p0 = (z2 + 1.0f) * 1.0f;
		return p0 + p1 + p2 + p3;
	}

	float Water::GetHeight(const float &x, const float &z) const
	{
		const float waveTime = Engine::Get()->GetTime() / WAVE_SPEED;

		const double val1 = 0.1;
		const double val2 = 0.3;
		const double radiansX = 2.0 * PI * ((Maths::Mod(x + z * x * val1, static_cast<double>(WAVE_LENGTH)) / static_cast<double>(WAVE_LENGTH)) + waveTime);
		const double radiansZ = 2.0 * PI * ((Maths::Mod(val2 * (z * x + x * z), static_cast<double>(WAVE_LENGTH)) / static_cast<double>(WAVE_LENGTH)) + waveTime * 2.0);
		const double result = 0.5 * AMPLITUDE * (cos(radiansZ + sin(x)) + sin(radiansX - cos(z)));
		return static_cast<float>(m_position->m_y + result);
	}

	void Water::SetPosition(const Vector3 &position)
	{
		m_position->Set(position);
		m_moved = true;
	}

	void Water::SetRotation(const Vector3 &rotation)
	{
		m_rotation->Set(rotation);
		m_moved = true;
	}
}
