#include "MGL.h"
#include "IndexedGeometryQuad.h"

namespace Aftr
{
   class MGLLife : public MGL
   {
   public:
      static MGLLife* New(WO* parent);
      void onCreate();
      void render(const Camera& cam) override;
      void update(GLSLShader* compute);
   protected:
      MGLLife(WO* parent);
      IndexedGeometryQuad* quad = nullptr;
      GLuint texture;
      GLuint texture2;
      int counter = 0;
   };
}