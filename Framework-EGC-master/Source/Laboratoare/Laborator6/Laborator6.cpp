#include "Laborator6.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"
#include "LabCamera.h"

using namespace std;


struct TVector
{
	float x, y, z;
};

struct TObject3D
{
	TVector position;
};

struct Intersect
{
	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
};

Intersect sph, cub;

TObject3D sphere;

//functie de determinare a coliziunilor
bool intersec(Intersect a, Intersect b) {
	return (a.minX <= b.maxX && a.maxX >= b.minX) &&
		(a.minY <= b.maxY && a.maxY >= b.minY) &&
		(a.minZ <= b.maxZ && a.maxZ >= b.minZ);
}

Laborator6::Laborator6()
{
}

Laborator6::~Laborator6()
{
}

void Laborator6::Init()
{
	//parametrii camera si set -> pozitie + centru
	{
		renderCameraTarget = false;
		projectionType = true;

		right = 10.f;
		left = .01f;
		bottom = .01f;
		top = 10.f;
		fov = 40.f;

		camera = new LaboratorC::Camera();
		camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	}

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, Z_NEAR, Z_FAR);

	//platforme
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// background
	{
		std::vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 1.15, 0.f), glm::vec3(0.f, 0.f, 0.f)),
			VertexFormat(glm::vec3(-1, 1.15, 0.f), glm::vec3(0.f, 0.f, 0.f)),
			VertexFormat(glm::vec3(-1, 1.25, 0.f), glm::vec3(0.f, 0.f, 0.f)),
			VertexFormat(glm::vec3(0, 1.25, 0.f), glm::vec3(0.f, 0.f, 0.f)),
		};

		std::vector<GLushort> indices =
		{
			0, 1, 2,
			0, 2, 3
		};

		CreateMesh("background", vertices, indices);
	}

	// combustibil
	{
		std::vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 1.15, 0.f), glm::vec3(0.f, 0.f, 0.f)),
			VertexFormat(glm::vec3(-1, 1.15, 0.f), glm::vec3(0.f, 0.f, 0.f)),
			VertexFormat(glm::vec3(-1, 1.25, 0.f), glm::vec3(0.f, 0.f, 0.f)),
			VertexFormat(glm::vec3(0, 1.25, 0.f), glm::vec3(0.f, 0.f, 0.f)),
		};

		std::vector<GLushort> indices =
		{
			0, 1, 2,
			0, 2, 3
		};

		CreateMesh("fuel", vertices, indices);
	}

	//player
	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	//shader program
	{
		Shader *shader = new Shader("ShaderLab6");
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//shader combustibil
	{
		Shader* shader1 = new Shader("ShaderCombustibil");
		shader1->AddShader("Source/Laboratoare/Laborator6/Shaders/VertexShaderCombustibil.glsl", GL_VERTEX_SHADER);
		shader1->AddShader("Source/Laboratoare/Laborator6/Shaders/FragmentShaderCombustibil.glsl", GL_FRAGMENT_SHADER);
		shader1->CreateAndLink();
		shaders[shader1->GetName()] = shader1;
	}

	//pozitie player
	sphere.position.x = 0;
	sphere.position.y = 2;
	sphere.position.z = -2;

}

Mesh* Laborator6::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}


