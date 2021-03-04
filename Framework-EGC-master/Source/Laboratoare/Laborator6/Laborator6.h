#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include "LabCamera.h"

#define GRAVITY 4.0
#define Z_FAR		(200.f)
#define Z_NEAR		(.01f)

class Laborator6 : public SimpleScene
{
	public:
		Laborator6();
		~Laborator6();

		void Init() override;

		Mesh * CreateMesh(const char * name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices);

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
		void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		bool col = true, col1 = true, col2 = true, col3 = true;
		float start = 3;
		float translateX = 0;
		float combustibil = 10;
		
		//culori platforme
		glm::vec3 color_obj_r = glm::vec3(1, 0, 0);
		glm::vec3 color_obj_g = glm::vec3(1, 1, 0);
		glm::vec3 color_obj_p = glm::vec3(1, 0.5, 0);
		glm::vec3 color_obj_v = glm::vec3(0, 1, 0);
		glm::vec3 color_obj_a = glm::vec3(0, 0, 1);
		glm::vec3 color_obj_m = glm::vec3(1, 0, 1);
		
		//vector de culori
		std::vector<glm::vec3> color = {
	   glm::vec3(1, 0, 0),
	   glm::vec3(1, 1, 0),
	   glm::vec3(1, 0.5, 0),
	   glm::vec3(0, 1, 0),
	   glm::vec3(0, 0, 1)
		};

		//vectori de indexi culori
		std::vector<float> c1 = { 3, 1, 2, 3, 4, 0, 1, 2, 3, 4, 0, 1};
		std::vector<float> c2 = { 3, 4, 1, 3, 4, 3, 3, 0, 1, 2, 3, 4};
		std::vector<float> c3 = { 3, 1, 3, 4, 0, 1, 2, 3, 4, 0, 1, 2};

		//vectori de pozitii initiale
		std::vector<float> z_pos1 = {-20, -25, -30, -35, -40, -45, -50, -55, -60, -65};
		std::vector<float> z_pos2 = {-23 ,-28, -33, -38, -43, -48, -53, -58, -63, -68, -73};
		std::vector<float> z_pos3 = {-20, -25, -30, -35, -40, -45, -50, -55, -60, -65};

		//vectori de scalari
		std::vector<float> z1 = { 2, 3, 4, 2, 3, 4, 2, 3, 4, 2};
		std::vector<float> z2 = { 6, 3, 3, 2, 2, 3, 4, 4, 3, 2, 3};
		std::vector<float> z3= { 2, 4, 3, 3, 2, 4, 4, 2, 3, 4 };

		float speed = 0.2;
		float aux_speed = 0.2;
		float x_bila = 1;
		float y_bila = 1;
		float z_bila = 1;
		float t = 0;
		float v_max = 0.5;
		float g_aux = -10;
		float speed_minge = 1;
		float speed_minge_aux = 1;
		bool press = false;
		bool colis_port = false;
		float v = 0;
		float g = -10;
		float x_scale = 0;
		bool cameraType = false;

protected:
	LaboratorC::Camera* camera;
	glm::mat4 projectionMatrix;
	bool renderCameraTarget;
	bool projectionType;

	GLfloat right;
	GLfloat left;
	GLfloat bottom;
	GLfloat top;
	GLfloat fov;
		
};
