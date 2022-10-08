#ifndef SPHERE_H
#define SPHERE_H

namespace gm {

template<typename _Scalar, int SEGX = 16, int SEGY = 8>
class Sphere : public BaseModel<Sphere<_Scalar>>
{
public:
    typedef _Scalar Scalar;
	template<typename Scalar>
	Sphere(Scalar x, Scalar y, Scalar z, Scalar d,
        Scalar pitch = 0, Scalar yaw = 0, Scalar roll = 0)
		:BaseModel()
	{
        this->type = MODEL_SPHERE;
        if (!_isInit) init();
        translate(x, y, z);
        rotateTo(pitch, yaw, roll);
        this->_d = d > 0 ? d : (Scalar)1;
        scaleTo(_d, _d, _d);
	}
    constexpr size_t vertexSize() override { return _vertexSize; }
    constexpr size_t indicesSize() override { return _indicesSize; }
    inline Scalar* getVertex() override { return _vertex; }
    inline unsigned int* getIndices() override { return _indices; }

private:
	Scalar _d;
    static constexpr int _vertexSize = ((SEGX + 1) * (SEGY + 1)) * EACH_VERTEX_SIZE;
    static constexpr int _indicesSize = SEGX * SEGY * 6;
    static Scalar _vertex[_vertexSize];
    static unsigned int _indices[_indicesSize];
    static bool _isInit;

    static void init()
    {
        // Init vertex
        int index = 0;
        for (int j = 0; j <= SEGY; j++)
        {
            for (int i = 0; i <= SEGX; i++)
            {
                Scalar cx = std::cos(i * (Scalar)2.0 * (Scalar)PI / (Scalar)SEGX);
                Scalar sx = std::sin(i * (Scalar)2.0 * (Scalar)PI / (Scalar)SEGX);
                Scalar cy = std::cos(j * (Scalar)PI / (Scalar)SEGY);
                Scalar sy = std::sin(j * (Scalar)PI / (Scalar)SEGY);
                Scalar x = cx * sy / (Scalar)2;
                Scalar y = cy / (Scalar)2;
                Scalar z = sx * sy / (Scalar)2;
                _vertex[index++] = x;
                _vertex[index++] = y;
                _vertex[index++] = z;
                _vertex[index++] = x;
                _vertex[index++] = y;
                _vertex[index++] = z;
                _vertex[index++] = (Scalar)i / (Scalar)SEGX;
                _vertex[index++] = (Scalar)j / (Scalar)SEGY;
            }
        }

        index = 0;
        // Init indices
        for (int j = 1; j < SEGY; j++)
        {
            for (int i = 0; i < SEGX; i++)
            {
                _indices[index++] = (j - 1) * (SEGX + 1) + i;
                _indices[index++] = j * (SEGX + 1) + i;
                _indices[index++] = j * (SEGX + 1) + i + 1;
                _indices[index++] = j * (SEGX + 1) + i;
                _indices[index++] = j * (SEGX + 1) + i + 1;
                _indices[index++] = (j + 1) * (SEGX + 1) + i + 1;
            }
        }
    }
};

template<typename _Scalar, int SEGX, int SEGY>
_Scalar Sphere<_Scalar, SEGX, SEGY>::_vertex[] = {0};

template<typename _Scalar, int SEGX, int SEGY>
unsigned int Sphere<_Scalar, SEGX, SEGY>::_indices[] = {0};

template<typename _Scalar, int SEGX, int SEGY>
bool Sphere<_Scalar, SEGX, SEGY>::_isInit = false;

template<typename _Scalar, int SEGX, int SEGY>
struct traits<Sphere<_Scalar, SEGX, SEGY>>
{
public:
    typedef _Scalar Scalar;
};

}


#endif