void Laborator6::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator6::Update(float deltaTimeSeconds)
{
	
	//camera type
	if (!cameraType) {
		camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	}
	else {
		camera->Set(glm::vec3(translateX, 0.2 + sphere.position.y, sphere.position.z - 0.2), glm::vec3(translateX, 0.2 + sphere.position.y, -3), glm::vec3(0, 1, 0));
	}

	//render fuel bar:
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Scalee(-1 + x_scale, 0.8, 1);
		RenderSimpleMesh(meshes["background"], shaders["ShaderCombustibil"], modelMatrix, glm::vec3(1, 0.5, 0.5));
		
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Scalee(-1 , 0.8, 1);
		RenderSimpleMesh(meshes["fuel"], shaders["ShaderCombustibil"], modelMatrix, glm::vec3(1, 1, 1));

	}

	//coordonate coliziune minge
	sph.minX = translateX - 0.2 + 0.05;
	sph.maxX = translateX + 0.2 - 0.05;
	sph.minY = 0.2 + sphere.position.y - 0.2;
	sph.maxY = 0.2 + sphere.position.y + 0.2;
	sph.minZ = -2 - 0.2 + 0.05;
	sph.maxZ = -2 + 0.2 - 0.05;

	//coliziune generala -> fac salt player daca nu am colisiune
	if (col == false) {
		sphere.position.y += v * deltaTimeSeconds + (g * speed_minge) * deltaTimeSeconds * deltaTimeSeconds / 2;
		v += g * deltaTimeSeconds;
	}
	else { 
		v = -5;
	}
	col = false;
	
	//timpul cu viteza maxima (platforme portocalii) scade
	if (t > 0) {
		t -= 0.4;
	}

	//prima coloana de platforme
	{	
		//verific daca exista coliziune si modific viteza + gravitatia:
		for (int i = 0; i < 7; i++) {
			cub.minX = -1 - 0.5;
			cub.maxX = -1 + 0.5;
			cub.minY = -0.1;
			cub.maxY = 0.05;
			cub.minZ = z_pos1[i] - z1[i] / 2;
			cub.maxZ = z_pos1[i] + z1[i] / 2;
			if (intersec(sph, cub)) {
				if (color.at(c1[i]) == color_obj_p) {
					speed = v_max;
					g = -16.1;
					speed_minge = 19.3;
					t = 5;
				}
			}
		}

			for (int i = 0; i < 7; i++) {
				// pozitie platforma
				z_pos1[i] = z_pos1[i] - deltaTimeSeconds * (-35) * speed;

				glm::mat4 modelMatrix = glm::mat4(1);
				
				modelMatrix *= Transform3D::Scalee(1, 0.1, 1);

				modelMatrix *= Transform3D::Translatee(-1, 0, z_pos1[i]);

				modelMatrix *= Transform3D::Scalee(1, 1, z1[i]);

				//coordonate coliziune platforma
				cub.minX = -1 - 0.5;
				cub.maxX = -1 + 0.5;
				cub.minY = -0.1;
				cub.maxY = 0.05;
				cub.minZ = z_pos1[i] - z1[i] / 2;
				cub.maxZ = z_pos1[i] + z1[i] / 2;

				//coliziune cu fiecare tip de platforma
				if (intersec(sph, cub)) {
					//coliziune generala
					col = true;
					//coliziune cu prima coloana
					col1 = true;
					//tasta SPACE
					press = false;

					if (color.at(c1[i]) == color_obj_g) {
						combustibil -= 0.1;
						x_scale += 0.0102;
					}

					if (color.at(c1[i]) == color_obj_v) {
						combustibil += 0.1;
						x_scale -= 0.0102;
					}

					if (color.at(c1[i]) == color_obj_r) {
						exit(-1);
					}
				  
				}

				if (col1 == true && z_pos1[i] >= -2 -z1[i]/2 ) {
					RenderMesh(meshes["box"], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 0, 1));
				}
				else {
					RenderMesh(meshes["box"], shaders["ShaderLab6"], modelMatrix, color.at(c1[i]));
				}
				
				//ca sa nu mai intre in platforme player
				if (sphere.position.y > -0.5 && sphere.position.y < 0 && col1) {
					sphere.position.y = 0;
				}

				col1 = false;
				
				//daca platforma trece de player o readuc intr-o pozitie fixa si generez aletoriu noua culoare
				//si cu cat o scalez
				if (z_pos1[i] > 0.5) {
					z_pos1[i] = -35;
					z1[i] = 2 + rand() % 3;
					c1[i] = rand() % 5;
				}	
			}	
	}

	//coloana doi de platforme
	{
		//verific daca exista coliziune si modific viteza + gravitatia:
		for (int i = 0; i < 7; i++) {
			cub.minX = 0 - 0.5;
			cub.maxX = 0 + 0.5;
			cub.minY = -0.1;
			cub.maxY = 0.05;
			cub.minZ = z_pos2[i] - z2[i] / 2;
			cub.maxZ = z_pos2[i] + z2[i] / 2;
			if (intersec(sph, cub)) {
				if (color.at(c2[i]) == color_obj_p) {
					speed = v_max;
					g = -16.1;
					speed_minge = 19.3;
					t = 5;
				}
			}
		}

		for (int i = 0; i < 7; i++) {
			// pozitie platforma
			z_pos2[i] = z_pos2[i] - deltaTimeSeconds * (-35) * speed;

			glm::mat4 modelMatrix = glm::mat4(1);
			
			modelMatrix *= Transform3D::Scalee(1, 0.1, 1);

			modelMatrix *= Transform3D::Translatee(0, 0, z_pos2[i]);

			modelMatrix *= Transform3D::Scalee(1, 1, z2[i]);
			
			//coordonate platforma
			cub.minX = 0 - 0.5;
			cub.maxX = 0 + 0.5;
			cub.minY = -0.1;
			cub.maxY = 0.05;
			cub.minZ = z_pos2[i] - z2[i] / 2;
			cub.maxZ = z_pos2[i] + z2[i] / 2;

			//coliziunea cu player
			if (intersec(sph, cub)) {
				col = true;
				col2 = true;
				press = false;

				//tipuri de coliziuni cu efectele specifice
				if (color.at(c2[i]) == color_obj_g) {
					combustibil -= 0.1;
					x_scale += 0.0102;
				}
				if (color.at(c2[i]) == color_obj_v) {
					combustibil += 0.1;
					x_scale -= 0.0102;
				}
				if (color.at(c2[i]) == color_obj_r) {
					exit(-1);
				}
			}

			if (col2 == true && z_pos2[i] >= -2 - z2[i] / 2) {
				RenderMesh(meshes["box"], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 0, 1));
			}
			else {
				RenderMesh(meshes["box"], shaders["ShaderLab6"], modelMatrix, color.at(c2[i]));
			}

			//conditie ca sfera sa nu mai intre in platforma
			if (sphere.position.y > -0.5 && sphere.position.y < 0 && col2) {
				sphere.position.y = 0;
			}
			col2 = false;

			//readuc platforma intr-un punct fix
			if (z_pos2[i] > 0.5) {
				z_pos2[i] = -35; //loc[i] - 15;
				z2[i] = 2 + rand() % 3;
				c2[i] = rand() % 5;
				//evit cazul in care am 3 platforme rosii pe acelasi rand
				if (c2[i] == 0 && c1[i] == 0) {
					c2[i] = 1 + rand() % 4;
				}
			}
		}
		
	}

	//coloana 3 de platforme
	{
		for (int i = 0; i < 7; i++) {
			//coordonate platforma
			cub.minX = 1 - 0.5;
			cub.maxX = 1 + 0.5;
			cub.minY = -0.1;
			cub.maxY = 0.05;
			cub.minZ = z_pos3[i] - z3[i] / 2;
			cub.maxZ = z_pos3[i] + z3[i] / 2;

			//determinare coliziune si efectele ei
			//vreau sa stiu de dinainte daca exista vreo coliziune ca sa pot aplica modificarile asupra
			//tuturor platformelor
			if (intersec(sph, cub)) {
				if (color.at(c3[i]) == color_obj_p) {
					speed = v_max;
					g = -16.1;
					speed_minge = 19.3;
					t = 5;
				}
			}
		}

		for (int i = 0; i < 7; i++) {
			//pozitie platforma
			z_pos3[i] = z_pos3[i] - deltaTimeSeconds * (-35) * speed;

			glm::mat4 modelMatrix = glm::mat4(1);

			modelMatrix *= Transform3D::Scalee(1, 0.1, 1);

			modelMatrix *= Transform3D::Translatee(1, 0, z_pos3[i]);

			modelMatrix *= Transform3D::Scalee(1, 1, z3[i]);
			
			//coordonate coliziune platforma
			cub.minX = 1 - 0.5;
			cub.maxX = 1 + 0.5;
			cub.minY = -0.1;
			cub.maxY = 0.05;
			cub.minZ = z_pos3[i] - z3[i] / 2;
			cub.maxZ = z_pos3[i] + z3[i] / 2;

			//intersectie player cu platforma
			if (intersec(sph, cub)) {
				//coliziunea generala
				col = true;
				//coliziune cu coloana 3
				col3 = true;
				//SPACE
				press = false;

				//cazurile de coliziuni
				if (color.at(c3[i]) == color_obj_g) {
					combustibil -= 0.1;
					x_scale += 0.0102;
				}
				if (color.at(c3[i]) == color_obj_v) {
					combustibil += 0.1;
					x_scale -= 0.0102;
				}
				if (color.at(c3[i]) == color_obj_r) {
					exit(-1);
				}

			}

			if (col3 == true && z_pos3[i] >= -2 - z3[i] / 2) {
				RenderMesh(meshes["box"], shaders["ShaderLab6"], modelMatrix, glm::vec3(1, 0, 1));
			}
			else {
				RenderMesh(meshes["box"], shaders["ShaderLab6"], modelMatrix, color.at(c3[i]));
			}

			//ca sa nu mai intre in scena
			if (sphere.position.y > -0.5 && sphere.position.y < 0 && col3) {
				sphere.position.y = 0;
			}

			col3 = false;

			//readuc platforma intr-o pozitie fixa
			if (z_pos3[i] > 0.5) {
				z_pos3[i] = -35;
				z3[i] = 2 + rand() % 3;
				c3[i] = rand() % 5;
				//evit cazul in care am 3 platforme rosii pe acelasi rand
				if (c3[i] == 0 && (c1[i] == 0 || c2[i] == 0)) {
					c3[i] =  1 + rand() % 4;
				}
			}
			
		}
		
	}

	//verific daca s-a terminat timpul de viteza maxima
	if (t <= 0) {
			speed = aux_speed;
			g = g_aux;
			speed_minge = speed_minge_aux;
	}

	//player
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		
		if (sphere.position.y < -2) {
			exit(-1);
		}

		modelMatrix *= Transform3D::Translatee(translateX, 0.2 + sphere.position.y, sphere.position.z);

		modelMatrix *= Transform3D::Scalee(0.4, 0.4, 0.4);

		//animatie bila cand cade de pe platforma
		if (sphere.position.y < -0.5) {
			modelMatrix *= Transform3D::Scalee(x_bila, y_bila, z_bila);
		}

		RenderMesh(meshes["sphere"], shaders["ShaderLab6"], modelMatrix, glm::vec3(0, 1, 1));
	}

	//caz in care raman fara combustibil
	if (combustibil <= 0) {
		exit(-1);
	}
	
}

