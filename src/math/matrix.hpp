#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <cstring>

#include "vector.hpp"

class Matrix
{
public:
	inline Matrix() {}

	void make_identify() {
		memset( m_, 0, sizeof( float ) * 16 );
		m_[0] = 1.f;
		m_[5] = 1.f;
		m_[10] = 1.f;
		m_[15] = 1.f;
	}

	inline const Matrix operator + ( const Matrix &m ) const {
		Matrix tmp;
		for ( int i = 0; i < 16; ++i ) {
			tmp.m_[i] = m_[i] + m.m_[i];
		}
		return tmp;
	}

	inline const Matrix operator - ( const Matrix &m ) const {
		Matrix tmp;
		for ( int i = 0; i < 16; ++i ) {
			tmp.m_[i] = m_[i] - m.m_[i];
		}
		return tmp;
	}

	inline const Matrix & operator += ( const Matrix &m ) {
		for ( int i = 0; i < 16; ++i ) {
			m_[i] += m.m_[i];
		}
		return *this;
	}

	inline const Matrix & operator -= ( const Matrix &m ) {
		for ( int i = 0; i < 16; ++i ) {
			m_[i] -= m.m_[i];
		}
		return *this;
	}

	inline const Matrix & scale( float sx, float sy, float sz ) {
		m_[0] *= sx;
		m_[5] *= sy;
		m_[10] *= sz;
		return *this;
	}

	inline const Matrix & translate( float a, float b, float c ) {
		m_[3] += a;
		m_[7] += b;
		m_[11] += c;
		return *this;
	}

	inline const Vector apply_origin() const {
		return Vector( m_[3], m_[7], m_[11] );
	}

	inline const Vector apply_point( const Vector &p ) const {
		Vector tmp;
		tmp.x_ = p.x_ * m_[0] + p.y_ * m_[1] + p.z_ * m_[2] + m_[3];
		tmp.y_ = p.x_ * m_[4] + p.y_ * m_[5] + p.z_ * m_[6] + m_[7];
		tmp.z_ = p.x_ * m_[8] + p.y_ * m_[9] + p.z_ * m_[10] + m_[11];
		return tmp;
	}

	inline const Vector apply_vector( const Vector &v ) const {
		Vector tmp;
		tmp.x_ = v.x_ * m_[0] + v.y_ * m_[1] + v.z_ * m_[2];
		tmp.y_ = v.x_ * m_[4] + v.y_ * m_[5] + v.z_ * m_[6];
		tmp.z_ = v.x_ * m_[8] + v.y_ * m_[9] + v.z_ * m_[10];
		return tmp;
	}

	inline void pre_mul( const Matrix &m ) {
		Matrix tmp = *this;
		m_[0] = tmp.m_[0]*m.m_[0] + tmp.m_[4]*m.m_[1] + tmp.m_[8]*m.m_[2];
		m_[1] = tmp.m_[1]*m.m_[0] + tmp.m_[5]*m.m_[1] + tmp.m_[9]*m.m_[2];
		m_[2] = tmp.m_[2]*m.m_[0] + tmp.m_[6]*m.m_[1] + tmp.m_[10]*m.m_[2];
		m_[3] = tmp.m_[3]*m.m_[0] + tmp.m_[7]*m.m_[1] + tmp.m_[11]*m.m_[2] + m.m_[3];

		m_[4] = tmp.m_[0]*m.m_[4] + tmp.m_[4]*m.m_[5] + tmp.m_[8]*m.m_[6];
		m_[5] = tmp.m_[1]*m.m_[4] + tmp.m_[5]*m.m_[5] + tmp.m_[9]*m.m_[6];
		m_[6] = tmp.m_[2]*m.m_[4] + tmp.m_[6]*m.m_[5] + tmp.m_[10]*m.m_[6];
		m_[7] = tmp.m_[3]*m.m_[4] + tmp.m_[7]*m.m_[5] + tmp.m_[11]*m.m_[6] + m.m_[7];

		m_[8] = tmp.m_[0]*m.m_[8] + tmp.m_[4]*m.m_[9] + tmp.m_[8]*m.m_[10];
		m_[9] = tmp.m_[1]*m.m_[8] + tmp.m_[5]*m.m_[9] + tmp.m_[9]*m.m_[10];
		m_[10] = tmp.m_[2]*m.m_[8] + tmp.m_[6]*m.m_[9] + tmp.m_[10]*m.m_[10];
		m_[11] = tmp.m_[3]*m.m_[8] + tmp.m_[7]*m.m_[9] + tmp.m_[11]*m.m_[10] + m.m_[11];

	}

	inline void post_mul( const Matrix &m ) {
		Matrix tmp = *this;
		m_[0] = m.m_[0]*tmp.m_[0] + m.m_[4]*tmp.m_[1] + m.m_[8]*tmp.m_[2];
		m_[1] = m.m_[1]*tmp.m_[0] + m.m_[5]*tmp.m_[1] + m.m_[9]*tmp.m_[2];
		m_[2] = m.m_[2]*tmp.m_[0] + m.m_[6]*tmp.m_[1] + m.m_[10]*tmp.m_[2];
		m_[3] = m.m_[3]*tmp.m_[0] + m.m_[7]*tmp.m_[1] + m.m_[11]*tmp.m_[2] + tmp.m_[3];

		m_[4] = m.m_[0]*tmp.m_[4] + m.m_[4]*tmp.m_[5] + m.m_[8]*tmp.m_[6];
		m_[5] = m.m_[1]*tmp.m_[4] + m.m_[5]*tmp.m_[5] + m.m_[9]*tmp.m_[6];
		m_[6] = m.m_[2]*tmp.m_[4] + m.m_[6]*tmp.m_[5] + m.m_[10]*tmp.m_[6];
		m_[7] = m.m_[3]*tmp.m_[4] + m.m_[7]*tmp.m_[5] + m.m_[11]*tmp.m_[6] + tmp.m_[7];

		m_[8] = m.m_[0]*tmp.m_[8] + m.m_[4]*tmp.m_[9] + m.m_[8]*tmp.m_[10];
		m_[9] = m.m_[1]*tmp.m_[8] + m.m_[5]*tmp.m_[9] + m.m_[9]*tmp.m_[10];
		m_[10] = m.m_[2]*tmp.m_[8] + m.m_[6]*tmp.m_[9] + m.m_[10]*tmp.m_[10];
		m_[11] = m.m_[3]*tmp.m_[8] + m.m_[7]*tmp.m_[9] + m.m_[11]*tmp.m_[10] + tmp.m_[11];
	}

	inline void set_scale( float s1, float s2, float s3 ) {
		memset( m_, 0, sizeof( float )*16 );
		m_[0] = s1;
		m_[5] = s2;
		m_[10] = s3;
		m_[15] = 1.0f;
	}

	inline void set_translate( float a, float b, float c ) {
		memset( m_, 0, sizeof( float ) * 16 );
		m_[3] = a;
		m_[7] = b;
		m_[11] = c;
		m_[15] = 1.f;
	}

	static Matrix identity;

public:
	float m_[16];
};

inline bool operator < ( const Matrix &m1, const Matrix &m2 )
{
	return memcmp( &m1, &m2, sizeof( Matrix ) ) < 0;
}

#endif
