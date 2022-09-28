#ifndef CUBE_H
#define CUBE_H

namespace GraphicModel {
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
    template<typename Scalar>
    Cube(Scalar x, Scalar y, Scalar z, Scalar l, Scalar w, Scalar h)
        : BaseModel()
    {
        this->_x = x > 0 ? x : 1;
        this->_y = y > 0 ? y : 1;
        this->_z = z > 0 ? z : 1;
        this->_l = l > 0 ? l : 1;
        this->_w = w > 0 ? w : 1;
        this->_h = h > 0 ? h : 1;
        updateVertex(x, y, z, l, w, h);
    }
    int vertexSize() override { return sizeof(_vertex); }
    int indicesSize() override { return sizeof(indices); }
    Scalar * getVertex() override { return _vertex; }
    int * getIndices() override { return indices; }

private:
    void updateVertex(Scalar x, Scalar y, Scalar z, Scalar l, Scalar w, Scalar h)
    {
        const Scalar x1 = x + l / 2.0;
        const Scalar x2 = x - l / 2.0;
        const Scalar y1 = y + w / 2.0;
        const Scalar y2 = y - w / 2.0;
        const Scalar z1 = z + h / 2.0;
        const Scalar z2 = z - h / 2.0;
        _vertex[0] = _vertex[3] = _vertex[12] = _vertex[15] = x1;
        _vertex[6] = _vertex[9] = _vertex[18] = _vertex[21] = x2;
        _vertex[4] = _vertex[7] = _vertex[16] = _vertex[19] = y1;
        _vertex[1] = _vertex[10] = _vertex[13] = _vertex[22] = y2;
        _vertex[2] = _vertex[5] = _vertex[8] = _vertex[11] = z1;
        _vertex[14] = _vertex[17] = _vertex[20] = _vertex[23] = z2;
    }
    Scalar _l, _w, _h;
    Scalar _vertex[24];
    static int indices[36];
};

template<typename Scalar>
int Cube<Scalar>::indices[36] = {
    0, 1, 3,
    1, 2, 3,
    0, 1, 4,
    1, 4, 5,
    0, 3, 4,
    3, 4, 7,
    2, 3, 6,
    3, 6, 7,
    4, 5, 6,
    4, 6, 7,
    1, 2, 5,
    2, 5, 6,
};

template<typename _Scalar>
struct traits<Cube<_Scalar>>
{
public:
    typedef _Scalar Scalar;
};
template<>
struct traits<Cube<int>>
{
public:
    typedef int Scalar;
};

} // namespace GraphicModel

#endif // !__CUBE_H__
