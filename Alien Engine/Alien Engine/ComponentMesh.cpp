#include "ComponentMesh.h"
#include "glew/include/glew.h"

ComponentMesh::ComponentMesh() : Component()
{
	type = ComponentType::MESH;
}

ComponentMesh::~ComponentMesh()
{
}

void ComponentMesh::Update()
{

		glEnableClientState(GL_VERTEX_ARRAY);

	//if (id_texture != -1) {
	//	// enable textures
	//	glEnable(GL_TEXTURE_2D);
	//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//	glBindTexture(GL_TEXTURE_2D, id_texture);
	//	// set UV
	//	glBindBuffer(GL_ARRAY_BUFFER, id_uv);
	//	glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	//	
	//}
	//glColor3f(color.r, color.g, color.b);

		glColor3f(1, 1, 1);

	if (normals != nullptr) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glNormalPointer(GL_FLOAT, 0, 0);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 0.1f);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, 0);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);



}
