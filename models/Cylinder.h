#ifndef CYLINDER_H
#define CYLINDER_H

namespace gm {

    template<typename _Scalar, int SEG = 16>
    class Cylinder : public BaseModel<Cylinder<_Scalar>>
    {
    public:
        typedef _Scalar Scalar;
        template<typename Scalar>
        Cylinder(Scalar x, Scalar y, Scalar z, 
            Scalar d, Scalar h,
            Scalar pitch = 0, Scalar yaw = 0, Scalar roll = 0)
            :BaseModel()
        {
            this->type = MODEL_CYLINDER;
            if (!_isInit) init();
            translate(x, y, z);
            rotateTo(pitch, yaw, roll);
            this->_d = d > 0 ? d : (Scalar)1;
            this->_h = h > 0 ? h : (Scalar)1;
            scaleTo(_d, _d, _h);
        }
        constexpr size_t vertexSize() override { return _vertexSize; }
        constexpr size_t indicesSize() override { return _indicesSize; }
        inline Scalar* getVertex() override { return _vertex; }
        inline unsigned int* getIndices() override { return _indices; }

    private:
        Scalar _d;
        Scalar _h;
        static constexpr int _vertexSize = ((SEG * 2 + 2) * 2 + (SEG + 1) * 2) * EACH_VERTEX_SIZE;
        static constexpr int _indicesSize = SEG * 3 * 2 + SEG * 2 * 3 + 12;
        static Scalar _vertex[_vertexSize];
        static unsigned int _indices[_indicesSize];
        static bool _isInit;

        static void init()
        {
            // Init vertex
            const float sectorStep = 2 * PI / SEG;
            float angle = 0.0f;
            int count = 0;
            for (int i = 0; i <= SEG; i++)
            {
                _vertex[count++] = std::cos(angle)/ (Scalar)2;
                _vertex[count++] = (Scalar)1 / (Scalar)2;
                _vertex[count++] = std::sin(angle)/ (Scalar)2;
                
                _vertex[count++] = 0;
                _vertex[count++] = 1;
                _vertex[count++] = 0;
                _vertex[count++] = (Scalar)i / (Scalar)SEG;
                _vertex[count++] = (Scalar)2 / (Scalar)3;
                angle += sectorStep;
            }
            for (int i = 0; i <= SEG; i++)
            {
                _vertex[count++] = 0;
                _vertex[count++] = (Scalar)1 / (Scalar)2;
                _vertex[count++] = 0;
                
                _vertex[count++] = 0;
                _vertex[count++] = 1;
                _vertex[count++] = 0;
                _vertex[count++] = (Scalar)i / (Scalar)SEG;
                _vertex[count++] = (Scalar)3 / (Scalar)3;
            }
            angle = 0;
            for (int i = 0; i <= SEG; i++)
            {
                _vertex[count++] = std::cos(angle)/ (Scalar)2;
                
                _vertex[count++] = -(Scalar)1 / (Scalar)2;
                _vertex[count++] = std::sin(angle) / (Scalar)2;
                _vertex[count++] = 0;
                _vertex[count++] = -1;
                _vertex[count++] = 0;
                _vertex[count++] = (Scalar)i / (Scalar)SEG;
                _vertex[count++] = (Scalar)1 / (Scalar)3;
                angle += sectorStep;
            }
            for (int i = 0; i <= SEG; i++)
            {
                _vertex[count++] = 0;
                _vertex[count++] = -(Scalar)1 / (Scalar)2;
                _vertex[count++] = 0;
                
                _vertex[count++] = 0;
                _vertex[count++] = -1;
                _vertex[count++] = 0;
                _vertex[count++] = (Scalar)i / (Scalar)SEG;
                _vertex[count++] = (Scalar)0 / (Scalar)3;
            }
            angle = 0;
            for (int i = 0; i <= SEG; i++)
            {
                _vertex[count++] = std::cos(angle)/ (Scalar)2;
                _vertex[count++] = (Scalar)1 / (Scalar)2;
                _vertex[count++] = std::sin(angle)/ (Scalar)2;
                
                _vertex[count++] = std::cos(angle);
                _vertex[count++] = 0;
                _vertex[count++] = std::sin(angle);
                
                _vertex[count++] = (Scalar)i / (Scalar)SEG;
                _vertex[count++] = (Scalar)2 / (Scalar)3;
                
                _vertex[count++] = std::cos(angle)/ (Scalar)2;
                _vertex[count++] = -(Scalar)1 / (Scalar)2;
                _vertex[count++] = std::sin(angle)/ (Scalar)2;
                _vertex[count++] = std::cos(angle);
                _vertex[count++] = 0;
                _vertex[count++] = std::sin(angle);
                
                _vertex[count++] = (Scalar)i / (Scalar)SEG;
                _vertex[count++] = (Scalar)1 / (Scalar)3;
                
                angle += sectorStep;
            }
            // Init indices
            count = 0;
            for (int i = 0; i <= SEG; i++)
            {
                _indices[count++] = i;
                _indices[count++] = i + 1;
                _indices[count++] = i + SEG + 1;
            }
            for (int i = 0; i <= SEG; i++)
            {
                _indices[count++] = i + SEG + SEG +2;
                _indices[count++] = i + 1 + SEG + SEG + 2;
                _indices[count++] = i  + SEG +SEG +2+ SEG+1;
            }
            for (int i = 0; i < 2*SEG; i+=2)
            {
                _indices[count++] = i + (SEG + SEG + 2)*2;
                _indices[count++] = i + 1 + (SEG + SEG + 2) * 2;
                _indices[count++] = i + 2 + (SEG + SEG + 2) * 2;

                _indices[count++] = i + 1 + (SEG + SEG + 2) * 2;
                _indices[count++] = i + 2 + (SEG + SEG + 2) * 2;
                _indices[count++] = i + 3 + (SEG + SEG + 2) * 2;
            }
        }
    };

    template<typename _Scalar, int SEG>
    _Scalar Cylinder<_Scalar, SEG>::_vertex[] = { 0 };

    template<typename _Scalar, int SEG>
    unsigned int Cylinder<_Scalar, SEG>::_indices[] = { 0 };

    template<typename _Scalar, int SEG>
    bool Cylinder<_Scalar, SEG>::_isInit = false;

    template<typename _Scalar, int SEG>
    struct traits<Cylinder<_Scalar, SEG>>
    {
    public:
        typedef _Scalar Scalar;
    };

}


#endif