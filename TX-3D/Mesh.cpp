#include "Mesh.h"
#include <stdlib.h>
#include "GE.h"
#include "obj_parser.h"
#include "Uniform.h"
#include <iostream>

namespace graphics
{
	Mesh::Mesh(const std::string filename, std::shared_ptr<Texture> tex) :
		vp(NULL), vn(NULL), vt(NULL), point_count(0), model(identity_mat4()), model_uniform(), vao(0), loaded(true), texture(tex)
	{
		load_obj_file(filename.c_str(), vp, vt, vn, point_count);

		build();
	}

	Mesh::Mesh(GLfloat* vpoints, GLfloat* vnormals, GLfloat* vtexture, int pt_count, std::shared_ptr<Texture> tex) :
		vp(vpoints), vn(vnormals), vt(vtexture), point_count(pt_count), model(identity_mat4()), model_uniform(), vao(0), loaded(false), texture(tex)
	{
		build();
	}

	void Mesh::build() {
		GLuint points_vbo = create_vbo(vp, point_count * 3);
		GLuint normals_vbo = create_vbo(vn, point_count * 3);
		GLuint text_coord_vbo = create_vbo(vt, point_count * 2);
		vao = create_vao({ points_vbo, normals_vbo, text_coord_vbo });

		model_uniform = Uniform::createUniformMatrix4fv("model", model.m);
	}

	Mesh::~Mesh()
	{
		if (loaded)
		{
			// Free load_obj_file function mallocs
			free(vp);
			free(vn);
			free(vt);
		}
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

	//TODO refactor vbo in its own class !
	GLuint create_vao(std::list<GLuint> vbo_ids) {
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		int i = 0;
		for (auto vbo_id : vbo_ids) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
			if (i == 2) glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 0, NULL); // texture coord
			else glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			i++;
		}

		for (int j = 0; j < i; ++j) {
			glEnableVertexAttribArray(j);
		}

		return vao;
	}
}