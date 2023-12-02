#include "MGLLife.h"
#include "GLSLShader.h"
#include "Camera.h"
using namespace Aftr;

const unsigned int TEXTURE_WIDTH = 16, TEXTURE_HEIGHT = 16;

MGLLife* MGLLife::New(WO* parent)
{
   MGLLife* ptr = new MGLLife(parent);
   ptr->onCreate();
   return ptr;
}

MGLLife::MGLLife(WO* parent) : MGL(parent)
{

}

void MGLLife::onCreate()
{
   // texture size
   

   GLfloat* data = new GLfloat[TEXTURE_WIDTH*TEXTURE_HEIGHT*4];
   for (int i = 0; i < TEXTURE_WIDTH; i++)
      for (int j = 0; j < TEXTURE_HEIGHT; j++)
         for(int k = 0; k < 4; k++)
            //if( k == 0)
               data[i*4* TEXTURE_HEIGHT + j * 4 + k] = rand() % 2;
            //else
              // data[i * 4 * TEXTURE_HEIGHT + j * 4 + k] = 0;

   glGenTextures(1, &texture);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA,
      GL_FLOAT, data);

   glGenTextures(1, &texture2);
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, texture2);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA,
      GL_FLOAT, NULL);

   glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
   glBindImageTexture(1, texture2, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

   quad = IndexedGeometryQuad::New(Aftr::QuadOrientation::qoYZ,100,100, Vector(1,1,1), true);
}

void MGLLife::render(const Camera& cam)
{
   //bind skin
   Mat4 modelMatrix = this->getModelMatrix(); ///< Returns the full 4x4 model matrix used by a shader, including orientation, position, scale.
   Mat4 normalMatrix = cam.getCameraViewMatrix() * this->dispMatrix; //for transforming normals in the shader

   std::tuple< const Mat4&, const Mat4&, const Camera& > shaderParams(modelMatrix, normalMatrix, cam);
   this->getSkin().bind(&shaderParams);
   glDisable(GL_BLEND);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, (counter % 2 == 0) ? texture : texture2);
   quad->render();

   this->getSkin().unbind();
}

void MGLLife::update(GLSLShader* compute)
{
   glBindImageTexture(0, (counter % 2 == 0) ? texture : texture2, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
   glBindImageTexture(1, (counter % 2 == 0) ? texture2 : texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
   compute->bind();
   glDispatchCompute(TEXTURE_WIDTH, TEXTURE_HEIGHT, 1);
   glMemoryBarrier(GL_ALL_BARRIER_BITS);
   ++counter;
}