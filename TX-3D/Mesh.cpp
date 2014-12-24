#include "Mesh.h"
#include <stdlib.h>
#include "GE.h"
#include "obj_parser.h"
#include "Uniform.h"

namespace graphics
{
	Mesh::Mesh(const std::string filename):
		vp(NULL), vn(NULL), vt(NULL), point_count(0), model(identity_mat4()),	model_uniform(), vao(0)
	{
		load_obj_file(filename.c_str(), vp, vt, vn, point_count);

		GLuint sphere_points_vbo = create_vbo(vp, point_count * 3);
		GLuint sphere_normals_vbo = create_vbo(vn, point_count * 3);
		vao = create_vao({ sphere_points_vbo, sphere_normals_vbo });

		model_uniform = Uniform::createUniformMatrix4fv("model", model.m);
	}

	Mesh::~Mesh()
	{
		// Free load_obj_file function mallocs
		free(vp);
		free(vn);
		free(vt);
	}

	void Mesh::draw()
	{
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, point_count);
	}

	void Mesh::setModel(mat4 m)
	{
		model = m;
		model_uniform->setChanged();
	}

	GLuint create_vao(std::list<GLuint> vbo_ids) {
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		int i = 0;
		for (auto vbo_id : vbo_ids) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			i++;
		}

		for (int j = 0; j < i; ++j) {
			glEnableVertexAttribArray(j);
		}

		return vao;
	}
}