void Laborator6::FrameEnd()
{
	//DrawCoordinatSystem();
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

//RenderMesh pentru platforme (Lab5)
void Laborator6::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color)
{
	if (!mesh || !shader || !shader->program)
		return;

	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));
	mesh->Render();
}

//Alt RenderMash (Lab7)
void Laborator6::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
	{
		return;
	}

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);
	
	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	
	GLint locObject = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(locObject, 1, glm::value_ptr(color));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator6::OnInputUpdate(float deltaTime, int mods)
{
	// miscare player
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		
		if (window->KeyHold(GLFW_KEY_A))
		{	
			translateX -= deltaTime * 3;
		}
		if (window->KeyHold(GLFW_KEY_D))
		{
			translateX += deltaTime * 3;
		}

		if (window->KeyHold(GLFW_KEY_W))
		{
			if (speed <= 0.5 && t <= 0) {
				speed += 0.005;
				aux_speed += 0.005;
				speed_minge += 0.3;
				speed_minge_aux += 0.3;
				g -= 0.1;
				g_aux -= 0.1;
			}
		}

		if (window->KeyHold(GLFW_KEY_S))
		{
			if (speed > 0.1 && t <= 0) {
				speed -= 0.005;
				aux_speed -= 0.005;
				speed_minge -= 0.3;
				speed_minge_aux -= 0.3;
				g += 0.1;
				g_aux += 0.1;
			}
		}
		
	}

	//animatie bila in cadere
	if (sphere.position.y < -0.5) {
		x_bila -= deltaTime*4.3;
		y_bila -= deltaTime*4.3;
		z_bila -= deltaTime*4.3;
	}

}

void Laborator6::OnKeyPress(int key, int mods)
{
	//salt player
	if (key == GLFW_KEY_SPACE && press != true && sphere.position.y >-0.5) {
		press = true;
		sphere.position.y = 0.1;
		col = false;
		v = 3;
	}

	// tip camera
	if (key == GLFW_KEY_C)
	{
		cameraType = !cameraType;
	}
}

void Laborator6::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator6::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator6::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator6::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator6::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator6::OnWindowResize(int width, int height)
{
}
