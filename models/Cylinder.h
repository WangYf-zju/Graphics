#ifndef CYLINDER_H
#define CYLINDER_H

namespace gm {

template<typename _Scalar>
class Cylinder : public BaseModel< Cylinder < _Scalar >>
{
public:
	template<typename Scalar>
	Cylinder(Scalar x, Scalar y, Scalar z, Scalar h, Scalar r, int sectorCount = 32)
		:BaseModel()
	{
		this->_x = x > 0 ? x : 1;
		this->_y = y > 0 ? y : 1;
		this->_z = z > 0 ? z : 1;
		this->_h = h > 0 ? h : 1;
		this->_r = r > 0 ? r : 1;
		this->_sectorCount = sectorCount > 2 ? sectorCount : 32;
		this->V_num = sectorCount * 3 *3* 2 + sectorCount * 2 * 3*3;
		this->_indices = new unsigned int[this->V_num];
		this->_vertex = new Scalar[this->V_num];
		for (int i = 0; i < V_num; i++)
		{
			_indices[i] = i;
		}
		updateVertex();

	}
	inline size_t vertexSize() override { return this->V_num; }
	inline size_t indicesSize() override { return this->V_num; }
	inline _Scalar* getVertex() override { return _vertex; }
	inline unsigned int* getIndices() override { return _indices; }

	~Cylinder()
	{
        delete[] _indices;
		delete[] _vertex;
	}
private:
	//更新顶点参数
	void updateVertex()
	{
		//更新上下两个圆面的顶点信息
		const float PI = 3.1415926f;
		const float sectorStep = 2 * PI / _sectorCount;
		float angle = 0.0f;
		for (int i = 0; i < _sectorCount; i++)
		{
			_vertex[i] = _x + _r * cos(angle);
			_vertex[i + 1] = _y + _r * sin(angle);
			_vertex[i + 2] = _z - _h / 2.0;
			angle += sectorStep;
			_vertex[i + 3] = _x + _r * cos(angle);
			_vertex[i + 4] = _y + _r * sin(angle);
			_vertex[i + 5] = _z - _h / 2.0;
			_vertex[i + 6] = _x;
			_vertex[i + 7] = _y;
			_vertex[i + 8] = _z - _h / 2.0;
			
		}
		angle = 0.0f;
		for (int i = _sectorCount*9; i < _sectorCount * 9+_sectorCount; i++)
		{
			_vertex[i] = _x + _r * cos(angle);
			_vertex[i + 1] = _y + _r * sin(angle);
			_vertex[i + 2] = _z + _h / 2.0;
			angle += sectorStep;
			_vertex[i + 3] = _x + _r * cos(angle);
			_vertex[i + 4] = _y + _r * sin(angle);
			_vertex[i + 5] = _z + _h / 2.0;
			_vertex[i + 6] = _x;
			_vertex[i + 7] = _y;
			_vertex[i + 8] = _z + _h / 2.0;

		}
		angle = 0.0f;
		//更新圆周顶点信息
		for (int i = _sectorCount * 18; i < _sectorCount * 18 + _sectorCount; i++)
		{
			//矩形分成两个三角形，每个三角形三个顶点，每个顶点x,y,z三个信息，所以一个矩形增加2*3*3个数据
			_vertex[i] = _x + _r * cos(angle);
			_vertex[i + 1] = _y + _r * sin(angle);
			_vertex[i + 2] = _z - _h / 2.0;
			
			_vertex[i + 3] = _x + _r * cos(angle);
			_vertex[i + 4] = _y + _r * sin(angle);
			_vertex[i + 5] = _z + _h / 2.0;
			angle += sectorStep;
			_vertex[i + 6] = _x + _r * cos(angle);
			_vertex[i + 7] = _y + _r * sin(angle);
			_vertex[i + 8] = _z - _h / 2.0;


			_vertex[i + 9] = _x + _r * cos(angle);
			_vertex[i + 10] = _y + _r * sin(angle);
			_vertex[i + 11] = _z - _h / 2.0;

			_vertex[i + 12] = _x + _r * cos(angle);
			_vertex[i + 13] = _y + _r * sin(angle);
			_vertex[i + 14] = _z + _h / 2.0;
			angle -= sectorStep;
			_vertex[i + 15] = _x + _r * cos(angle);
			_vertex[i + 16] = _y + _r * sin(angle);
			_vertex[i + 17] = _z + _h / 2.0;
			angle += sectorStep;

		}
	}

	int V_num;
	_Scalar _h, _r;
	int _sectorCount;
	_Scalar* _vertex;
	unsigned int* _indices;

};


template<typename _Scalar>
struct traits<Cylinder<_Scalar>>
{
public:
	typedef _Scalar Scalar;
};
template<>


} // namespace gm


#endif //!__CYLINDER_H