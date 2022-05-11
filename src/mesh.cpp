
#include "mesh.h"

using namespace std;
using namespace Eigen;


void mesh::readObjFile(char* fileName)
{
	string line, token;
	ifstream infile(fileName);
	if (!infile) {
		cerr << "Error: unable to open input file: " << fileName << endl;
		exit(1);
	}

	while (getline(infile, line)) {
		istringstream stin(line);

		if (!(stin >> token)) continue;

		if (token == "v") {
			Vector3f p;
			stin >> p[0] >> p[1] >> p[2];
			vertices.push_back(p);

		} else
            if (token == "vn") {
                Vector3f p;
                stin >> p[0] >> p[1] >> p[2];
                normals.push_back(p);
            }
            else
                if (token == "vt") {
                    Vector3f p;
                    stin >> p[0] >> p[1];
                    textures.push_back(p);
                }
        else
            if (token == "f") {
			vector<unsigned int> faceV;
			string n;
                while (stin >> n){
                    istringstream temp(n); // 10/10/10
                    getline(temp, n, '/'); // 10
                    
                    float f = atof(n.c_str());
                    f--;
                    faceV.push_back(f);
                }
			faceVertices.push_back(faceV);
		}
	}
	infile.close();
}


void mesh::writeObjFile(char* fileName)
{
	ofstream outfile(fileName);
	if (!outfile) {
		cerr << "Error: unable to open output file: " << fileName << endl;
		exit(1);
	}
	for (unsigned int i = 0; i < vertices.size(); ++i)
		outfile << "v " << vertices[i][0] << " " << vertices[i][1]
						<< " " << vertices[i][2] << endl;
	for (unsigned int i = 0; i < faceVertices.size(); ++i) {
		outfile << "f ";
		for (unsigned int j = 0; j < faceVertices[i].size(); ++j) {
			outfile << faceVertices[i][j] + 1;
			outfile << " ";
		}
		outfile << endl;
	}
	outfile.close();
}


void mesh::normalize()
{
	Vector3f totals(0, 0, 0);
	Vector3f maxs(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Vector3f mins(FLT_MAX, FLT_MAX, FLT_MAX);

	for (unsigned int i = 0; i < vertices.size(); ++i)
		for (unsigned int j = 0; j < 3; ++j) {
			maxs[j] = max(maxs[j], vertices[i][j]);
			mins[j] = min(mins[j], vertices[i][j]);
			totals[j] += vertices[i][j];
		}
	center = totals / (float)vertices.size();
	Vector3f scales = maxs - mins;
	float scale = (float)scales.maxCoeff();
  meshScale = 1.0/scale;
}


void mesh::glCreateDisplayList()
{
	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);
		for (unsigned int f = 0; f < faceVertices.size(); ++f) {
			glBegin(GL_TRIANGLE_FAN);
				for (unsigned int j = 0; j < faceVertices[f].size(); ++j) {
					unsigned int v = faceVertices[f][j];
					glVertex3fv(vertices[v].data()); 
				}
			glEnd();
		}
	glEndList();
}


void mesh::glCallDisplayList()
{
	if (displayList)
		glCallList(displayList);
	else {
		cerr << "Error: display list incorrectly initialized" << endl;
		exit(1);
	}
}


void mesh::createVertexArray()
{
    numVertices = vertices.size(); numNormals = normals.size(); numTextures = textures.size();
    vertexArray = new float[3*numVertices];
    normalArray = new float[3*numNormals];
    TextureArray = new float[2*numTextures];

	for (unsigned int v = 0; v < numVertices; ++v) { // FOR VERTEX ARRAY
		vertexArray[3*v + 0] = vertices[v][0];
		vertexArray[3*v + 1] = vertices[v][1];
		vertexArray[3*v + 2] = vertices[v][2];
	}
    for (unsigned int n = 0; n < numNormals; n ++) { // FOR NORMAL ARRAY
        normalArray[3*n + 0] = normals[n][0];
        normalArray[3*n + 1] = normals[n][1];
        normalArray[3*n + 2] = normals[n][2];
    }
    for (unsigned int t = 0; t < numNormals; t ++) { // FOR TEXTURE COORDINATES
        TextureArray[2*t + 0] = textures[t][0];
        TextureArray[2*t + 1] = textures[t][1];
    }
    
    
    
    // THIS IS FOR FACES

	numTriangles = 0;
	for (unsigned int f = 0; f < faceVertices.size(); ++f)
		numTriangles += faceVertices[f].size() - 2; // return the number of triangles in the mesh

	triangleIndices = new unsigned int[3*numTriangles];
	int t(0);

	for (unsigned int f = 0; f < faceVertices.size(); ++f) {
		vector<unsigned int> face(faceVertices[f]);
		unsigned int i0(face[0]);
		unsigned int i1;
		unsigned int i2;

		for (unsigned int i = 1; i < face.size() - 1; ++i) {
			// triangle fan
			i1 = face[i];
			i2 = face[i + 1];
			triangleIndices[3*t + 0] = i0;
			triangleIndices[3*t + 1] = i1;
			triangleIndices[3*t + 2] = i2;
			++t;
		}
	}
}


void mesh::glEnableVertexArray()
{
	if (vertexArray == NULL) {
		cerr << "Error: vertex array incorrectly initialized" << endl;
		exit(1);
	} else {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertexArray);
	}
}


void mesh::glDrawVertexArray()
{
	if (triangleIndices == NULL) {
		cerr << "Error: triangle indices incorrectly initialized" << endl;
		exit(1);
	} else
		glDrawElements(GL_TRIANGLES, 3*numTriangles, GL_UNSIGNED_INT,
		               triangleIndices);
}


void mesh::glEnableVBO()
{
    
	unsigned int bufferIds[4];
	glGenBuffers(4, bufferIds);

    // VERTICES
	glBindBuffer(GL_ARRAY_BUFFER, bufferIds[0]);
	glBufferData(GL_ARRAY_BUFFER, 3*numVertices*sizeof(float),
	             vertexArray, GL_STATIC_DRAW);
	//glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);
    
    // NORMALS
    glBindBuffer(GL_ARRAY_BUFFER, bufferIds[1]);
    glBufferData(GL_ARRAY_BUFFER, 3*numNormals*sizeof(float), normalArray, GL_STATIC_DRAW);
    //glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, 0);
    
    
    // TEXTURES
    // glEnableClientState(GL_TEXTURE_COORD_ARRAY);
     glBindBuffer(GL_ARRAY_BUFFER, bufferIds[2]);
     glBufferData(GL_ARRAY_BUFFER, 2*numTextures*sizeof(float), TextureArray, GL_STATIC_DRAW);
     glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIds[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*numTriangles*sizeof(unsigned int),
	             triangleIndices, GL_STATIC_DRAW);
}


void mesh::glDrawVBO()
{
	glDrawElements(GL_TRIANGLES, 3*numTriangles, GL_UNSIGNED_INT, 0);
}


void mesh::glEnableVAO()
{

#ifdef __APPLE__
	glGenVertexArraysAPPLE(1, vaoId);
	glBindVertexArrayAPPLE(vaoId[0]);
#else
	glGenVertexArrays(1, vaoId);
	glBindVertexArray(vaoId[0]);
#endif
	mesh::glEnableVBO();
}


void mesh::glDrawVAO()
{
#ifdef __APPLE__
	glBindVertexArrayAPPLE(vaoId[0]);
#else
	glBindVertexArray(vaoId[0]);
#endif
	glDrawElements(GL_TRIANGLES, 3*numTriangles, GL_UNSIGNED_INT, 0);
}

