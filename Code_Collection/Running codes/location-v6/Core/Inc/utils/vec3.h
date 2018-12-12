/*
 * vec3.h
 * 
 * Created : 11/9/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _VEC3_H_
#define _VEC3_H_

#include "stm32f4xx_hal.h"

template <typename T>
void swap_Element(T &first, T &second)
{
        T temp = first;
        first = second;
        second = temp;
}

template <class T>
class Vec3
{
public:
        Vec3() { set_Values(0,0,0); }
        Vec3(T x, T y, T z) { set_Values(x,y,z); }
        Vec3(Vec3 &&) = default;
        Vec3 &operator=(Vec3 &&) = default;
        ~Vec3();

        void set_Values(T x, T y, T z);
        T getX() const;
        T getY() const;
        T getZ() const;
        void setX(T x) { x_ = x; }
        void setY(T y) { y_ = y; }
        void setZ(T z) { z_ = z; }

        template <typename T1, typename T2>
        void divide(const Vec3<T1> &v, T2 num) {
                x_ = v.getX() / num;
                y_ = v.getY() / num;
                z_ = v.getZ() / num;
        }

        Vec3 mult_EW(float num) {
                Vec3 temp(*this);
                temp.x_ *= num;
                temp.y_ *= num;
                temp.z_ *= num;

                return temp;
        }

        Vec3 add_EW(float num) {
                Vec3 temp(*this);
                temp.x_ += num;
                temp.y_ += num;
                temp.z_ += num;

                return temp;
        }

        Vec3 sub_EW(float num) {
                Vec3 temp(*this);
                temp.x_ -= num;
                temp.y_ -= num;
                temp.z_ -= num;

                return temp;
        }

        template <typename T1>
        Vec3 div_EW(Vec3<T1> v) {
                Vec3 temp(*this);
                temp.x_ /= (float)v.x_;
                temp.y_ /= (float)v.y_;
                temp.z_ /= (float)v.z_;

                return temp;
        }

        template <typename T1>
        Vec3 mult_EW(Vec3<T1> v) {
                Vec3 temp(v);
                temp.x_ *= x_;
                temp.y_ *= y_;
                temp.z_ *= z_;

                return temp;
        }

        friend void swap(Vec3 &first, Vec3 &second) {
                swap_Element(first.x_, second.x_);
                swap_Element(first.y_, second.y_);
                swap_Element(first.z_, second.z_);
        }
        
        Vec3(const Vec3 &v) {
                x_ = v.x_;
                y_ = v.y_;
                z_ = v.z_;
        }
        
        Vec3 &operator=(const Vec3 &v) {
                Vec3 temp(v);
                swap(*this, temp);

                return *this;
        }

        Vec3 &operator+=(const Vec3 &rhs) {
                x_ += rhs.x_;
                y_ += rhs.y_;
                z_ += rhs.z_;

                return *this;
        }
        
        Vec3 &operator-=(const Vec3 &rhs) {
                x_ -= rhs.x_;
                y_ -= rhs.y_;
                z_ -= rhs.z_;

                return *this;
        }

        friend Vec3 operator+(Vec3 lhs, const Vec3 &rhs) {
                lhs += rhs;
                return lhs;
        }

        friend Vec3 operator-(Vec3 lhs, const Vec3 &rhs) {
                lhs -= rhs;
                return lhs;
        }

        void print() const {
                printf("%ld, %ld, %ld", (int32_t)x_, (int32_t)y_, (int32_t)z_);
        }

private:
        T x_,y_,z_;
};

template <class T>
Vec3<T>::~Vec3()
{
}

template <class T>
void Vec3<T>::set_Values(T x, T y, T z)
{
        x_ = x;
        y_ = y;
        z_ = z;
}

template <class T>
T Vec3<T>::getX() const { return x_;}

template <class T>
T Vec3<T>::getY() const { return y_;}

template <class T>
T Vec3<T>::getZ() const { return z_;}

#endif // !_VEC3_H_
