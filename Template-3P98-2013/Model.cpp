#include "Model.h"


//adds initial vector to the face vector so that faces can be added
Model::Model()
{
	std::vector<std::array<std::array<int, 3>, 2>> tmp;
	faces.push_back(tmp);
}

//deletes all the materials in the material table
Model::~Model()
{
	for (int i = 0; i < matToUse.size(); i++){
		if (matTable.find(matToUse.at(i)) != matTable.end()){
			delete matTable[matToUse.at(i)];
		}
	}
}

//adds a vertex to the model
void Model::addVertex(std::array<float, 3> vertex){
	v.push_back(vertex);
}
//adds a vertex normal to the model
void Model::addVertexNormal(std::array<float, 3> vertexNorm){
	vn.push_back(vertexNorm);
}
//adds a vertex texture to the model
void Model::addVertexTexture(std::array<float, 3> vertexTex){
	vt.push_back(vertexTex);
}

//adds a face to the model
void Model::addFace(int vp1i, int vp2i, int vp3i, int vnp1i, int vnp2i, int vnp3i){
	std::array<std::array<int, 3>, 2> tmpSet;
	std::array<int, 3> tmp = { vp1i, vp2i, vp3i };
	tmpSet.at(0) = tmp;
	std::array<int, 3> tmp2 = { vnp1i, vnp2i, vnp3i };
	tmpSet.at(1) = tmp2;
	faces.at(faceSet).push_back(tmpSet);
}

//sets the next face set. Each face set contains only one material
void Model::nextFaceSet(){
	faceSet++;
	std::vector<std::array<std::array<int, 3>, 2>> tmp;
	faces.push_back(tmp);
}

//adds the name of the material to use for the face set
void Model::addMaterialToUse(std::string name){
	matToUse.push_back(name);
}

//adds a material with the name as the key to the map for later lookup
void Model::addMaterial(std::string name, Material* material){
	matTable[name]  = material;
}

//draws the model
void Model::drawModel(){
	for (int i = 0; i < faces.size()-1; i++){
		Material m = *matTable[matToUse.at(i)];
		glMaterialfv(GL_FRONT, GL_AMBIENT, &(m.getAmbient()[0]));
		glMaterialfv(GL_FRONT, GL_DIFFUSE, &(m.getDiffuse()[0]));
		glMaterialfv(GL_FRONT, GL_SPECULAR, &(m.getSpecular()[0]));
		glMaterialf(GL_FRONT, GL_SHININESS, m.getShininess());

		for (int j = 0; j < faces.at(i).size(); j++){
			glBegin(GL_TRIANGLES);
			std::array<std::array<int, 3>, 2> tmp = faces.at(i).at(j);

			glNormal3fv(getNormal(tmp, 0));
			glVertex3fv(getPoint(tmp, 0));
			glNormal3fv(getNormal(tmp, 1));
			glVertex3fv(getPoint(tmp, 1));
			glNormal3fv(getNormal(tmp, 2));
			glVertex3fv(getPoint(tmp, 2));
	
			glEnd();
		}
	}
}

//returns the point referenced by the face
float* Model::getPoint(std::array<std::array<int, 3>, 2> points, int index){
	float* coord = v.at((points[0][index])-1).data();
	return coord;
}

//returns the normal referenced by the face
float* Model::getNormal(std::array<std::array<int, 3>, 2> points, int index){
	float* coord = vn.at((points[1][index])-1).data();
	return coord;
}

//adds the model to the display list
void Model::addToList(GLuint* list, int num){
	glPushMatrix();
	glNewList((*list)+num, GL_COMPILE);
		for (int i = 0; i < faces.size() - 1; i++){
			Material m = *matTable[matToUse.at(i)];
			glMaterialfv(GL_FRONT, GL_AMBIENT, &(m.getAmbient()[0]));
			glMaterialfv(GL_FRONT, GL_DIFFUSE, &(m.getDiffuse()[0]));
			glMaterialfv(GL_FRONT, GL_SPECULAR, &(m.getSpecular()[0]));
			glMaterialf(GL_FRONT, GL_SHININESS, m.getShininess());

			for (int j = 0; j < faces.at(i).size(); j++){
				glBegin(GL_TRIANGLES);
				std::array<std::array<int, 3>, 2> tmp = faces.at(i).at(j);

				glNormal3fv(getNormal(tmp, 0));
				glVertex3fv(getPoint(tmp, 0));
				glNormal3fv(getNormal(tmp, 1));
				glVertex3fv(getPoint(tmp, 1));
				glNormal3fv(getNormal(tmp, 2));
				glVertex3fv(getPoint(tmp, 2));

				glEnd();
			}
		}
		glPopMatrix();
	glEndList();
}