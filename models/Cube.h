#ifndef CUBE_H
#define CUBE_H

namespace gm {
/*
 *      (x2, y2, z1) 3 ________ 0 (x1, y2, z1)
 *                    /|      /|
 *    (x2, y1, z1) 2 /_______/ |
 *                   | |____1|_|
 *                   | /7    | / 4 (x1, y2, z2)
 *                   |/______|/
 *      (x2, y1, z2) 6        5 (x1, y1, z2)
 *
 *  1 (x1, y1, z1)
 *  7 (x2, y2, z2)
 */

template<typename _Scalar>
class Cube : public BaseModel<Cube<_Scalar>>
{
public:
    template<typename _Scalar>
    Cube(_Scalar x, _Scalar y, _Scalar z, 
        _Scalar l, _Scalar w, _Scalar h, 
        _Scalar pitch = 0, _Scalar yaw = 0, _Scalar roll = 0)
        : BaseModel<Cube<_Scalar>>()
    {
        this->type = MODEL_CUBE;
        this->_l = l > 0 ? l : (_Scalar)1;
        this->_w = w > 0 ? w : (_Scalar)1;
        this->_h = h > 0 ? h : (_Scalar)1;
        scaleTo(_l, _w, _h);
        rotateTo(pitch, yaw, roll);
        translate(x, y, z);
    }
    constexpr size_t vertexSize() override { return sizeof(_vertex) / sizeof(float); }
    constexpr size_t indicesSize() override { return 0; }
    inline _Scalar * getVertex() override { return _vertex; }
    inline unsigned int * getIndices() override { return nullptr; }

private:
    //static void init()
    //{
    //    const _Scalar x1 = + (_Scalar)0.5;
    //    const _Scalar x2 = - (_Scalar)0.5;
    //    const _Scalar y1 = + (_Scalar)0.5;
    //    const _Scalar y2 = - (_Scalar)0.5;
    //    const _Scalar z1 = + (_Scalar)0.5;
    //    const _Scalar z2 = - (_Scalar)0.5;
    //    _vertex[5*0+0] = _vertex[5*1+0] = _vertex[5*4+0] = _vertex[5*5+0] = x1;
    //    _vertex[5*2+0] = _vertex[5*3+0] = _vertex[5*6+0] = _vertex[5*7+0] = x2;
    //    _vertex[5*1+1] = _vertex[5*2+1] = _vertex[5*5+1] = _vertex[5*6+1] = y1;
    //    _vertex[5*0+1] = _vertex[5*3+1] = _vertex[5*4+1] = _vertex[5*7+1] = y2;
    //    _vertex[5*0+2] = _vertex[5*1+2] = _vertex[5*2+2] = _vertex[5*3+2] = z1;
    //    _vertex[5*4+2] = _vertex[5*5+2] = _vertex[5*6+2] = _vertex[5*7+2] = z2;
    //    for (int i = 0; i < 36; i++) _indices[i] = i;
    //}
    Scalar _l, _w, _h;
    static _Scalar _vertex[36*EACH_VERTEX_SIZE];
    //static bool _isInit;
};

//template<typename _Scalar>
//unsigned int Cube<_Scalar>::_indices[36] = {
//    0, 1, 3,
//    1, 2, 3,
//    0, 1, 4,
//    1, 4, 5,
//    0, 3, 4,
//    3, 4, 7,
//    2, 3, 6,
//    3, 6, 7,
//    4, 5, 6,
//    4, 6, 7,
//    1, 2, 5,
//    2, 5, 6,
//};


template<typename _Scalar>
_Scalar Cube<_Scalar>::_vertex[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

//template<typename _Scalar>
//bool Cube<_Scalar>::_isInit = false;

template<typename _Scalar>
struct traits<Cube<_Scalar>>
{
public:
    typedef _Scalar Scalar;
};

} // namespace gm

#endif // !__CUBE_H__
